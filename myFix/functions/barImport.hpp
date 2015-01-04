#ifndef functions_bar_import_hpp
#define functions_bar_import_hpp

#include <fstream>
//#include <algorithm>

#include <boost/timer.hpp>

#include "recordset/functions/requestBulkTrade.hpp"
#include "recordset/functions/insertBulkBar.hpp"
#include "utilities/settings/settings.hpp"

// feed a bar table from a dataset
// we want to create a function which turns 
// {id time price volume}s into a bar, ie a struct 
// {id time, open, close, high, low, volume, duration}
// in our case, we need to have 1ms bars
void barImport(const thOth::instrument & inst_	,
			   thOth::dateTime startDate_		,
			   const thOth::dateTime & endDate_	,
			   const thOth::period & period_	) {

	boost::timer t;										// timer

	// strategy:
	// 1 - bulk request over the trade table of 10 minutes increments
	// 2 - parse in 100 ms 
	// 3 - bulk insert
	const thOth::period chunkLen(						// chunk of data  
		thOth::timeUnit::hour, 1);	

	std::vector<std::pair<thOth::dateTime, thOth::bar> > bars;

	while (startDate_ < endDate_) {
	
		std::vector<thOth::tradeMessage> trd =			// 1 - bulk request
			requestBulkTrade(
				inst_, 
				startDate_, 
				thOth::dateTime::advance(startDate_, chunkLen),
				true);

		if (!trd.empty()) {
		
			thOth::dateTime barEnd = thOth::dateTime::advance(startDate_, period_);

			// create the current bar (don't fill the volume)
			std::pair<thOth::dateTime, thOth::bar> currentBar(
				startDate_, thOth::bar(
				trd.cbegin()->price(), 
				trd.cbegin()->price(), 
				trd.cbegin()->price(), 
				trd.cbegin()->price(), 
				period_, 0));

			for (std::vector<thOth::tradeMessage>::const_iterator It = trd.cbegin(); It < trd.cend(); It++) {

				if (It->time() > barEnd) {					// is the trade past the range ?

					if (currentBar.second.volume() != 0)	// no insert if empty
						bars.push_back(currentBar);			// insert

					// we can't ensure that there is a trade 
					// for the current interval hence, we need
					//to rely on the trade time to create the bar
					currentBar.first = thOth::dateTime::adjust100ms(It->time());

					// reset values
					currentBar.second.open	(It->price		());
					currentBar.second.low	(It->price		());
					currentBar.second.high	(It->price		());
					currentBar.second.volume(It->quantity	());

					// in case there is only 1 trade
					currentBar.second.close	(It->price		());

					barEnd = thOth::dateTime::advance(currentBar.first, period_);

				}
				else {										// we are still in the range

					// stack up
					currentBar.second.close	(It->price());
					currentBar.second.high	(max(currentBar.second.high(), It->price()));
					currentBar.second.low	(min(currentBar.second.low(), It->price()));
					currentBar.second.volume(currentBar.second.volume() + It->quantity());

				}
			}	
		}

		if (!bars.empty() && insertBulkBar(inst_, bars)) {	// insert in the database
		
			std::cout										//message
				<< "inserted "
				<< bars.size()
				<< " bars in the database "
				<< " starting at "
				<< startDate_
				<< std::endl;

			bars.clear();									// clears the bars
		
		}

		// move to the next chunk
		startDate_ = thOth::dateTime::advance(startDate_, chunkLen);		

	}

	std::cout
		<< "file read in "
		<< t.elapsed()
		<< " seconds"
		<< std::endl;

}

#endif