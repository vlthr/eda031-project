#include "news.h"
#include <ctime>
#include <algorithm>

// Newsgroup
news::Newsgroup::Newsgroup(std::string n, unsigned int i, std::time_t c):name(n), id(i), created(c) {}

news::Newsgroup::Newsgroup(){}

news::Newsgroup::~Newsgroup(){
    articles.clear();
}

std::vector<news::Article> news::Newsgroup::to_list(){
    return articles;
}

void news::Newsgroup::sort(){
  std::sort(articles.begin(), articles.end());
}

// Can pass by reference aswell to avoid unnecessary copy
bool news::Newsgroup::del(unsigned int id){
    auto before = articles.size();
    auto a = std::find_if(articles.begin(), articles.end(), [id] (const Article& art) { return art.id == id; });
    articles.erase(a);
    auto after = articles.size();
    return before != after;

}
news::Article* news::Newsgroup::get(unsigned int id){
   auto a = std::find(articles.begin(), articles.end(), id);
    if(a == articles.end()){
        return nullptr;
    }
    return &*a;
}
bool news::Newsgroup::add(news::Article a){
    if(get(a.id) != nullptr){
        return false;
    }
    auto before = articles.size();
    articles.push_back(a);
    auto after = articles.size();
    return before != after;
}

bool news::Newsgroup::operator<(const news::Newsgroup& a) const{
   return std::difftime(created, a.created) > 0;
}

bool news::Newsgroup::operator==(const news::Newsgroup& a) const{
   return id == a.id;
}
bool news::Newsgroup::operator==(const unsigned int id) const{
  return this->id == id;
}

// Article
news::Article::Article(std::string t, std::string a, std::string c, unsigned int i): title(t), author(a), content(c), id(i){}

news::Article::Article(){}

news::Article::~Article(){}
bool news::Article::operator<(const news::Article& a) const{
   return std::difftime(created, a.created) > 0;
}
bool news::Article::operator==(const news::Article& a) const{
   return id == a.id;
}
bool news::Article::operator==(const unsigned int id) const{
  return this->id == id;
}
