#ifndef ANALYZERINTERFACE_H
#define ANALYZERINTERFACE_H
#include <map>
#include <QStringList>
class AnalyzerInterface
{
public:
    AnalyzerInterface(){};
    virtual ~AnalyzerInterface(){};
    virtual QStringList GetLogFileNameList(const char* path)=0;
    virtual int StartAnalysis(const char *filename)=0;
    virtual const QString& GetLogData(int dataid,int y)=0;
protected:
    std::map<int,std::map<int,std::string> > logdata;
};

#endif // ANALYZERINTERFACE_H
