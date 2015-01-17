#ifndef table_instrument_recordset_hpp
#define table_instrument_recordset_hpp

#include "utilities/settings/settings.hpp"

#include <map>

#include <thOth/time/DateTime.hpp>
#include <thOth/time/timeSeries.hpp>
#include <thOth/message.hpp>

#include "recordset/recordset.hpp"
#include "recordset/recordsetException.hpp"

namespace myFix {

	namespace dataBase {

		class tableInstrumentRecordset 
			: public recordset<thOth::BigInt,
							   std::string,
							   std::map> {

			public:

				tableInstrumentRecordset(MYSQL * connection)
					: recordset<thOth::BigInt, std::string, std::map>(connection) {};

				// recordset interface
				bool selectStr(const std::string &);					// run a select statement
				
				// refined interface
				bool insert(const std::vector<std::string> &);			// bulk insert
				
			protected:

				tableInstrumentRecordset() {};
				
		};
	}
}

#endif 