#include "parser/parsers/bookOrderParser.hpp"

namespace myFix {

	// ctor with dictionary ptr
	bookOrderParser::bookOrderParser(const std::shared_ptr<FIX::DataDictionary> & dict)		
		: parser(dict) {};

	void bookOrderParser::parse(const std::string & msg) {

		FIX::Message fixMsg(msg, *dict_, false);

		std::string		dateVal			;
		size_t			groups_count = 0;

		// for debug
		//std::string messageType = fixMsg.getHeader().getField(35);
		//

		// Try to get the number of groups in the message. If not available, discard the message.
		try {

			groups_count = boost::lexical_cast<size_t>(fixMsg.getField(268));	// convert group 268 "group count"
			dateVal = fixMsg.getHeader().getField(75);							// get date string

		} catch (...)	{

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

				std::string arrivalTime = dateVal;									// compute the arrival time
				arrivalTime.append(fixMsg.getField(273));

				// build date from string
				thOth::dateTime time = thOth::dateTime::strToDate(arrivalTime, stream_) +
					thOth::dateTime::milliSeconds(boost::lexical_cast<int>(
					arrivalTime.substr(14, 3)));

				//unsigned int		order_count,
				//unsigned int		level,	// level ?
				//unsigned long		seq_number,
				//std::string		sender_id

				// update action
				thOth::bookOrderMessage::update_action act =
					boost::lexical_cast<thOth::bookOrderMessage::update_action>(group.getField(279));

				if (group.getField(269) == "0") {			// is an offer ?

					messages_.push_back(
						thOth::bookOrderMessage(
							thOth::instrument(symbolMap_.right.find(value)->second, value),
								time, thOth::bookOrder(
									thOth::bookOrder::ask_,
									boost::lexical_cast<thOth::volume>(group.getField(271)),	// volume
									boost::lexical_cast<thOth::real>(group.getField(270))),		// price
							act, 0, 0, 0, ""));

				} else if (group.getField(269) == "1") {	// is a bid

					messages_.push_back(
						thOth::bookOrderMessage(
							thOth::instrument(symbolMap_.right.find(value)->second, value),
								time, thOth::bookOrder(
									thOth::bookOrder::bid_,
									boost::lexical_cast<thOth::volume>(group.getField(271)),	// volume
									boost::lexical_cast<thOth::real>(group.getField(270))),		// price
							act, 0, 0, 0, ""));
				
				}
			} 
			catch (undefinedInstrumentException & e) {

				throw e;															// throw outside of the class

			}
			catch (...) {															// continue on other exceptions

				continue;
			}
		}
	}
}
