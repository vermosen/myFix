#ifndef functions_debug_hpp
#define functions_debug_hpp

#include <fstream>

#include <boost/timer.hpp>
#include <boost/bimap.hpp>

#include "recordset/functions/insertSingleTrade.hpp"
#include "recordset/functions/insertBulkTrade.hpp"
#include "utilities/settings/settings.hpp"
#include "parser/parsers/tradeParser.hpp"

#define BUFFER_SIZE 5000

// feed a bar table from a dataset
// we want to create a function which turns 
// {id time price volume}s into a bar, ie a struct 
// {id time, open, close, high, low, volume, duration}
// in our case, we need to have 1ms bars
void debug(const std::string & file_) {

	boost::timer t;										// timer

	std::ifstream infile(file_);						// open data file

	// local variables
	int									n_valid = 0;	// valid record counter
	int                                 n_error = 0;	// errors
	std::vector<myFix::tradeMessage>	tradeMsg;
	long								nline = 1;		// line counter
	std::string							line("");		// current line

	myFix::tradeParser ps(								// create the file parser
		myFix::settings::instance().dictionary());

	ps.loadInstrumentTable();							// load the instrument table

	// the message buffer
	std::vector<std::shared_ptr<myFix::tradeMessage> > buffer;			

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

			// step 1: parse the current line
			std::vector<std::shared_ptr<myFix::message> > msg = ps.parse(line);

			// step 2: fill the buffer
			for (std::vector<std::shared_ptr<myFix::message> >::const_iterator It
				= msg.cbegin(); It != msg.cend(); It++, n_valid++)

				buffer.push_back(It->get()->accept(It->get()));

			// is the buffer full ?
			if (buffer.size() >= BUFFER_SIZE) {

				if (insertBulkTrade(buffer)) {

					std::cout
						<< n_valid
						<< " records inserted"
						<< std::endl;

				}
				else {									// if unsuccessful, try to insert line by line

					for (std::vector<myFix::tradeMessage>::const_iterator It
						= buffer.cbegin(); It != buffer.cend(); It++) {

						if (insertSingleTrade(*It) == true) {

							if (n_valid++ % 100 == 0) 		// info

								std::cout
								<< n_valid
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