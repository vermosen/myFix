#ifndef table_bar_recordset
#define table_bar_recordset

#include <thOth/time/DateTime.hpp>
#include <thOth/time/timeSeries.hpp>
#include <thOth/message.hpp>
#include <thOth/bar/bar.hpp>

#include "utilities/settings/settings.hpp"
#include "recordset/recordset.hpp"
#include "recordset/recordsetException.hpp"

namespace myFix {

	namespace dataBase {

		class tableBarRecordset
			: public recordset<thOth::dateTime,
			thOth::bar,
			thOth::timeSeries> {

		public:

			tableBarRecordset(MYSQL * connection)
				: recordset(connection) {};

			// recordset interface
			bool selectStr(const std::string &);					// run a select statement

			// specific insert
			bool insert(
				const thOth::instrument &,
				const thOth::timeSeries<thOth::dateTime, thOth::bar> &);

		};

		//namespace utilities {
		//
		//	class periodConverter {};
		//
		//}
	}
}

#endif 