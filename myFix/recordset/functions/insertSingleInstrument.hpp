#ifndef recordset_functions_insert_single_instrument_hpp
#define recordset_functions_insert_single_instrument_hpp

#include "recordset/recordset.hpp"
#include "thOth/message.hpp"
#include "utilities/settings/settings.hpp"

bool insertSingleInstrument(const std::string & code) {

	std::string fieldStr, valueStr;

	try{

		fieldStr.append("INSTRUMENT_NAME");						// contract id
		SQL_INSERT_STR(valueStr, code)

		std::string insertStatement("INSERT INTO table_instrument (");

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