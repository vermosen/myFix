#ifndef recordset_functions_bulk_insert_bar_hpp
#define recordset_functions_bulk_insert_bar_hpp

#include <thOth/bar/bar.hpp>

#include "recordset/recordset.hpp"
#include "utilities/message/message.hpp"
#include "utilities/settings/settings.hpp"

bool insertBulkBar(const myFix::instrument & instrument_,
				   const std::vector<std::pair<thOth::dateTime, thOth::bar> > & msgs) {

	std::string fieldStr, valueStr;

	try{

		fieldStr.append("INSTRUMENT_ID,");						// contract id
		fieldStr.append("BAR_DATETIME,");						// barStart
		fieldStr.append("BAR_OPEN,");							// open
		fieldStr.append("BAR_CLOSE,");							// close
		fieldStr.append("BAR_HIGH,");							// high
		fieldStr.append("BAR_LOW,");							// low
		fieldStr.append("BAR_VOLUME,");							// volume
		fieldStr.append("BAR_LENGTH");							// low

		std::string insertStatement("INSERT INTO table_bar (");
		insertStatement.append(fieldStr).append(") VALUES ");

		for (std::vector<std::pair<thOth::dateTime, thOth::bar> >::const_iterator It 
			= msgs.cbegin(); It != msgs.cend(); It++) {

			valueStr.append("(");

			SQL_INSERT_NUM(valueStr, instrument_.first)
				valueStr.append(",");
			SQL_INSERT_DATE(valueStr, It->first, true)
				valueStr.append(",");
			SQL_INSERT_NUM(valueStr, It->second.open())
				valueStr.append(",");
			SQL_INSERT_NUM(valueStr, It->second.close())
				valueStr.append(",");
			SQL_INSERT_NUM(valueStr, It->second.high())
				valueStr.append(",");
			SQL_INSERT_NUM(valueStr, It->second.low())
				valueStr.append(",");
			SQL_INSERT_NUM(valueStr, It->second.volume())
				valueStr.append(",");
			SQL_INSERT_NUM(valueStr, 1)				// id for 100 ms

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