#include <string>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <memory>
#include <cstdlib>
#include "server.h"
#include "news.h"
#include "connection.h"
#include "protocol.h"
#include "connectionclosedexception.h"
#include "protocolexception.h"

std::string protocol_to_str(int proto) {
  switch (proto){
  case 1:
    return "COM_LIST_NG";
  case 2:
    return "COM_CREATE_NG";
  case 3:
    return "COM_DELETE_NG";
  case 4:
    return "COM_LIST_ART";
  case 5:
    return "COM_CREATE_ART";
  case 6:
    return "COM_DELETE_ART";
  case 7:
    return "COM_GET_ART";
  case 8:
    return "COM_END";
  case 20:
    return "ANS_LIST_NG";
  case 21:
    return "ANS_CREATE_NG";
  case 22:
    return "ANS_DELETE_NG";
  case 23:
    return "ANS_LIST_ART";
  case 24:
    return "ANS_CREATE_ART";
  case 25:
    return "ANS_DELETE_ART";
  case 26:
    return "ANS_GET_ART";
  case 27:
    return "ANS_END";
  case 28:
    return "ANS_ACK";
  case 29:
    return "ANS_NAK";
  case 40:
    return "PAR_STRING";
  case 41:
    return "PAR_NUM";
  case 50:
    return "ERR_NG_ALREADY_EXISTS";
  case 51:
    return "ERR_NG_DOES_NOT_EXIST";
  case 52:
    return "ERR_ART_DOES_NOT_EXIST";
  case 53:
    return "ERR_UNEXPECTED_BYTE";
  default:
    return "dunno";
  }

}

void protocol_error(int errorType) {
  ProtocolException r;
  r.errType = errorType;
  r.msg = "Protocol Error: " + protocol_to_str(errorType);
  throw r;
}

void expect(std::shared_ptr<Connection>& conn, char byte){
  unsigned char msg = conn->read();
  if (msg != byte) {
    std::stringstream ss;
    ss << "ERROR: Expected [" << std::to_string(byte) << "] but got [" << std::to_string(msg) << "]" << std::endl;
    ProtocolException r;
    r.errType = Protocol::ERR_UNEXPECTED_BYTE;
    r.msg = ss.str();
    throw r;
  }
}

int read_int(std::shared_ptr<Connection>& conn){
	unsigned char byte1 = conn->read();
	unsigned char byte2 = conn->read();
	unsigned char byte3 = conn->read();
	unsigned char byte4 = conn->read();
	return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}

void write_int(std::shared_ptr<Connection>& conn, int value){
  conn->write((value >> 24) & 0xFF);
  conn->write((value >> 16) & 0xFF);
  conn->write((value >> 8) & 0xFF);
  conn->write(value & 0xFF);
}

void write_num_p(std::shared_ptr<Connection>& conn, int num){
  conn->write(Protocol::PAR_NUM);
  write_int(conn, num);
}

int read_num_p(std::shared_ptr<Connection>& conn){
  expect(conn, Protocol::PAR_NUM);
  int n = read_int(conn);
  return n;
}
void write_string_p(std::shared_ptr<Connection>& conn, std::string str){
  conn->write(Protocol::PAR_STRING);
  write_int(conn, str.size());
  for (char& c : str) {
    conn->write(c);
  }
}

std::string read_string_p(std::shared_ptr<Connection>& conn){
  expect(conn, Protocol::PAR_STRING);
  int n = read_int(conn);
  int received = 0;
  std::stringstream ss;
  while (received < n){
    ss << conn->read();
    received += 1;
  }
  return ss.str();
}

void write_get_art(std::shared_ptr<Connection>& conn, int group_id, int article_id){
  conn->write(Protocol::COM_GET_ART);
  write_num_p(conn, group_id);
  write_num_p(conn, article_id);
  conn->write(Protocol::COM_END);
}

void write_ans_get_art(std::shared_ptr<Connection>& conn, const news::Article& art){
  conn->write(Protocol::ANS_GET_ART);
  conn->write(Protocol::ANS_ACK);
  write_string_p(conn, art.title);
  write_string_p(conn, art.author);
  write_string_p(conn, art.content);
  conn->write(Protocol::ANS_END);
}


void write_com_list_ng(std::shared_ptr<Connection>& conn) {
  conn->write(Protocol::COM_LIST_NG);
  conn->write(Protocol::COM_END);
}
void read_com_list_ng(std::shared_ptr<Connection>& conn) {
  expect(conn, Protocol::COM_END);
}

void write_ans_list_ng(std::shared_ptr<Connection>& conn, const std::vector<news::Newsgroup>& ngs) {
  conn->write(Protocol::ANS_LIST_NG);
  write_num_p(conn, ngs.size());
  for (auto& ng : ngs){
    write_num_p(conn, ng.id);
    write_string_p(conn, ng.name);
  }
  conn->write(Protocol::ANS_END);
}
std::vector<news::Newsgroup> read_ans_list_ng(std::shared_ptr<Connection>& conn) {
  //expect(conn, Protocol::ANS_LIST_NG);
  int count = read_num_p(conn);
  std::vector<news::Newsgroup> ngs;
  for (int i = 0; i < count; ++i) {
    news::Newsgroup ng;
    ng.id = read_num_p(conn);
    ng.name = read_string_p(conn);
    ngs.push_back(ng);
  }
  expect(conn, Protocol::ANS_END);
  return ngs;
}

news::Article read_ans_get_art(std::shared_ptr<Connection>& conn) {
  int type = conn->read();
  news::Article art;
  if (type == Protocol::ANS_ACK) {
    art.title = read_string_p(conn);
    art.author = read_string_p(conn);
    art.content = read_string_p(conn);
  }
  else {
    int errType = conn->read();
    protocol_error(errType);
  }
  expect(conn, Protocol::ANS_END);
  return art;
}

void write_com_create_ng(std::shared_ptr<Connection>& conn, std::string& name) {
  conn->write(Protocol::COM_CREATE_NG);
  write_string_p(conn, name);
  conn->write(Protocol::COM_END);
}


std::string read_com_create_ng(std::shared_ptr<Connection>& conn) {
  std::string name = read_string_p(conn);
  expect(conn, Protocol::COM_END);
  return name;
}

void read_ack(std::shared_ptr<Connection>& conn, int expectType) {
  expect(conn, expectType);
  int type = conn->read();
  switch (type){
  case Protocol::ANS_ACK:
    break;
  case Protocol::ANS_NAK:
    int errType = conn->read();
    protocol_error(errType);
    break;
  }
  expect(conn, Protocol::ANS_END);
}

void write_com_delete_ng(std::shared_ptr<Connection>& conn, int id) {
  conn->write(Protocol::COM_DELETE_NG);
  write_num_p(conn, id);
  conn->write(Protocol::COM_END);
}


int read_com_delete_ng(std::shared_ptr<Connection>& conn) {
  int id = read_num_p(conn);
  expect(conn, Protocol::COM_END);
  return id;
}

void write_com_list_art(std::shared_ptr<Connection>& conn, int ng_id) {
  conn->write(Protocol::COM_LIST_ART);
  write_num_p(conn, ng_id);
  conn->write(Protocol::COM_END);
}

int read_com_list_art(std::shared_ptr<Connection>& conn) {
  int ng_id = read_num_p(conn);
  expect(conn, Protocol::COM_END);
  return ng_id;
}

void write_ans_list_art(std::shared_ptr<Connection>& conn, const std::vector<news::Article>& articles) {
  conn->write(Protocol::ANS_LIST_ART);
  conn->write(Protocol::ANS_ACK);
  write_num_p(conn, articles.size());
  for (auto& article : articles){
    write_num_p(conn, article.id);
    write_string_p(conn, article.title);
  }
  conn->write(Protocol::ANS_END);
}

void write_nak(std::shared_ptr<Connection>& conn,int ans_type, int err_type) {
  conn->write(ans_type);
  conn->write(Protocol::ANS_NAK);
  conn->write(err_type);
  conn->write(Protocol::ANS_END);
}

std::vector<std::pair<int, std::string>> read_ans_list_art(std::shared_ptr<Connection>& conn) {
  expect(conn, Protocol::ANS_LIST_ART);
  int type = conn->read();
  std::vector<std::pair<int, std::string>> arts;
  switch (type){
  case Protocol::ANS_ACK: {
    int count = read_num_p(conn);
    for (int i = 0; i < count; ++i) {
      int id = read_num_p(conn);
      std::string title = read_string_p(conn);
      arts.push_back(std::make_pair(id, title));
    }
    break;
  }
  case Protocol::ANS_NAK: {
    int errType = conn->read();
    protocol_error(errType);
    break;
  }
  }
  expect(conn, Protocol::ANS_END);
  return arts;
}

void write_com_create_art(std::shared_ptr<Connection>& conn, int ng_id, std::string& title, std::string& author, std::string& text) {
  conn->write(Protocol::COM_CREATE_ART);
  write_num_p(conn, ng_id);
  write_string_p(conn, title);
  write_string_p(conn, author);
  write_string_p(conn, text);
  conn->write(Protocol::COM_END);
}

std::tuple<int, std::string, std::string, std::string> read_com_create_art(std::shared_ptr<Connection>& conn) {
  int ng_id = read_num_p(conn);
  std::string title = read_string_p(conn);
  std::string author = read_string_p(conn);
  std::string text = read_string_p(conn);
  expect(conn, Protocol::COM_END);
  return std::make_tuple(ng_id, title, author, text);
}

void write_ack(std::shared_ptr<Connection>& conn, int type) {
  conn->write(type);
  conn->write(Protocol::ANS_ACK);
  conn->write(Protocol::ANS_END);
}
