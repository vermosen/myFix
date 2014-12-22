#ifndef recordset_functions_insert_single_trade_hpp
#define recordset_functions_insert_single_trade_hpp

#include <thOth/message/tradeMessage.hpp>

#include "recordset/recordset.hpp"
#include "utilities/settings/settings.hpp"

bool insertSingleTrade(const thOth::tradeMessage & msg) {

	std::string fieldStr, valueStr;

	try{
	
		fieldStr.append("INSTRUMENT_ID,");						// contract id
		SQL_INSERT_NUM(valueStr, msg.symbol().first)
			valueStr.append(",");

		fieldStr.append("TRADE_DATETIME,");						// barStart
		SQL_INSERT_DATE(valueStr, msg.time(), true)
			valueStr.append(",");

		fieldStr.append("TRADE_PRICE,");						// open
		SQL_INSERT_NUM(valueStr, msg.price())
			valueStr.append(",");

		fieldStr.append("TRADE_VOLUME");						// close
		SQL_INSERT_NUM(valueStr, msg.quantity())

		std::string insertStatement("INSERT INTO table_trade (");

		insertStatement
			.append(fieldStr)
			.append(") VALUES (")
			.append(valueStr)
			.append(")");

		if (mysql_query(myFix::settings::instance().connection(), insertStatement.c_str()) != 0)	// throw on an error
			throw std::exception(mysql_error(myFix::settings::instance().connection()));
	
	}
	catch (...){
	
		return false;
	
	}

	return true;

};

#endif