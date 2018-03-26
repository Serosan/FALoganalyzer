#include "faultprocesslog.h"
#include <iostream>
#include <QRegExp>
#include <QTextCodec>
#include <QTreeWidget>
#include <QTextEdit>
#include <QPlainTextEdit>
#include <QLabel>
inline QString GBK2UTF8(const QString &inStr)
{
    QTextCodec *gbk = QTextCodec::codecForName("GB18030");
    QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");

    QString g2u = gbk->toUnicode(gbk->fromUnicode(inStr));			  // gbk  convert utf8
    return g2u;
}

inline QString UTF82GBK(const QString &inStr)
{
    QTextCodec *gbk = QTextCodec::codecForName("GB18030");
    QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");

    QString utf2gbk = gbk->toUnicode(inStr.toLocal8Bit());
    return utf2gbk;
}

inline std::string gbk2utf8(const QString &inStr)
{
    return GBK2UTF8(inStr).toStdString();
}

inline QString utf82gbk(const std::string &inStr)
{
    QString str = QString::fromStdString(inStr);

    return UTF82GBK(str);
}
FaultProcessLog::FaultProcessLog()
{
    StartAnalysis();
    std::vector<fplog_element>::iterator itt;
    for(itt=result_data.begin();itt!=result_data.end();++itt)
    {
        std::cout<<itt->fault_no<<","<<itt->fault_id<<","<<itt->trip_cb<<","<<itt->trip_time<<","<<itt->run_mode<<","<<itt->perform_mode<<","<<itt->start_method<<","<<itt->fault_area_desc.toStdString()<<std::endl;
    }
}
FaultProcessLog::~FaultProcessLog()
{

}
QStringList FaultProcessLog::GetLogFileNameList(const char *path)
{
    QDir dir(path);
    QStringList nameFilters;
    nameFilters << "faultprocess*";// << "*.png";
    QStringList files = dir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);
    return files;
}
int FaultProcessLog::StartAnalysis()
{
    std::map<int,QStringList> resultmap;
    QStringList matchresult;
    int cur_postion=0;
    Pretreat("/Users/sero/Workspace/testdir/faultProcess20180201.log_2");

//    Pretreat("/Users/sero/Workspace/testdir/test.log");
//    for(int loop=0;loop<sizeof(relist)/sizeof(MATCHER);++loop)
//    {
//        matchresult.clear();
//        if(relist[loop].loop_flag==true)
//        {
//            cur_postion=MultiMatch(text,cur_postion,loop,matchresult);
//        }
//        else
//        {
//            cur_postion=SingleMatch(text,cur_postion,loop,matchresult);
//        }
//        resultmap.insert(make_pair(loop,matchresult));
//    }
}
int FaultProcessLog::Pretreat(const char * filepath)
{
    QFile logfile(filepath);
    logfile.open(QIODevice::ReadOnly);
    QString alltext=logfile.readAll();
    QStringList fault_text_list=alltext.split("开始一次新的故障分析过程");
//    int cout=fault_text_list.count();
    int current_p=0;
    QStringList result;
    for(int loop=0;loop<20;++loop)
    {
        if(matcherlist[loop].next_index==0)
        {
            current_p=SingleMatch(fault_text_list[1],current_p,-1,loop,result);
        }
        else
        {
            current_p=MultiMatch(fault_text_list[1],current_p,loop,result);
        }
    }
    for(int i=0;i<result.count();++i)
    {
        std::cout<<result[i].toStdString()<<std::endl;
    }
}
const QString& FaultProcessLog::GetLogData(int dataid,int y)
{
    return logdata[dataid][y];
}
int FaultProcessLog::GetLogMap(std::map<int,std::vector< QString > >& logmap)
{
    logmap=logdata;
}
int FaultProcessLog::MultiMatch(const QString &content_qstr, const int &current, int& loop, QStringList &result)
{
    int current_index=loop;
    int former_index=loop;
    int index=0;
    QRegExp re_next(matcherlist[current_index].retext);
    QRegExp re_local;
    re_local.setMinimal(true);
    re_next.setMinimal(true);
    int run_num=0;
    int next_start=content_qstr.indexOf(re_next,current);
    int current_p=next_start+re_next.cap(0).length();;
    while(current_index)
    {
        if(logdata.find(current_index)==logdata.end())
        {
            std::vector<QString> tempvec;
            logdata.insert(make_pair(current_index,tempvec));
        }
        ++run_num;
        re_local.setPattern(matcherlist[current_index].retext);
        if(matcherlist[current_index].must_flag)
        {
            if(next_start<0)
            {
                std::cout<<"must:next_start"<<next_start<<former_index<<std::endl;
                break;
            }
            else
            {
                for(int cap_loop=0;cap_loop<re_next.captureCount()+1;++cap_loop)
                {
                    logdata[current_index].push_back(re_next.cap(cap_loop));
                }
                result.push_back(re_next.cap(0));
            }
            current_p=next_start+re_next.cap(0).length();
            std::cout<<run_num<<":"<<"next_index="<<current_index<<"current_p="<<current_p<<re_next.cap(0).toStdString()<<std::endl;
            next_start=content_qstr.indexOf(re_next,current_p);
        }
        else
        {
            former_index=current_index;
            index=content_qstr.indexOf(re_local,current_p);
            if(index>0&&index<next_start)
            {
                for(int cap_loop=0;cap_loop<re_local.captureCount()+1;++cap_loop)
                {
                    logdata[current_index].push_back(re_local.cap(cap_loop));
                }
                result.push_back(re_local.cap(0));
                current_p=index+re_local.cap(0).length();
            }
            else if(index>0&&next_start<0)
            {
                for(int cap_loop=0;cap_loop<re_local.captureCount()+1;++cap_loop)
                {
                    logdata[current_index].push_back(re_local.cap(cap_loop));
                }
                result.push_back(re_local.cap(0));
                current_p=index+re_local.cap(0).length();
                break;
            }
            else
            {
                for(int cap_loop=0;cap_loop<re_local.captureCount()+1;++cap_loop)
                {
                    logdata[current_index].push_back("");
                }
            }
            std::cout<<run_num<<":"<<"next_index="<<current_index<<"current_p="<<current_p<<re_local.cap(0).toStdString()<<std::endl;
        }
        current_index=matcherlist[current_index].next_index;
    }
    loop=former_index;
    return current_p;
}
int FaultProcessLog::SingleMatch(const QString &text, const int &from,const int &to, int loop, QStringList &result)
{
    QRegExp localre(matcherlist[loop].retext);
    localre.setMinimal(true);
    if(logdata.find(loop)==logdata.end())
    {
        std::vector<QString> tempvec;
        logdata.insert(make_pair(loop,tempvec));
    }
    int next_p=text.indexOf(localre,from);
    if(to<0&&next_p>0)
    {
        for(int cap_loop=0;cap_loop<localre.captureCount()+1;++cap_loop)
        {
            logdata[loop].push_back(localre.cap(cap_loop));
        }
        result.push_back(localre.cap(0));
        return next_p;
    }
    if(next_p>0&&next_p<to+1)
    {
        for(int cap_loop=0;cap_loop<localre.captureCount()+1;++cap_loop)
        {
            logdata[loop].push_back(localre.cap(cap_loop));
        }
        result.push_back(localre.cap(0));
        return next_p;
    }
    for(int cap_loop=0;cap_loop<localre.captureCount()+1;++cap_loop)
    {
        logdata[loop].push_back("");
    }
    if(matcherlist[loop].must_flag)
    {
        return from;
    }
    else
    {
        return from;
    }
}
QWidget *FaultProcessLog::InitContentWidget(QWidget*parent)
{
    QTreeWidget* content=new QTreeWidget(parent);
    content->setSelectionMode(QAbstractItemView::NoSelection);
    for(int index=0;index<sizeof(logindex)/sizeof(logindex[0]);++index)
    {
        QTreeWidgetItem* item=new QTreeWidgetItem(content);
        QTreeWidgetItem* item_sub;
        item->setText(0,logindex[index]);
        switch (index) {
        case 0:
            item_sub=new QTreeWidgetItem(item);
            content->setItemWidget(item_sub,0,InitBaseInfoContent());
            break;
        case 1:
            item_sub=new QTreeWidgetItem(item);
            content->setItemWidget(item_sub,0,InitConfigInfoContent());
            break;
        case 2:
            item_sub=new QTreeWidgetItem(item);
            content->setItemWidget(item_sub,0,InitTopoInfoContent());
            break;//InitTimeConsumeInfoContent
        case 3:
            item_sub=new QTreeWidgetItem(item);
            content->setItemWidget(item_sub,0,InitGLInfoContent());
            break;//InitTimeConsumeInfoCont
//        case 5:
//            item_sub=new QTreeWidgetItem(item);
//            content->setItemWidget(item_sub,0,InitTimeConsumeInfoContent());
//            break;
//        case 6:
//            item_sub=new QTreeWidgetItem(item);
//            content->setItemWidget(item_sub,0,InitAreaDevInfoContent());
//            break;
//        case 7:
//            item_sub=new QTreeWidgetItem(item);
//            content->setItemWidget(item_sub,0,InitDBInfoContent());
//            break;
//        case 8:
//            item_sub=new QTreeWidgetItem(item);
//            content->setItemWidget(item_sub,0,InitAutoRunInfoContent());
//            break;
        default:
            break;
        }
    }
    return content;
}
QWidget* FaultProcessLog::InitBaseInfoContent()
{
    QString sub_content;
    int index_x,index_y;
    for(int index=1;index<10;++index)
    {
        if(baseindex[index]<0)
        {
            continue;
        }
        index_x=baseindex[index]/100;
        index_y=baseindex[index]%100;
        switch (index) {
        case 6:
            sub_content=sub_content+basename[index];
            if(logdata[index_x][index_y].length()>0)
            {
                sub_content=sub_content+"进程启动";
            }
            else
            {
                sub_content=sub_content+"消息启动";
            }
            sub_content=sub_content+"\n";
            break;
        case 8:
            sub_content=sub_content+basename[index]+":\n"+"force_mm_flag="+logdata[index_x][index_y+1]+"other_info="+logdata[index_x][index_y+1]+"\n";
            break;
        default:
            sub_content=sub_content+basename[index]+logdata[index_x][index_y]+"\n";
            break;
        }
    }
    QTextEdit* subcontent_widget=new QTextEdit();
    subcontent_widget->setText(sub_content);
    return subcontent_widget;
}
QWidget* FaultProcessLog::InitConfigInfoContent()
{
    QString sub_content;
    sub_content=logdata[confindex[0]/100][confindex[0]%100]+"\n";
    QTextEdit* subcontent_widget=new QTextEdit();
    for(int i=0;i<logdata[2].size();++i)
    {
        if(i%2==1)
        {
            sub_content=sub_content+logdata[confindex[2]/100][i]+"\n";
        }
    }
    subcontent_widget->setText(sub_content);
    return subcontent_widget;
}
QWidget* FaultProcessLog::InitTopoInfoContent()
{
    QString sub_content;
    QTextEdit* subcontent_widget=new QTextEdit();
    for(int i=0;i<logdata[8].size()/3;++i)
    {
        for(int j=0;j<sizeof(topoindex)/sizeof(int);++j)
        {
            int num=logdata[topoindex[j]/100].size()/logdata[8].size()*3;
            sub_content=sub_content+logdata[topoindex[j]/100][topoindex[j]%100+i*num]+"\n";
        }
    }
    subcontent_widget->setText(sub_content);
    return subcontent_widget;
}
QWidget* FaultProcessLog::InitGLInfoContent()
{
    QFile logfile("/Users/sero/Workspace/testdir/faultProcess20180201.log_2");
    logfile.open(QIODevice::ReadOnly);
    QString alltext=logfile.readAll();
    QStringList fault_text_list=alltext.split("开始一次新的故障分析过程");
    ContentMatcher mp("/Users/sero/Desktop/mydocument.xml");
    mp.MatchStart(fault_text_list[1]);
}
QWidget* FaultProcessLog::InitTranInfoContent()
{

}
QWidget* FaultProcessLog::InitTimeConsumeInfoContent()
{
    QString sub_content;
    QTextEdit* subcontent_widget=new QTextEdit();
    for(int i=0;i<sizeof(timeindex)/sizeof(int);++i)
    {
        sub_content=sub_content+timename[i]+":"+logdata[timeindex[i]/100][timeindex[i]%100]+"s"+"\n";
    }
    subcontent_widget->setText(sub_content);
    return subcontent_widget;
}
QWidget* FaultProcessLog::InitAreaDevInfoContent()
{
    QString sub_content;
    QTextEdit* subcontent_widget=new QTextEdit();
//    QLabel *subcontent_widget=new QLabel();
    for(int i=0;i<logdata[16].size();++i)
    {
        if(i%2==0)
        {
            sub_content=sub_content+logdata[16][i].mid(1)+"\n";
        }
    }
    subcontent_widget->setText(sub_content);
    return subcontent_widget;
}
QWidget* FaultProcessLog::InitDBInfoContent()
{
    QString sub_content;
    QTextEdit* subcontent_widget=new QTextEdit();
    for(int i=0;i<logdata[18].size();++i)
    {
        if(i%5==0)
        {
            sub_content=sub_content+logdata[18][i].mid(1)+"\n";
        }
    }
    subcontent_widget->setText(sub_content);
    return subcontent_widget;
}
QWidget* FaultProcessLog::InitAutoRunInfoContent()
{
    QString sub_content;
    QTextEdit* subcontent_widget=new QTextEdit();
    if(logdata.find(17)!=logdata.end()&&logdata[17].size()!=0)
    {
        sub_content=sub_content+logdata[17][0].mid(1)+"\n";
    }
    else
    {
        sub_content="NULL";
    }
    subcontent_widget->setText(sub_content);
    return subcontent_widget;
}
QWidget* FaultProcessLog::InitAbrInfoContent()
{

}
int MatchContent(const QString& content,int from,matcher* matcher_p)
{
    QRegExp re("matcher_p->text");
    int ret_code=content.indexOf(re);
    if(ret_code>0)
    {
        ret_code=ret_code+re.cap(0).length();
        if(matcher_p->child_p!=nullptr)
        {
            MatchContent(content,ret_code,matcher_p->child_p);
        }
        else
        {
            if(matcher_p->next_p!=nullptr)
            {
                MatchContent(content,ret_code,matcher_p->next_p);
            }
            else
            {
                if(matcher_p->loop_flag)
                {
                    MatchContent(content,ret_code,matcher_p);
                }
                else
                {
                    return 1;
                }
            }
        }
    }
    else
    {
        return 1;
    }
}
