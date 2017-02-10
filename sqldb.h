#ifndef SQLDB_H
#define SQLDB_H
#include "news.h"
#include "abstractdb.h"
#include <list>
#include <sqlite3.h>

class Sqldb: public Abstractdb{
public:
	Sqldb(std::string);
	~Sqldb();
	
        // returns a list with newsgroups in sorted order;
        std::vector<std::pair<int, std::string>> list_newsgroups();
        std::vector<news::Article> list_articles(int ng_id);

        bool exists(std::string);
        bool exists(int ng_id);
        bool create_newsgroup(std::string name);
        bool delete_newsgroup(int id);

        news::Article get_article(int ng_id, int article_id);
        bool create_article(int ng_id, std::string title,std::string author, std::string text);
        bool delete_article(int ng_id, int article_id);
        void sort();
private:
        int count_articles(int ng);
        int count_newsgroups();
	std::vector<news::Newsgroup> newsgroups;
        unsigned int id_ctr;
        sqlite3 *db;
};




#endif
