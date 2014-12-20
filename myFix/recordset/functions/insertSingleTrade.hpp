#ifndef recordset_functions_insert_single_trade_hpp
#define recordset_functions_insert_single_trade_hpp

#include "recordset/recordset.hpp"
#include "message/message.hpp"
#include "utilities/settings/settings.hpp"

bool insertSingleTrade(const myFix::tradeMessage & msg) {

	std::string fieldStr, valueStr;

	try{
	
		fieldStr.append("INSTRUMENT_ID,");						// contract id
		SQL_INSERT_NUM(valueStr, msg.symbol_.first)
			valueStr.append(",");

		fieldStr.append("TRADE_DATETIME,");						// barStart
		SQL_INSERT_DATE(valueStr, msg.time_, true)
			valueStr.append(",");

		fieldStr.append("TRADE_PRICE,");						// open
		SQL_INSERT_NUM(valueStr, msg.price_)
			valueStr.append(",");

		fieldStr.append("TRADE_VOLUME");						// close
		SQL_INSERT_NUM(valueStr, msg.quantity_)

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