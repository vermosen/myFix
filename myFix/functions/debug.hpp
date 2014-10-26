#ifndef functions_debug_hpp
#define functions_debug_hpp

#include <fstream>

#include <boost/timer.hpp>

#include <thOth/time/DateTime.hpp>

#include "utilities/settings/settings.hpp"
#include "fixParser/fixParser.hpp"

#define LINE 5113

void debug(const std::string & data_) {

	boost::timer t;										// timer

	try {
	
		// facet test
		std::string myDateStr1("20021112000000.112");
		std::string myDateStr2("20021112123112.765");
		thOth::dateTime dt1 = thOth::dateTime::fixStrToDate(myDateStr1);
		thOth::dateTime dt2 = thOth::dateTime::fixStrToDate(myDateStr1);
		std::cout << dt1 << std::endl;
		std::cout << dt2 << std::endl;

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
		<< "file read in "
		<< t.elapsed()
		<< " seconds"
		<< std::endl;

	system("pause");

}

#endif