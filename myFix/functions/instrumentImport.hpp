#ifndef functions_instrument_import_hpp
#define functions_instrument_import_hpp

#include <fstream>

#include <boost/timer.hpp>

#include "recordset/functions/insertSingleInstrument.hpp"
#include "utilities/settings/settings.hpp"
//#include "parser/parsers/tradeParser.hpp"

// import instruments defined by the file
void instrumentImport(const std::string & data_) {

	boost::timer t;										// timer

	std::ifstream infile(data_);						// open data file
	long nline = 1;										// line counter
	std::string line("");								// current line
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

			// try to convert the current line
			std::vector<myFix::tradeMessage> tradeMsg = ps.parse(line);

		}
		catch (myFix::undefinedInstrumentException & e){

			// new instrument detected -> insert in the db
			if (insertSingleInstrument(e.code())) {
			
				// TODO: if success, insert into the parser
				std::cout
					<< "successfully inserted new contract "
					<< e.code()
					<< std::endl;

			}

		}
		catch (...) {									// other error

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