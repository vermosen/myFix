#ifndef functions_bar_import__old_hpp
#define functions_bar_import__old_hpp

#include <fstream>

#include <boost/timer.hpp>

#include "recordset/functions/insertSingleBar.hpp"
#include "utilities/settings/settings.hpp"
#include "parser/parser.hpp"

// feed a bar table from a dataset
// we want to create a function which turns 
// {id time price volume}s into a bar, ie a struct 
// {id time, open, close, high, low, volume, duration}
// in our case, we need to have 1ms bars
void barImport(const std::string & file_, 
			   const myFix::instrument & inst_,
			   const myFix::parser & ps) {

	boost::timer t;										// timer

	std::ifstream infile(file_);						// open data file

	// local variables 
	int									n_valid = 0	;	// valid record counter
	int                                 n_error = 0	;	// errors
	std::vector<myFix::tradeMessage>	tradeMsg	;
	long								nline = 1	;	// line counter
	std::string							line("")	;	// current line

	// strategy:
	// 1 - create a dummy bar with date = 1/1/1901
	// 2 - get a line
	// 3 - convert into a vector of messages
	// 4 - if date == bar.date, pile up
	// 5 - else insert the bar and replace by the new data
	// 6 - loop 
	// 7 - finally, remove the first element 
	const thOth::period p(thOth::timeUnit::milliSecond, 100);	
														// bar frequency
	
	std::pair<thOth::dateTime, thOth::bar> current(		// dummy bar
		thOth::dateTime(1901, 1, 1), thOth::bar(
			0.0, 0.0, 0.0, 0.0, p, 0));

	// end of bar
	thOth::dateTime eob = thOth::dateTime::advance(current.first, p);

	// stack of bars
	std::vector<std::pair<thOth::dateTime, thOth::bar> > bars;
	
	// get through the lines
	while (std::getline(infile, line)) {

		std::istringstream iss(line);					// istringstream

		if (!(iss >> line)) { break; }					// can't read the line

		if (nline++ % 1000 == 0)						// message every 1000 line
			std::cout
			<< "read "
			<< nline - 1
			<< " lines."
			<< std::endl;

		try {

			std::vector<myFix::tradeMessage> msg		// convert into a message stream
				= ps.parse_trade(line);

			if (msg.empty()) continue;					// if message is empty

			if (msg.cbegin()->time_ < eob){	// if current bar, pile up

				// loop through messages
				for (std::vector<myFix::tradeMessage>::const_iterator It =
					msg.cbegin(); It != msg.cend(); It++) {

					current.second.close	(It->price_								);
					current.second.high		(max(current.second.high(), It->price_)	);
					current.second.low		(min(current.second.low(), It->price_ )	);
					current.second.volume	(current.second.volume() + It->quantity_);

				}
			} 
			else {

				bars.push_back(current);				// otherwise, pile up the old bar 

				// for the bar time, we need to ensure that
				// a multiple of the period selected
				current.first = thOth::dateTime::adjust100ms(msg.cbegin()->time_);
				
				// replace the bar fundamentals
				current.second.open		(msg.cbegin()->price_	);
				current.second.low		(msg.cbegin()->price_	);
				current.second.high		(msg.cbegin()->price_	);
				current.second.volume	(0						);
				eob = thOth::dateTime::advance(current.first, p	);

				// and pile up the current message
				for (std::vector<myFix::tradeMessage>::const_iterator It =
					msg.cbegin(); It != msg.cend(); It++) {

					current.second.close	(It->price_									);
					current.second.high		(max(current.second.high(), It->price_	)	);
					current.second.low		(min(current.second.low(), It->price_	)	);
					current.second.volume	(current.second.volume() + It->quantity_		);

				}
			}
		}
		catch (...) {									// todo : exception management & log

			std::cout 
				<< "big mess on line" 
				<< nline
				<< std::endl;

		}
	}

	if (!bars.empty()) {
	
		bars.erase(bars.begin());
		// TODO: bulk insert
		//insertBulkBar(bars);

	}

	

	std::cout
		<< "file read in "
		<< t.elapsed()
		<< " seconds"
		<< std::endl;

}

#endif