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
   std::string name = argv[0];
   int id = std::stoi(argv[1]);
   std::pair<int, std::string> p(id, name);
   std::vector<std::pair<int, std::string>>* grp = (std::vector<std::pair<int, std::string>>*)data;
   grp->push_back(p);
   return 0;
}

Sqldb::Sqldb(std::string fn="Database.db"){
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

std::vector<std::pair<int, std::string>> Sqldb::list_newsgroup(){
    char *zErrMsg = "Failed to get newsgroup list from database";    
    int rc;
    std::vector<std::pair<int, std::string>>* data = new std::vector<std::pair<int, std::string>>();

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
    sqlite3_close(db);
    return *data;
}







bool Sqldb::add_newsgroup(std::string name){
return true;
}

bool Sqldb::exists(std::string name){
return true;
}
void Sqldb::sort(){
}
news::Newsgroup* Sqldb::get(unsigned int id){
    return nullptr;
}
