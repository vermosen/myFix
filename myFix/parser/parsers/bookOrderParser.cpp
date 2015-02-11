#include "parser/parsers/bookOrderParser.hpp"

namespace myFix {

	// ctor with dictionary ptr
	bookOrderParser::bookOrderParser(const std::shared_ptr<FIX::DataDictionary> & dict)		
		: parser(dict) {};

	void bookOrderParser::parse(const std::string & msg) {

		FIX::Message	fixMsg(msg, *dict_, false)	;
		std::string		dateVal						;								// the current date string
		std::string		sender						;
		size_t			groups_count = 0			;
		thOth::bigInt	sequenceNumber				;

		// accept only messages of type "X"
		if (fixMsg.getHeader().getField(35) != "X") {
		
			return;

		}

		// attempt to translate the header
		try {

			groups_count	= boost::lexical_cast<size_t>(fixMsg.getField(268))						;
			sequenceNumber	= boost::lexical_cast<thOth::bigInt>(fixMsg.getHeader().getField(34))	;
			dateVal			= fixMsg.getField(75)													;
			sender			= fixMsg.getHeader().getField(49)										;
			
		} catch (...)	{

			std::cout << "uncoherent header detected" << std::endl;					// debug
			return;

		}

		std::string description										;							
		FIX50SP2::MarketDataIncrementalRefresh::NoMDEntries group	;
		
		for (size_t i = 1; i <= groups_count; ++i) {								// iterate over the group messages

			fixMsg.getGroup(i, group);

			try	{

				// 0 - validity check
				if (group.getField(22) != "8")										// contract identifier type

					throw unsupportedIdentifierException();

				// 1 - get the instrument optional id
				description = group.getField(107);

				if (symbolMap_.size() != 0 &&										// try to find the symbol in the table
					symbolMap_.right.find(description) == symbolMap_.right.end())

					throw undefinedInstrumentException(description);				// undefined instrument

				thOth::instrument instrument(										// create the instrument
					symbolMap_.right.find(description)->second, 
					description);

				// 2 - get the arrival date/time and build the date
				std::string arrivalTime = dateVal;									// compute the arrival time
				arrivalTime.append(group.getField(273));
				
				thOth::dateTime time = thOth::dateTime::strToDate(arrivalTime, stream_) +
					thOth::dateTime::milliSeconds(boost::lexical_cast<int>(
					arrivalTime.substr(14, 3)));
				
				// 3 - update action, convert from int
				thOth::bookOrder::actionType act =
					(thOth::bookOrder::actionType)
					boost::lexical_cast<int>(group.getField(279));

				// 4 - bid, offer
				thOth::bookOrder::orderType type = 
					(thOth::bookOrder::orderType)
					boost::lexical_cast<char>(group.getField(269));

				// only keep track of the bid and offers
				if (type == thOth::bookOrder::bid_ || type == thOth::bookOrder::ask_) {
				
					// 5 - price and volume
					thOth::real		price		= boost::lexical_cast<thOth::real>	(group.getField(270));
					thOth::volume	quantity	= boost::lexical_cast<thOth::volume>(group.getField(271));

					// 6 - create the book order
					thOth::bookOrder order(
						type	,
						act		,
						price	,
						quantity);

					// 7 - order count (field 83)
					unsigned int	order_count	= boost::lexical_cast<unsigned int>	(group.getField(83));

					// 8 - level (field 1023)
					unsigned int	level		= boost::lexical_cast<unsigned int>	(group.getField(1023));

					messages_.push_back(
						thOth::bookOrderMessage(
							instrument		,
							time			,
							order			,
							order_count		,
							level			,
							sequenceNumber	,
							sender			));
				
				}

			// exceptions management
			} catch (unsupportedIdentifierException & e) {

				throw e;															// interrupt message read 

			} catch (undefinedInstrumentException & e) {

				throw e;															// interrupt message read 

			} catch (std::exception & e) {
			
				std::cout << e.what() << std::endl;									// log and continue
				continue;

			} catch (...) {															// continue on other exceptions

				std::cout << "unknown error occured" << std::endl;
				continue;

			}
		}
	}
}
