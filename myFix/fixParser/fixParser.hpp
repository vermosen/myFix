#pragma once

//#include <ats/message/messages.hpp>                   // incremental refresh message class
#include <thOth/time/DateTime.hpp>

#include <quickfix/DataDictionary.h>
#include <quickfix/fix50sp2/MarketDataIncrementalRefresh.h>

#include <vector>
#include <string>
#include <unordered_set>
#include <functional>

namespace myFix {

	struct tradeMessage
	{
		std::string symbol;
		thOth::dateTime time;
		double price;
		int quantity;
	};

	struct bookMessage {

		enum update_action { a_new = 1, a_change = 2, a_delete = 3 };
		enum entry_type    { bid = 1, ask = 2, trade = 3 };
		thOth::dateTime time;
		update_action action;            // new, change, delete
		entry_type type;                 // bid, ask, trade
		double price = .0;
		int quantity = 0;                // number of units (contracts, shares, etc.)
		unsigned int order_count = 0;    // number of orders at the level
		unsigned int level = 0;          // book level
		std::string symbol = "";         // symbol name (FIX field 107)
		unsigned long seq_number = 0;    // sequence number (FIX field 34)
		std::string sender_id = "";      // firm sending message (FIX field 49), e.g., CME

		bool operator<(const bookMessage& msg) const { return seq_number < msg.seq_number; }

		std::string to_string() {

			int act = static_cast<int>(action);
			int etype = static_cast<int>(type);
			std::stringstream ss;
			ss << symbol << " (" << sender_id << "); " << time.to_string("%Y-%m-%d %H:%M:%S%F")
				<< "; SeqNo(" << seq_number << "); " << "UpdateAction(" << act
				<< "); EntryType(" << etype << "); "
				<< price << "; " << quantity << "; " << order_count << "; " << level;
			std::string str = ss.str();
			return str;
		
		}
	};

	class fixParser {

	public:

		fixParser(const std::string & specs_path);					// provide dictionary path

		//parse a FIX message and return its components that change the book + trades
		std::vector<bookMessage> parse(const FIX50SP2::MarketDataIncrementalRefresh& msg) const;
		std::vector<bookMessage> parse(const std::string& msg) const;

		std::vector<tradeMessage> parse_trades(const FIX50SP2::MarketDataIncrementalRefresh& msg) const;
		std::vector<tradeMessage> parse_trades(const std::string& msg) const;

		std::vector<bookMessage> operator()(const FIX50SP2::MarketDataIncrementalRefresh& msg) const;
		std::vector<bookMessage> operator()(const std::string& msg) const;

	private:
		FIX::DataDictionary dic;
		std::unordered_set<std::string> symbols;
	};
}
