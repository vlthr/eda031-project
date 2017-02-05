#include "news.h"
//#include "database.h"
#include <iostream>
#include <ctime>
#include <iostream>
#include <vector>
#include <sqlite3.h>
#include <sstream>
#include "sqldb.h"

int main(){
        Sqldb sqldb("Database.db");
        std::vector<std::pair<int, std::string>> ls = sqldb.list_newsgroup();
        for(auto p: ls){
        std::cout << p.first << " " << p.second << std::endl;
        }
}
