#ifndef __DB_AGENT_H__
#define __DB_AGENT_H__


#include "util/tc_common.h"
#include "util/tc_config.h"
#include "util/tc_monitor.h"
#include "util/tc_mysql.h"
#include "util/tc_file.h"
#include "servant/TarsLogger.h"
#include "Order.h"


using namespace WmsPlatform;
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

    uint64_t generaterID();

    string replaceStr(string sString);

    int generaterOrderID(const WmsPlatform::CreateRoomReq &in, string &order);

    int updateUserToken(const WmsPlatform::WxoauthReq& req, const string &token);
    
    int checkUserToken(const WxUserinfoReq &req, string &ip);

    int reportSuggestion(const ReportSuggestionReq &req);

    int reportApplyAgent(const ReportApplyAgentReq &req);

    int sysNotice(const SysNoticeReq& req, SysNoticeRes& res);

private:
    //mysql连接对象
    tars::TC_Mysql _mysqlReg;

};











#endif