#include "recordset/tableTradeRecordset/tableTradeRecordset.hpp"

namespace myFix {

	namespace dataBase {

		bool tableTradeRecordset::selectStr(const std::string & selectStr) {

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
			thOth::instrument	dummy_(0, "")	;
			std::string			exchange		;
			//thOth::BigInt		id_				;						// the id of the record to insert
			thOth::dateTime		time_			;
			thOth::real			price_			;
			thOth::volume		volume_			;

			while (row_ = mysql_fetch_row(reception_)) {				// loop over the results

				for (unsigned long i = 0; i < reception_->field_count; i++) {

					// drops the bar Id
					// drops the instrument id (already recorded)
					if (std::string(reception_->fields[i].name)
						== "TRADE_DATETIME" && row_[i] != NULL)
						time_ = thOth::dateTime::convertSqlDateTime(std::string(row_[i]));

					else if (std::string(reception_->fields[i].name)
						== "TRADE_PRICE" && row_[i] != NULL)
						price_ = boost::lexical_cast<double>(row_[i]);

					else if (std::string(reception_->fields[i].name)
						== "TRADE_VOLUME" && row_[i] != NULL)
						volume_ = boost::lexical_cast<int>(row_[i]);

				}

				std::pair<thOth::dateTime, thOth::tradeMessage> temp(
					time_, thOth::tradeMessage(dummy_, time_, price_, volume_));

				records_.insert(temp);

			}

			return true;

		};

		bool tableTradeRecordset::insert(
			const std::pair<thOth::BigInt, std::string> & contract_,
			const thOth::timeSeries<thOth::dateTime, thOth::tradeMessage> & records_) {

			std::string fieldStr, valueStr;

			try{

				fieldStr.append("INSTRUMENT_ID,");						// contract id
				fieldStr.append("TRADE_DATETIME,");						// barStart
				fieldStr.append("TRADE_PRICE,");						// open
				fieldStr.append("TRADE_VOLUME,");						// close

				fieldStr.pop_back();									// remove the last column

				std::string insertStatement("INSERT INTO table_trade (");
				insertStatement.append(fieldStr).append(") VALUES ");

				for (thOth::timeSeries<thOth::dateTime, thOth::tradeMessage>::const_iterator It =
					records_.cbegin(); It != records_.cend(); It++) {

					valueStr.append("(");

					SQL_INSERT_NUM(valueStr, It->second.symbol().first)
						valueStr.append(",");
					SQL_INSERT_DATE(valueStr, It->second.time(), true)
						valueStr.append(",");
					SQL_INSERT_NUM(valueStr, It->second.price())
						valueStr.append(",");
					SQL_INSERT_NUM(valueStr, It->second.quantity())
						valueStr.append("),");

				}

				valueStr.pop_back();						// suppress the last column

				insertStatement.append(valueStr);			// add the value fields to insert statement 

				// this should throw internally
				if (this->insertStr(insertStatement) != true)

					throw std::exception(mysql_error(myFix::settings::instance().connection()));

			}
			catch (...){

				return false;

			}

			return true;

		}
	}
}