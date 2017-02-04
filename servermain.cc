/* myserver.cc: sample server program */
#include "server.h"
#include "connection.h"
#include "connectionclosedexception.h"

#include <memory>
#include <iostream>
#include <string>
#include <ctime>
#include <stdexcept>
#include <cstdlib>
#include <memory>
#include "news.h"
#include "connection.h"
#include "protocol.h"

using namespace std;

int main(int argc, char* argv[]){
	if (argc != 2) {
		cerr << "Usage: myserver port-number" << endl;
		exit(1);
	}
	int port = -1;
	try {
		port = stoi(argv[1]);
	} catch (exception& e) {
		cerr << "Wrong port number. " << e.what() << endl;
		exit(1);
	}
	Server server(port);
	if (!server.isReady()) {
		cerr << "Server initialization error." << endl;
		exit(1);
	}
	while (true) {
		auto conn = server.waitForActivity();
		if (conn != nullptr) {
			try {
        int type = conn->read();
        read_com_list_ng(conn);
        std::vector<news::Newsgroup> ngs;
        ngs.push_back(news::Newsgroup("newsgroup alpha", 0, std::time(0)));
        write_ans_list_ng(conn, ngs);
			} catch (ConnectionClosedException&) {
				server.deregisterConnection(conn);
				cout << "Client closed connection" << endl;
			}
		} else {
			conn = make_shared<Connection>();
			server.registerConnection(conn);
			cout << "New client connects" << endl;
		}
	}
}
