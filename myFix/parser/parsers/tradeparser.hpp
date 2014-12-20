#ifndef myfix_trade_parser_hpp
#define myfix_trase_parser_hpp

#include "parser/parser.hpp"

namespace myFix {

	// TODO:
	// We need to pass the std::pair<recordId, std::string> to extract the right contracts
	// and get homogeneous vectors in return
	// we also need to comeup with a solution for spread trades
	class tradeParser : public parser {

	public:

		tradeParser  () = delete			;							// no default ctor
		tradeParser  (const std::shared_ptr<FIX::DataDictionary> &);	// provide dictionary ptr
		~tradeParser ();

		// parser interface
		std::vector<std::shared_ptr<myFix::message> > parse(const std::string &) const;

	};
}

#endif