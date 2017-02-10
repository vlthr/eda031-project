#include "connection.h"
#include "protocol.h"
#include "connectionclosedexception.h"
#include "protocolexception.h"

#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>

using namespace std;

int get_user_command(){
  std::cout << "Please select a thingy: " << std::endl;
  for (int i = 1; i <= 8; ++i) {
    std::cout << "\t" << i << ": " << protocol_to_str(i) << std::endl;
  }
  int choice;
  std::cin >> choice;
  if (!std::cin.good()) throw std::invalid_argument("Invalid command.");
  cin.clear();
  cin.ignore(10000, '\n');
  return choice;
}

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

  //int command = Protocol::COM_CREATE_NG;
  while (true){
    try {
      int command = get_user_command();
      switch (command) {
      case Protocol::COM_LIST_NG: {
        write_com_list_ng(conn);
        conn->read();
        std::vector<news::Newsgroup> ngs = read_ans_list_ng(conn);
        for (auto& x : ngs){
          std::cout << x.id << ": "<< x.name << std::endl;
        }
        break;
      }
      case Protocol::COM_CREATE_NG:{
        std::string name;
        std::cout << "Name: ";
        std::getline(std::cin, name);
        write_com_create_ng(conn, name);
        read_ack(conn, Protocol::ANS_CREATE_NG);
        break;
      }
      case Protocol::COM_DELETE_NG: {
        int id = 1;
        write_com_delete_ng(conn, id);
        read_ack(conn, Protocol::ANS_DELETE_NG);
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
        int ng_id;
        std::string title;
        std::string author;
        std::string text;
        std::cout << "Newsgroup ID: ";
        std::cin >> ng_id;
        std::cin.ignore(10000, '\n');
        std::cout << "Title: ";
        std::getline(std::cin, title);
        std::cout << "Author: ";
        std::getline(std::cin, author);
        std::cout << "Text: ";
        std::getline(std::cin, text);
        write_com_create_art(conn, ng_id, title, author, text);
        read_ack(conn, Protocol::ANS_CREATE_ART);
        break;
      }
      case Protocol::COM_DELETE_ART: {
        int ng_id;
        int art_id;
        std::cout << "Newsgroup ID: ";
        std::cin >> ng_id;
        std::cout << "Article ID: ";
        std::cin >> art_id;
        write_com_delete_art(conn, ng_id, art_id);
        break;
      }
      case Protocol::COM_GET_ART: {
        int ng_id;
        int art_id;
        std::cout << "Newsgroup ID: ";
        std::cin >> ng_id;
        std::cout << "Article ID: ";
        std::cin >> art_id;
        write_com_get_art(conn, ng_id, art_id);
        break;
      }
      default:
        std::cout << "Invalid command. Please try again." << std::endl;
        break;
      }
    }
    catch (const ProtocolException& p ){
      std::cout << p.msg << std::endl;
    }
    catch (const std::invalid_argument& e ){
      std::cout << e.what() << std::endl;
			cin.clear();
			cin.ignore(10000, '\n');
    }
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

