#include "fixParser/fixParser.hpp"

#include <istream>

namespace myFix {

	fixParser::fixParser(FIX::DataDictionary * d)					// provide dictionary ptr 
		: symbols_() {

		dic_ = d;

	};

	fixParser::~fixParser() {};

		// @brief parse a FIX message and return its components that change the book + trades
	std::vector<bookMessage> fixParser::parse(const FIX50SP2::MarketDataIncrementalRefresh& msg) const {

		std::vector<bookMessage> messages;

		std::string value; // FIX field values will be recorder here

		size_t groups_count = 0;
		thOth::dateTime time;
		unsigned long seq_number;
		std::string sender_id;

		// get data from the header
		try {

			groups_count = boost::lexical_cast<size_t>(msg.getField(268));

			FIX::Header header = msg.getHeader();
			value = header.getField(52);
			long millis = strtol(&value[value.size() - 3], nullptr, 10);
			// build date from string
			//time = thOth::dateTime(value, "%Y%m%d%H%M%S") + boost::posix_time::milliseconds(millis);
			seq_number = strtol(&header.getField(34)[0], nullptr, 10);
			sender_id = header.getField(49);

		}
		catch (...) {

			return messages;

		}

		// get data from groups of the message
		FIX::SecurityDesc securityDesc;
		FIX50SP2::MarketDataIncrementalRefresh::NoMDEntries group;
		for (size_t i = 1; i <= groups_count; ++i) {

			msg.getGroup(i, group);

			try {

				value = group.getField(securityDesc).getString();
				if (symbols_.size() != 0 && symbols_.find(value) == symbols_.cend())
					continue;

				// discard groups with quote condition = exchange best (field 276 = 'C')
				// as they aren't book updates
				try
				{
					value = group.getField(276);
					if (value == "C") continue;
				}
				catch (...) {}

				bookMessage m_msg;
				m_msg.time_			= time;
				m_msg.seq_number_	= seq_number;
				m_msg.price_		= std::stod(group.getField(270));
				m_msg.quantity_		= std::stoi(group.getField(271));
				m_msg.symbol_		= value;
				m_msg.sender_id_	= sender_id;

				// MDUpdateAction
				value = group.getField(279);
				if (value == "0")
					m_msg.action_ = bookMessage::update_action::new_;
				else if (value == "1")
					m_msg.action_ = bookMessage::update_action::change_;
				else if (value == "2")
					m_msg.action_ = bookMessage::update_action::delete_;
				else
					continue;

				// MDEntryType: discard anythind beyond Bid, Ask, or Trade
				value = group.getField(269);
				if (value == "0")
					m_msg.type_ = bookMessage::entry_type::bid_;
				else if (value == "1")
					m_msg.type_ = bookMessage::entry_type::ask_;
				else
				{
					if (value == "2")
					{
						m_msg.type_ = bookMessage::entry_type::trade_;
						messages.push_back(m_msg);
					}
					continue;
				}

				m_msg.level_		= stoi(group.getField(1023));
				m_msg.order_count_	= stoi(group.getField(346 ));

				messages.push_back(m_msg);

			}
			catch (...)
			{
				continue;
			}
		}

		return messages;
	}

	std::vector<bookMessage> fixParser::parse(const std::string& msg) const {
		FIX::Message fix_msg(msg, *dic_, false);
		return parse(FIX50SP2::MarketDataIncrementalRefresh(fix_msg));
	}

	std::vector<tradeMessage> fixParser::parse_trade(
		const FIX50SP2::MarketDataIncrementalRefresh & msg) const {
		
		std::vector<tradeMessage> messages;

		std::string value;

		size_t groups_count = 0;
		thOth::dateTime time;

		// Try to get the number of groups in the message. If not available, discard the message.
		try {
		
			groups_count = boost::lexical_cast<size_t>(msg.getField(268));
			//groups_count = strtol(msg.getField(268).c_str(), nullptr, 10);
			FIX::Header header = msg.getHeader();
			
			// parsing time
			value = header.getField(52);
			value = msg.getHeader().getField(52); // didn't work in GCC for some reason
			long millis = strtol(&value[value.size() - 3], nullptr, 10);
			time = thOth::dateTime::currentTime();
			//time = thOth::dateTime(value, "%Y%m%d%H%M%S") + boost::posix_time::milliseconds(millis);
		
		}
		catch (...)	{

			return messages;
		
		}

		FIX::SecurityDesc securityDesc;
		FIX50SP2::MarketDataIncrementalRefresh::NoMDEntries group;
		for (size_t i = 1; i <= groups_count; ++i) {

			msg.getGroup(i, group);

			try	{
				
				value = group.getField(securityDesc).getString();
				if (symbols_.size() != 0 && symbols_.find(value) == symbols_.cend())
					continue;

				// is a trade
				if (group.getField(269) == "2") {

					tradeMessage m_msg;
					m_msg.time_		= time;
					m_msg.price_	= std::stod(group.getField(270));
					m_msg.quantity_ = std::stoi(group.getField(271));
					m_msg.symbol_	= value;
					messages.push_back(m_msg);
				
				}
			}
			catch (...)
			{
				continue;
			}
		}

		return messages;
	}

	std::vector<tradeMessage> fixParser::parse_trade(const std::string& msg) const
	{
		FIX::Message fix_msg(msg, *dic_, false);
		return parse_trade(FIX50SP2::MarketDataIncrementalRefresh(fix_msg));
	}

	std::vector<bookMessage> fixParser::operator()(const FIX50SP2::MarketDataIncrementalRefresh& msg) const {
		return parse(msg);
	}

	std::vector<bookMessage> fixParser::operator()(const std::string& msg) const {
		return parse(msg);
	}
}
