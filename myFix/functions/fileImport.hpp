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
#include <boost/bimap.hpp>

#include "recordset/functions/insertSingleTrade.hpp"
#include "utilities/settings/settings.hpp"
#include "parser/parser.hpp"

#define ADD_CONTRACT(X, Y, Z) \
X.addSymbol(boost::bimap<myFix::dataBase::recordId, std::string>::value_type(Y, Z));

void fileImport(const std::string & data_) {

	
	boost::timer t;										// timer

	myFix::parser parser(								// create the file parser
		myFix::settings::instance().dictionary());											

	// build symbols map
	ADD_CONTRACT(parser, 1 , "ESH4"     )
	ADD_CONTRACT(parser, 2 , "ESM4"     )
	ADD_CONTRACT(parser, 3 , "ESU4"     )
	ADD_CONTRACT(parser, 4 , "ESZ4"     )
	ADD_CONTRACT(parser, 5 , "ESH5"     )
	ADD_CONTRACT(parser, 6 , "ESH4-ESU4")
	ADD_CONTRACT(parser, 7 , "ESM4-ESU4")
	ADD_CONTRACT(parser, 8 , "ESU4-ESZ4")
	ADD_CONTRACT(parser, 9 , "ESU4-ESH5")
	ADD_CONTRACT(parser, 10, "ESZ4-ESH5")
	ADD_CONTRACT(parser, 11, "ESM4-ESH5")
	ADD_CONTRACT(parser, 12, "ESH4-ESM4")
	ADD_CONTRACT(parser, 13, "ESH4-ESH5")
	ADD_CONTRACT(parser, 14, "ESH4-ESZ4")
	ADD_CONTRACT(parser, 15, "ESM4-ESZ4")
	
	std::ifstream infile(data_);						// open data file

	// local variables 
	int									n_valid = 0	;	// valid record counter
	int                                 n_error = 0 ;	// errors
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

		try {

			// step 1: trying to convert current line
			std::vector<myFix::tradeMessage> msg		// parse the line
				= parser.parse_trade(line);

			// step 2: write each msg in the db		
			for (std::vector<myFix::tradeMessage>::const_iterator It
				= msg.cbegin(); It != msg.cend(); It++) {

				if (insertSingleTrade(*It) == true) {

					if (n_valid++ % 100 == 0) 			// info

						std::cout
							<< n_valid
							<< "records inserted"
							<< std::endl;

				}
				else {

					std::cout
						<< ++n_error
						<< "errors occured"
						<< std::endl;

				}
			}
		}
		
		catch (...) {									// todo : exception management & log
		
			std::cout << "big mess !!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
		
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