#ifndef table_trade_recordset_hpp
#define table_trade_recordset_hpp

#include <thOth/time/DateTime.hpp>
#include <thOth/time/timeSeries.hpp>

#include "recordset/recordset.hpp"
#include "utilities/message/message.hpp"
#include "recordset/recordsetException.hpp"
#include "recordset/conversion.hpp"

namespace myFix {

	namespace dataBase {

		class tableTradeRecordset 
			: public recordset<myFix::tradeMessage,
							   thOth::dateTime, 
							   thOth::timeSeries> {

			public:

				tableTradeRecordset(MYSQL *);
				~tableTradeRecordset() {};

				tableTradeRecordset & operator =(const tableTradeRecordset &);

				// recordset interface
				bool selectStr(const std::string &);					// run a select statement
				bool deleteStr(const std::string &);					// delete records 

				// insert
				bool insert(
					const std::pair<recordId, std::string> &, 
					const thOth::timeSeries<myFix::tradeMessage> &);
				
			protected:

				tableTradeRecordset() {};
				
		};
	}
}

#endif 