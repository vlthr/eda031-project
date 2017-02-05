#ifndef SQLDB_H
#define SQLDB_H
#include "news.h"
#include <list>
#include <sqlite3.h>

class Sqldb{
public:
	Sqldb(std::string);

	~Sqldb();
	
        // returns a list with newsgroups in sorted order;
        std::vector<news::Newsgroup> list_newsgroup();
	
        bool exists(std::string);
        news::Newsgroup* get(unsigned int); 
        bool add_newsgroup(std::string);
        void sort();	
	/* inserts d into this list as the first element */
	void insertFirst(int d);
	
private:
	std::vector<news::Newsgroup> newsgroups;
        unsigned int id_ctr;
        sqlite3 *db;
};




#endif
