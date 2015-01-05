#include "parser/parsers/tradeParser.hpp"

namespace myFix {

	tradeParser::tradeParser(const std::shared_ptr<FIX::DataDictionary> & dict)		// ctor with dictionary ptr
		: parser(dict) {};	

	void tradeParser::parse(const std::string & msg) {

		FIX::Message fixMsg(msg, *dict_, false);

		std::string dateVal; dateVal.reserve(17);

		size_t groups_count = 0;
		thOth::dateTime time;

		// Try to get the number of groups in the message. If not available, discard the message.
		try {

			groups_count	= boost::lexical_cast<size_t>(fixMsg.getField(268));	// convert group 268 "group count"
			dateVal			= fixMsg.getHeader().getField(52);						// get time string
			time			= thOth::dateTime::strToDate(dateVal, stream_) +		// build date from string
				thOth::dateTime::milliSeconds(boost::lexical_cast<int>(				// (poorly) manage fix ms format....
				dateVal.substr(14, 3)));

		}
		catch (...)	{

			return;

		}

		FIX::SecurityDesc description;
		FIX50SP2::MarketDataIncrementalRefresh::NoMDEntries group;
		std::string value;

		for (size_t i = 1; i <= groups_count; ++i) {

			fixMsg.getGroup(i, group);

			try	{

				value = group.getField(description).getString();

				if (symbolMap_.size() != 0 &&										// test on the contract
					symbolMap_.right.find(value) == symbolMap_.right.end())

					throw undefinedInstrumentException(value);						// undefined instrument

				if (group.getField(269) == "2") {									// is a trade ?

					messages_.push_back(thOth::tradeMessage(
						std::pair<thOth::BigInt, std::string>(
						symbolMap_.right.find(value)->second, value), 
						time, std::stod(group.getField(270)), 
						std::stoi(group.getField(271))));

				}
			}
			catch (undefinedInstrumentException & e) {

				throw e;						// throw outside of the class

			}
			catch (...)							// continue on other exceptions
			{
				continue;
			}
		}
	}
}
