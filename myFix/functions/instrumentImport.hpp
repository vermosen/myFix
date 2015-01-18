#ifndef functions_instrument_import_hpp
#define functions_instrument_import_hpp

#include <fstream>

#include <boost/timer.hpp>

#include "recordset/functions/insertSingleInstrument.hpp"
#include "utilities/settings/settings.hpp"
#include "parser/parsers/tradeParser.hpp"

// import the instruments defined in the given file
void instrumentImport(const std::string & data_) {

	boost::timer t				;						// timer
	std::ifstream infile(data_)	;						// open data file
	long nline = 1				;						// line counter
	std::string line("")		;						// current line

	myFix::tradeParser ps(								// create the file parser
		myFix::settings::instance().dictionary());

	ps.loadInstrumentTable();							// load the instrument table

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

			ps.parse(line);								// try to parse the current line

		}
		catch (myFix::undefinedInstrumentException & e){

			if (insertSingleInstrument(e.code())) {		// new instrument detected -> insert in the db
			
				std::cout								// TODO: if success, insert into the parser
					<< "successfully inserted new contract "
					<< e.code()
					<< std::endl;

			}

		}
		catch (...) {									// other error

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