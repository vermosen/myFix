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

#include "utilities/settings/settings.hpp"
#include "fixParser/fixParser.hpp"

void fileImport(const std::string & data_) {

	
	boost::timer t;										// timer

	myFix::fixParser parser(							// create the file parser
		myFix::settings::instance().dictionary());											

	std::ifstream infile(data_);						// open data file


	// local variables 
	int									n_valid = 0	;	// valid record counter
	std::vector<myFix::tradeMessage>	tradeMsg	;
	long								nline = 1	;	// line counter
	std::string							line("")	;	// current line

	while (std::getline(infile, line)) {				// get through the lines

		std::istringstream iss(line);					// istringstream
		
		if (!(iss >> line)) { break; }					// can't read the line

		if (nline++ % 1000 == 0)						// message every 1000 line
			std::cout
				<< "read "
				<< nline - 1
				<< " lines."
				<< std::endl;

		// trying to convert into fix messages
		try {
		
			std::vector<myFix::tradeMessage> msg		// parse the line
				= parser.parse_trade(line);

			// now that we get the trade messages,
			// convert into	mysql records	
		
			if (msg.size() > 0) {
			
				n_valid += msg.size();
				std::cout 
					<< "valid records: " 
					<< n_valid
					<< std::endl;
			
			}

		}

		// todo : exception management & log
		catch (...) {
		
			//std::cout 
			//	<< "parsing error on line " 
			//	<< nline 
			//	<< std::endl;
		
		}

		line.clear();									// clears temporary line

	}

	std::cout 
		<< "file read in "
		<< t.elapsed() 
		<< " seconds" 
		<< std::endl;

}

#endif