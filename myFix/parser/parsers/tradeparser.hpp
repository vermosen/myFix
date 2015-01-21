#ifndef myfix_trade_parser_hpp
#define myfix_trade_parser_hpp

#include <thOth/message/tradeMessage.hpp>

#include "parser/parser.hpp"


namespace myFix {

	class tradeParser : public parser<thOth::tradeMessage> {
		
	public:

		tradeParser(const std::shared_ptr<FIX::DataDictionary>&);	// ctor with dictionary ptr
		void parse(const std::string &);							// parsing method

	};
}

#endif