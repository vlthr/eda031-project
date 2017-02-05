#include "sqldb.h"
#include "news.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <sqlite3.h>
#include <stdio.h>

static int callbackArticle(void *data, int argc, char **argv, char **azColName){
   int i;
   std::string articledata; 
   for(i=0; i<argc; i++){
       articledata+=argv[i];
       articledata+="\n";
   }
   std::istringstream ss(articledata);
   news::Article art;
      
   ss >> art;
   std::vector<news::Article>* grp = (std::vector<news::Article>*)data;
   grp->push_back(art);

   return 0;
}
static int callbackNewsgroup(void *data, int argc, char **argv, char **azColName){
   int i;
   std::string newsgroupdata;
   for(i=0; i<3; i++){
       newsgroupdata+=argv[i];
       newsgroupdata+="\n";
       printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }

   std::istringstream ss(newsgroupdata);
   news::Newsgroup ng;
   ss >> ng;
   std::vector<news::Newsgroup>* grp = (std::vector<news::Newsgroup>*)data;
   grp->push_back(ng);
   std::cout << grp->size(); 
   return 0;
}

Sqldb::Sqldb(std::string fn){
    char *zErrMsg = 0;
    int rc;
    const char * c = fn.c_str();   
    rc = sqlite3_open(c, &db);
 
    if(rc){
       fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    }else{
       fprintf(stderr, "Opened database successfully\n");
    }
 }
 

Sqldb::~Sqldb(){
    sqlite3_close(db);   
    fprintf(stderr, "Database Shutting Down");
}

std::vector<news::Newsgroup> Sqldb::list_newsgroup(){

    char *zErrMsg = "Failed to get newsgroup list from database";    
    int rc;
    std::vector<news::Newsgroup>* data = new std::vector<news::Newsgroup>();
    
    /* Create SQL statement  */
    char* sql = "SELECT * FROM newsgroups";  

    /* Execute SQL statement */
    
    rc = sqlite3_exec(db, sql, callbackNewsgroup, (void*)data, &zErrMsg);
    if( rc != SQLITE_OK ){
       fprintf(stderr, "SQL error: %s\n", zErrMsg);
       sqlite3_free(zErrMsg);
    }else{
       fprintf(stdout, "Operation done successfully\n");
    }
    for(news::Newsgroup a: *data){
        std::cout << "Printing newsgroups name " << std::endl;
        std::cout << a.name;
    }
    sqlite3_close(db);
    return *data;
}

bool Sqldb::add_newsgroup(std::string name){
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

bool Sqldb::exists(std::string name){
    for(auto n: newsgroups){
        if(n.name == name) return true;
    }
    return false;
}
void Sqldb::sort(){
    
  std::sort(newsgroups.begin(), newsgroups.end());
}

news::Newsgroup* Sqldb::get(unsigned int id){
   auto a = std::find(newsgroups.begin(), newsgroups.end(), id);
    if(a == newsgroups.end()){
        return nullptr;
    }

    return &*a;
}
