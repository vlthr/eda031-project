#include "news.h"
#include <ctime>
#include <sstream>
#include <algorithm>

// Newsgroup
news::Newsgroup::Newsgroup(std::string n, unsigned int i, std::time_t c):name(n), id(i), created(c), id_ctr(0) {}

news::Newsgroup::Newsgroup() : id_ctr(0) {}

news::Newsgroup::~Newsgroup(){
    articles.clear();
}

std::vector<news::Article>& news::Newsgroup::to_list(){
    return articles;
}

void news::Newsgroup::sort(){
  std::sort(articles.begin(), articles.end());
}

// Can pass by reference aswell to avoid unnecessary copy
bool news::Newsgroup::del(unsigned int id){
    auto before = articles.size();
    auto a = std::find_if(articles.begin(), articles.end(), [id] (const Article& art) { return art.id == id; });
    if (a != articles.end()) articles.erase(a);
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

bool news::Newsgroup::add(std::string title, std::string author, std::string text){
  id_ctr++;
  news::Article a(title, author, text, id_ctr);
  articles.push_back(a);
  return true;
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

std::istream& news::operator>>(std::istream&  input, news::Newsgroup& n){
    input >> n.name;
    input >> n.id;
    long long c;
    input >> c;
    n.created = c;
    return input;
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
std::istream& news::operator>>(std::istream&  input, news::Article& a ){
    input >> a.title >> a.author >> a.content >> a.id;
    long long c;
    input >> c;
    a.created = c;
    return input;
}

