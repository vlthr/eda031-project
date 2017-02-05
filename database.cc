#include "database.h"
#include "news.h"
#include <iostream>
#include <algorithm>
#include <ctime>
Database::Database(){}
Database::~Database(){}

std::vector<std::pair<int, std::string>> Database::list_newsgroups() {
    std::vector<std::pair<int, std::string>> pairs;
    for (auto& ng : newsgroups) {
      pairs.push_back(std::make_pair(ng.id, ng.name));
    }
    return pairs;
}
bool Database::create_newsgroup(std::string name){
    if(exists(name)) return false;
    auto before = newsgroups.size();
    news::Newsgroup n(name, id_ctr,  time(0));
    newsgroups.push_back(n);
    auto after = newsgroups.size();
    if(before != after){
        id_ctr++;
        return true;
    }else return false;
}

bool delete_newsgroup(int id){
  return true;
}

news::Article get_article(int ng_id, int article_id) {
  return news::Article();
}

bool create_article(int ng_id, std::string title,std::string author, std::string text) {
  return true;
}
bool delete_article(int ng_id, int article_id) {
  return true;
}

bool Database::exists(std::string name){
    for(auto n: newsgroups){
        if(n.name == name) return true;
    }
    return false;
}
void Database::sort(){
  std::sort(newsgroups.begin(), newsgroups.end());
}

std::vector<news::Article> Database::list_articles(int ng_id){
   auto a = std::find(newsgroups.begin(), newsgroups.end(), ng_id);
    if(a == newsgroups.end()){
      //todo: throw
    }
    return (&*a)->to_list();
}
