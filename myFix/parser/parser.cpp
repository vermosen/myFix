# include "parser/parser.hpp"

namespace myFix {

	parser::parser(const std::shared_ptr<FIX::DataDictionary>& d) {		// ctor with dictionary ptr

		dic_ = d;

		ss_.imbue(std::locale(std::locale(), 
			new boost::posix_time::time_input_facet("%Y%m%d%H%M%S")));

	};

	void parser::loadInstrumentTable() {

		// load the instruments
		std::vector<myFix::instrument> inst = dataBase::requestBulkInstrument();

		// load the contract table
		for (std::vector<myFix::instrument>::const_iterator It
			= inst.cbegin(); It != inst.cend(); It++)

			this->addSymbol(instrumentMap::value_type(It->first, It->second));

	}

	void parser::addSymbol(const instrumentMap::value_type & i) {

		symbolMap_.insert(i);

	};
}
