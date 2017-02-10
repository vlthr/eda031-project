#ifndef ABSTRACTDB_H
#define ABSTRACTDB_H
class Abstractdb{
 public:
  virtual std::vector<std::pair<int, std::string>> list_newsgroups()=0;
  virtual std::vector<news::Article> list_articles(int ng_id)=0;

  virtual bool exists(std::string)=0;
  virtual bool create_newsgroup(std::string name)=0;
  virtual bool delete_newsgroup(int id)=0;

  virtual news::Article get_article(int ng_id, int article_id)=0;
  virtual bool create_article(int ng_id, std::string title,std::string author, std::string text)=0;
  virtual bool delete_article(int ng_id, int article_id)=0;
  virtual void sort()=0;
};
#endif
