/* myserver.cc: sample server program */
#include "server.h"
#include "connection.h"
#include "connectionclosedexception.h"
#include "database.h"
#include "news.h"
#include "protocol.h"

#include <memory>
#include <iostream>
#include <string>
#include <ctime>
#include <stdexcept>
#include <cstdlib>
#include <memory>

void initialize_db(Database& db){
  news::Newsgroup n("a", 3, time(0));
  news::Article a("title", "Author", "content", 1);
  news::Article b("title1", "Author", "content", 2);
  news::Article c("title2", "Author", "content", 2);
  db.add_newsgroup("std::yalla");
  std::cout << n.add(a) << std::endl;
  std::cout << n.add(c) << std::endl;
  std::cout << n.add(b) << std::endl;
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
            std::vector<news::Newsgroup> ngs = db.list_newsgroup();
            write_ans_list_ng(conn, ngs);
          }
          break;
        case Protocol::COM_CREATE_NG:
          break;
        case Protocol::COM_DELETE_NG:
          break;
        case Protocol::COM_LIST_ART: {
          int ng_id = read_com_list_art(conn);
          break;
        }
        case Protocol::COM_CREATE_ART: {
          break;
        }
        case Protocol::COM_DELETE_ART: {
          break;
        }
        case Protocol::COM_GET_ART:{
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
