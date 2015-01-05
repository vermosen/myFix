#ifndef myfix_trade_parser_hpp
#define myfix_trade_parser_hpp

#include <thOth/message/tradeMessage.hpp>

#include "parser/parser.hpp"


namespace myFix {

	// instrument not defined
	class undefinedInstrumentException : public std::exception {

	public:
		undefinedInstrumentException(const std::string & code) : code_(code) {};

		std::string code() { return code_; };

	protected:
		std::string code_;

	};

	class tradeParser : public parser<thOth::tradeMessage> {
		
	public:

		tradeParser(const std::shared_ptr<FIX::DataDictionary>&);	// ctor with dictionary ptr
		virtual void parse(const std::string &);					// parsing method

	};
}

#endif