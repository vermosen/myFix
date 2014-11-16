#ifndef functions_debug_hpp
#define functions_debug_hpp

#include <fstream>

#include <boost/timer.hpp>
#include <boost/bimap.hpp>

#include "recordset/functions/insertSingleTrade.hpp"
#include "recordset/functions/insertBulkTrade.hpp"
#include "utilities/settings/settings.hpp"
#include "parser/parser.hpp"


// feed a bar table from a dataset
// we want to create a function which turns 
// {id time price volume}s into a bar, ie a struct 
// {id time, open, close, high, low, volume, duration}
// in our case, we need to have 1ms bars

void debug(const std::string & file_, const myFix::parser & ps) {

	boost::timer t;										// timer

	try {
	
		// date
		thOth::dateTime dt(1980, 5, 5, boost::posix_time::milliseconds(1));
		thOth::dateTime dt2 = thOth::dateTime::adjust100ms(dt);

		std::cout
			<< "before: "
			<< dt
			<< std::endl;

		std::cout
			<< "after: "
			<< dt2
			<< std::endl;

	}
	catch (std::exception & e) {
	
	
	}
	std::cout
		<< "file read in "
		<< t.elapsed()
		<< " seconds"
		<< std::endl;

}

#endif