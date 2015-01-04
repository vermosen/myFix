#ifndef recordset_functions_request_bulk_trade_hpp
#define recordset_functions_request_bulk_trade_hpp

#include <thOth/message.hpp>

#include "recordset/recordset.hpp"
#include "utilities/settings/settings.hpp"

// request all the trades for the given instrument between two given dates
std::vector<thOth::tradeMessage> requestBulkTrade(const thOth::instrument & instrument_,
												  const thOth::dateTime & startDate_,
												  const thOth::dateTime & endDate_,
												  bool ordered_ = false) {

	std::string selectStr;
	std::vector<thOth::tradeMessage> ret;

	try{

		// build select statement
		selectStr.append("SELECT * FROM TABLE_TRADE WHERE (INSTRUMENT_ID = ");
		SQL_INSERT_NUM(selectStr, instrument_.first)
			selectStr.append(" AND TRADE_DATETIME >=");
		SQL_INSERT_DATE(selectStr, startDate_, true)
			selectStr.append(" AND TRADE_DATETIME <=");
		SQL_INSERT_DATE(selectStr, endDate_, true)

			if (ordered_ == true) {

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

		MYSQL_ROW		row_		;								// SQL row												
		thOth::dateTime time_		;								// temp data
		double			price_		;
		int				volume_		;

		while (row_ = mysql_fetch_row(reception_)) {					// loop over the results

			for (unsigned long i = 0; i < reception_->field_count; i++) {

				// drops the bar Id
				// drops the instrument id (already recorded)
				if (std::string(reception_->fields[i].name)
					== "TRADE_DATETIME" && row_[i] != NULL)
					time_ = thOth::dateTime::convertSqlDateTime(std::string(row_[i]));

				else if (std::string(reception_->fields[i].name)
					== "TRADE_PRICE" && row_[i] != NULL)
					price_ = boost::lexical_cast<double>(row_[i]);

				else if (std::string(reception_->fields[i].name)
					== "TRADE_VOLUME" && row_[i] != NULL)
					volume_ = boost::lexical_cast<int>(row_[i]);

			}

			ret.push_back(thOth::tradeMessage(							// push back
				instrument_,
				time_,
				price_,
				volume_));

		}
	}
	catch (...){

		//return false;

	}

	return ret;

};

#endif

