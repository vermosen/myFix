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

#include "recordset/functions/insertSingleTrade.hpp"
#include "recordset/functions/insertBulkTrade.hpp"
#include "utilities/settings/settings.hpp"
#include "parser/parsers/tradeParser.hpp"

#define BUFFER_SIZE 5000

void tradeImport(const std::string & data_) {

	boost::timer t;										// timer

	std::ifstream infile(data_);						// open data file

	// local variables
	int									n_valid = 0	;	// valid record counter
	int                                 n_error = 0	;	// errors
	std::vector<thOth::tradeMessage>	tradeMsg	;
	long								nline = 1	;	// line counter
	std::string							line("")	;	// current line

	myFix::tradeParser ps(								// create the file parser
		myFix::settings::instance().dictionary());

	ps.loadInstrumentTable();							// load the instruments from the db

	std::vector<thOth::tradeMessage> buffer;			// the message buffer

	buffer.reserve(BUFFER_SIZE);						// reserve the size

	while (std::getline(infile, line)) {				// get through the lines

		std::istringstream iss(line);					// istringstream

		if (!(iss >> line)) { break; }					// can't read the line

		if (nline++ % 10000 == 0)						// message every 1000 line
			std::cout
			<< "read "
			<< nline - 1
			<< " lines."
			<< std::endl;

		try {
			
			ps.parse(line);								// tries to parse the current line

			// is the buffer full ?
			if (ps.size() >= BUFFER_SIZE) {
			
				if (insertBulkTrade(buffer)) {

					std::cout
						<< n_valid
						<< " records inserted"
						<< std::endl;

				} else {									// if unsuccessful, try to insert line by line

					for (std::vector<thOth::tradeMessage>::const_iterator It
						= buffer.cbegin(); It != buffer.cend(); It++) {

						if (insertSingleTrade(*It) == true) {

							if (n_valid++ % 100 == 0) 		// info

								std::cout
								<< n_valid
								<< " records inserted"
								<< std::endl;

						} else {

							std::cout
								<< ++n_error
								<< " errors occured"
								<< std::endl;

						}
					}
				}

				buffer.clear();

			}
		}
		catch (myFix::undefinedInstrumentException & e) {	// missing instrument
		
			std::cout
				<< "missing instrument code: "
				<< e.code()
				<< std::endl;
		
		}
		catch (std::exception & e){
		
			std::cout
				<< "error: "
				<< e.what()
				<< std::endl;
		
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