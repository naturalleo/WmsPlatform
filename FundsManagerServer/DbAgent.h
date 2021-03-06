#ifndef __DB_AGENT_H__
#define __DB_AGENT_H__


#include "util/tc_common.h"
#include "util/tc_config.h"
#include "util/tc_monitor.h"
#include "util/tc_mysql.h"
#include "util/tc_file.h"
#include "servant/TarsLogger.h"
#include "Funds.h"


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

  	int insertFunds(const WmsPlatform::FundsNewUserReq& sIn, WmsPlatform::FundsUserInfoRes& sOut) ;

    int getFunds(const WmsPlatform::FundsUserInfoReq& sIn, WmsPlatform::FundsUserInfoRes& sOut);

    int getOtherFunds(const WmsPlatform::FundsUserInfoReq& sIn, WmsPlatform::FundsUserInfoRes& sOut);

    int modifyFunds(const WmsPlatform::FundsUserModifyReq& sIn, WmsPlatform::FundsUserInfoRes& sOut);
    
    int insertUseCardLog(const WmsPlatform::FundsUserModifyReq& sIn);

    int insertExchangeLog(const WmsPlatform::FundsUserModifyOtherReq& sIn);

    int modifyFundsOther(const WmsPlatform::FundsUserModifyOtherReq& sIn, WmsPlatform::FundsUserModifyOtherRes& sOut);

    int insertWinnerLog(const WmsPlatform::SetWinnerReq& sIn);
private:
    //mysql连接对象
    tars::TC_Mysql _mysqlReg;

};











#endif