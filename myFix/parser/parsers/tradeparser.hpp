#ifndef myfix_trade_parser_hpp
#define myfix_trade_parser_hpp

#include <thOth/message/tradeMessage.hpp>

#include "parser/parser.hpp"


namespace myFix {

	class tradeParser : public parser<thOth::tradeMessage> {
		
	public:

		tradeParser(const std::shared_ptr<FIX::DataDictionary>&);	// ctor with dictionary ptr
		virtual void parse(const std::string &);					// parsing method

	};

	// instrument parser
	class undefinedInstrumentException : public std::exception {

	public:
		undefinedInstrumentException(const std::string & code) : code_(code) {};

		std::string code() { return code_; };

	protected:
		std::string code_;

	};
}

#endif