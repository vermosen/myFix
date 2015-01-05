#ifndef table_trade_recordset_hpp
#define table_trade_recordset_hpp

#include <thOth/time/DateTime.hpp>
#include <thOth/time/timeSeries.hpp>
#include <thOth/bar/bar.hpp>
#include <thOth/message/trademessage.hpp>

#include "recordset/recordset.hpp"
#include "recordset/recordsetException.hpp"


namespace myFix {

	namespace dataBase {

		class tableTradeRecordset 
			: public recordset<thOth::tradeMessage,
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
					const std::pair<thOth::BigInt, std::string> &, 
					const thOth::timeSeries<thOth::tradeMessage> &);
				
			protected:

				tableTradeRecordset() {};
				
		};
	}
}

#endif 