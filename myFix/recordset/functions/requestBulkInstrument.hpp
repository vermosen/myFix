#ifndef recordset_functions_request_bulk_instrument_hpp
#define recordset_functions_request_bulk_instrument_hpp

#include <thOth/message.hpp>

#include "utilities/settings/settings.hpp"

namespace myFix {

	namespace dataBase {
	
		// request all the trades for the given instrument between two given dates
		std::vector<thOth::instrument> requestBulkInstrument();
	}
}

#endif

