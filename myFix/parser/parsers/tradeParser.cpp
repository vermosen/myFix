#include "parser/parsers/tradeParser.hpp"

namespace myFix {

	tradeParser::tradeParser(const std::shared_ptr<FIX::DataDictionary> & d)		// provide dictionary ptr 
		: parser (d) {};

	tradeParser::~tradeParser() {};

	std::vector<std::shared_ptr<myFix::message> > tradeParser::parse(const std::string& msg) const {

		FIX::Message fixMsg(msg, *dic_, false);

		std::vector<std::shared_ptr<myFix::message> > messages;

		std::string dateVal; dateVal.reserve(17);

		size_t groups_count = 0;
		thOth::dateTime time;

		// Try to get the number of groups in the message. If not available, discard the message.
		try {

			groups_count = boost::lexical_cast<size_t>(fixMsg.getField(268));		// convert group 268 "group count"
			dateVal      = fixMsg.getHeader().getField(52);							// get time string
			time         = thOth::dateTime::strToDate(dateVal, ss_);				// build date from string
			time += thOth::dateTime::milliSeconds(									// (poorly) manage fix ms format....
				boost::lexical_cast<int>(dateVal.substr(14, 3)));

		}
		catch (...)	{

			return messages;

		}

		FIX::SecurityDesc description;
		FIX50SP2::MarketDataIncrementalRefresh::NoMDEntries group;
		std::string value;

		for (size_t i = 1; i <= groups_count; ++i) {

			fixMsg.getGroup(i, group);

			try	{

				value = group.getField(description).getString();

				// test on the contract
				if (symbolMap_.size() != 0 && symbolMap_.right.find(value) == symbolMap_.right.end()) {

					// undefined instrument
					throw undefinedInstrumentException(value);

				}

				// is a trade ?
				if (group.getField(269) == "2") {

					std::shared_ptr<message> msg(new tradeMessage);
					msg->time_     = time;
					msg->price_    = std::stod(group.getField(270));
					msg->quantity_ = std::stoi(group.getField(271));
					msg->symbol_   = std::pair<dataBase::recordId, std::string>(
						symbolMap_.right.find(value)->second, value);

					messages.push_back(msg);

				}
			}
			catch (undefinedInstrumentException & e) {

				throw e;						// throw outside of the class

			}
			catch (...)
			{
				continue;
			}
		}

		return messages;

	}
}
