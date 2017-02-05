#include "news.h"
#include "database.h"
#include <iostream>
#include <ctime>
#include <iostream>
#include <vector>
#include <sqlite3.h>
#include <sstream>
#include "sqldb.h"

int main(){
        Sqldb sqldb("Database.db");
        int cmd;


        std::cout << "1 Create grp \n2 Delete grp \n3 List grp \n4 Create Article\n5 Exists \n6 Delete Article \n7 List Articles (newsgroup id) " << std::endl;

        while(std::cin >> cmd){ 
        
       
            switch(cmd){
                case 1:
                    {
                    std::cout << "Enter name" << std::endl;
                    std::string n = "";
                    std::cin >> n;
                    sqldb.create_newsgroup(n);
                    }
                    break;
                case 2:
                    {

                    std::cout << "Enter id" << std::endl;
                    int id;
                    std::cin >> id;
                    sqldb.delete_newsgroup(id);
                    }
                    break;
                case 3:
                    {
                    std::vector<std::pair<int, std::string>> ls = sqldb.list_newsgroups();
                    std::cout << "Amount of newsgroups: " << ls.size() << std::endl;
                    for(auto p: ls){
                    std::cout << p.first << " " << p.second << std::endl;
                    }
                    }
                    break;
                case 4:
                    {
                    std::string title, author, content;
                    int id;
                    std::cout << "Enter title " << std::endl;
                    std::cin >> title;
                    std::cout << "Enter author " << std::endl;
                    std::cin >> author;
                    std::cout << "Enter content " << std::endl;
                    std::cin >> content;
                    std::cout << "Enter newsgroup ID " << std::endl;
                    std::cin >> id;
                    bool res = sqldb.create_article(id, title, author, content);
                    std::cout << "Creation response(1 or 0): " << res << std::endl;
                    std::cout << title << author << content << id << std::endl;
                    }

                    break;

                case 5:
                    {
                    std::string k;
                    std::cin >> k;
                    std::cout << sqldb.exists(k);
                    }

                    break;

                case 6:
                    {
                    std::cout << "Enter newsgrp id " << std::endl;
                    int ng_id;
                    std::cin >> ng_id;


                    std::cout << "Enter art id " << std::endl;
                    int art_id;
                    std::cin >> art_id;
                    std::cout << sqldb.delete_article(ng_id, art_id);
                    }

                    break;
                case 7:
                    {
                    std::cout << "Enter newsgroup to list articles from " << std::endl;
                    int ng;
                    std::cin >> ng;
                    std::vector<news::Article> ls = sqldb.list_articles(ng);
                    for(news::Article a: ls){
                    std::cout << "Id: " <<a.id << "\nAuth: " << a.author << "\nTitle: " << a.title << "\nContent:\n" << a.content << std::endl;

                    }

                    }
                    break;

                default:
                    std::cout << "Not recognized command " << std::endl;


            }
        std::cout << "\n\n";

        std::cout << "1 Create grp \n2 Delete grp \n3 List grp \n4 Create Article\n5 Exists \n6 Delete Article(newsgrp id, art id) \n7 List Articles (newsgroup id) " << std::endl;
        }

}
