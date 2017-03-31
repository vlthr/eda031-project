#ifndef NEWS_H
#define NEWS_H
#include <ctime>
#include <vector>
#include <iostream>
#include <string>
#include <set>

namespace news{
    class Article{
        public:
            Article();
            Article(std::string, std::string, std::string, unsigned int);
            ~Article();
            std::string title;
            std::string author;
            std::string content;
            unsigned int id;
            std::time_t created;
            bool operator<(const Article&) const;
            bool operator==(const Article&) const;
            bool operator==(const unsigned int) const;
            friend std::istream& operator>>(std::istream& input, Article& a);

    };
    class Newsgroup{ 
        public:
            Newsgroup();
            Newsgroup(std::string, unsigned int id, std::time_t);
            ~Newsgroup();
            std::string name;
            unsigned int id;
            std::time_t created;
            bool add(std::string,std::string,std::string);
            Article* get(unsigned int);
            
            bool operator<(const Newsgroup&) const;
            bool operator==(const Newsgroup&) const;
            bool operator==(const unsigned int) const;
            friend std::istream& operator>>(std::istream&  input, Newsgroup& n);


                  /* sorts the list of articles */

            void sort();

            /* return list of articles
             * @return: articles listed in current order
             */

            std::vector<Article>& to_list();

            /* delete an article
             * @param: id of article to be deleted 
             */
            bool del(unsigned int id);
        private:
            std::vector<Article> articles; 
            int id_ctr;
    };
     
std::istream& operator>>(std::istream& is, Article& a);
std::istream& operator>>(std::istream& is, Newsgroup& n);
}

#endif
