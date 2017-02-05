#include "connection.h"
#include "protocol.h"
#include "connectionclosedexception.h"
#include "protocolexception.h"

#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>

using namespace std;

int main(int argc, char* argv[]) {
	if (argc != 3) {
		cerr << "Usage: myclient host-name port-number" << endl;
		exit(1);
	}

	int port = -1;
	try {
		port = stoi(argv[2]);
	} catch (exception& e) {
		cerr << "Wrong port number. " << e.what() << endl;
		exit(1);
	}

  auto conn = std::make_shared<Connection>(argv[1], port);
	if (!conn->isConnected()) {
		cerr << "Connection attempt failed" << endl;
		exit(1);
	}

	cout << "Sending ls...\n";
  int command = Protocol::COM_LIST_NG;
  try {
    switch (command) {
    case Protocol::COM_LIST_NG: {
      write_com_list_ng(conn);
      conn->read();
      std::vector<news::Newsgroup> ngs = read_ans_list_ng(conn);
      for (auto& x : ngs){
        std::cout << x.name << std::endl;
      }
      break;
    }
    case Protocol::COM_CREATE_NG:{
      std::string name = "new ng";
      write_com_create_ng(conn, name);
      read_ans_create_ng(conn);
      break;
    }
    case Protocol::COM_DELETE_NG: {
      int id = 1;
      write_com_delete_ng(conn, id);
      read_ans_delete_ng(conn);
      break;
    }
    case Protocol::COM_LIST_ART: {
      int ng_id = 1;
      write_com_list_art(conn, ng_id);
      auto arts = read_ans_list_art(conn);
      for (auto& pair : arts) {
        std::cout << pair.first << ": " << pair.second << std::endl;
      }
      break;
    }
    case Protocol::COM_CREATE_ART: {
      break;
    }
    case Protocol::COM_DELETE_ART: {
      break;
    }
    case Protocol::COM_GET_ART: {
      break;
    }
    }
  }
  catch (const ProtocolException& p){
    std::cout << "ERROR:" << std::endl;
    std::cout << p.msg << std::endl;
  }
	// int nbr;
	// while (cin >> nbr) {
	// 	try {
	// 		cout << nbr << " is ...";
	// 		writeNumber(conn, nbr);
	// 		string reply = readString(conn);
	// 		cout << " " << reply << endl;
	// 		cout << "Type another number: ";
	// 	} catch (ConnectionClosedException&) {
	// 		cout << " no reply from server. Exiting." << endl;
	// 		exit(1);
	// 	}
	// }
}

