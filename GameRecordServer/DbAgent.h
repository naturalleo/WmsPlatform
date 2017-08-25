#ifndef __DB_AGENT_H__
#define __DB_AGENT_H__


#include "util/tc_common.h"
#include "util/tc_config.h"
#include "util/tc_monitor.h"
#include "util/tc_mysql.h"
#include "util/tc_file.h"
#include "servant/TarsLogger.h"
#include "Game.h"



using namespace WmsPlatform;
using namespace std;
/**
 *  数据库操作类
 */
class DbAgent
{
public:
    /**
     * 构造函数
     */
    DbAgent(){}

    int init();

    int init(TC_DBConf conf);

  	int getGameRecord(const WmsPlatform::GameRecordReq& sIn, vector<GameRecordItem>& sOut) ;

    int getGameRecordDetail(const WmsPlatform::GameRecordDetailReq& sIn, vector<GameRecordDetailItem>& sOut);

    int getGameSharesRecord(const WmsPlatform::GameShareRecordReq& sIn, WmsPlatform::GameShareRecordRes& sOut);
    

    string specialStrChange(string &s);

private:
    //mysql连接对象
    tars::TC_Mysql _mysqlReg;

    static map<string, string> createLib()
    {
        map<string,string> lib;
        lib["\""] = "\\\"";
        return lib;
    }

    static const map<string, string> _charLib;

};











#endif