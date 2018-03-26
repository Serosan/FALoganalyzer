#ifndef CONTENTMATCHER_H
#define CONTENTMATCHER_H
#include <QString>
#include <QRegExp>
#include <QDomDocument>
#include <QFile>
#include <map>
#include <vector>
#include <iostream>
using namespace std;
typedef struct matcher_struct
{
    char text[200];
    int loop_flag;
    int next_start;
    int mark;
    int magic_no;
    int former_no;
    matcher_struct *next_p;
    matcher_struct *child_p;
    matcher_struct *parent_p;
}matcher;
typedef struct magic_list
{
    int magic_no;
    QStringList body;
}m_qlist;
class ContentMatcher
{
public:
    ContentMatcher(const char* configfile);
    ~ContentMatcher();
    int MatchStart(const QString &inputcontent);
    int GetMapInfo(map<int, map<int, vector<m_qlist> > > &des_map);
private:
    int MatchContent(const QString& content,const int& from,const int& to,matcher* matcher_p);
    int InsertMatchResult(int mark, const  QRegExp& re, int magic_no, int former_no);
    int DelMatcher(matcher *matcher_point);
    matcher * BuildMatcherStruct(QDomElement& docElem,matcher * mymathcer);
    matcher* matcher_head;
    QString filename;
    map<int ,map<int,vector<m_qlist> > > resultmap;
    QString content;

};

#endif // CONTENTMATCHER_H
