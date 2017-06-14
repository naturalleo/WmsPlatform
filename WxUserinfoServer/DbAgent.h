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

  	int isUniqueUser(const std::string unionId, const std::string appGroupId) ;

    int insertNewUser(const WmsPlatform::WxUserinfoReq &in, string &userId);

    int getDbUserinfo(const WmsPlatform::WxUserinfoReq &in, string &userId);
    


private:
    //mysql连接对象
    tars::TC_Mysql _mysqlReg;

};











#endif