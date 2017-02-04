#include "news.h"
#include "database.h"
#include <iostream>
#include <ctime>
#include <iostream>
#include <vector>


using namespace std;
using namespace news;

int main(){
       Database* db = new Database();
       Newsgroup n("a", 3, time(0));
       Article a("title", "Author", "content", 1);
       Article b("title1", "Author", "content", 2);
       Article c("title2", "Author", "content", 2);
       db->add_newsgroup("yalla");
       cout << n.add(a) << endl;
       cout << n.add(c) << endl;
       cout << n.add(b) << endl;
       vector<Article> v = n.to_list();
        

       for(Article& a: v){
        cout << a.title<<endl;
        //cout << "Title " + v.title + " Author " + v.author + " Content " + content << endl;
       }
       for(Article& a: v){
        cout << a.id<<endl;
        //cout << "Title " + v.title + " Author " + v.author + " Content " + content << endl;
       }
        cout << "Testing if newsgroup can be found : " << db->exists("yalla") << endl;


}
