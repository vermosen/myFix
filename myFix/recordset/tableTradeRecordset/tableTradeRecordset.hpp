#ifndef table_trade_recordset_hpp
#define table_trade_recordset_hpp

#include <thOth/time/DateTime.hpp>
#include <thOth/time/timeSeries.hpp>
#include <thOth/bar/bar.hpp>
#include <thOth/message/trademessage.hpp>

#include "utilities/settings/settings.hpp"
#include "recordset/recordset.hpp"
#include "recordset/recordsetException.hpp"

namespace myFix {

	namespace dataBase {

		class tableTradeRecordset 
			: public recordset<thOth::dateTime, 
							   thOth::tradeMessage,
							   thOth::timeSeries> {

			public:

				tableTradeRecordset(MYSQL * connection)
					: recordset<thOth::dateTime, thOth::tradeMessage, thOth::timeSeries>(connection) {};

				// recordset interface
				bool selectStr(const std::string &);					// run a select statement

				// insert
				bool insert(
					const std::pair<thOth::BigInt, std::string> &, 
					const thOth::timeSeries<thOth::dateTime, thOth::tradeMessage> &);
				
			protected:

				tableTradeRecordset() {};
				
		};
	}
}

#endif 