#ifndef fix_recordset_hpp
#define fix_recordset_hpp

#include <string>
#include <map>

#include <mysql.h>

#include <thOth/time/dateTime.hpp>

// insert a string value
#define SQL_INSERT_STR(X,Y) \
	X.append("'" )          \
	 .append(Y   )          \
	 .append("'");

// insert a num value
#define SQL_INSERT_NUM(X,Y) \
	 X.append(boost::lexical_cast<std::string>(Y));

// insert a date value
#define SQL_INSERT_DATE(X,Y,Z)        \
	X.append("'")                     \
	 .append(Y.convertSqlDateTime(Z)) \
	 .append("'");

namespace myFix {

	namespace dataBase {

		enum deletionPolicy : unsigned char {					// defines deletion policy

			authorizeCopy      = 0,
			preserveFormerData = 1,
			EraseFormerData    = 2

		};

		template<
			typename Key, 
			typename T, 
			template<
				typename Key, 
				typename T,
				typename Rel = std::less<Key>, 
				typename All = std::allocator<std::pair<const Key, T>>>
			class Container>
		class recordset {

		public:

			typedef typename Container<Key, T>::const_iterator const_iterator;
			typedef typename Container<Key, T>::iterator iterator;

			recordset() = delete;							// no default ctor

			recordset(MYSQL * connection)					// ctor
				: connection_(connection) {};				
			
			recordset(const recordset & o) {				// copy ctor

				connection_ = o.connection_;
				reception_  = o.reception_ ;

			};

			virtual ~recordset() {};						// virtual destructor

			recordset & operator =(const recordset & o) {

				if (this != &o) {

					// copy stuff
					// should we delete this one for performance concern ?
					connection_ = o.connection_;
					reception_  = o.reception_ ;
					records_    = o.records_   ;

				}

				return * this;

			};

			// database methods, return true is the statement succeed, throw otherwise
			virtual bool selectStr (const std::string &) = 0;
			bool insertStr (const std::string &)			;
			bool deleteStr (const std::string &)			;

			//TODO: synchronization method with deletion policy defined + error management
			//void synchronize(deletionPolicy);

			// iterators
			typename const_iterator cbegin () const { return records_.cbegin(); };
			typename const_iterator cend   () const { return records_.cend  (); };

			typename iterator begin() { return records_.begin(); };
			typename iterator end  () { return records_.end  (); };

			thOth::size size() const { return records_.size(); };

		protected:

			MYSQL *     connection_;						// connection objects
			MYSQL_RES * reception_ ;

			// a pile of records for data management, 
			Container<Key, T> records_;
			
		};

		template<
			typename Key,
			typename T,
			template<				
				typename Key, 
				typename T,
				typename Rel,
				typename All>
			class Container>
		bool recordset<Key, T, Container>::deleteStr(const std::string & deleteStr) {

			if (mysql_query(connection_, deleteStr.c_str()) != 0) {		// throw on an error

				std::string tt(mysql_error(connection_));
				throw std::exception(mysql_error(connection_));

			}

			// todo: error management
			return true;

		}

		template<
			typename Key,
			typename T,
			template<
				typename Key,
				typename T,
				typename Rel,
				typename All>
			class Container>
		bool recordset<Key, T, Container>::insertStr(const std::string & insertStr) {

			try{

				// throw on an error
				if (mysql_query(myFix::settings::instance().connection(), insertStr.c_str()) != 0)
					throw std::exception(mysql_error(myFix::settings::instance().connection()));

			} catch (...){

				return false;

			}

			return true;

		}
	}
}

#endif