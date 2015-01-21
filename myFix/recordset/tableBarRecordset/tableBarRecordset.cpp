#include "recordset/tableBarRecordset/tableBarRecordset.hpp"

namespace myFix {

	namespace dataBase {

		bool tableBarRecordset::selectStr(const std::string & selectStr) {

			mysql_query(												// query to run
				connection_,
				selectStr.c_str());

			reception_ = mysql_store_result(connection_);

			if (!reception_)											// sql statement failed
				throw selectQueryExceptionSelectFailed();

			if (reception_->row_count == 0)								// no record
				throw selectQueryExceptionNoSelection();

			MYSQL_ROW row_;												// current row

			// todo: get the instrument details on the instrument table
			std::string		exchange;
			thOth::dateTime	time_	;
			thOth::real		open_	;
			thOth::real		close_	;
			thOth::real		high_	;
			thOth::real		low_	;
			thOth::volume	volume_	;
			thOth::timeUnit	length_	;
			thOth::size		size_	;

			while (row_ = mysql_fetch_row(reception_)) {				// loop over the results

				for (unsigned long i = 0; i < reception_->field_count; i++) {

					// drops the bar Id
					// drops the instrument id (already recorded)
					if (std::string(reception_->fields[i].name)
						== "BAR_DATETIME" && row_[i] != NULL)
						time_ = thOth::dateTime::convertSqlDateTime(std::string(row_[i]));

					else if (std::string(reception_->fields[i].name)
						== "BAR_OPEN" && row_[i] != NULL)
						open_ = boost::lexical_cast<thOth::real>(row_[i]);

					else if (std::string(reception_->fields[i].name)
						== "BAR_CLOSE" && row_[i] != NULL)
						close_ = boost::lexical_cast<thOth::real>(row_[i]);

					else if (std::string(reception_->fields[i].name)
						== "BAR_HIGH" && row_[i] != NULL)
						high_ = boost::lexical_cast<thOth::real>(row_[i]);

					else if (std::string(reception_->fields[i].name)
						== "BAR_LOW" && row_[i] != NULL)
						low_ = boost::lexical_cast<thOth::real>(row_[i]);

					else if (std::string(reception_->fields[i].name)
						== "BAR_VOLUME" && row_[i] != NULL)
						volume_ = boost::lexical_cast<thOth::volume>(row_[i]);

					// need to set the conversion
					else if (std::string(reception_->fields[i].name)
						== "BAR_LENGTH" && row_[i] != NULL) {

						//thOth::period temp = boost::lexical_cast<int>(row_[i]);
						length_ = thOth::timeUnit::day;
						size_ = 1;

					}
						

					
				}

				std::pair<thOth::dateTime, thOth::bar> temp(
					time_, thOth::bar(
						open_, close_, high_, 
						low_, thOth::period(length_, size_), volume_));

				records_.insert(temp);

			}

			return true;

		};

		bool tableBarRecordset::insert(
			const thOth::instrument & instrument_,
			const thOth::timeSeries<thOth::dateTime, thOth::bar> & bars_) {

			std::string fieldStr, valueStr;

			try{

				fieldStr.append("INSTRUMENT_ID,");						// contract id
				fieldStr.append("BAR_DATETIME,");						// barStart
				fieldStr.append("BAR_OPEN,");							// open
				fieldStr.append("BAR_CLOSE,");							// close
				fieldStr.append("BAR_HIGH,");							// high
				fieldStr.append("BAR_LOW,");							// low
				fieldStr.append("BAR_VOLUME,");							// volume
				fieldStr.append("BAR_LENGTH");							// volume

				std::string insertStatement("INSERT INTO table_trade (");
				insertStatement.append(fieldStr).append(") VALUES ");

				for (thOth::timeSeries<thOth::dateTime, thOth::bar>::const_iterator It =
					records_.cbegin(); It != records_.cend(); It++) {

					valueStr.append("(");

					SQL_INSERT_NUM(valueStr, instrument_.first)
						valueStr.append(",");
					SQL_INSERT_DATE(valueStr, It->first, true)
						valueStr.append(",");
					SQL_INSERT_NUM(valueStr, It->second.open())
						valueStr.append(",");
					SQL_INSERT_NUM(valueStr, It->second.close())
						valueStr.append(",");
					SQL_INSERT_NUM(valueStr, It->second.high())
						valueStr.append(",");
					SQL_INSERT_NUM(valueStr, It->second.low())
						valueStr.append(",");
					SQL_INSERT_NUM(valueStr, It->second.volume())
						valueStr.append(",");
					// TODO insert length
						valueStr.append("), ");
				}

				valueStr.pop_back();						// suppress the last column

				insertStatement.append(valueStr);			// add the value fields to insert statement 

				// this should throw internally
				if (this->insertStr(insertStatement) != true)

					throw std::exception(mysql_error(myFix::settings::instance().connection()));

			} catch (...){

				return false;

			}

			return true;

		}
	}
}