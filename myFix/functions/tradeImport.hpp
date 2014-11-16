#ifndef functions_trade_import_hpp
#define functions_trade_import_hpp

// FAST file import function
// field 1128 with value 9 means FIX50SP2
// field 35 with value X means MarketDataIncrementalRefresh message
// structure:
// 
// Component Block - <MDIncGrp>	Y	Number of entries following.
// 75	TradeDate

#include <fstream>

#include <boost/timer.hpp>

#include "recordset/functions/insertSingleTrade.hpp"
#include "recordset/functions/insertBulkTrade.hpp"
#include "utilities/settings/settings.hpp"
#include "parser/parser.hpp"

void tradeImport(const std::string & data_, const myFix::parser & ps) {

	boost::timer t;										// timer

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
				= ps.parse_trade(line);

			// step 2: try a bulk insert in the db	
			if (insertBulkTrade(msg) == true) {

				if (n_valid++ % 100 == 0) 				// info

					std::cout
					<< n_valid - 1
					<< " records inserted"
					<< std::endl;

			}
			else {										// if unsuccessful, try to insert line by line

				for (std::vector<myFix::tradeMessage>::const_iterator It
					= msg.cbegin(); It != msg.cend(); It++) {

					if (insertSingleTrade(*It) == true) {

						if (n_valid++ % 100 == 0) 		// info

							std::cout
							<< n_valid - 1
							<< " records inserted"
							<< std::endl;

					}
					else {

						std::cout
							<< ++n_error
							<< " errors occured"
							<< std::endl;

					}
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