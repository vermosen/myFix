#ifndef recordset_functions_insert_single_bar_hpp
#define recordset_functions_insert_single_bar_hpp

#include <thOth/bar/bar.hpp>

#include "recordset/recordset.hpp"
#include "utilities/message/message.hpp"
#include "utilities/settings/settings.hpp"

bool insertSingleBar(const myFix::instrument & instrument_,
					 const thOth::dateTime & time_,
					 const thOth::bar & bar_) {

	std::string fieldStr, valueStr;

	try{

		fieldStr.append("INSTRUMENT_ID,");						// contract id
		SQL_INSERT_NUM(valueStr, instrument_.first)
			valueStr.append(",");

		fieldStr.append("BAR_DATETIME,");						// barStart
		SQL_INSERT_DATE(valueStr, time_, true)
			valueStr.append(",");

		fieldStr.append("BAR_OPEN,");						// open
		SQL_INSERT_NUM(valueStr, bar_.open())
			valueStr.append(",");

		fieldStr.append("BAR_CLOSE,");						// close
		SQL_INSERT_NUM(valueStr, bar_.close())
			valueStr.append(",");

		fieldStr.append("BAR_HIGH,");						// high
		SQL_INSERT_NUM(valueStr, bar_.high())
			valueStr.append(",");

		fieldStr.append("BAR_LOW,");						// low
		SQL_INSERT_NUM(valueStr, bar_.low())
			valueStr.append(",");

		fieldStr.append("BAR_VOLUME,");						// low
		SQL_INSERT_NUM(valueStr, bar_.volume())
			valueStr.append(",");

		fieldStr.append("BAR_LENGTH");						// low
		SQL_INSERT_NUM(valueStr, 1)							// id for 100 ms
			
		std::string insertStatement("INSERT INTO table_bar (");

		insertStatement
			.append(fieldStr)
			.append(") VALUES (")
			.append(valueStr)
			.append(")");

		if (mysql_query(myFix::settings::instance().connection(), insertStatement.c_str()) != 0)	// throw on an error
			throw std::exception(mysql_error(myFix::settings::instance().connection()));

	} catch (...){

		return false;

	}

	return true;

};

#endif