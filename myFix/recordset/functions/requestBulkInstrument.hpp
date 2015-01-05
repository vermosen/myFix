#ifndef recordset_functions_request_bulk_instrument_hpp
#define recordset_functions_request_bulk_instrument_hpp

#include "utilities/settings/settings.hpp"

#include "recordset/tableInstrumentRecordset/tableInstrumentRecordset.hpp"

namespace myFix {

	namespace dataBase {
	
		// request all the instruents in the database
		std::vector<thOth::instrument> requestBulkInstrument();

	}
}

#endif

