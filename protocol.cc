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

void expect(std::shared_ptr<Connection>& conn, char byte){
  unsigned char msg = conn->read();
  if (msg != byte) {
    std::stringstream ss;
    ss << "ERROR: Expected " << byte << " but got " << msg << std::endl;
    auto r =  new ProtocolException();
    r->errType = Protocol::ERR_UNEXPECTED_BYTE;
    r->msg = ss.str();
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

void write_get_article(std::shared_ptr<Connection>& conn, int group_id, int article_id){
  conn->write(Protocol::COM_GET_ART);
  write_num_p(conn, group_id);
  write_num_p(conn, article_id);
  conn->write(Protocol::COM_END);
}

void write_ans_get_article(std::shared_ptr<Connection>& conn, news::Article& art){
  conn->write(Protocol::ANS_GET_ART);
  conn->write(Protocol::ANS_ACK);
  write_string_p(conn, art.title);
  write_string_p(conn, art.author);
  write_string_p(conn, art.content);
  conn->write(Protocol::ANS_END);
}

void write_ans_get_article_err(std::shared_ptr<Connection>& conn, int errType){
  conn->write(Protocol::ANS_GET_ART);
  conn->write(Protocol::ANS_NAK);
  conn->write(errType);
  conn->write(Protocol::ANS_END);
}

news::Article read_ans_get_article(std::shared_ptr<Connection>& conn) {
  int type = conn->read();
  news::Article art;
  if (type == Protocol::ANS_ACK) {
    art.title = read_string_p(conn);
    art.author = read_string_p(conn);
    art.content = read_string_p(conn);
  }
  else {
    int errType = conn->read();
    auto r =  new ProtocolException();
    r->errType = errType;
    r->msg = "Protocol Error";
    throw r;
  }
  expect(conn, Protocol::ANS_END);
  return art;
}


void write_com_list_ng(std::shared_ptr<Connection>& conn) {
  conn->write(Protocol::COM_LIST_NG);
  conn->write(Protocol::COM_END);
}
void read_com_list_ng(std::shared_ptr<Connection>& conn) {
  expect(conn, Protocol::COM_END);
}

void write_ans_list_ng(std::shared_ptr<Connection>& conn, std::vector<news::Newsgroup>& ngs) {
  conn->write(Protocol::ANS_LIST_NG);
  write_num_p(conn, ngs.size());
  for (auto& ng : ngs){
    write_num_p(conn, ng.id);
    write_string_p(conn, ng.name);
  }
  conn->write(Protocol::ANS_END);
}
std::vector<news::Newsgroup> read_ans_list_ng(std::shared_ptr<Connection>& conn) {
  expect(conn, Protocol::ANS_LIST_NG);
  int count = read_num_p(conn);
  std::vector<news::Newsgroup> ngs;
  for (int i = 0; i < count; ++i) {
    news::Newsgroup ng;
    ng.id = read_num_p(conn);
    ng.name = read_string_p(conn);
    ngs.push_back(ng);
  }
  expect(conn, Protocol::ANS_END);
}

news::Article read_ans_list_art(std::shared_ptr<Connection>& conn) {
  int type = conn->read();
  news::Article art;
  if (type == Protocol::ANS_ACK) {
    art.title = read_string_p(conn);
    art.author = read_string_p(conn);
    art.content = read_string_p(conn);
  }
  else {
    int errType = conn->read();
    auto r =  new ProtocolException();
    r->errType = errType;
    r->msg = "Protocol Error";
    throw r;
  }
  expect(conn, Protocol::ANS_END);
  return art;
}
