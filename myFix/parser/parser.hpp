#ifndef myfix_parser_hpp
#define myfix_parser_hpp

#include <vector>
#include <string>
#include <unordered_set>
#include <functional>
#include <istream>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>
#include <boost/bimap.hpp>

#include <thOth/time/DateTime.hpp>
#include <thOth/message.hpp>

#include <quickfix/DataDictionary.h>
#include <quickfix/fix50sp2/MarketDataIncrementalRefresh.h>

#include "recordset/functions/requestBulkInstrument.hpp"

namespace myFix {

	template <typename T>
	class parser {

	protected:

		// define instrumentMap
		typedef boost::bimap < thOth::bigInt, std::string > instrumentMap;

	public:

		parser(const std::shared_ptr<FIX::DataDictionary>&);	// ctor with dictionary ptr
		virtual ~parser() {};									// virtual destructor

		virtual void parse(const std::string &) = 0;			// message parsing method

		void addSymbol(const instrumentMap::value_type & i);	// add an instrument to the parser		
		void loadInstrumentTable();								// load the db instrument table
		size_t size();											// the current data size

	protected:

		std::shared_ptr<FIX::DataDictionary> dict_		;		// FIX dictionary 
		mutable std::stringstream            stream_	;		// stringstream with the date facet
		instrumentMap						 symbolMap_	;		// bimap
		std::vector<T>						 messages_  ;		// the messages buffer

	};

	template<typename T>										// ctor with dictionary ptr
	parser<T>::parser(					
		const std::shared_ptr<FIX::DataDictionary>& d) : dict_(d) {	

		stream_.imbue(std::locale(std::locale(),
			new boost::posix_time::time_input_facet("%Y%m%d%H%M%S")));

	};

	template<typename T>
	void parser<T>::loadInstrumentTable() {

		std::vector<thOth::instrument> inst =					// load the instruments
			dataBase::requestBulkInstrument();

		for (std::vector<thOth::instrument>::const_iterator It	// load the contract table
			= inst.cbegin(); It != inst.cend(); It++)

			this->addSymbol(instrumentMap::value_type(
				It->first, It->second));

	}

	template<typename T>										// add a symbol
	void parser<T>::addSymbol(const instrumentMap::value_type & i) {
		symbolMap_.insert(i);
	}

	template<typename T>
	size_t parser<T>::size() {
		return messages_.size(); 
	};

}

#endif