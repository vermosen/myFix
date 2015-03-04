#ifndef simple_trading_rule_hpp
#define simple_trading_rule_hpp

#include <fstream>

#include <boost/timer.hpp>

#include <thOth/time/TimeSeries.hpp>
#include <thOth/time/DateTime.hpp>

#include "recordset/tableTradeRecordset/tableTradeRecordset.hpp"
#include "recordset/functions/insertSingleTrade.hpp"
#include "utilities/settings/settings.hpp"

// test an insert on the database
void simpleTradingRule() {

	boost::timer t;										// timer

	try {

		thOth::instrument	inst		(2, "ESM3"	);
		thOth::dateTime		startDate	(2014, 3, 2	);
		thOth::dateTime		endDate		(2014, 3, 2	);

		// step 1: fetch the data
		myFix::dataBase::tableTradeRecordset rs(					// a tableTradeRecordset
			myFix::settings::instance().connection());

		if (rs.select(inst, startDate, endDate) == true) {
		
			for (myFix::dataBase::tableTradeRecordset::const_iterator It = rs.cbegin(); It != rs.cend(); It++) {

				std::cout
					<< "trade time: "
					<< It->first
					<< "trade price: "
					<< It->second.price()
					<< ", trade volume: "
					<< It->second.quantity()
					<< std::endl;
				
			}
		}
	}
	catch (std::exception & e) {

		std::cout << "an exception occured: "
			<< std::endl
			<< e.what()
			<< std::endl;

	}
	catch (...) {

		std::cout << "unknown exception occured" << std::endl;

	}

	std::cout
		<< "test ended in "
		<< t.elapsed()
		<< " seconds"
		<< std::endl;

}

#endif