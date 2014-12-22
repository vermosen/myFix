#ifndef recordset_functions_request_bulk_trade_hpp
#define recordset_functions_request_bulk_trade_hpp

#include "recordset/recordset.hpp"
#include "utilities/message/message.hpp"
#include "utilities/settings/settings.hpp"

// request all the trades for the given instrument between two given dates
std::vector<myFix::tradeMessage> requestBulkTrade(const myFix::instrument & instrument,
												  const thOth::dateTime & startDate_,
												  const thOth::dateTime & endDate_,
												  bool ordered = false) {

	std::string selectStr;
	std::vector<myFix::tradeMessage> ret;

	try{

		// build select statement
		selectStr.append("SELECT * FROM TABLE_TRADE WHERE (INSTRUMENT_ID = ");
		SQL_INSERT_NUM(selectStr, instrument.first)
			selectStr.append(" AND TRADE_DATETIME >=");
		SQL_INSERT_DATE(selectStr, startDate_, true)
			selectStr.append(" AND TRADE_DATETIME <=");
		SQL_INSERT_DATE(selectStr, endDate_, true)

			if (ordered == true) {

			selectStr.append(") ORDER BY TRADE_DATETIME");

			} else{
		
				selectStr.append(")");
			
			}

		

		// TODO: exception management
		if (mysql_query(myFix::settings::instance().connection(), selectStr.c_str()) != 0)	// throw on an error
			throw std::exception(mysql_error(myFix::settings::instance().connection()));

		mysql_query(												// query to run
			myFix::settings::instance().connection(),
			selectStr.c_str());

		MYSQL_RES * reception_ = mysql_store_result(				// store the results
			myFix::settings::instance().connection());				

		if (!reception_)											// sql statement failed
			return ret;

		if (reception_->row_count == 0)								// no record
			return ret;

		MYSQL_ROW row;												// otherwise, run...

		myFix::tradeMessage mess;
		mess.symbol_ = instrument;

		while (row = mysql_fetch_row(reception_)) {					// loop over the results

			for (unsigned long i = 0; i < reception_->field_count; i++) {

				// drops the bar Id
				// drops the instrument id (already recorded)
				if (std::string(reception_->fields[i].name)
					== "TRADE_DATETIME" && row[i] != NULL)
					mess.time_ = thOth::dateTime::convertSqlDateTime(std::string(row[i]));

				else if (std::string(reception_->fields[i].name)
					== "TRADE_PRICE" && row[i] != NULL)
					mess.price_ = boost::lexical_cast<double>(row[i]);

				else if (std::string(reception_->fields[i].name)
					== "TRADE_VOLUME" && row[i] != NULL)
					mess.quantity_ = boost::lexical_cast<int>(row[i]);

			}

			// push back
			ret.push_back(mess);

		}
	}
	catch (...){

		//return false;

	}

	return ret;

};

#endif

