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

#include "parser/parsers/tradeParser.hpp"
#include "recordset/tableTradeRecordset/tableTradeRecordset.hpp"
#include "utilities/settings/settings.hpp"

#define TRADE_BUFFER_SIZE 5000

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
		myFix::settings::instance().dictionary())	;

	// instrument rs
	myFix::dataBase::tableInstrumentRecordset instrumentRs(	
		myFix::settings::instance().connection());

	myFix::dataBase::tableTradeRecordset rs(
		myFix::settings::instance().connection());

	ps.loadInstrumentTable(instrumentRs);				// load the instruments from the db

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

			if (ps.size() >= TRADE_BUFFER_SIZE) {		// is the buffer full ?
			
				if (rs.insert(ps.messages())) {			// bulk insert successfull ?

					std::cout
						<< n_valid
						<< " records inserted"
						<< std::endl;

				}

				ps.clear();								// clear the buffer

			}
		} catch (myFix::undefinedInstrumentException & e) {
		
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

			std::cout << "unexpected error, aborting procedure..." << std::endl;
			throw std::exception();

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