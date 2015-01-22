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

#include "recordset/tableInstrumentRecordset/tableInstrumentRecordset.hpp"
//#include "recordset/functions/requestBulkInstrument.hpp"

namespace myFix {

	// exceptions 
	class undefinedInstrumentException : public std::exception {

	public:
		undefinedInstrumentException(const std::string & code) : code_(code) {};

		std::string code() { return code_; };

	protected:
		std::string code_;

	};

	// the parser class contains some instrument list
	// and process the generic type T according to it.
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
		
		void loadInstrumentTable(								// load the db instrument table from a recordset
			dataBase::tableInstrumentRecordset & rs);								

		size_t size();											// the current data size
		std::vector<T> & messages() { return messages_; };		// messages
		void clear()				{ messages_.clear()			; };
		void reserve(size_t)		{ messages_.reserve(size_t)	; };

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
	void parser<T>::loadInstrumentTable(dataBase::tableInstrumentRecordset & rs) {

		this->symbolMap_.clear();								// clears the bymap

		// select statement
		rs.selectStr("SELECT INSTRUMENT_ID, INSTRUMENT_NAME \
					  FROM TABLE_INSTRUMENT");

		for (std::map<thOth::bigInt, std::string>::const_iterator It	// copy the contract table
			= rs.cbegin(); It != rs.cend(); It++)

			this->addSymbol(*It);

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