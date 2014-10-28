#ifndef recordset_conversion_hpp
#define recordset_conversion_hpp

#include <thOth/time/DateTime.hpp>

namespace myFix {

	thOth::dateTime convertDateTime_sql(const std::string & dtStr) {

		return thOth::dateTime(								// smallest increments is second
			thOth::dateTime::years  (boost::lexical_cast<int>(dtStr.substr(0 , 4))),
			thOth::dateTime::months (boost::lexical_cast<int>(dtStr.substr(5 , 2))),
			thOth::dateTime::days   (boost::lexical_cast<int>(dtStr.substr(8 , 2))),
			thOth::dateTime::hours  (boost::lexical_cast<int>(dtStr.substr(11, 2))),
			thOth::dateTime::minutes(boost::lexical_cast<int>(dtStr.substr(14, 2))),
			thOth::dateTime::seconds(boost::lexical_cast<int>(dtStr.substr(17, 2))));

	}

	std::string convertDateTime_sql(const thOth::dateTime & date) {

		std::string temp = boost::posix_time::to_iso_string(date);

		std::string sql(""); sql
			.append(temp.substr(0, 8))
			.append(temp.substr(9, 6));

		return sql;

	}
}
#endif