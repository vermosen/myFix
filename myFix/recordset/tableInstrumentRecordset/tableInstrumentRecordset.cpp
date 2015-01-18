#include "recordset/tableInstrumentRecordset/tableInstrumentRecordset.hpp"

namespace myFix {

	namespace dataBase {

		bool tableInstrumentRecordset::selectStr(const std::string & selectStr) {
		
			mysql_query(												// query to run
				connection_,
				selectStr.c_str());

			reception_ = mysql_store_result(connection_);
		
			if (!reception_)											// sql statement failed
				throw selectQueryExceptionSelectFailed();

			if (reception_->row_count == 0)								// no record
				throw selectQueryExceptionNoSelection();

			MYSQL_ROW row				;								// row ptr
			std::string name			;								// request, we generate the key
			thOth::bigInt instrumentId  ;								// the id of the record to insert

			while (row = mysql_fetch_row(reception_)) {					// loop over the results
				
				for (unsigned long i = 0;								// loop over the fields
					i < reception_->field_count; i++) {	

					if (std::string(reception_->fields[i].name)
						== "INSTRUMENT_ID" && row[i] != NULL)
						instrumentId = boost::lexical_cast<thOth::bigInt>(row[i]);

					else if (std::string(reception_->fields[i].name)
						== "INSTRUMENT_NAME" && row[i] != NULL)
						name = boost::lexical_cast<std::string>(row[i]);

					// TODO: spreads

					else

						throw selectQueryExceptionUnknownField();

				}

				records_.insert(
					std::pair<thOth::bigInt, std::string>(
						instrumentId, name));

			}

			return true;

		};
	}
}