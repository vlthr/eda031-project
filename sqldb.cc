#include "sqldb.h"
#include "news.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <sqlite3.h>
#include <stdio.h>

/* Callback Functions */

static int empty(void *data, int argc, char **argv, char **azColName){
   return 0; 
}

static int flip(void *data, int argc, char **argv, char **azColName){
    *((bool*)data) = true;
   return 0;
}

static int callbackArticle(void *data, int argc, char **argv, char **azColName){
   int i;
   std::string articledata; 
   articledata+=argv[1]; // Title
   articledata+="\n";
   articledata+=argv[2]; // Author
   articledata+="\n";
   articledata+=argv[3]; //Content
   articledata+="\n";
   articledata+=argv[0]; // ID
   std::istringstream ss(articledata);
   news::Article art;
   ss >> art;
   std::vector<news::Article>* grp = (std::vector<news::Article>*)data;
   grp->push_back(art);
   return 0;
}
static int callbackNewsgroup(void *data, int argc, char **argv, char **azColName){
   int i;
   int id = std::stoi(argv[0]);
   std::string name = argv[1];
   std::pair<int, std::string> p(id, name);
   std::vector<std::pair<int, std::string>>* grp = (std::vector<std::pair<int, std::string>>*)data;
   grp->push_back(p);
   return 0;
}

/* Database Definition */

Sqldb::Sqldb(std::string fn="Database.db"){
    char *zErrMsg = 0;
    int rc;
    id_ctr = 40;
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


/* Member functions */


std::vector<std::pair<int, std::string>> Sqldb::list_newsgroups(){
    char *zErrMsg = "Failed to get newsgroup list from database";    
    int rc;
    std::vector<std::pair<int, std::string>>* data = new std::vector<std::pair<int, std::string>>();

    /* Create SQL statement  */
    char* sql = "SELECT rowid,name FROM newsgroups";  
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callbackNewsgroup, (void*)data, &zErrMsg);
    if( rc != SQLITE_OK ){
       fprintf(stderr, "SQL error: %s\n", zErrMsg);
       sqlite3_free(zErrMsg);
    }else{
       fprintf(stdout, "Operation done successfully\n");
    }
    return *data;
}


std::vector<news::Article> Sqldb::list_articles(int ng_id){
    char *zErrMsg = "Failed to get article list from database";    
    int rc;
    std::vector<news::Article>* data = new std::vector<news::Article>();

    /* Create SQL statement  */



    std::string sql = "SELECT rowid,* FROM articles WHERE newsgroup=(SELECT name FROM newsgroups ng WHERE ng.rowid="+std::to_string(ng_id)+");";


    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql.c_str(), callbackArticle, (void*)data, &zErrMsg);
    if( rc != SQLITE_OK ){
       fprintf(stderr, "SQL error: %s\n", zErrMsg);
       sqlite3_free(zErrMsg);
    }else{
       fprintf(stdout, "Operation done successfully\n");
    }
    return *data;
}






bool Sqldb::create_newsgroup(std::string name){
    char *zErrMsg = "Failed to create newsgroup ";
    int rc;
    std::vector<std::pair<int, std::string>>* data = new std::vector<std::pair<int, std::string>>();

    /* Create SQL statement  */
    std::time_t t = time(0);
    long long created = t;
    std::string sql = "INSERT INTO newsgroups VALUES ('"
        +name+"',"
        +std::to_string(created)+
        ");";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql.c_str(), empty, (void*)data, &zErrMsg);
    if( rc != SQLITE_OK ){
       fprintf(stderr, "SQL error: %s\n", zErrMsg);
        return false;
    }else{ 
       fprintf(stdout, "Operation done successfully\n");
        id_ctr++;
        return true;
    }
}

bool Sqldb::delete_newsgroup(int id){
    char *zErrMsg = "Failed to delete newsgroup ";
    int rc;
    std::vector<std::pair<int, std::string>>* data = new std::vector<std::pair<int, std::string>>();

    /* Create SQL statement  */
    std::time_t t = time(0);
    long long created = t;
    std::string sql = "DELETE FROM newsgroups WHERE rowid = "+std::to_string(id);


    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql.c_str(), empty, (void*)data, &zErrMsg);
    if( rc != SQLITE_OK ){
       fprintf(stderr, "SQL error: %s\n", zErrMsg);
        return false;
    }else{ 
       fprintf(stdout, "Operation done successfully\n");
        id_ctr++;
        return true;
    }
}

bool Sqldb::create_article(int ng_id, std::string title,std::string author, std::string text){
    char *zErrMsg = "Failed to create article ";
    int rc;
    std::vector<std::pair<int, std::string>>* data = new std::vector<std::pair<int, std::string>>();
    /* Create SQL statement  */
    std::time_t t = time(0);
    long long created = t;
    std::string sql = "INSERT INTO articles VALUES('"+
        title + "','" + author + "','" + text + 
        "', (SELECT name FROM newsgroups WHERE rowid = " + 
        std::to_string(ng_id)+")," +
        std::to_string(created) +");";
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql.c_str(), empty, (void*)data, &zErrMsg);
    if( rc != SQLITE_OK ){
       fprintf(stderr, "SQL error: %s\n", zErrMsg);
        return false;
    }else{ 
       fprintf(stdout, "Operation done successfully\n");
        id_ctr++;
        return true;
    }
}

bool Sqldb::delete_article(int ng_id, int article_id){
    char *zErrMsg = "Failed to create article ";
    int rc;
    bool val = false;
    bool* data = &val;
    /* Create SQL statement  */
    std::time_t t = time(0);
    long long created = t;
    if(!Sqldb::exists(ng_id)){
       return false; 
    }
    std::string sql = "DELETE FROM articles WHERE rowid="+std::to_string(article_id)+
        " AND newsgroup=(SELECT name FROM newsgroups ng WHERE ng.rowid="+std::to_string(ng_id)+");";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql.c_str(), flip, (void*)data, &zErrMsg);
    if( rc != SQLITE_OK ){
       fprintf(stderr, "SQL error: %s\n", zErrMsg);
        return false;
    }else{ 
       fprintf(stdout, "Operation done successfully\n");
        return *data;
    }
}
bool Sqldb::exists(std::string name){
    char *zErrMsg = "Failed to create article ";
    int rc;
    bool val = false;
    bool* data = &val;
    /* Create SQL statement  */
    std::time_t t = time(0);
    long long created = t;
    std::string sql = "SELECT rowid FROM newsgroups WHERE name='"+name+"';";
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql.c_str(), flip, (void*)data, &zErrMsg);
    if( rc != SQLITE_OK ){
       fprintf(stderr, "SQL error: %s\n", zErrMsg);
        return false;
    }else{ 
       fprintf(stdout, "Operation done successfully\n");
        return *data;
    }
}
bool Sqldb::exists(int ng_id){
    char *zErrMsg = "Failed to create article ";
    int rc;
    bool val = false;
    bool* data = &val;
    /* Create SQL statement  */
    std::time_t t = time(0);
    long long created = t;
    std::string sql = "SELECT name FROM newsgroups WHERE rowid="+std::to_string(ng_id)+";";
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql.c_str(), flip, (void*)data, &zErrMsg);
    if( rc != SQLITE_OK ){
       fprintf(stderr, "SQL error: %s\n", zErrMsg);
        return false;
    }else{ 
       fprintf(stdout, "Operation done successfully\n");
        return *data;
    }
}


void Sqldb::sort(){
}
news::Article Sqldb::get_article(int ng_id, int article_id){
    news::Article ng;
    return ng;
}
