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
						price_ = boost::lexical_cast<thOth::real>(row_[i]);

					else if (std::string(reception_->fields[i].name)
						== "TRADE_VOLUME" && row_[i] != NULL)
						volume_ = boost::lexical_cast<thOth::volume>(row_[i]);

				}

				std::pair<thOth::dateTime, thOth::trade> temp(
					time_, thOth::trade(volume_, price_));

				records_.insert(temp);

			}

			return true;

		};

		// fix message interface
		bool tableTradeRecordset::insert(const std::vector <thOth::tradeMessage> & mess_) {
		
			std::string fieldStr, valueStr;

			try{

				fieldStr.append("INSTRUMENT_ID,");						// contract id
				fieldStr.append("TRADE_DATETIME,");						// barStart
				fieldStr.append("TRADE_PRICE,");						// open
				fieldStr.append("TRADE_VOLUME");						// close

				std::string insertStatement("INSERT INTO table_trade (");
				insertStatement.append(fieldStr).append(") VALUES ");

				for (std::vector <thOth::tradeMessage>::const_iterator It =
					mess_.cbegin(); It != mess_.cend(); It++) {

					valueStr.append("(");

					SQL_INSERT_NUM(valueStr, It->symbol().first)
						valueStr.append(",");
					SQL_INSERT_DATE(valueStr, It->time(), true)
						valueStr.append(",");
					SQL_INSERT_NUM(valueStr, It->messageTrade().price())
						valueStr.append(",");
					SQL_INSERT_NUM(valueStr, It->messageTrade().quantity())
						valueStr.append("),");

				}

				valueStr.pop_back();						// suppress the last colon

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

		// object interface
		bool tableTradeRecordset::insert(
			const thOth::instrument & contract_,
			const thOth::timeSeries<thOth::dateTime, thOth::tradeMessage> & records_) {

			std::string fieldStr, valueStr;

			try{

				fieldStr.append("INSTRUMENT_ID,"	);					// contract id
				fieldStr.append("TRADE_DATETIME,"	);					// barStart
				fieldStr.append("TRADE_PRICE,"		);					// open
				fieldStr.append("TRADE_VOLUME"		);					// close

				std::string insertStatement("INSERT INTO table_trade (");
				insertStatement.append(fieldStr).append(") VALUES ");

				for (thOth::timeSeries<thOth::dateTime, thOth::tradeMessage>::const_iterator It =
					records_.cbegin(); It != records_.cend(); It++) {

					valueStr.append("(");

					SQL_INSERT_NUM(valueStr, contract_.first)
						valueStr.append(",");
					SQL_INSERT_DATE(valueStr, It->first, true)
						valueStr.append(",");
					SQL_INSERT_NUM(valueStr, It->second.messageTrade().price())
						valueStr.append(",");
					SQL_INSERT_NUM(valueStr, It->second.messageTrade().quantity())
						valueStr.append("),");

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