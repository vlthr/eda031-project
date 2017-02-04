#include "news.h"
#include <iostream>
#include <ctime>
#include <iostream>
#include <vector>
using namespace std;
using namespace news;
int main(){
       cout << "hej " << endl;
       Newsgroup n("a", 3, time(0));
       Article a("title", "Author", "content", 1);
       Article b("title1", "Author", "content", 2);
       Article c("title2", "Author", "content", 2);
       cout << n.add(a);
       cout << n.add(c);
       cout << n.add(b);
       vector<Article> v = n.to_list();


       for(Article& a: v){
        cout << a.title<<endl;
        //cout << "Title " + v.title + " Author " + v.author + " Content " + content << endl;
       }




}
