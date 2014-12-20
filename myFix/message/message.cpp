#include "message/message.hpp"

namespace myFix{

	void message::accept(thOth::acyclicVisitor & v) {
		thOth::visitor<message>* v1 =
			dynamic_cast<thOth::visitor<message>*>(&v);
		if (v1 != 0)
			v1->visit(*this);
		else
			throw std::exception();
	}

	void tradeMessage::accept(thOth::acyclicVisitor & v) {
		thOth::visitor<tradeMessage>* v1 =
			dynamic_cast<thOth::visitor<tradeMessage>*>(&v);
		if (v1 != 0)
			v1->visit(*this);
		else
			message::accept(v);
	}
	
	void bookMessage::accept(thOth::acyclicVisitor & v) {
		thOth::visitor<bookMessage>* v1 =
			dynamic_cast<thOth::visitor<bookMessage>*>(&v);
		if (v1 != 0)
			v1->visit(*this);
		else
			message::accept(v);
	}

	bool bookMessage::operator<(const bookMessage & msg) const {
		
		return seq_number_ < msg.seq_number_; 
	
	};

	std::ostream & operator << (std::ostream &out, const bookMessage & msg) {

		out << msg.symbol_.second
			<< " ("
			<< msg.sender_id_
			<< "); "
			<< boost::posix_time::to_simple_string(msg.time_)
			<< "; SeqNo("
			<< msg.seq_number_
			<< "); "
			<< "UpdateAction("
			<< static_cast<int>(msg.action_)
			<< "); EntryType("
			<< static_cast<int>(msg.type_)
			<< "); "
			<< msg.price_
			<< "; "
			<< msg.quantity_
			<< "; "
			<< msg.order_count_
			<< "; "
			<< msg.level_;

		return out;
	
	}
}
