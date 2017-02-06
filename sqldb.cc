#include "sqldb.h"
#include "news.h"
#include <stdexcept>
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

    std::cout.flush();
    std::string title    =       argv[0];
    std::string author   =       argv[1];
    std::string content  =       argv[2];
    std::string id       =       argv[3];
    //news::Article** temp = (news::Article**)data;
    //*art = new news::Article(title, author, content, std::stoi(id));
    news::Article* art = (news::Article*)data;
    *art = news::Article(title, author, content, std::stoi(id));
    return 0;
}
static int callbackArticleList(void *data, int argc, char **argv, char **azColName){

    std::string title    =       argv[0];
    std::string author   =       argv[1];
    std::string content  =       argv[2];
    std::string id       =       argv[3];
    news::Article art(title, author, content, std::stoi(id));
    std::vector<news::Article>* grp = (std::vector<news::Article>*)data;
    grp->push_back(art);
    return 0;
}
static int callbackNewsgroup(void *data, int argc, char **argv, char **azColName){
    int id = std::stoi(argv[0]);
    std::string name = argv[1];
    std::pair<int, std::string> p(id, name);
    std::vector<std::pair<int, std::string>>* grp = (std::vector<std::pair<int, std::string>>*)data;
    grp->push_back(p);
    return 0;
}

/* Database Definition */

Sqldb::Sqldb(std::string fn="Database.db"){
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
    const char *zErrMsg = "Failed to get newsgroup list from database";
    int rc;
    std::vector<std::pair<int, std::string>>* data = new std::vector<std::pair<int, std::string>>();

    /* Create SQL statement  */
    std::string sql = "SELECT rowid,name FROM newsgroups";  
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql.c_str(), callbackNewsgroup, (void*)data, nullptr);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        // sqlite3_free(zErrMsg); // Is this a good idea?
    }else{
        fprintf(stdout, "Operation done successfully\n");
    }
    return *data;
}


std::vector<news::Article> Sqldb::list_articles(int ng_id){
    char *zErrMsg = 0;
    int rc;
    std::vector<news::Article>* data = new std::vector<news::Article>();
    if(!exists(ng_id)){
        throw std::invalid_argument("Newsgroup doesnt exist");
    }
    /* Create SQL statement  */

    std::string sql = "SELECT author, title, content, rowid FROM articles WHERE newsgroup=(SELECT name FROM newsgroups ng WHERE ng.rowid="+std::to_string(ng_id)+");";
    rc = sqlite3_exec(db, sql.c_str(), callbackArticleList, (void*)data, &zErrMsg);
    /* Execute SQL statement */
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);


    }else{
        fprintf(stdout, "Operation done successfully\n");
    }
    return *data;
}






bool Sqldb::create_newsgroup(std::string name){
    const char *zErrMsg = "Failed to create newsgroup ";
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
    rc = sqlite3_exec(db, sql.c_str(), nullptr, (void*)data, nullptr);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        return false;
    }else{ 
        fprintf(stdout, "Operation done successfully\n");
        id_ctr++;
        return true;
    }
}

void Sqldb::delete_newsgroup(int id){
    const char *zErrMsg = "Failed to delete newsgroup ";
    int rc;
    /* Create SQL statement  */
    std::string sql = "DELETE FROM newsgroups WHERE rowid = "+std::to_string(id);
    

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql.c_str(), empty, (void*)0, nullptr);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
    }else{ 

        fprintf(stderr, "ERRMSG: %s\n", zErrMsg);
        fprintf(stdout, "Operation done successfully\n");
    }
}

bool Sqldb::create_article(int ng_id, std::string title,std::string author, std::string text){
    const char *zErrMsg = "Failed to create article ";
    int rc;
    /* Create SQL statement  */
    bool val = false;
    bool* data = &val;
    std::time_t t = time(0);
    long long created = t;
    std::string sql = "INSERT INTO articles VALUES('"+
        title + "','" + author + "','" + text + 
        "', (SELECT name FROM newsgroups WHERE rowid = " + 
        std::to_string(ng_id)+")," +
        std::to_string(created) +");";
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql.c_str(), flip, (void*)data, nullptr);

    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        return false;
    }else{ 
        fprintf(stdout, "Operation done successfully\n");
        id_ctr++;
        return true;
    }
}

void Sqldb::delete_article(int ng_id, int article_id){
    const char *zErrMsg = "Failed to create article ";
    int rc;
    /* Create SQL statement  */
    std::string sql = "DELETE FROM articles WHERE rowid="+std::to_string(article_id)+
        " AND newsgroup=(SELECT name FROM newsgroups ng WHERE ng.rowid="+std::to_string(ng_id)+");";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql.c_str(), empty, 0, nullptr);

    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
    }else{ 
        fprintf(stdout, "Operation done successfully\n");
    }
}
bool Sqldb::exists(std::string name){
    const char *zErrMsg = "Failed to create article ";
    int rc;
    bool val = false;
    bool* data = &val;
    /* Create SQL statement  */
    std::string sql = "SELECT rowid FROM newsgroups WHERE name='"+name+"';";
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql.c_str(), flip, (void*)data, nullptr);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);

        return false;
    }else{ 
        fprintf(stdout, "Operation done successfully\n");
        return *data;
    }
}
bool Sqldb::exists(int ng_id){
    const char *zErrMsg = "Failed to create article ";
    int rc;
    bool val = false;
    bool* data = &val;
    /* Create SQL statement  */
    std::string sql = "SELECT name FROM newsgroups WHERE rowid="+std::to_string(ng_id)+";";
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql.c_str(), flip, (void*)data, nullptr);
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
    const char *zErrMsg = "Failed to get article ";
    int rc;
    news::Article* data = new news::Article("Invalid","Invalid","Invalid",-1);
    /* Create SQL statement  */

    std::string sql = "SELECT title, author, content, rowid FROM articles WHERE rowid="+std::to_string(article_id)+" AND newsgroup=(SELECT name FROM newsgroups ng WHERE ng.rowid="+std::to_string(ng_id)+");";
    //std::cout << sql << std::endl;
    /* Execute SQL statement */
    //std::cout << " Before casting " << std::endl;
    rc = sqlite3_exec(db, sql.c_str(), callbackArticle, (void*)data, nullptr);
    if((data->id == -1)){
        throw std::invalid_argument("Couldnt find...");
    }
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        throw std::invalid_argument("Unknown article_id...");
    }else{ 
        fprintf(stdout, "Operation done successfully\n");

        news::Article copy= *data;
        delete data;
        return copy;
    }

}
