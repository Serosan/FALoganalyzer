#include "FaultProcessAnalyser.h"
#include <QDir>
#include <QTreeWidget>
#include <QTextEdit>
#include <QTextStream>
#include <QTreeWidgetItem>
#include <QTime>
FaultProcessAnalyser::FaultProcessAnalyser()
{
    matcher=new ContentMatcher("/Users/sero/Desktop/mydocument.xml");
    //暂时放在这里
}
FaultProcessAnalyser::~FaultProcessAnalyser()
{

}
QStringList FaultProcessAnalyser::GetLogFileNameList(const char *path)
{
    QDir dir(path);
    QStringList nameFilters;
    nameFilters << "faultprocess*";
    QStringList files = dir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);
    return files;
}
int FaultProcessAnalyser::StartAnalysis(const char * filename)
{
    SplitFile(filename);
    map<long,map<int,QString> >::iterator fault_it;
    map<int,QString>::iterator time_it;
    int index=0;
    for(fault_it=faultmap.begin();fault_it!=faultmap.end();++fault_it)
    {
        if(maptovec.find(fault_it->first)==maptovec.end())
        {
            map<int,int> tempmap;
            maptovec.insert(make_pair(fault_it->first,tempmap));
        }
        for(time_it=fault_it->second.begin();time_it!=fault_it->second.end();++time_it)
        {
            matcher->MatchStart(time_it->second);
            matcher->GetMapInfo(resultmap);
            resultvec.push_back(resultmap);
            if(maptovec[fault_it->first].find(time_it->first)==maptovec[fault_it->first].end())
            {
                maptovec[fault_it->first].insert(make_pair(time_it->first,index));
                cout<<"insert"<< fault_it->first<<","<<time_it->first<<","<<index<<endl;
            }
            index++;
        }
    }
    menuHelper=new FaultProcessMenu(resultmap);
    menuHelper->BuildUpElement();
}
const QString& FaultProcessAnalyser::GetLogData(int dataid,int y)
{

}
//QWidget* FaultProcessAnalyser::InitContentWidget(QWidget* parent)
//{
//    QTreeWidget* content=new QTreeWidget(parent);
//    content->setSelectionMode(QAbstractItemView::NoSelection);
//    for(int index=0;index<sizeof(logindex)/sizeof(logindex[0]);++index)
//    {
//        QTreeWidgetItem* item=new QTreeWidgetItem(content);
//        QTreeWidgetItem* item_sub;
//        item->setText(0,logindex[index]);
//        switch (index) {
//        case 0:
//            item_sub=new QTreeWidgetItem(item);
//            content->setItemWidget(item_sub,0,InitBaseInfoContent());
//            break;
//        case 1:
//            item_sub=new QTreeWidgetItem(item);
//            content->setItemWidget(item_sub,0,InitConfigInfoContent());
//            break;
//        case 2:
//            item_sub=new QTreeWidgetItem(item);
//            content->setItemWidget(item_sub,0,InitTopoInfoContent());
//            break;//InitTimeConsumeInfoContent
//        case 3:
//            item_sub=new QTreeWidgetItem(item);
////            content->setItemWidget(item_sub,0,InitGLInfoContent());
//            break;//InitTimeConsumeInfoCont
//        case 5:
////            item_sub=new QTreeWidgetItem(item);
////            InitTimeConsumeInfoContent();
//            content->addTopLevelItem(menuHelper->GetWidget(2));
//            break;
////        case 6:
////            item_sub=new QTreeWidgetItem(item);
////            content->setItemWidget(item_sub,0,InitAreaDevInfoContent());
////            break;
////        case 7:
////            item_sub=new QTreeWidgetItem(item);
////            content->setItemWidget(item_sub,0,InitDBInfoContent());
////            break;
////        case 8:
////            item_sub=new QTreeWidgetItem(item);
////            content->setItemWidget(item_sub,0,InitAutoRunInfoContent());
////            break;
//        default:
//            break;
//        }
//    }
//    return content;
//}
QWidget* FaultProcessAnalyser::InitContentWidget(QWidget* parent)
{
    QTreeWidget* content=new QTreeWidget(parent);
    content->setSelectionMode(QAbstractItemView::NoSelection);
    char configfile[100]="/Users/sero/Desktop/Menu.xml";
    QString filename=configfile;
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
    QDomNodeList menuList=docElem.elementsByTagName(QString("submenu"));
    int this_no=1;
    for(int i=0;i<menuList.count();++i)
    {
//        QTreeWidgetItem* topitem=new QTreeWidgetItem(content);
//        topitem->setText(0,menuList.at(i).toElement().attribute("title"));
//        cout<<"nihaoa"<<menuHelper->BuildMenuRefer(menuList.at(i).toElement(),this_no)<<endl;
        int start= menuHelper->BuildMenuRefer(menuList.at(i).toElement(),this_no);
        cout<<"shensq start"<<start<<endl;
        content->addTopLevelItem(menuHelper->GetWidget(menuList.at(i).toElement(),start));
    }
    return content;
}
QWidget* FaultProcessAnalyser::InitBaseInfoContent()
{
    int nolist[10]={2,5,16};
    char name[10][50]={"跳闸开关:","跳闸时间:","故障ID:","运行模式:","执行模式:","故障区域描述:","启动方式:","传入参数:","特殊标志:"};
    QStringList qs;
//    QString name=infoname[i];
    map<int,vector<m_qlist> >::iterator myit;
    map<int ,map<int,vector<m_qlist> > >::iterator it1;
    map<int,vector<m_qlist> >::iterator it2;
    for(it1=resultmap.begin();it1!=resultmap.end();++it1)
    {
        cout<<it1->first<<endl;
        for(it2=it1->second.begin();it2!=it1->second.end();++it2)
        {
            cout<<"    "<<it2->first<<endl;
            for(int i=0;i<it2->second.size();++i)
            {
                cout<<"        "<<it2->second[i].magic_no<<" "<<it2->second[i].body.join(",").toStdString()<<endl;
            }
        }
    }
    cout<<qs.join(",").toStdString()<<endl;
//    map<int ,map<int,vector<m_qlist> > >
    QTextEdit* subcontent_widget=new QTextEdit();
    myit=resultmap[nolist[0]].begin();
    QString result_str;
    QString info=QString(name[0])+":"+myit->second[0].body[1]+"\n";
    QString trip_time=QString(name[1])+myit->second[0].body[3]+"\n";
    QString fault_id=QString(name[2])+myit->second[0].body[2]+"\n";
    QString input_para=QString(name[7])+myit->second[0].body[0];
    result_str=info+trip_time+fault_id;
    if(resultmap.find(nolist[1])!=resultmap.end())
    {
        myit=resultmap[nolist[1]].begin();

        QString run_mode=QString(name[3])+myit->second[0].body[1]+"\n";
        QString pro_mode=QString(name[4])+myit->second[0].body[2]+"\n";
        result_str=result_str+run_mode+pro_mode+input_para;
    }
    if(resultmap.find(nolist[2])!=resultmap.end())
    {
        myit=resultmap[nolist[2]].begin();
        result_str=result_str+QString(name[5])+myit->second[0].body[1]+"\n";
        if(resultmap.find(161)!=resultmap.end())
        {
           result_str=result_str+ resultmap[161][myit->second[0].magic_no][0].body[1]+"\n";
        }
        else
        {
            result_str=result_str+ "cccccc";
        }
    }
    if(resultmap.find(nolist[0])!=resultmap.end())
    {
        result_str=result_str+QString(name[6])+"进程启动\n";
    }
    else
    {
        result_str=result_str+QString(name[6])+"消息启动\n";
    }
    subcontent_widget->setText(result_str);
    return subcontent_widget;

}
QWidget* FaultProcessAnalyser::InitConfigInfoContent()
{
    int nolist[10]={3,4,16};
    char name[10][50]={"未定义参数信息：\n\t","方式设备信息:\n\t"};
    map<int,vector<m_qlist> >::iterator myit;
    vector<m_qlist>::iterator vit;
    QTextEdit* subcontent_widget=new QTextEdit();
    QString result_str=name[0];
    for(myit=resultmap[nolist[0]].begin();myit!=resultmap[nolist[0]].end();++myit)
    {
        for(vit=myit->second.begin();vit!=myit->second.end();++vit)
        {
            result_str=result_str+vit->body[1]+"\n\t";
        }
    }
    result_str=result_str+"\n"+name[1];
    if(resultmap.find(nolist[1])!=resultmap.end())
    {
        myit=resultmap[nolist[1]].begin();
        result_str=result_str+myit->second[0].body[0];
    }
    subcontent_widget->setText(result_str);
    return subcontent_widget;

}
QWidget* FaultProcessAnalyser::InitTopoInfoContent()
{
    int nolist[10]={19};
    QTreeWidget * subcontent_widget=new QTreeWidget();
    map<int,vector<m_qlist> >::iterator myit;
    vector<m_qlist>::iterator vit;
    if(resultmap.find(nolist[0])!=resultmap.end())
    {
        myit=resultmap[nolist[0]].begin();
    }
    for(vit=myit->second.begin();vit!=myit->second.end();++vit)
    {
        QTreeWidgetItem *item=new QTreeWidgetItem(subcontent_widget);
        item->setText(0,vit->body[1]);
        if(resultmap.find(72)!=resultmap.end())
        {
            if(resultmap[72].find(vit->magic_no)!=resultmap[72].end())
            {
                QTreeWidgetItem *item_sub=new QTreeWidgetItem(item);
                item_sub->setText(0,"动作");
            }
        }
        if(resultmap.find(73)!=resultmap.end())
        {
            if(resultmap[73].find(vit->magic_no)!=resultmap[73].end())
            {
                QTreeWidgetItem *item_sub=new QTreeWidgetItem(item);
                item_sub->setText(0,resultmap[73][vit->magic_no][0].body[1]);
            }
        }
    }
    return subcontent_widget;
}
QTreeWidgetItem* FaultProcessAnalyser::InitTimeConsumeInfoContent()
{
//    return menuHelper->GetWidget(2);
    return nullptr;
}
int FaultProcessAnalyser::SplitFile(const char* filename)
{
    QFile dataFile(filename);
    faultmap.clear();
    if (dataFile.open(QIODevice::ReadOnly))
       {
           QTextStream data(&dataFile);
           QString line;
           while(!data.atEnd())
           {
               line=data.readLine();
               long fault_id=line.section(" ",0,0).toLong();
               int trip_time=line.section(" ",1,1).toInt();
               if(fault_id<1||trip_time<1)
               {
                   continue;
               }
               if(faultmap.find(fault_id)==faultmap.end())
               {
                   map<int,QString> tempmap;
                   faultmap.insert(make_pair(fault_id,tempmap));
               }
               if(faultmap[fault_id].find(trip_time)==faultmap[fault_id].end())
               {
                   QString tempstr;
                   faultmap[fault_id].insert(make_pair(trip_time,tempstr));
               }
               faultmap[fault_id][trip_time]=faultmap[fault_id][trip_time]+line+"\n";
           }
    }
    map<long,map<int,QString> >::iterator itest;
    map<int,QString>::iterator itest2;
    for(itest=faultmap.begin();itest!=faultmap.end();++itest)
    {
        cout<<itest->first<<":\n"<<endl;
        for(itest2=itest->second.begin();itest2!=itest->second.end();++itest2)
        {
            cout<<itest2->first<<":\n"<<endl;
            cout<<itest2->second.toStdString()<<endl;
        }
    }
}
int FaultProcessAnalyser::SetTableInfo(QTableWidget* tablep)
{
    map<long,map<int,QString> >::iterator fault_it;
    map<int,QString>::iterator time_it;
    long fault_id;
    int trip_time;
    tablep->setSortingEnabled(true);
    tablep->setRowCount(resultvec.size());
    int row_num=0;
    for(fault_it=faultmap.begin();fault_it!=faultmap.end();++fault_it)
    {
        fault_id=fault_it->first;
        for(time_it=fault_it->second.begin();time_it!=fault_it->second.end();++time_it)
        {
            trip_time=time_it->first;
            row_num=maptovec[fault_id][trip_time];
            QTableWidgetItem *item_col1=new QTableWidgetItem();
            item_col1->setData(Qt::DisplayRole,QVariant::fromValue(row_num+1));
            QTableWidgetItem *item_col2=new QTableWidgetItem();
            item_col2->setData(Qt::DisplayRole,QVariant::fromValue(fault_id));
            QDateTime temp_time;
            temp_time.setTime_t(trip_time);
            QTableWidgetItem *item_col3=new QTableWidgetItem();
            item_col3->setData(Qt::DisplayRole,QVariant(temp_time));
            tablep->setItem(row_num,0,item_col1);
            tablep->setItem(row_num,1,item_col2);
            tablep->setItem(row_num,2,item_col3);
        }
    }
}
int FaultProcessAnalyser::ChangePageInfo(int index)
{
    resultmap=resultvec[index];
}
FaultProcessMenuElement::FaultProcessMenuElement(int v_current, int v_mode, int v_need, int v_index, int v_child, int v_next, int v_parent)
{
    current=v_current;
    parent=v_parent;
    child=v_child;
    mode=v_mode;
    index=v_index;
    postion=v_need;
    next=v_next;
}
FaultProcessMenuElement::FaultProcessMenuElement(const FaultProcessMenuElement& other)
{
    current=other.current;
    parent=other.parent;
    child=other.child;
    mode=other.mode;
    index=other.index;
    postion=other.postion;
    next=other.next;
}
FaultProcessMenuElement::~FaultProcessMenuElement()
{

}

int FaultProcessMenuElement::GetChild()
{
    return child;
}
int FaultProcessMenuElement::GetIndex()
{
    return index;
}
int FaultProcessMenuElement::GetNext()
{
    return next;
}
int FaultProcessMenuElement::GetID()
{
    return current;
}
int FaultProcessMenuElement::GetPostion()
{
    return postion;
}
int FaultProcessMenuElement::SetChild(int id)
{
    child=id;
    return 1;
}
int FaultProcessMenuElement::SetIndex(int id)
{
    index=id;
    return 1;
}
int FaultProcessMenuElement::SetMode(int id)
{
    mode=id;
    return 1;
}
int FaultProcessMenuElement::SetNext(int id)
{
    next=id;
    return 1;
}
int FaultProcessMenuElement::SetPostion(int id)
{
    postion=id;
    return 1;
}
QString FaultProcessMenuElement::GetString(int index, int magic_no, int nbr,  map<int, map<int, vector<m_qlist> > > &datamap)
{
    if(mode==1)
    {
        if(datamap[index][magic_no].size()>0)
        {
            return QString("1");
        }
        else
        {
            return QString("!1");
        }
    }
    else
    {
        return datamap[index][magic_no][nbr].body[postion];
    }
}
FaultProcessMenu::FaultProcessMenu(const map<int, map<int, vector<m_qlist> > > &data)
{
    datamap=data;
}
FaultProcessMenu::~FaultProcessMenu()
{

}
int FaultProcessMenu::BuildUpElement()
{
//      BuildUpElementByConfig("/Users/sero/Desktop/Menu.xml");
      map<int,FaultProcessMenuElement>::iterator it;
      for(it=elementmap.begin();it!=elementmap.end();++it)
      {
          cout<<"sshensq"<<it->first<<":"<<it->second.GetNext()<<","<<it->second.GetChild()<<":"<<it->second.GetIndex()<<":"<<it->second.GetPostion()<<endl;
      }
}
int FaultProcessMenu::BuildMenuRefer(const QDomElement& element,int &this_no)
{
    elementmap.clear();
    int no_record=this_no+1;
    BuildMenuStruct(this_no,element);
    map<int,FaultProcessMenuElement>::iterator it;
    for(it=elementmap.begin();it!=elementmap.end();++it)
    {
        cout<<"sshensq"<<it->first<<":"<<it->second.GetNext()<<","<<it->second.GetChild()<<":"<<it->second.GetIndex()<<":"<<it->second.GetPostion()<<endl;
    }
    return no_record;
}
int FaultProcessMenu::BuildMenuStruct(int& parent_no,const QDomElement& docElem)
{
    QDomNode node = docElem.firstChild();
    int local_former=-1;
    map<int,FaultProcessMenuElement>::iterator menu_it;
    map<int,FaultProcessMenuElement>::iterator former_it;
    menu_it=elementmap.find(parent_no);
    while(!node.isNull())
    {
        QDomElement element = node.toElement();
        cout<<"shensq"<<element.tagName().toStdString()<<endl;
        if(element.tagName()=="content")
        {
            int new_id=parent_no+1;
            elementmap.insert(make_pair(new_id,FaultProcessMenuElement(parent_no)));
            //本次ID，先不改变唯一ID的值，this_no为上一次ID，递归调用的ID
            //之前没有在本次函数调用出现content，说明本次是第一次content，传入的是父节点的ID，设置其child
            if(local_former==-1)
            {
                former_it=elementmap.find(parent_no);
                former_it->second.SetChild(new_id);
            }
            //之前出现过content，即已经对elementmap进行过insert，但是还是在本次函数内，所以是next
            else
            {
                former_it=elementmap.find(local_former);
                former_it->second.SetNext(new_id);
            }
            parent_no=new_id;
            local_former=new_id;
            BuildMenuStruct(parent_no,element);
        }
        else if(element.tagName()=="index")
        {
            cout<<"set index"<<element.text().toInt()<<endl;
            menu_it->second.SetIndex(element.text().toInt());
        }
        else if(element.tagName()=="postion")
        {
//            element.attribute("prefix");
            menu_it->second.SetPostion(element.text().toInt());
        }
        else if(element.tagName()=="mode")
        {
            menu_it->second.SetMode(element.text().toInt());
        }
        node = node.nextSibling();
    }
}
QTreeWidgetItem *FaultProcessMenu::GetWidget(const QDomElement& element,int start)
{
    cout<<"start="<<start<<endl;
    QTreeWidgetItem* item=new QTreeWidgetItem();
    item->setText(0,element.attribute("title"));
    if(elementmap.find(start)==elementmap.end())
    {
        return nullptr;
    }
    BuildInner(0,start,item);
    return item;
}
int FaultProcessMenu::BuildInner(int magic_no,int num,QTreeWidgetItem* parent)
{
    map<int,FaultProcessMenuElement>::iterator ele_it=elementmap.find(num);
    if(ele_it==elementmap.end())
    {
        cout<<"can not find num.."<<endl;
        return -1;
    }
    FaultProcessMenuElement fe_p=ele_it->second;
    QTreeWidgetItem* new_item;
//    int index=elementmap[num].GetIndex();
    int index=ele_it->second.GetIndex();
    cout<<"BuildInner index="<<index<<"magic="<<magic_no<<endl;
    if(datamap.find(index)!=datamap.end())
    {
        if(datamap[index].find(magic_no)!=datamap[index].end())
        {
            for(int i=0;i<datamap[index][magic_no].size();++i)
            {

                new_item=new QTreeWidgetItem(parent);
                new_item->setText(0,fe_p.GetString(index,magic_no,i,datamap));
                if(fe_p.GetChild()>0)
                {
                    cout<<"buildchild ex"<<endl;
                    BuildInner(datamap[index][magic_no][i].magic_no,fe_p.GetChild(),new_item);
                }
                if(fe_p.GetNext()>0)
                {
                    cout<<"buildnext ex"<<endl;
                    BuildInner(magic_no,fe_p.GetNext(),parent);
                }
            }
        }
    }
    else
    {
        BuildInner(magic_no,fe_p.GetNext(),parent);
    }
}
