#ifndef functions_debug_hpp
#define functions_debug_hpp

#include <fstream>

#include <boost/timer.hpp>

#include <thOth/time/DateTime.hpp>

#include "utilities/settings/settings.hpp"
#include "parser/parser.hpp"

// test an insert on the database
void debug(const std::string & data_) {

	boost::timer t;										// timer

	try {

		// vector to insert
		std::vector<myFix::tradeMessage> messages;

		{

			// build a dateTime
			std::string dtStr("20021112000000123");
			std::stringstream ss1;
			boost::posix_time::time_input_facet * facet1		// create the facet
				= new boost::posix_time::time_input_facet("%Y%m%d%H%M%S");
			ss1.imbue(std::locale(std::locale(), facet1));

			thOth::dateTime tradeDate = thOth::dateTime::strToDate(dtStr, ss1)
				+ thOth::dateTime::milliSeconds(boost::lexical_cast<int>(dtStr.substr(15, 3)));


			myFix::tradeMessage t;
			t.price_ = 100.13;
			t.quantity_ = 1234;
			t.symbol_ = std::pair<myFix::dataBase::recordId, std::string>(1, "FAKE");
			t.time_ = tradeDate;

			messages.push_back(t);
		
		}
		
		// insert tradeMessage into mysql database
		std::string fieldStr, valueStr;								// the two fields to build together

		// TODO: need to iterate over the ts
		for (std::vector<myFix::tradeMessage>::const_iterator It 
			= messages.cbegin(); It != messages.cend(); It++) {

			fieldStr.append("INSTRUMENT_ID,");						// contract id
			SQL_INSERT_NUM(valueStr, It->symbol_.first)
				valueStr.append(",");

			fieldStr.append("TRADE_DATETIME,");						// barStart
			SQL_INSERT_DATE(valueStr, It->time_)
				valueStr.append(",");

			fieldStr.append("TRADE_PRICE,");						// open
			SQL_INSERT_NUM(valueStr, It->price_)
				valueStr.append(",");

			fieldStr.append("TRADE_VOLUME,");						// close
			SQL_INSERT_NUM(valueStr, It->quantity_)
				valueStr.append(",");

				std::string insertStatement("INSERT INTO table_historical_bar (");

			insertStatement
				.append(fieldStr)
				.append(") VALUES (")
				.append(valueStr)
				.append(")");

			if (mysql_query(myFix::settings::instance().connection(), insertStatement.c_str()) != 0)	// throw on an error
				throw std::exception(mysql_error(myFix::settings::instance().connection()));

			fieldStr.clear(), valueStr.clear();

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
		<< "file read in "
		<< t.elapsed()
		<< " seconds"
		<< std::endl;

	system("pause");

}

#endif