#include "database.h"
#include "news.h"
#include <algorithm>
#include <ctime>
Database::Database(){}

Database::~Database(){}

std::vector<news::Newsgroup> Database::list_newsgroup(){
    return newsgroups;
}

bool Database::add_newsgroup(std::string name){
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

bool Database::exists(std::string name){
    for(auto n: newsgroups){
        if(n.name == name) return true;
    }
    return false;
}
void Database::sort(){
    
  std::sort(newsgroups.begin(), newsgroups.end());
}

news::Newsgroup* Database::get(unsigned int id){
   auto a = std::find(newsgroups.begin(), newsgroups.end(), id);
    if(a == newsgroups.end()){
        return nullptr;
    }
    return &*a;
}
