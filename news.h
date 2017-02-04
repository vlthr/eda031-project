#ifndef NEWS_H
#define NEWS_H
#include <ctime>
#include <vector>
#include <string>

namespace news{
    class Article{
        public:
            Article();
            Article(std::string, std::string, std::string);
            ~Article();
            std::string title;
            std::string author;
            std::string content;
            unsigned int id;
            std::time_t created;
    };
    class Newsgroup{ 
        public:
            Newsgroup();
            Newsgroup(std::string, unsigned int id, std::time_t);
            ~Newsgroup();
            std::string name;
            unsigned int id;
            std::time_t created;
            bool add(Article);
            // returns a vector with articles listed in chronological order
            std::vector<Article> to_list();
            /* delete an article
             * @param: id of article to be deleted 
             */
            bool del(unsigned int id);
        private:
            std::vector<Article> articles;
    };

}

#endif
