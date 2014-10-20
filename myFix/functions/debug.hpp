#ifndef functions_debug_hpp
#define functions_debug_hpp

#include <fstream>

#include <boost/timer.hpp>

#include "utilities/settings/settings.hpp"
#include "fixParser/fixParser.hpp"

#define LINE 5113

void debug(const std::string & data_) {

	boost::timer t;										// timer

	myFix::fixParser parser(							// create the file parser
		myFix::settings::instance().dictionary()); 

	std::ifstream infile(data_);						// open data file

	// local variables 
	int									n_valid = 0;	// valid record counter
	std::vector<myFix::tradeMessage>	tradeMsg;
	long								nline = 1;		// line counter
	std::string							line("");		// current line
		
	while (std::getline(infile, line)) {				// get through the lines

		std::istringstream iss(line);					// istringstream

		if (!(iss >> line)) { break; }					// can't read the line

		if (nline++ == (long)LINE - 1) {					// message every 1000 line

			try {

				std::vector<myFix::tradeMessage> msg		// parse the line
					= parser.parse_trade(line);

			}
			catch (...) {}

		}

		line.clear();
	}

	std::cout
		<< "file read in "
		<< t.elapsed()
		<< " seconds"
		<< std::endl;

}

#endif