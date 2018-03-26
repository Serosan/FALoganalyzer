#ifndef FAULTPROCESSANALYSER_H
#define FAULTPROCESSANALYSER_H
#include "analyzerinterface.h"
#include "ContentMatcher.h"
#include <QString>
#include <QStringList>
#include <QTableWidget>
#include <QTreeWidget>
typedef struct faultmsg_str
{
    long fault_id;
    long trip_time;
    QString content;
}faultmsg;
class FaultProcessMenuElement
{
public:
    FaultProcessMenuElement(int v_current, int v_mode=-1, int v_need=-1, int v_index=-1, int v_child=-1, int v_next=-1, int v_parent=-1);
    FaultProcessMenuElement(const FaultProcessMenuElement& other);
    ~FaultProcessMenuElement();
    inline int GetNext();
    inline int GetChild();
    inline int GetIndex();
    inline int GetID();
    inline int SetMode(int );
    inline int SetChild(int );
    inline int SetIndex(int );
    inline int SetNext(int );
    inline int SetPostion(int);
    inline int GetPostion();
    QString GetString(int index, int magic_no, int nbr, map<int, map<int, vector<m_qlist> > > &datamap);
private:
    int current;
    int parent;
    int child;
    int mode;
    int index;
    int postion;
    int next;
};
class FaultProcessMenu
{
public:
    FaultProcessMenu(const map<int ,map<int,vector<m_qlist> > >& data);
    ~FaultProcessMenu();
    int BuildInner(int magic_no, int num, QTreeWidgetItem *parent);
    QTreeWidgetItem* GetWidget(const QDomElement &element, int start);
    int BuildUpElement();
    int BuildMenuRefer(const QDomElement& element, int &this_no);
private:
    int BuildMenuStruct(int & this_no, const QDomElement& docElem);
    map<int,FaultProcessMenuElement> elementmap;
    map<int ,map<int,vector<m_qlist> > > datamap;
};
class FaultProcessAnalyser:public AnalyzerInterface
{
public:
    FaultProcessAnalyser();
    ~FaultProcessAnalyser();
    QStringList GetLogFileNameList(const char* path);
    int StartAnalysis(const char *filename);
    const QString& GetLogData(int dataid,int y);
    QWidget *InitContentWidget(QWidget* parent);
    int SplitFile(const char *filename);
    int SetTableInfo(QTableWidget* tablep);
    int ChangePageInfo(int index);
protected:
//    int InitWidget(QTreeWidget* treewidget, int start);
    QWidget* InitBaseInfoContent();
    QWidget* InitConfigInfoContent();
    QWidget* InitTopoInfoContent();
    QWidget* InitGLInfoContent();
    QWidget* InitTranInfoContent();
    QTreeWidgetItem *InitTimeConsumeInfoContent();
    QWidget* InitAreaDevInfoContent();
    QWidget* InitDBInfoContent();
    QWidget* InitAutoRunInfoContent();
    QWidget* InitAbrInfoContent();
    std::map<int,std::map<int,std::string> > logdata;
    char logindex[10][40]={
        "基础数据",
        "配置信息",
        "拓扑及保护",
        "隔离异常信息",
        "转供方案",
        "耗时记录",
        "区域设备信息",
        "存入实时库情况",
        "自动执行情况",
        "异常信息显示"
    };
private:
    map<int ,map<int,vector<m_qlist> > > resultmap;
    map<long,map<int,QString > > faultmap;
    map<long,map<int,int> > maptovec;
    vector< map<int ,map<int,vector<m_qlist> > > > resultvec;
    ContentMatcher* matcher;
    FaultProcessMenu* menuHelper;

};

#endif // FAULTPROCESSANALYSER_H
