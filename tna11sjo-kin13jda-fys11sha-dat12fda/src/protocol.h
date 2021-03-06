#include "news.h"
#include <memory>
/*
 * Class Protocol: definitions of command and error codes
 * for the messaging protocol used in the news system project,
 * C++ Programming course.
 */

#ifndef PROTOCOL_H
#define PROTOCOL_H

struct Protocol {
  enum {
    /* Command codes, client -> server */
    COM_LIST_NG    = 1, // list newsgroups
    COM_CREATE_NG  = 2, // create newsgroup
    COM_DELETE_NG  = 3, // delete newsgroup
    COM_LIST_ART   = 4, // list articles
    COM_CREATE_ART = 5, // create article
    COM_DELETE_ART = 6, // delete article
    COM_GET_ART    = 7, // get article
    COM_END        = 8, // command end
    
    /* Answer codes, server -> client */
    ANS_LIST_NG    = 20, // answer list newsgroups
    ANS_CREATE_NG  = 21, // answer create newsgroup
    ANS_DELETE_NG  = 22, // answer delete newsgroup
    ANS_LIST_ART   = 23, // answer list articles
    ANS_CREATE_ART = 24, // answer create article
    ANS_DELETE_ART = 25, // answer delete article
    ANS_GET_ART    = 26, // answer get article
    ANS_END        = 27, // answer end
    ANS_ACK        = 28, // acknowledge
    ANS_NAK        = 29, // negative acknowledge
    
    /* Parameters */
    PAR_STRING = 40, // string
    PAR_NUM    = 41, // number

    /* Error codes */
    ERR_NG_ALREADY_EXISTS  = 50, // newsgroup already exists
    ERR_NG_DOES_NOT_EXIST  = 51, // newsgroup does not exist
    ERR_ART_DOES_NOT_EXIST = 52,  // article does not exist
    ERR_UNEXPECTED_BYTE = 53  // unexpected byte
  };
};

std::string protocol_to_str(int proto);
void expect(std::shared_ptr<Connection>& conn, char byte);
int read_int(std::shared_ptr<Connection>& conn);
void write_int(std::shared_ptr<Connection>& conn, int value);
void write_num_p(std::shared_ptr<Connection>& conn, int num);
int read_num_p(std::shared_ptr<Connection>& conn);
void write_string_p(std::shared_ptr<Connection>& conn, std::string str);
std::string read_string_p(std::shared_ptr<Connection>& conn);
void write_com_get_art(std::shared_ptr<Connection>& conn, int group_id, int article_id);
std::pair<int, int> read_com_get_art(std::shared_ptr<Connection>& conn);
void write_ans_get_art(std::shared_ptr<Connection>& conn, const std::tuple<std::string, std::string, std::string>& art);
std::tuple<std::string, std::string, std::string> read_ans_get_art(std::shared_ptr<Connection>& conn);
void write_com_list_ng(std::shared_ptr<Connection>& conn);
void read_com_list_ng(std::shared_ptr<Connection>& conn);
void write_ans_list_ng(std::shared_ptr<Connection>& conn, const std::vector<std::pair<int, std::string>>& pairs);
std::vector<news::Newsgroup> read_ans_list_ng(std::shared_ptr<Connection>& conn);
void write_com_create_ng(std::shared_ptr<Connection>& conn, std::string& name);
std::string read_com_create_ng(std::shared_ptr<Connection>& conn);
void write_com_delete_ng(std::shared_ptr<Connection>& conn, int id);
int read_com_delete_ng(std::shared_ptr<Connection>& conn);
void write_com_list_art(std::shared_ptr<Connection>& conn, int ng_id);
int read_com_list_art(std::shared_ptr<Connection>& conn);
void write_ans_list_art(std::shared_ptr<Connection>& conn, const std::vector<news::Article>& articles);
void write_nak(std::shared_ptr<Connection>& conn,int ans_type, int err_type);
void write_ack(std::shared_ptr<Connection>& conn, int type);
std::vector<std::pair<int, std::string>> read_ans_list_art(std::shared_ptr<Connection>& conn);
void read_ack(std::shared_ptr<Connection>& conn, int expectType);
void write_com_create_art(std::shared_ptr<Connection>& conn, int ng_id, std::string& title, std::string& author, std::string& text);
std::tuple<int, std::string, std::string, std::string> read_com_create_art(std::shared_ptr<Connection>& conn);
void write_com_delete_art(std::shared_ptr<Connection>& conn, int ng_id, int art_id);
std::tuple<int, int> read_com_delete_art(std::shared_ptr<Connection>& conn);

#endif
