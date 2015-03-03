#include "recordset/tableBookOrderRecordset/tableBookOrderRecordset.hpp"

namespace myFix {

	namespace dataBase {

		bool tableBookOrderRecordset::selectStr(const std::string & selectStr) {

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
			thOth::instrument				instrument_	;
			thOth::dateTime					time_		;
			thOth::bookOrder::orderType		type_		;
			thOth::bookOrder::actionType	action_		;
			thOth::real						price_		;
			thOth::volume					volume_		;

			while (row_ = mysql_fetch_row(reception_)) {				// loop over the results

				for (unsigned long i = 0; i < reception_->field_count; i++) {

					// drops the bar Id
					// drops the instrument id (already recorded)
					if (std::string(reception_->fields[i].name)
						== "INSTRUMENT_ID" && row_[i] != NULL) {

						instrument_.first = boost::lexical_cast<thOth::bigInt>(row_[i]);
						instrument_.second = "DUMMY";					// TODO: return the correct identifier
					
					}

					else if (std::string(reception_->fields[i].name)
						== "ORDER_DATETIME" && row_[i] != NULL)
						time_ = thOth::dateTime::convertSqlDateTime(std::string(row_[i]));

					else if (std::string(reception_->fields[i].name)
						== "ORDER_TYPE" && row_[i] != NULL)
						type_ = (thOth::bookOrder::orderType)row_[i][0];

					else if (std::string(reception_->fields[i].name)
						== "ORDER_PRICE" && row_[i] != NULL)
						price_ = boost::lexical_cast<thOth::real>(row_[i]);

					else if (std::string(reception_->fields[i].name)
						== "ORDER_VOLUME" && row_[i] != NULL)
						volume_ = boost::lexical_cast<thOth::volume>(row_[i]);

					else if (std::string(reception_->fields[i].name)
						== "ORDER_ACTION" && row_[i] != NULL)
						action_ = (thOth::bookOrder::actionType)boost::lexical_cast<int>(row_[i]);

				}

				records_.insert(
					std::pair<thOth::dateTime, thOth::bookOrder>(
						time_, thOth::bookOrder(type_, action_, price_, volume_)));

			}

			return true;

		};

		// fix message interface
		bool tableBookOrderRecordset::insert(const std::vector <thOth::bookOrderMessage> & mess_) {
		
			std::string fieldStr, valueStr;

			try{

				fieldStr.append("INSTRUMENT_ID,"		);				// instrument id
				fieldStr.append("ORDER_DATETIME,"		);				// order date/time
				fieldStr.append("ORDER_TYPE,"			);				// order type
				fieldStr.append("ORDER_PRICE,"			);				// order price
				fieldStr.append("ORDER_VOLUME,"			);				// order volume
				fieldStr.append("ORDER_ACTION,"			);				// order action
				fieldStr.append("ORDER_COUNT,"			);				// order count
				fieldStr.append("ORDER_MD_PRICE_LEVEL,"	);				// order md price level
				fieldStr.append("ORDER_SENDER_ID"		);				// order sender id

				std::string insertStatement("INSERT INTO table_marketOrder (");
				insertStatement.append(fieldStr).append(") VALUES ");

				for (std::vector <thOth::bookOrderMessage>::const_iterator It =
					mess_.cbegin(); It != mess_.cend(); It++) {

					valueStr.append("(");

					SQL_INSERT_NUM(valueStr, It->symbol().first)
						valueStr.append(",");
					SQL_INSERT_DATE(valueStr, It->time(), true)
						valueStr.append(",");
					SQL_INSERT_NUM(valueStr, It->order().type())
						valueStr.append(",");
					SQL_INSERT_NUM(valueStr, It->order().price())
						valueStr.append(",");
					SQL_INSERT_NUM(valueStr, It->order().quantity())
						valueStr.append(",");
					SQL_INSERT_NUM(valueStr, It->order().action())
						valueStr.append(",");
					SQL_INSERT_NUM(valueStr, It->order_count())
						valueStr.append(",");
					SQL_INSERT_NUM(valueStr, It->level())
						valueStr.append(",");
					SQL_INSERT_NUM(valueStr, 1)
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
		bool tableBookOrderRecordset::insert(
			const thOth::instrument & contract_,
			const thOth::timeSeries<thOth::dateTime, thOth::bookOrderMessage> & records_) {

			std::string fieldStr, valueStr;

			try{

				fieldStr.append("INSTRUMENT_ID,"	);					// contract id
				fieldStr.append("TRADE_DATETIME,"	);					// barStart
				fieldStr.append("TRADE_PRICE,"		);					// open
				fieldStr.append("TRADE_VOLUME"		);					// close

				std::string insertStatement("INSERT INTO table_trade (");
				insertStatement.append(fieldStr).append(") VALUES ");

				for (thOth::timeSeries<thOth::dateTime, thOth::bookOrderMessage>::const_iterator It =
					records_.cbegin(); It != records_.cend(); It++) {

					valueStr.append("(");

					SQL_INSERT_NUM(valueStr, contract_.first)
						valueStr.append(",");
					SQL_INSERT_DATE(valueStr, It->first, true)
						valueStr.append(",");
					SQL_INSERT_NUM(valueStr, It->second.order().price())
						valueStr.append(",");
					SQL_INSERT_NUM(valueStr, It->second.order().quantity())
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