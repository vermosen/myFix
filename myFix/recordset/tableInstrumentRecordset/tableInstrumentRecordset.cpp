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
			thOth::BigInt instrumentId  ;								// the id of the record to insert

			while (row = mysql_fetch_row(reception_)) {					// loop over the results
				
				for (unsigned long i = 0;								// loop over the fields
					i < reception_->field_count; i++) {	

					if (std::string(reception_->fields[i].name)
						== "INSTRUMENT_ID" && row[i] != NULL)
						instrumentId = boost::lexical_cast<thOth::BigInt>(row[i]);

					else if (std::string(reception_->fields[i].name)
						== "INSTRUMENT_NAME" && row[i] != NULL)
						name = boost::lexical_cast<std::string>(row[i]);

					// TODO: spreads

					else

						throw selectQueryExceptionUnknownField();

				}

				records_.insert(
					std::pair<thOth::BigInt, std::string>(
						instrumentId, name));

			}

			return true;

		};

		bool tableInstrumentRecordset::insertStr(						// single insert
			const std::string & code) {

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
		
		}
	}
}