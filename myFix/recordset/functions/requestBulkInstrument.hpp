#ifndef recordset_functions_request_bulk_instrument_hpp
#define recordset_functions_request_bulk_instrument_hpp

#include <thOth/message.hpp>

#include "recordset/recordset.hpp"
#include "utilities/settings/settings.hpp"

namespace myFix {

	namespace dataBase {
	
		// request all the trades for the given instrument between two given dates
		std::vector<thOth::instrument> requestBulkInstrument() {

			std::string selectStr;
			std::vector<thOth::instrument> ret;

			try{

				// select
				selectStr.append("SELECT INSTRUMENT_ID, INSTRUMENT_NAME FROM TABLE_INSTRUMENT");

				// TODO: exception management
				if (mysql_query(settings::instance().connection(), selectStr.c_str()) != 0)	// throw on an error
					throw std::exception(mysql_error(settings::instance().connection()));

				mysql_query(												// query to run
					settings::instance().connection(),
					selectStr.c_str());

				MYSQL_RES * reception_ = mysql_store_result(				// store the results
					settings::instance().connection());

				if (!reception_)											// sql statement failed
					return ret;

				if (reception_->row_count == 0)								// no record
					return ret;

				MYSQL_ROW row;												// otherwise, run...

				thOth::instrument inst;

				while (row = mysql_fetch_row(reception_)) {					// loop over the results

					for (unsigned long i = 0; i < reception_->field_count; i++) {

						// drops the bar Id
						// drops the instrument id (already recorded)
						if (std::string(reception_->fields[i].name)
							== "INSTRUMENT_ID" && row[i] != NULL)
							inst.first = boost::lexical_cast<dataBase::recordId>(std::string(row[i]));

						else if (std::string(reception_->fields[i].name)
							== "INSTRUMENT_NAME" && row[i] != NULL)
							inst.second = boost::lexical_cast<std::string>(row[i]);

					}

					// push back
					ret.push_back(inst);

				}
			}
			catch (...){

				//return false;

			}

			return ret;

		};
	}
}

#endif

