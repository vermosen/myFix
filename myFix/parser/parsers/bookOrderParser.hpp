#ifndef myfix_book_order_parser_hpp
#define myfix_book_order_parser_hpp

#include <thOth/message/bookOrderMessage.hpp>

#include "parser/parser.hpp"


namespace myFix {

	class bookOrderParser : public parser<thOth::tradeMessage> {

	public:

		// ctor with dictionary ptr
		bookOrderParser(const std::shared_ptr<FIX::DataDictionary>&);	
		void parse(const std::string &);							// parsing method

	};
}

#endif