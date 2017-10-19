#include "DbAgent.h"




using namespace std;
using namespace tars;

int DbAgent::init()
{
	TLOGDEBUG("begin DbAgent init"<<endl);
	try{
        //_mysqlReg.init("10.17.174.171", "linweixiong", "qipai123987", "db_tars_game");
        //_mysqlReg.init("192.168.1.103", "tars", "tars2015", "db_player");
        //_mysqlReg.connect();
        _mysqlReg.init("10.66.205.133", "root", "QDi9zPVycg", "db_player");

     }catch(exception &ex)
     {
      	TLOGDEBUG("exception "<< ex.what() << endl);
      	return -1;
     }
	 TLOGDEBUG("DbAgent init ok"<<endl);
	 return 0;
}
int DbAgent::init(TC_DBConf conf)
{
  TLOGDEBUG("begin DbAgent init"<<endl);
  try{
        _mysqlReg.init(conf) ;

     }catch(exception &ex)
     {
        TLOGDEBUG("exception "<< ex.what() << endl);
        return -1;
     }
   TLOGDEBUG("DbAgent init ok"<<endl);
   return 0;
}

int DbAgent::updateLoginTime(const std::string userId)
{
    try
    {
        string sSql;
        sSql = "update t_user set lastLoginTime = " + TC_Common::tostr(TC_Common::now2ms()/1000) + " "
                        "where userId = " + userId + "";

        TLOGDEBUG("update loginTime sql: " << sSql << endl);
        _mysqlReg.execute(sSql);

        TLOGDEBUG(__FUNCTION__ << pthread_self() <<" affected: " << _mysqlReg.getAffectedRows() << endl);
        return 0 ;
    }
    catch (TC_Mysql_Exception& ex)
    {
        TLOGERROR(__FUNCTION__ << " exception: " << ex.what() << endl);
        return -1;
    }
    catch (exception& ex)
    {
        TLOGERROR(__FUNCTION__ << " exception: " << ex.what() << endl);
        return -1;
    }
}


int DbAgent::selectUserinfo(const std::string unionId, const std::string appGroupId, const std::string appCode, WxUserinfoRes &sOut) 
{
    try
    {
        string sSql = "select `userId`, `nickName`, `avatar`, `gender`, `lastLoginIp`, `status`, `activationCode` from `t_user` where unionId = '" + unionId + "' and appGroupId = '" + appGroupId + "' and appCode = '" + appCode + "' limit 0,1";
        tars::TC_Mysql::MysqlData item = _mysqlReg.queryRecord(sSql);
        if (item.size() == 0)
        {
            return 0;
        }
        else
        {
            sOut.userId     = item[0]["userId"];
            sOut.headimgurl = item[0]["avatar"];
            sOut.nickname   = item[0]["nickName"];
            sOut.sex        = item[0]["gender"];
            sOut.ip         = item[0]["lastLoginIp"];
            sOut.status     = item[0]["status"];
            sOut.activationCode = item[0]["activationCode"];

            TLOGDEBUG("selectUserinfo sOut.userId  : " <<  sOut.userId  <<endl);
            TLOGDEBUG("selectUserinfo sOut.headimgurl  : "<<  sOut.headimgurl  <<endl);
            return 1;
        }
    }
    catch (TC_Mysql_Exception& ex)
    {
        TLOGERROR(__FUNCTION__ << " exception: " << ex.what() << endl);
        return -1;
    }
    catch (exception& ex)
    {
        TLOGERROR(__FUNCTION__ << " exception: " << ex.what() << endl);
        return -1;
    }
}
int DbAgent::selectUserinfo(const std::string userId, WxUserinfoRes &sOut)
{
    try
    {
        string sSql = "select `userId`, `nickName`, `avatar`, `gender`, `lastLoginIp`, `status`, `activationCode` from `t_user` where userId = '" + userId + "' limit 0,1";

        tars::TC_Mysql::MysqlData item = _mysqlReg.queryRecord(sSql);
        if (item.size() == 0)
        {
            return 0;
        }
        else
        {

            sOut.userId = item[0]["userId"];
            sOut.headimgurl = item[0]["avatar"];
            sOut.nickname = item[0]["nickName"];
            sOut.sex = item[0]["gender"];
            sOut.ip = item[0]["lastLoginIp"];
            sOut.status     = item[0]["status"];
            sOut.activationCode = item[0]["activationCode"];

            return 1;
        }
    }
    catch (TC_Mysql_Exception& ex)
    {
        TLOGERROR(__FUNCTION__ << " exception: " << ex.what() << endl);
        return -1;
    }
    catch (exception& ex)
    {
        TLOGERROR(__FUNCTION__ << " exception: " << ex.what() << endl);
        return -1;
    }   

}

int DbAgent::selectUserGameType(const std::string userId, std::string &appId, std::string &appCode)
{
    try
    {
        string sSql = "select `appId`, `appCode` from `t_user` where userId = '" + userId + "' limit 0,1";

        tars::TC_Mysql::MysqlData item = _mysqlReg.queryRecord(sSql);
        if (item.size() == 0)
        {
            return 0;
        }
        else
        {

            appId = item[0]["appId"];
            appCode = item[0]["appCode"];
            return 1;
        }
    }
    catch (TC_Mysql_Exception& ex)
    {
        TLOGERROR(__FUNCTION__ << " exception: " << ex.what() << endl);
        return -1;
    }
    catch (exception& ex)
    {
        TLOGERROR(__FUNCTION__ << " exception: " << ex.what() << endl);
        return -1;
    }   

}

int DbAgent::insertNewUser(const WmsPlatform::WxLoginUserinfoReq &in, std::string &uid)
{
    try
    {
        TLOGDEBUG(__FUNCTION__<<"sex,appId ,unionId, appCode,appGroupId : "<< in.sex <<" , " << in.appId << " , " <<in.unionId <<endl);
        string sSql = "insert into t_user(`nickName`,`avatar`, `password`, `gender`, `regTime`, `loginTimes`, `lastLoginTime`,`status`, `userType`, `appId`, `unionId`, `appCode`, `appGroupId`)"
                            "values"
                            "( '" + in.nickname + "',"
                            "'"   + in.headimgurl + "',"
                            "'test' , "
                            "" + in.sex + ","
                            " " + TC_Common::tostr(TC_Common::now2ms()/1000)  + ","
                            " " + TC_Common::tostr(TC_Common::now2ms()/1000)  + ","
                            " " + TC_Common::tostr(TC_Common::now2ms()/1000)  + "," 
                            "1 ,"
                            "0 ,"
                            "" + in.appId + " ,"
                            "'" + in.unionId + "' ,"
                            "'" + in.appCode + "' ,"
                            "" + in.appGroupId + ")";

        _mysqlReg.execute(sSql);
        uid = TC_Common::tostr(_mysqlReg.lastInsertID());
        TLOGDEBUG(__FUNCTION__ << pthread_self() <<" affected: " << _mysqlReg.getAffectedRows() << "uid: "<< uid <<"last id :" << _mysqlReg.lastInsertID()  << endl);
        return 0;
    }
    catch (TC_Mysql_Exception& ex)
    {
        TLOGERROR(__FUNCTION__ << " exception: " << ex.what() << endl);
        return -1;
    }
    catch (exception& ex)
    {
        TLOGERROR(__FUNCTION__ << " exception: " << ex.what() << endl);
        return -1;
    }
}

int DbAgent::getLoginDbUserinfo(const WmsPlatform::WxLoginUserinfoReq &sIn, string &sOut, string &sActivationCode)
{
    try
    {   
        WxUserinfoRes res;
        int ret = selectUserinfo(sIn.unionId , sIn.appGroupId, sIn.appCode, res);
        if ( ret == 0)
        {
            if (insertNewUser(sIn, sOut) == 0)
            {
                // 新用户直接返回
                sActivationCode = "0";
                TLOGDEBUG("getLoginDbUserinfo insertNewUser uid : "<<  sOut << endl);
                return 1;
            }
        }
        else if (ret == -1)
        {
            TLOGERROR(" DbAgent::getLoginDbUserinfo exception: " << -1 << endl);
            return -1 ;
        }
        TLOGDEBUG("getLoginDbUserinfo uid : "<<  res.userId <<endl);
        if(res.status == "0") //黑名单
        {
            TLOGDEBUG("玩家登录，黑名单uid : "<<  res.userId << endl);
            return -1;
        }
        updateLoginTime(res.userId);
        sOut = res.userId ;
        sActivationCode = res.activationCode;
        return 0;
    }
    catch (TC_Mysql_Exception& ex)
    {
        TLOGERROR(__FUNCTION__ << " exception: " << ex.what() << endl);
        return -1;
    }
    catch (exception& ex)
    {
        TLOGERROR(__FUNCTION__ << " exception: " << ex.what() << endl);
        return -1;
    }

}


int DbAgent::getDbUserinfo(const WmsPlatform::WxUserinfoReq &sIn, WxUserinfoRes &sOut )
{
    try
    {
        if (selectUserinfo(sIn.userId, sOut) == 1)
        {
            return 0 ;
        }
        else
        {
            TLOGERROR(" DbAgent::getDbUserinfo exception: " << -1 << endl);
            return -1 ;
        }
    }
    catch (TC_Mysql_Exception& ex)
    {
        TLOGERROR(__FUNCTION__ << " exception: " << ex.what() << endl);
        return -1;
    }
    catch (exception& ex)
    {
        TLOGERROR(__FUNCTION__ << " exception: " << ex.what() << endl);
        return -1;
    }
    return 0;
}


int DbAgent::isUserAgent(const std::string userId ,string &agentType)
{
    try
    {
        string sSql = "select `userId`,`agentType` from `t_user` where userId = " + userId + " limit 0,1";

        tars::TC_Mysql::MysqlData item = _mysqlReg.queryRecord(sSql);
        if (item.size() == 0)
        {
            return 0;
        }
        else
        {
            agentType = item[0]["agentType"];
            return 0;
        }

        TLOGDEBUG(__FUNCTION__ << pthread_self() <<" affected: " << _mysqlReg.getAffectedRows() << endl);
    }
    catch (TC_Mysql_Exception& ex)
    {
        TLOGERROR(__FUNCTION__ << " exception: " << ex.what() << endl);
        return -1;
    }
    catch (exception& ex)
    {
        TLOGERROR(__FUNCTION__ << " exception: " << ex.what() << endl);
        return -1;
    }  
}

int DbAgent::activationCodeSet(const std::string userId ,const std::string activationCode)
{
    try
    {
        string sSql;
        sSql = "update t_user set activationCode = " + activationCode + " "
                        "where userId = " + userId + "";

        TLOGDEBUG("set activationCodeSet sql: " << sSql << endl);
        _mysqlReg.execute(sSql);

        TLOGDEBUG(__FUNCTION__ << pthread_self() <<" affected: " << _mysqlReg.getAffectedRows() << endl);
        return 0 ;
    }
    catch (TC_Mysql_Exception& ex)
    {
        TLOGERROR(__FUNCTION__ << " exception: " << ex.what() << endl);
        return -1;
    }
    catch (exception& ex)
    {
        TLOGERROR(__FUNCTION__ << " exception: " << ex.what() << endl);
        return -1;
    }
}