#include "recordset/functions/requestBulkInstrument.hpp"

namespace myFix {

	namespace dataBase {

		// request all the instruents in the database
		std::vector<thOth::instrument> requestBulkInstrument() {

			std::string selectStr;
			std::vector<thOth::instrument> ret;

			try{

				// create the recordset
				tableInstrumentRecordset rs(settings::instance().connection());

				rs.selectStr("SELECT * FROM TABLE_INSTRUMENT");

				for (auto It = rs.cbegin(); It != rs.cend(); It++)
				
					// iterate over the rs map
					ret.push_back(*It);
				
			}
			catch (...) {}													// return false

			return ret;

		};

		//// request all the trades for the given instrument between two given dates
		//std::vector<thOth::instrument> requestBulkInstrument() {

		//	std::string selectStr;
		//	std::vector<thOth::instrument> ret;

		//	try{

		//		// select
		//		selectStr.append("SELECT INSTRUMENT_ID, INSTRUMENT_NAME FROM TABLE_INSTRUMENT");


		//		if (mysql_query(settings::instance().connection(), selectStr.c_str()) != 0)
		//			throw std::exception(									// throw on exceptions
		//			mysql_error(settings::instance().connection()));

		//		mysql_query(												// query to run
		//			settings::instance().connection(),
		//			selectStr.c_str());

		//		MYSQL_RES * reception_ = mysql_store_result(				// store the results
		//			settings::instance().connection());

		//		if (!reception_)											// sql statement failed
		//			return ret;

		//		if (reception_->row_count == 0)								// no record
		//			return ret;

		//		MYSQL_ROW row;												// otherwise, run...

		//		thOth::instrument inst;

		//		while (row = mysql_fetch_row(reception_)) {					// loop over the results

		//			for (unsigned long i = 0; i < reception_->field_count; i++) {

		//				if (std::string(reception_->fields[i].name)			// drops the instrument id
		//					== "INSTRUMENT_ID" && row[i] != NULL)
		//					inst.first = boost::lexical_cast<thOth::BigInt>(std::string(row[i]));

		//				else if (std::string(reception_->fields[i].name)
		//					== "INSTRUMENT_NAME" && row[i] != NULL)
		//					inst.second = boost::lexical_cast<std::string>(row[i]);

		//			}

		//			ret.push_back(inst);									// push back

		//		}
		//	}
		//	catch (...){}													// return false

		//	return ret;

		//};
	}
}