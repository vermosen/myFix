#ifndef functions_test_fix_parser_hpp
#define functions_test_fix_parser_hpp

#include <fstream>

#include <boost/timer.hpp>

#include "fixParser/fixParser.hpp"

#define PATH     "C://Temp/"
#define SETTINGS "C://Users/vermosen/Documents/GitHub/quickfix/spec/FIX50SP2.xml"
#define DATA     "XCME_MD_ES_20140303_20140307"

void testFixParser() {

	// create the data dictionary
	boost::timer t;

	try {

		// create the parser
		myFix::fixParser(SETTINGS);
	
	}
	catch (std::exception & e) {
	
		std::cout << e.what() << std::endl;
	
	}

	std::cout
		<< "file read in "
		<< t.elapsed()
		<< " seconds"
		<< std::endl;

}

#endif