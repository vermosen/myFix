#pragma once

//#include <ats/message/messages.hpp>                   // incremental refresh message class

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>

#include <thOth/time/DateTime.hpp>

#include <quickfix/DataDictionary.h>
#include <quickfix/fix50sp2/MarketDataIncrementalRefresh.h>

#include <vector>
#include <string>
#include <unordered_set>
#include <functional>

namespace myFix {

	class message {

	public:

		// common structure
		std::string		symbol_		= "";		// symbol name (FIX field 107)
		thOth::dateTime time_			;		// quote time
		double          price_		= .0;		// price
		int             quantity_	= 0	;		// number of units (contracts, shares, etc.)
		
	};

	struct tradeMessage : public message {};

	struct bookMessage : public message {

		enum update_action { new_ = 1, change_ = 2, delete_ = 3 };
		enum entry_type    { bid_ = 1, ask_    = 2, trade_  = 3 };

		update_action   action_				;	// new, change, delete
		entry_type      type_				;	// bid, ask, trade
		unsigned int    order_count_ =  0	;	// number of orders at the level
		unsigned int    level_       =	0	;	// book level
		unsigned long   seq_number_  =	0	;	// sequence number (FIX field 34)
		std::string     sender_id_	 =	""	;	// firm sending message (FIX field 49), e.g., CME

		bool operator<(const bookMessage& msg) const { return seq_number_ < msg.seq_number_; };

		std::string to_string() const {

			std::stringstream ss;
			ss << symbol_
			   << " (" 
			   << sender_id_
			   << "); " 
			   << boost::posix_time::to_simple_string(time_)
			   << "; SeqNo(" 
			   << seq_number_ 
			   << "); " 
			   << "UpdateAction(" 
			   << static_cast<int>(action_)
			   << "); EntryType(" 
			   << static_cast<int>(type_) 
			   << "); "
			   << price_
			   << "; " 
			   << quantity_
			   << "; " 
			   << order_count_
			   << "; " 
			   << level_;

			return ss.str();
		
		}
	};

	class fixParser {

	public:

		fixParser(FIX::DataDictionary *);		// provide dictionary ptr
		~fixParser();

		//parse a FIX message and return its components that change the book + trades
		std::vector<bookMessage> parse(const FIX50SP2::MarketDataIncrementalRefresh& msg) const;
		std::vector<bookMessage> parse(const std::string& msg) const;

		std::vector<tradeMessage> parse_trade(const FIX50SP2::MarketDataIncrementalRefresh& msg) const;
		std::vector<tradeMessage> parse_trade(const std::string& msg) const;

		std::vector<bookMessage> operator()(const FIX50SP2::MarketDataIncrementalRefresh& msg) const;
		std::vector<bookMessage> operator()(const std::string& msg) const;

		// TODO: change the parse, parse_trades and operator() to manage 

	protected:

		FIX::DataDictionary * dic_;
		std::unordered_set<std::string> symbols_;
		mutable std::stringstream ss_;

	};
}
