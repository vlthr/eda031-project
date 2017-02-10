/* myserver.cc: sample server program */
#include "server.h"
#include "connection.h"
#include "connectionclosedexception.h"
#include "database.h"
#include "news.h"
#include "protocol.h"
#include "protocolexception.h"

#include <memory>
#include <iostream>
#include <string>
#include <ctime>
#include <stdexcept>
#include <cstdlib>
#include <memory>

void initialize_db(Database& db){
  news::Newsgroup n("a", 3, time(0));
  db.create_newsgroup("SQL Jokes");
  db.create_article(0, "A SQL Query walks into a bar.", "IT Guy", "In the corner of the bar are two tables. The query walks up to the tables and asks: Mind if I join you?");
  db.create_article(0, "SELECT * FROM [Users] WHERE [CLUE] > 0", "", "No records found.");
  std::vector<news::Article> v = n.to_list();

  for(news::Article& a: v){
    std::cout << a.title<<std::endl;
    //std::cout << "Title " + v.title + " Author " + v.author + " Content " + content << std::endl;
  }
  for(news::Article& a: v){
    std::cout << a.id<<std::endl;
    //std::cout << "Title " + v.title + " Author " + v.author + " Content " + content << std::endl;
  }
  std::cout << "Testing if newsgroup can be found : " << db.exists("std::yalla") << std::endl;
}

int main(int argc, char* argv[]){
  Database db;
  initialize_db(db);
	if (argc != 2) {
		std::cerr << "Usage: myserver port-number" << std::endl;
    exit(1);
	}
	int port = -1;
	try {
		port = std::stoi(argv[1]);
	} catch (std::exception& e) {
		std::cerr << "Wrong port number. " << e.what() << std::endl;
		exit(1);
	}
	Server server(port);
	if (!server.isReady()) {
		std::cerr << "Server initialization error." << std::endl;
		exit(1);
	}
	while (true) {
		auto conn = server.waitForActivity();
		if (conn != nullptr) {
			try {
        int type = conn->read();
        switch (type) {
        case Protocol::COM_LIST_NG: {
            read_com_list_ng(conn);
            auto ngs = db.list_newsgroups();
            write_ans_list_ng(conn, ngs);
          }
          break;
        case Protocol::COM_CREATE_NG:{
          std::string name = read_com_create_ng(conn);
          if (db.create_newsgroup(name)){
            write_ack(conn, Protocol::ANS_CREATE_NG);
          } else {
            write_nak(conn, Protocol::ANS_CREATE_NG, Protocol::ERR_NG_ALREADY_EXISTS);
          }
          break;
        }
        case Protocol::COM_DELETE_NG: {
          int ng_id = read_com_delete_ng(conn);
          if (db.delete_newsgroup(ng_id)){
            write_ack(conn, Protocol::ANS_DELETE_NG);
          } else {
            write_nak(conn, Protocol::ANS_DELETE_NG,Protocol::ERR_NG_DOES_NOT_EXIST);
          }
          break;
        }
        case Protocol::COM_LIST_ART: {
          int ng_id = read_com_list_art(conn);
          try {
            std::vector<news::Article> articles = db.list_articles(ng_id);
            write_ans_list_art(conn, articles);
          }
          catch (const std::invalid_argument& e) {
            write_nak(conn, Protocol::ANS_LIST_ART, Protocol::ERR_NG_DOES_NOT_EXIST);
          }
          break;
        }
        case Protocol::COM_CREATE_ART: {
          auto art = read_com_create_art(conn);
          bool created = db.create_article(std::get<0>(art), std::get<1>(art), std::get<2>(art),std::get<3>(art));
          if (created) {
            write_ack(conn, Protocol::ANS_CREATE_ART);
          } else {
            write_nak(conn, Protocol::ANS_CREATE_ART,Protocol::ERR_NG_DOES_NOT_EXIST);
          }
          break;
        }
        case Protocol::COM_DELETE_ART: {
          auto art = read_com_delete_art(conn);
          bool deleted = db.delete_article(std::get<0>(art), std::get<1>(art));
          if (deleted) {
            write_ack(conn, Protocol::ANS_DELETE_ART);
          } else {
            write_nak(conn, Protocol::ANS_DELETE_ART,Protocol::ERR_ART_DOES_NOT_EXIST);
          }
          break;
        }
        case Protocol::COM_GET_ART:{
          auto request = read_com_get_art(conn);
          try {
            auto art = db.get_article(request.first, request.second);
            write_ans_get_art(conn, std::make_tuple(art.title, art.author, art.content));
          }
          catch (const std::invalid_argument& e) {
            write_nak(conn, Protocol::ANS_GET_ART, Protocol::ERR_ART_DOES_NOT_EXIST);
          }
          break;
        }
        default: {
          // TODO: BREAK
          break;
        }
        }
			} catch (ConnectionClosedException&) {
				server.deregisterConnection(conn);
				std::cout << "Client closed connection" << std::endl;
			}
		} else {
			conn = std::make_shared<Connection>();
			server.registerConnection(conn);
			std::cout << "New client connects" << std::endl;
		}
	}
}
