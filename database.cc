#include "database.h"
#include "news.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <stdexcept>
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

bool Database::delete_newsgroup(int id){
  int to_delete = -1;
  for (int i = 0; i < newsgroups.size(); ++i) {
    news::Newsgroup ng = newsgroups.at(i);
    if (ng.id == id) {
      to_delete = i;
      break;
    }
  }
  if (to_delete >= 0) {
    newsgroups.erase(newsgroups.begin()+to_delete); 
    return true;
  }
  return false;
}

news::Article Database::get_article(int ng_id, int article_id) {
  for (auto& art : list_articles(ng_id)){
    if (art.id == article_id) return art;
  }
  throw std::invalid_argument("Unknown article_id...");
}

bool Database::create_article(int ng_id, std::string title,std::string author, std::string text) {
  for (auto& ng : newsgroups) {
    if (ng.id == ng_id) {
      ng.add(title, author, text);
      return true;
    }
  }
  return false;
}
bool Database::delete_article(int ng_id, int art_id) {
    for (int i = 0; i < newsgroups.size(); ++i) {
      news::Newsgroup& ng = newsgroups.at(i);
      if (ng.id == ng_id) {
        return ng.del(art_id);
      }
    }
    return false;
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
      throw std::invalid_argument("Unknown ng_id...");
    }
    return (&*a)->to_list();
}
