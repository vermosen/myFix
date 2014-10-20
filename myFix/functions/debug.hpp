#ifndef functions_debug_hpp
#define functions_debug_hpp

#include <fstream>

#include <boost/timer.hpp>

void debug() {

	boost::timer t;

	try {
	
	}
	catch (std::exception & e) {
	
		std::cout << e.what() << std::endl;
	
	}

	std::cout
		<< "debug test achieved in "
		<< t.elapsed()
		<< " seconds"
		<< std::endl;

}

#endif