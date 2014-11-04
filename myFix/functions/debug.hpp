#ifndef functions_debug_hpp
#define functions_debug_hpp

#include <fstream>

#include <boost/timer.hpp>

#include <thOth/time/DateTime.hpp>

#include "recordset/functions/insertBulkTrade.hpp"
#include "utilities/settings/settings.hpp"
#include "parser/parser.hpp"

// feed a bar table from a dataset
// we want to create a function which turns 
// {id time price volume}s into a bar, ie a struct 
// {id time, open, close, high, low, volume, duration}
// in our case, we need to have 1ms bars

struct trade {

	thOth::dateTime time_	;
					price_	;
					volume_	;
}

void debug(const std::vector<trade> & trades_) {

	boost::timer t;										// timer

	try {

		// 
		
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


			myFix::tradeMessage t1;
			t1.price_ = 100.1321;
			t1.quantity_ = 1234;
			t1.symbol_ = std::pair<myFix::dataBase::recordId, std::string>(999, "FAKE");
			t1.time_ = tradeDate;

			messages.push_back(t1);

			myFix::tradeMessage t2;
			t2.price_ = 100.1311;
			t2.quantity_ = 1111;
			t2.symbol_ = std::pair<myFix::dataBase::recordId, std::string>(999, "FAKE");
			t2.time_ = tradeDate + thOth::dateTime::time_duration(thOth::dateTime::milliSeconds(1));

			messages.push_back(t2);
		
		}

		if (insertBulkTrade(messages) != true)
			throw std::exception("insertion error");

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