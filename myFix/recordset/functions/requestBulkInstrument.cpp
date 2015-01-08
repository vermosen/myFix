#include "recordset/functions/requestBulkInstrument.hpp"

namespace myFix {

	namespace dataBase {

		std::vector<thOth::instrument> requestBulkInstrument() {			// request all the instruents in the database

			std::vector<thOth::instrument> ret;

			try{

				// create the recordset
				tableInstrumentRecordset rs(settings::instance().connection());

				rs.selectStr("SELECT INSTRUMENT_ID, INSTRUMENT_NAME \
							  FROM TABLE_INSTRUMENT");

				for (auto It = rs.cbegin(); It != rs.cend(); It++)
					ret.push_back(*It);										// iterate over the rs map
				
			} catch (...) {}												// return false

			return ret;

		};
	}
}