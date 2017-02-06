#include "news.h"
#include "database.h"
#include <iostream>
#include <stdexcept>
#include <ctime>
#include <iostream>
#include <vector>
#include <sqlite3.h>
#include <sstream>
#include "sqldb.h"
void printInfo(){
    std::cout << "1 Create grp \n2 Delete grp \n3 List grp \n4 Create Article\n5 Exists \n6 Delete Article \n7 List Articles \n8 Get Article\n9 Clear Database\n0 Exit" << std::endl;
    std::cout << "----------------------------------------------\nDatabase$ ";
}
void printAction(int cmd){
    std::cout << cmd << " selected " << std::endl;
}
int readInt(){
    int a;
    if(std::cin >> a){
        std::cin.ignore(100000, '\n');
        return a;
    }else{
        throw std::invalid_argument("Invalid Input Type");
    }
}
void linebreak(){
    std::cout << "==============================================\n" << std::endl;
}
void printResult(bool b){
    if(b){
        std::cout << "Success" << std::endl;
    }else{

        std::cout << "Failure" << std::endl;
    }
}
int main(){
    Sqldb *sqldb = new Sqldb("Database.db");
    int cmd;
    printInfo();


    while((cmd = readInt())){ 
        linebreak();
        printAction(cmd);
        switch(cmd){
            case 9:
                std::cout << "Clearing Database" << std::endl;
                system("cp Database.template Database.db");
                delete sqldb;
                sqldb = new Sqldb("Database.db");
                break;
            case 1:
                {
                    std::cout << "Enter name" << std::endl;
                    std::string n; 
                    std::getline(std::cin, n);
                    sqldb->create_newsgroup(n);
                }
                break;
            case 2:
                {

                    std::cout << "Enter id" << std::endl;
                    int id = readInt();
                    printResult(sqldb->delete_newsgroup(id));
                }
                break;
            case 3:
                {
                    std::vector<std::pair<int, std::string>> ls = sqldb->list_newsgroups();
                    std::cout << "Amount of newsgroups: " << ls.size() << std::endl;
                    for(auto p: ls){
                        std::cout << p.first << " " << p.second << std::endl;
                    }
                }
                break;
            case 4:
                {
                    std::string title, author, content;
                    std::cout << "Enter title " << std::endl;
                    std::getline(std::cin, title);
                    std::cout << "Enter author " << std::endl;
                    std::getline(std::cin, author);
                    std::cout << "Enter content " << std::endl;
                    std::getline(std::cin, content);
                    std::cout << "Enter newsgroup ID " << std::endl;
                    int id = readInt();
                    bool res = sqldb->create_article(id, title, author, content);
                    if(res){
                        std::cout << "Article Succesfully created"  << std::endl;
                    }else{

                        std::cout << "Failed to Create Article, most likely invalid newsgroup." << std::endl;
                    }
                }

                break;

            case 5:
                {

                    std::cout << "Enter name of newsgroup" <<  std::endl;
                    std::string k;
                    std::getline(std::cin, k);
                    if( sqldb->exists(k)){
                        std::cout<< "Group found" << std::endl;
                    }else{
                        std::cout<< "Couldn't find the group" << std::endl;
                    }
                }

                break;

            case 6:
                {
                    std::cout << "Enter newsgrp id " << std::endl;
                    int ng_id = readInt();
                    std::cout << "Enter art id " << std::endl;
                    int art_id = readInt();
                    printResult(sqldb->delete_article(ng_id, art_id) );
                }

                break;
            case 7:
                {
                    std::cout << "Enter newsgroup to list articles from " << std::endl;
                    int ng = readInt();
                    try{
                        std::vector<news::Article> ls = sqldb->list_articles(ng);
                        for(news::Article a: ls){
                            std::cout << "Id: " <<a.id << "\nAuth: " << a.author << "\nTitle: " << a.title << "\nContent:\n" << a.content << std::endl;
                        }
                    }catch(const std::invalid_argument& ia){
                        std::cout << "No such grp"<< std::endl;

                    }

                }
                break;
            case 8:
                {
                    std::cout << "Enter newsgroup id " << std::endl;
                    int a_id = readInt();
                    std::cout << "Enter article id " << std::endl;
                    int ng = readInt();
                    news::Article a = sqldb->get_article(a_id, ng);
                    std::cout << "Id: " <<a.id << "\nAuth: " << a.author << "\nTitle: " << a.title << "\nContent:\n" << a.content << std::endl;

                }
                break;

            default:
                std::cout << "Not recognized command " << std::endl;


        }
        linebreak();
        printInfo();
    }

}
