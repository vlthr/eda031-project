#include "connection.h"
#include "protocol.h"
#include "connectionclosedexception.h"

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
	write_com_list_ng(conn);
  conn->read();
  read_ans_list_ng(conn);
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

