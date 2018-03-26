#include "ContentMatcher.h"


ContentMatcher::ContentMatcher(const char *configfile)
{
    filename=QString(configfile);
    QDomDocument doc("mydocument");
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
        cout<<"!"<<endl;
    if (!doc.setContent(&file))
    {
        file.close();
        cout<<"!"<<endl;
    }
    file.close();
    QDomElement docElem = doc.documentElement();
    matcher_head=BuildMatcherStruct(docElem,nullptr);

}
ContentMatcher::~ContentMatcher()
{
    DelMatcher(matcher_head);
}
int ContentMatcher::DelMatcher(matcher *matcher_point)
{
    if(matcher_point->child_p!=nullptr)
    {
        DelMatcher(matcher_point->child_p);
    }
    else
    {
        if(matcher_point->next_p!=nullptr)
        {
            DelMatcher(matcher_point->next_p);
        }
        else
        {
            delete matcher_point;
        }
    }
}
int ContentMatcher::MatchStart(const QString &inputcontent)
{
    resultmap.clear();
    content=inputcontent;
    MatchContent(content,0,-1,matcher_head);
    cout<<resultmap.size()<<endl;
    return 1;
}
int ContentMatcher::MatchContent(const QString& content,const int& from,const int& to,matcher* matcher_p)
{
    QRegExp re(matcher_p->text);
    matcher_p->magic_no++;
    if(matcher_p->parent_p!=nullptr)
    {
        matcher_p->former_no=matcher_p->parent_p->magic_no;
    }
    re.setMinimal(true);
    int next_start=-1;
    int ret_code=-1;
    ret_code=content.indexOf(re,from);
    cout<<re.pattern().toStdString()<<endl;
    //Æ¥Åäµ½
    cout<<"ret_code"<<ret_code<<"from"<<from<<"to"<<to<<endl;
    if(ret_code>-1&&(to<0||ret_code<to))
    {
        InsertMatchResult(matcher_p->mark,re,matcher_p->magic_no,matcher_p->former_no);
        cout<<"ret_code"<<ret_code<<"from"<<from<<"to"<<to<<endl;
        cout<<re.cap(0).toStdString()<<endl;
        ret_code=ret_code+re.cap(0).length();
        if(matcher_p->child_p!=nullptr)
        {
            if(matcher_p->loop_flag)
            {
                next_start=content.indexOf(re,ret_code);
                if(next_start>0&&(next_start<to||to<0))
                {
                    matcher_p->next_start=next_start;
                }
                else
                {
                    next_start=to;
                    matcher_p->next_start=next_start;
                }
            }
            else
            {
                next_start=to;
                matcher_p->next_start=next_start;
            }
            cout<<"child ex,next_start="<<next_start<<endl;
            MatchContent(content,ret_code,next_start,matcher_p->child_p);
        }
        else
        {
            cout<<"child not ex"<<endl;
            if(matcher_p->loop_flag)//matcher_p->next_p!=nullptr)
            {
                cout<<"loop  ex"<<endl;
                MatchContent(content,ret_code,to,matcher_p);
            }
            else
            {
                cout<<"loop not ex"<<endl;
                if(matcher_p->next_p!=nullptr)
                {
                    cout<<"next  ex"<<endl;
                    MatchContent(content,ret_code,to,matcher_p->next_p);
                }
                else
                {
                    cout<<"next not ex"<<endl;
                    if(matcher_p->parent_p!=nullptr)
                    {
                        cout<<"parent  ex"<<endl;
                        if(matcher_p->parent_p->parent_p!=nullptr)
                        {
                            MatchContent(content,from,matcher_p->parent_p->parent_p->next_start,matcher_p->parent_p);
                        }
                        else
                        {
                            MatchContent(content,from,-1,matcher_p->parent_p);
                        }
                    }
                    else
                    {
                        cout<<"parent  not ex"<<endl;
                        return 1;
                    }
                }
            }
        }
    }
    //Î´Æ¥Åä
    else
    {
        //InsertMatchResult(matcher_p->mark,re,1);
        cout<<"ret_code<0"<<endl;
        if(matcher_p->next_p!=nullptr)
        {
            cout<<"next ex,ret_code="<<ret_code<<"to="<<to<<endl;
            MatchContent(content,from,to,matcher_p->next_p);
        }
        else
        {
            cout<<"next not ex"<<endl;
            if(matcher_p->parent_p!=nullptr)
            {
                cout<<"parent  ex"<<endl;
                if(matcher_p->parent_p->parent_p!=nullptr)
                {
                    MatchContent(content,from,matcher_p->parent_p->parent_p->next_start,matcher_p->parent_p);
                }
                else
                {
                    MatchContent(content,from,-1,matcher_p->parent_p);
                }
            }
            else
            {
                cout<<"parent not ex"<<endl;
                return 1;
            }
        }
    }
}
matcher * ContentMatcher::BuildMatcherStruct(QDomElement& docElem,matcher * mymathcer)
{
    QDomNode node = docElem.firstChild();
    matcher * nematch;
    matcher* former_m=nullptr;
    matcher* initmatcher=nullptr;
    while(!node.isNull())
    {
        QDomElement element = node.toElement(); // try to convert the node to an element.
        if( !element.isNull() )
        {
            if(element.tagName()=="matcher")
            {
                nematch=new matcher();
                nematch->parent_p=mymathcer;
                nematch->child_p=nullptr;
                nematch->next_p=nullptr;
                cout << qPrintable(element.tagName()) << endl ;
                cout << qPrintable(element.text()) << endl ;
                if(former_m!=nullptr)
                {
                    former_m->next_p=nematch;
                }
                else
                {
                    if(mymathcer!=nullptr)
                    {
                        mymathcer->child_p=nematch;
                    }
                    else
                    {
                        initmatcher=nematch;
                    }
                }
                former_m=nematch;
                BuildMatcherStruct(element,nematch);
            }
            else if(element.tagName()=="text")
            {
                cout << qPrintable(element.tagName()) << endl ;
                cout << qPrintable(element.text()) << endl ;
                sprintf(mymathcer->text,element.text().toStdString().c_str());

            }
            else if(element.tagName()=="loop")
            {
                cout << qPrintable(element.tagName()) << endl ;
                cout << qPrintable(element.text()) << endl ;
                if(element.text()=="Y")
                {
                    mymathcer->loop_flag=1;
                }
                else
                {
                    mymathcer->loop_flag=0;
                }
            }
            else if(element.tagName()=="mark")
            {
                mymathcer->mark=element.text().toInt();
            }
        }

        node = node.nextSibling();
    }
    return initmatcher;
}
int ContentMatcher::InsertMatchResult(int mark,const  QRegExp& re,int magic_no,int former_no)
{
    m_qlist qlist;
    vector<m_qlist> temp_vec;
    map<int,vector<m_qlist> > temp_map;
    qlist.magic_no=magic_no;
    qlist.body=re.capturedTexts();
    if(resultmap.find(mark)==resultmap.end())
    {
        resultmap.insert(make_pair(mark,temp_map));
    }
    if(resultmap[mark].find(former_no)==resultmap[mark].end())
    {
        resultmap[mark].insert(make_pair(former_no,temp_vec));
    }
    resultmap[mark][former_no].push_back(qlist);
    return 1;
}
int ContentMatcher::GetMapInfo(map<int ,map<int,vector<m_qlist> > >& des_map)
{
    if(resultmap.size()>0)
    {
        des_map=resultmap;
    }
    return resultmap.size();
}
