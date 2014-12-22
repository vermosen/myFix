#ifndef recordset_functions_bulk_insert_trade_hpp
#define recordset_functions_bulk_insert_trade_hpp

#include <thOth/message/tradeMessage.hpp>

#include "recordset/recordset.hpp"
#include "utilities/settings/settings.hpp"

bool insertBulkTrade(const std::vector<thOth::tradeMessage> & msgs) {

	std::string fieldStr, valueStr;

	try{

		fieldStr.append("INSTRUMENT_ID," );						// contract id
		fieldStr.append("TRADE_DATETIME,");						// barStart
		fieldStr.append("TRADE_PRICE,"   );						// open
		fieldStr.append("TRADE_VOLUME,"  );						// close
		
		fieldStr.pop_back();									// remove the last column

		std::string insertStatement("INSERT INTO table_trade (");
		insertStatement.append(fieldStr).append(") VALUES ");

		for (std::vector<thOth::tradeMessage>::const_iterator It = 
			msgs.cbegin(); It != msgs.cend(); It++) {
	
			valueStr.append("(");

			SQL_INSERT_NUM(valueStr, It->symbol().first)
				valueStr.append(",");
			SQL_INSERT_DATE(valueStr, It->time(), true)
				valueStr.append(",");
			SQL_INSERT_NUM(valueStr, It->price())
				valueStr.append(",");
			SQL_INSERT_NUM(valueStr, It->quantity())

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