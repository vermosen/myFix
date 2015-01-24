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
			thOth::instrument	dummy_(0, "")	;
			std::string			exchange		;
			//thOth::bigInt		id_				;						// the id of the record to insert
			thOth::dateTime		time_			;
			thOth::real			price_			;
			thOth::volume		volume_			;

			while (row_ = mysql_fetch_row(reception_)) {				// loop over the results

				for (unsigned long i = 0; i < reception_->field_count; i++) {

					// drops the bar Id
					// drops the instrument id (already recorded)
					if (std::string(reception_->fields[i].name)
						== "MARKET_ORDER_DATETIME" && row_[i] != NULL)
						time_ = thOth::dateTime::convertSqlDateTime(std::string(row_[i]));

					else if (std::string(reception_->fields[i].name)
						== "MARKET_ORDER_PRICE" && row_[i] != NULL)
						price_ = boost::lexical_cast<thOth::real>(row_[i]);

					else if (std::string(reception_->fields[i].name)
						== "MARKET_ORDER_VOLUME" && row_[i] != NULL)
						volume_ = boost::lexical_cast<thOth::volume>(row_[i]);

				}

				std::pair<thOth::dateTime, thOth::bookOrder> temp(
					time_, thOth::bookOrder(thOth::bookOrder::bid_, volume_, price_));

				records_.insert(temp);

			}

			return true;

		};

		// fix message interface
		bool tableBookOrderRecordset::insert(const std::vector <thOth::bookOrderMessage> & mess_) {
		
			std::string fieldStr, valueStr;

			try{

				fieldStr.append("INSTRUMENT_ID,");						// contract id
				fieldStr.append("TRADE_DATETIME,");						// barStart
				fieldStr.append("TRADE_PRICE,");						// open
				fieldStr.append("TRADE_VOLUME");						// close

				std::string insertStatement("INSERT INTO table_trade (");
				insertStatement.append(fieldStr).append(") VALUES ");

				for (std::vector <thOth::bookOrderMessage>::const_iterator It =
					mess_.cbegin(); It != mess_.cend(); It++) {

					valueStr.append("(");

					SQL_INSERT_NUM(valueStr, It->symbol().first)
						valueStr.append(",");
					SQL_INSERT_DATE(valueStr, It->time(), true)
						valueStr.append(",");
					SQL_INSERT_NUM(valueStr, It->order().price())
						valueStr.append(",");
					SQL_INSERT_NUM(valueStr, It->order().quantity())
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
			const std::pair<thOth::bigInt, std::string> & contract_,
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