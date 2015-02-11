#ifndef table_trade_recordset_hpp
#define table_trade_recordset_hpp

#include <thOth/time/DateTime.hpp>
#include <thOth/time/timeSeries.hpp>
#include <thOth/message/trademessage.hpp>

#include "utilities/settings/settings.hpp"
#include "recordset/recordset.hpp"
#include "recordset/recordsetException.hpp"

namespace myFix {

	namespace dataBase {

		class tableTradeRecordset 
			: public recordset<thOth::dateTime, 
							   thOth::trade,
							   thOth::timeSeries> {

			public:

				tableTradeRecordset(MYSQL * connection)
					: recordset(connection) {};

				// recordset interface
				bool selectStr(const std::string &);					// run a select statement

				bool insert(const std::vector <thOth::tradeMessage> &);	// message vector
					
				bool insert(											// object vector
					const thOth::instrument & contract_,
					const thOth::timeSeries<thOth::dateTime, thOth::tradeMessage> &);
				
		};
	}
}

#endif 