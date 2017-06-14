#include "DbAgent.h"




using namespace std;
using namespace tars;

int DbAgent::init()
{
	TLOGDEBUG("begin DbAgent init"<<endl);
	try{
        _mysqlReg.init("10.17.174.171", "linweixiong", "qipai123987", "db_tars_game");
        //_mysqlReg.init("192.168.1.103", "tars", "tars2015", "db_games");
        //_mysqlReg.connect();

     }catch(exception &ex)
     {
      	TLOGDEBUG("exception "<< ex.what() << endl);
      	return -1;
     }
	 TLOGDEBUG("DbAgent init ok"<<endl);
	 return 0;
}



int DbAgent::isUniqueUser(const std::string unionId, const std::string appGroupId) 
{
    try
    {
        string sSql = "select `userId` from `t_user` where unionId = " + unionId + " limit 0,1";
        tars::TC_Mysql::MysqlData item = _mysqlReg.queryRecord(sSql);
        if (item.size() == 0)
        {
            return -1;
        }
        else
        {
            return TC_Common::strto<int>(item[0]["userId"])
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

/*

struct WxUserinfoReq
{
    0 require  string unionid;
    1 require  string headimgurl;
    2 require  string nickname;
    3 require  string sex;
    4 require  string country;
    5 require  string province;
    6 require  string city;
    7 require  string openid;    
};

*/

int DbAgent::insertNewUser(const WmsPlatform::WxUserinfoReq &in, string &userId)
{
    try
    {
        string sSql = "insert into t_user(`unionid`, `headimgurl`, `nickname`, `sex`, `openid`) values"
                            "('" + in.unionid + "' ,"
                            "'" + in.headimgurl + "' ,"
                            "'" + in.sex + "' ,"
                            "'" + in.openid + "')";

        _mysqlReg.execute(sSql);


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


int DbAgent::getDbUserinfo(const WmsPlatform::WxUserinfoReq &in, string &userId)
{
    try
    {
        int uid;
        if ((uid == isUniqueUser(in.unionid , "")) == -1)
        {
            insertNewUser(in,userId);
            uid = isUniqueUser(in.unionid , "");
        }
        
        userId = TC_Common::tostr(uid);

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
