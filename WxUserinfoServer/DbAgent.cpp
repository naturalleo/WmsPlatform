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
        string sSql = "select `userId` from `t_user` where unionId = " + unionId + " and appGroupId = " + appGroupId + " limit 0,1";
        tars::TC_Mysql::MysqlData item = _mysqlReg.queryRecord(sSql);
        if (item.size() == 0)
        {
            return 0;
        }
        else
        {
            return TC_Common::strto<int>(item[0]["userId"]);
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
    1 require  string appGroupId;
    2 require  string headimgurl;
    3 require  string nickname;
    4 require  string sex;
    5 require  string openid;    
};

CREATE TABLE `t_user` (
  `userId` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT '用户ID',
  `nickName` varchar(32) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL DEFAULT '' COMMENT '昵称',
  `avatar` varchar(255) NOT NULL DEFAULT '' COMMENT '头像',
  `phoneNumber` varchar(13) NOT NULL DEFAULT '' COMMENT '手机号码',
  `password` varchar(34) NOT NULL COMMENT '密码',
  `gender` tinyint(3) unsigned NOT NULL DEFAULT '1' COMMENT '性别 1 男 2 女',
  `regTime` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '注册时间',
  `regIp` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '注册IP',
  `loginTimes` int(10) unsigned NOT NULL DEFAULT '1' COMMENT '登录次数',
  `lastLoginTime` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '最近登录时间',
  `lastLoginIp` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '最近登录IP',
  `status` tinyint(3) unsigned NOT NULL DEFAULT '1' COMMENT '状态 0 锁定 1 正常',
  `userType` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT '用户类型 0 普通 1 vip1 2 vip2  10 普通测试账号 11 访客账号 ',
  `appId` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '应用ID',
  `appCode` varchar(12) NOT NULL DEFAULT '' COMMENT '应用Code',
  `regFrom` tinyint(3) unsigned NOT NULL DEFAULT '1' COMMENT '注册来源 0 未知 1 微信 2手机',
  `clientFrom` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT '客户端来源0未知 1 IOS 2 Android',
  `chanId` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '渠道来源 0 未知',
  `openId` varchar(28) NOT NULL DEFAULT '' COMMENT '微信公众平台OpenID 一个平台对应一个Openid',
  `unionId` varchar(29) NOT NULL DEFAULT '' COMMENT '微信开发平台UnionID 一个开发者平台对应一个ID',
  `appGroupId` smallint(5) unsigned NOT NULL DEFAULT '0' COMMENT '应用分组ID 房卡通用为一组',
  PRIMARY KEY (`userId`),
  KEY `appId` (`appId`,`regFrom`)
) ENGINE=InnoDB AUTO_INCREMENT=2000726 DEFAULT CHARSET=utf8 COMMENT='用户模块-用户主表';
struct WxUserinfoReq
{
    0 require  string unionid;
    1 require  string appGroupId;
    2 require  string headimgurl;
    3 require  string nickname;
    4 require  string sex;
    5 require  string openid; 
    6 require  string appId;  
};


*/

int DbAgent::insertNewUser(const WmsPlatform::WxUserinfoReq &in, string &userId)
{
    try
    {
        string sSql = "insert into t_user(`nickName`, `password`, `gender`, `regTime`, `loginTimes`, `lastLoginTime`,`status`, `userType`, `appId`, `unionId`, `appGroupId`)"
                            "values"
                            "( " + in.nickname + " ,"
                            "'test' , "
                            " " + in.sex + " ,"
                            " " + TC_Common::tostr(TC_Common::now2ms()/1000)  + ","
                            " " + TC_Common::tostr(TC_Common::now2ms()/1000)  + ","
                            " " + TC_Common::tostr(TC_Common::now2ms()/1000)  + "," 
                            "'" + TC_Common::tostr(1) + "' ,"
                            "'" + TC_Common::tostr(0) + "' ,"
                            " " + in.appId + " ,"
                            "'" + in.unionId + "' ,"
                            "'" + in.appGroupId + "')";

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


int DbAgent::getDbUserinfo(const WmsPlatform::WxUserinfoReq &in, string &userId)
{
    try
    {
        int uid;
        if ((uid == isUniqueUser(in.unionId , "")) == 0)
        {
            insertNewUser(in,userId);
            uid = isUniqueUser(in.unionId , "");
            return 1;
        }
        else if ((uid == isUniqueUser(in.unionId , "")) == -1)
        {
            TLOGERROR(" DbAgent::getDbUserinfo exception: " << -1 << endl);
            return -1;
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