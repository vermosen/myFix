#ifndef recordset_functions_bulk_insert_trade_hpp
#define recordset_functions_bulk_insert_trade_hpp

#include "recordset/recordset.hpp"
#include "utilities/message/message.hpp"
#include "utilities/settings/settings.hpp"

bool insertBulkTrade(const std::vector<myFix::tradeMessage> & msgs) {

	std::string fieldStr, valueStr;

	try{

		fieldStr.append("INSTRUMENT_ID,");						// contract id
		fieldStr.append("TRADE_DATETIME,");						// barStart
		fieldStr.append("TRADE_PRICE,");						// open
		fieldStr.append("TRADE_VOLUME,");						// close
		
		fieldStr.pop_back();									// remove the last column

		std::string insertStatement("INSERT INTO table_trade (");
		insertStatement.append(fieldStr).append(") VALUES ");

		for (std::vector<myFix::tradeMessage>::const_iterator It = msgs.cbegin(); It != msgs.cend(); It++) {
	
			valueStr.append("(");

			SQL_INSERT_NUM(valueStr, It->symbol_.first)
				valueStr.append(",");
			SQL_INSERT_DATE(valueStr, It->time_, true)
				valueStr.append(",");
			SQL_INSERT_NUM(valueStr, It->price_)
				valueStr.append(",");
			SQL_INSERT_NUM(valueStr, It->quantity_)

			valueStr.append("),");

		}

		valueStr.pop_back();						// suppress the last column
		
		insertStatement.append(valueStr);			// add the value fields to insert statement 
			
		if (mysql_query(myFix::settings::instance().connection(), insertStatement.c_str()) != 0)	// throw on an error
			throw std::exception(mysql_error(myFix::settings::instance().connection()));

	}
	catch (...){

		return false;

	}

	return true;

};

#endif