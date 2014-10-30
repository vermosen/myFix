#ifndef functions_debug_hpp
#define functions_debug_hpp

#include <fstream>

#include <boost/timer.hpp>

#include <thOth/time/DateTime.hpp>

#include "recordset/functions/insertSingleTrade.hpp"
#include "utilities/settings/settings.hpp"
#include "parser/parser.hpp"

// test an insert on the database
void debug(const std::string & data_) {

	boost::timer t;										// timer

	try {

		// vector to insert
		std::vector<myFix::tradeMessage> messages;

		{

			// build a dateTime
			std::string dtStr("20021112000000123");
			std::stringstream ss1;
			boost::posix_time::time_input_facet * facet1		// create the facet
				= new boost::posix_time::time_input_facet("%Y%m%d%H%M%S");
			ss1.imbue(std::locale(std::locale(), facet1));

			thOth::dateTime tradeDate = thOth::dateTime::strToDate(dtStr, ss1)
				+ thOth::dateTime::milliSeconds(boost::lexical_cast<int>(dtStr.substr(15, 3)));


			myFix::tradeMessage t;
			t.price_ = 100.13;
			t.quantity_ = 1234;
			t.symbol_ = std::pair<myFix::dataBase::recordId, std::string>(999, "FAKE");
			t.time_ = tradeDate;

			messages.push_back(t);
		
		}

		// TODO: need to iterate over the ts
		for (std::vector<myFix::tradeMessage>::const_iterator It
			= messages.cbegin(); It != messages.cend(); It++) {

			if (insertSingleTrade(*It) != true)
				throw std::exception("insertion error");

		}
	}
	catch (std::exception & e) {
	
		std::cout << "an exception occured: " 
				  << std::endl
				  << e.what()
				  << std::endl;
	
	}
	catch (...) {
	
		std::cout << "unknown exception occured" << std::endl;
	
	}
	
	std::cout
		<< "test ended in "
		<< t.elapsed()
		<< " seconds"
		<< std::endl;

}

#endif