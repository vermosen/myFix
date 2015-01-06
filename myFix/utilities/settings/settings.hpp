/*
 *
 * custom TwsApi
 * Copyright (C) 2014 Jean-Mathieu Vermosen
 *
 */

#ifndef fix_settings_hpp
#define fix_settings_hpp

#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/lock_guard.hpp>

#include <thOth/pattern/singleton.hpp>

#include <quickfix/DataDictionary.h>

#include <mysql.h>

#include "utilities/logger/logger.hpp"

// default setup
#define DB_SERVER   "192.168.0.10"										
#define DB_USER     "admin"
#define DB_PASSWORD "DIAle050580$"
#define DB_ID       "fixdb"
#define DB_PORT     3308
#define LOGPATH		"C://Temp/"
#define DICT        "C://Temp/FIX50SP2_CME.xml"

namespace myFix {

	// concurrency compatible setting class
	class settings : public thOth::singleton < settings > {
												
		friend class thOth::singleton < settings >;

		private:

			settings();										// ctor
			void updateConnection ();						// update the mySQL connection

		public: 

			void verbosity  (const unsigned int  v);		//accessors
			void port       (const unsigned int  v);
			void server     (const std::string & s);
			void user       (const std::string & s);
			void password   (const std::string & s);
			void dataBase   (const std::string & s);
			void dictionary (const std::string & s);

			inline void logPath(const std::string & s) { log_.path(s); };

			unsigned int verbosity () const;
			unsigned int port      () const;
			std::string server     () const;
			std::string user       () const;
			std::string password   () const;
			std::string dataBase   () const;

			inline logger & log                     () { return log_      ;  };
			inline MYSQL * connection               () { return connect_  ;  };
			inline std::shared_ptr<FIX::DataDictionary> dictionary () { return dictionary_; };


		private:

			// members
			unsigned int          verbosity_ ;					// verbosity settings				
			logger                log_       ;					// log file
			MYSQL *               connect_   ;					// mysql connection object
			std::shared_ptr<FIX::DataDictionary> dictionary_;	// quickfix dictionary

			// dataBase settings
			unsigned int port_     ;     				// db server port
			std::string  server_   ;					// db server name
			std::string  user_     ;					// user name
			std::string  password_ ;					// user password
			std::string  dataBase_ ;					// dataBase name

			// mutexes
			mutable boost::mutex verbosityMutex_	;	// locks/unlocks the verbosity variable
			mutable boost::mutex portMutex_			;	// locks/unlocks the port variable
			mutable boost::mutex serverMutex_		;	// locks/unlocks the server variable
			mutable boost::mutex userMutex_			;	// locks/unlocks the user variable
			mutable boost::mutex passwordMutex_		;	// locks/unlocks the password variable
			mutable boost::mutex dataBaseMutex_		;	// locks/unlocks the database variable
			mutable boost::mutex connectionMutex_	;	// locks/unlocks the database variable
			mutable boost::mutex dictionaryMutex_	;	// locks/unlocks the fix dictionary
	
	};

}

#endif