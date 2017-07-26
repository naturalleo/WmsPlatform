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


int DbAgent::selectUserinfo(const std::string unionId, const std::string appGroupId, WxUserinfoRes &sOut) 
{
    try
    {
        string sSql = "select `userId`, `nickName`, `avatar`, `gender`, `lastLoginIp` from `t_user` where unionId = '" + unionId + "' and appGroupId = '" + appGroupId + "' limit 0,1";
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
        string sSql = "select `userId`, `nickName`, `avatar`, `gender`, `lastLoginIp` from `t_user` where userId = '" + userId + "' limit 0,1";

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



int DbAgent::insertNewUser(const WmsPlatform::WxLoginUserinfoReq &in)
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

        TLOGDEBUG(__FUNCTION__ << pthread_self() <<" affected: " << _mysqlReg.getAffectedRows() << endl);
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

int DbAgent::getLoginDbUserinfo(const WmsPlatform::WxLoginUserinfoReq &sIn, string &sOut )
{
    try
    {   
        WxUserinfoRes res;

        if (selectUserinfo(sIn.unionId , sIn.appGroupId, res) == 0)
        {

            insertNewUser(sIn);
            selectUserinfo(sIn.unionId , sIn.appGroupId, res);
        }
        else if (selectUserinfo(sIn.unionId , sIn.appGroupId, res) == -1)
        {
            TLOGERROR(" DbAgent::getLoginDbUserinfo exception: " << -1 << endl);
            return -1 ;
        }
        TLOGDEBUG("getLoginDbUserinfo uid : "<<  res.userId <<endl);
        sOut = res.userId ;
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


int DbAgent::isUserAgent(const std::string userId ,int &result)
{
    try
    {
        string sSql = "select `userId` from `t_user` where userId = " + userId + " and userType = 1"  " limit 0,1";

        tars::TC_Mysql::MysqlData item = _mysqlReg.queryRecord(sSql);
        if (item.size() == 0)
        {
            result = -1;
            return 0;
        }
        else
        {
            result = 0;
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