#ifndef __DB_AGENT_H__
#define __DB_AGENT_H__


#include "util/tc_common.h"
#include "util/tc_config.h"
#include "util/tc_monitor.h"
#include "util/tc_mysql.h"
#include "util/tc_file.h"
#include "servant/TarsLogger.h"
#include "WxUserinfo.h"

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

  	int selectUserinfo(const std::string unionId, const std::string appGroupId, WxUserinfoRes &sOut) ;

    int selectUserinfo(const std::string userId, WxUserinfoRes &sOut) ;

    int insertNewUser(const WmsPlatform::WxLoginUserinfoReq &sIn, std::string &uid);

    int getDbUserinfo(const WmsPlatform::WxUserinfoReq &sIn, WxUserinfoRes &sOut);

    int getLoginDbUserinfo(const WmsPlatform::WxLoginUserinfoReq &sIn, string &sOut);
    
    int isUserAgent(const std::string userId ,int &result ,int &level);

private:
    //mysql连接对象
    tars::TC_Mysql _mysqlReg;

};











#endif