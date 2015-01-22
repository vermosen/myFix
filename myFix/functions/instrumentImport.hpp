#ifndef functions_instrument_import_hpp
#define functions_instrument_import_hpp

#include <fstream>

#include <boost/timer.hpp>

#include "recordset/tableInstrumentRecordset/tableInstrumentRecordset.hpp"
#include "utilities/settings/settings.hpp"
#include "parser/parsers/tradeParser.hpp"

// import the instruments defined in the given file
void instrumentImport(const std::string & data_) {

	boost::timer t				;						// timer
	std::ifstream infile(data_)	;						// open data file
	long nline = 1				;						// line counter
	std::string line("")		;						// current line

	myFix::dataBase::tableInstrumentRecordset rs(		// table recordset
		myFix::settings::instance().connection());

	myFix::tradeParser ps(								// create the file parser
		myFix::settings::instance().dictionary());

	ps.loadInstrumentTable(rs);							// load the instrument table from the rs

	while (std::getline(infile, line)) {				// get through the lines

		std::istringstream iss(line);					// istringstream

		if (!(iss >> line)) { break; }					// can't read the line

		if (nline++ % 1000 == 0)						// message every 1000 line
			std::cout
			<< "read "
			<< nline - 1
			<< " lines."
			<< std::endl;

		// keep going into the line until there are no new instrument
		bool past = false; while (past != true) {
		
			try {

				ps.parse(line);								// try to parse the current line

			}

			// on error, insert the new instrumentin the db
			catch (myFix::undefinedInstrumentException & e){

				if (rs.insert(e.code())) {

					ps.loadInstrumentTable(rs);				// update the parser instruments list
															// TODO: notify the parser

					std::cout								
						<< "successfully inserted new contract "
						<< e.code()
						<< " in the database."
						<< std::endl;

				}

			} catch (...) {									// other error, rethrow

				std::cout << "unexpected error, aborting procedure..." << std::endl;
				throw std::exception();

			}
		
		}

		line.clear();										// clears temporary line

	}

	std::cout
		<< "file read in "
		<< t.elapsed()
		<< " seconds"
		<< std::endl;

}

#endif