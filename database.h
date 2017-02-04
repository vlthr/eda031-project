#ifndef DATABASE_H
#define DATABASE_H
#include "news.h"
#include <list>

class Database{
public:
	Database();

	~Database();
	
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
};




#endif
