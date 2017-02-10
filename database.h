#ifndef DATABASE_H
#define DATABASE_H
#include "news.h"
#include "abstractdb.h"
#include <list>

class Database: public Abstractdb{
 public:
  Database();

  ~Database();

  // returns a list with newsgroups in sorted order;
  std::vector<std::pair<int, std::string>> list_newsgroups();
  std::vector<news::Article> list_articles(int ng_id);

  bool exists(std::string);
  bool create_newsgroup(std::string name);
  bool delete_newsgroup(int id);

  news::Article get_article(int ng_id, int article_id);
  bool create_article(int ng_id, std::string title,std::string author, std::string text);
  bool delete_article(int ng_id, int article_id);
  void sort();
  /* inserts d into this list as the first element */
  void insertFirst(int d);
 private:
  std::vector<news::Newsgroup> newsgroups;
  unsigned int id_ctr;
};




#endif
