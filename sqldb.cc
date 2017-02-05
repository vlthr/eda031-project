#include "sqldb.h"
#include "news.h"
#include <iostream>
#include <algorithm>
#include <ctime>
#include <sqlite3.h>
#include <stdio.h>


static int callbackArticle(void *data, int argc, char **argv, char **azColName){
   int i;
   fprintf(stderr, "%s: ", (const char*)data);
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}
static int callbackNewsgroup(void *data, int argc, char **argv, char **azColName){
   int i;
   fprintf(stderr, "%s: ", (const char*)data);
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
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
    std::vector<news::Newsgroup> *data;
    
    /* Create SQL statement  */
    char* sql = "SELECT * FROM NEWSGROUPS";  

    /* Execute SQL statement */
    

    rc = sqlite3_exec(db, sql, callbackNewsgroup, (void*)data, &zErrMsg);
    if( rc != SQLITE_OK ){
       fprintf(stderr, "SQL error: %s\n", zErrMsg);
       sqlite3_free(zErrMsg);
    }else{
       fprintf(stdout, "Operation done successfully\n");
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
