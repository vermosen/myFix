#ifndef table_book_order_recordset_hpp
#define table_book_order_recordset_hpp

#include <thOth/time/DateTime.hpp>
#include <thOth/time/timeSeries.hpp>
#include <thOth/message/bookOrderMessage.hpp>

#include "utilities/settings/settings.hpp"
#include "recordset/recordset.hpp"
#include "recordset/recordsetException.hpp"

namespace myFix {

	namespace dataBase {

		class tableBookOrderRecordset 
			: public recordset<thOth::dateTime, 
							   thOth::bookOrder,
							   thOth::timeSeries> {

			public:

				tableBookOrderRecordset(MYSQL * connection)
					: recordset(connection) {};

				// recordset interface
				bool selectStr(const std::string &);					// run a select statement

				bool insert(const std::vector <thOth::bookOrderMessage> &);	// message vector
					
				bool insert(											// object vector
					const std::pair<thOth::bigInt, std::string> &, 
					const thOth::timeSeries<thOth::dateTime, thOth::bookOrderMessage> &);
				
		};
	}
}

#endif 