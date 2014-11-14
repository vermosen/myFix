#ifndef functions_debug_hpp
#define functions_debug_hpp

#include <fstream>

#include <boost/timer.hpp>

#include <thOth/time/DateTime.hpp>
#include <thOth/bar/bar.hpp>

#include "recordset/functions/requestBulkTrade.hpp"
#include "recordset/functions/insertSingleBar.hpp"

// feed a bar table from a dataset
// we want to create a function which turns 
// {id time price volume}s into a bar, ie a struct 
// {id time, open, close, high, low, volume, duration}
// in our case, we need to have 1ms bars

void debug(const std::string & file_) {

	boost::timer t;										// timer

	try {

		// request settings
		thOth::dateTime start(2014, 3, 2);
		start += thOth::dateTime::hours(23) ;
		thOth::dateTime end = start + thOth::dateTime::milliSeconds(100);	// db code 1
		myFix::instrument instrument(1, "ESH4");

		// retrieve data
		std::vector<myFix::tradeMessage> data = requestBulkTrade(instrument, start, end);

		// create the bar based on that
		double open = data.cbegin()->price_;
		double close = data.crbegin()->price_;
		double volume = 0;
		double high = open;
		double low = open;
		
		for (std::vector<myFix::tradeMessage>::const_iterator It = data.cbegin(); It != data.cend(); It++){
		
			volume += It->quantity_;
			high = max(high, It->price_);
			low = min(low, It->price_);
		
		}

		bool ret = insertSingleBar(
			instrument,
			start,
			thOth::bar(
				open,
				close,
				high,
				low,
				thOth::period(thOth::timeUnit::milliSecond, 100),
				volume));
		

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