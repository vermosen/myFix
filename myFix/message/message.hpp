#ifndef myfix_messages
#define myfix_messages

#include <string>
#include <istream>

#include <thOth/time/DateTime.hpp>
#include <thOth/pattern/acyclicVisitor.hpp>
#include <thOth/pattern/visitor.hpp>

#include "recordset/recordset.hpp"

namespace myFix {

	// TODO: bring more structure on that one
	typedef std::pair<myFix::dataBase::recordId, std::string> instrument;

	class message {

	public:

		// common structure
		instrument      symbol_			;	// symbol name (FIX field 107) together with db identifier
		thOth::dateTime time_			;	// quote time
		double          price_     = .0	;	// price
		int             quantity_  =  0	;	// number of units (contracts, shares, etc.)

		// visitability
		virtual void accept(thOth::acyclicVisitor &);

	};

	class tradeMessage : public message {
	
	public:

		virtual void accept(thOth::acyclicVisitor &);
	
	};

	class bookMessage : public message {

	public:

		enum update_action { new_ = 1, change_ = 2, delete_ = 3 };
		enum entry_type    { bid_ = 1, ask_    = 2, trade_  = 3 };

		update_action   action_			;	// new, change, delete
		entry_type      type_			;	// bid, ask, trade
		unsigned int    order_count_ = 0;	// number of orders at the level
		unsigned int    level_ = 0		;	// book level
		unsigned long   seq_number_ = 0	;	// sequence number (FIX field 34)
		std::string     sender_id_ = ""	;	// firm sending message (FIX field 49), e.g., CME

		// sort by sequence number
		bool operator<(const bookMessage & msg);

		friend std::ostream & operator << (std::ostream &, const bookMessage &);

		// visitability
		virtual void accept(thOth::acyclicVisitor &);

	};
}

#endif