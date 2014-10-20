#ifndef functions_file_import_hpp
#define functions_file_import_hpp

// FAST file import function
// field 1128 with value 9 means FIX50SP2
// field 35 with value X means MarketDataIncrementalRefresh message
// structure:
// 
// Component Block - <MDIncGrp>	Y	Number of entries following.
// 75	TradeDate

#include <fstream>

#include <boost/timer.hpp>

#include "fixParser/fixParser.hpp"

void fileImport(
	const std::string & dict_, 
	const std::string & data_) {

	
	boost::timer t;										// timer

	myFix::fixParser parser(dict_);						// create the file parser

	std::ifstream infile(data_);						// open data file

	int nline = 1; std::string line;					// current line and counter
	
	std::vector<myFix::tradeMessage> tradeMsg;

	int g = 0;											// record counter
	while (std::getline(infile, line)) {				// get through the lines

		std::istringstream iss(line);					// istringstream
		std::string a;									// empty line
		
		if (!(iss >> a)) { break; }						// can't read the line

		if (nline++ % 1000 == 0)						// message every 1000 line
			std::cout
				<< "read "
				<< nline - 1
				<< " lines."
				<< std::endl;

		try {
		
			std::vector<myFix::tradeMessage> msg = parser.parse_trade(a);
		
			if (msg.size() > 0) {
			
				std::cout << g++ << std::endl;
			
			}

		}

		catch (...) {}

	}

	std::cout 
		<< "file read in "
		<< t.elapsed() 
		<< " seconds" 
		<< std::endl;

}

#endif