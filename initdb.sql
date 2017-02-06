PRAGMA foreign_keys=OFF;
DROP TABLE IF EXISTS articles;
DROP TABLE IF EXISTS newsgroups;
PRAGMA foreign_keys=ON;
CREATE TABLE newsgroups(name CHAR(60) PRIMARY KEY, created BIGINT);
CREATE TABLE articles(title CHAR(60), author CHAR(60), content LONGTEXT, newsgroup CHAR(60) NOT NULL, created BIGINT, FOREIGN KEY(newsgroup) REFERENCES newsgroups(newsgroup));
