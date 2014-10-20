/*
*
* custom TwsApi
* Copyright (C) 2014 Jean-Mathieu Vermosen
*
*/

#ifndef fix_logger_hpp
#define fix_logger_hpp

#include <thOth/utilities/csvBuilder.hpp>

namespace myFix {

	// a basic logger class
	class logger : protected thOth::utilities::csvBuilder {

	public:

		logger(const std::string & str) : csvBuilder(str) {}; 	// specifies the file path
		logger(const logger & o       ) = delete;				// no copy ctor

		inline void path(const std::string & path_) {			// set the current path

			csvBuilder::path(path_); 
		
		};
																
		void push_back(const std::string &);					// defines a push_back method

	};

}

#endif