#include "DbAgent.h"
#include <sstream>



using namespace std;
using namespace tars;

int DbAgent::init()
{
	TLOGDEBUG("begin DbAgent init"<<endl);
	try{
        //_mysqlReg.init("10.17.174.171", "linweixiong", "qipai123987", "db_tars_game");
        //_mysqlReg.init("192.168.1.103", "tars", "tars2015", "db_games");
         _mysqlReg.init("10.66.205.133", "root", "QDi9zPVycg", "db_game");
        //_mysqlReg.connect();

     }catch(exception &ex)
     {
      	TLOGDEBUG("exception "<< ex.what() << endl);
      	return -1;
     }
	 TLOGDEBUG("DbAgent init ok"<<endl);
	 return 0;
}

uint64_t DbAgent::generaterID()
{

    uint64_t time = TC_Common::now2ms();
 
    uint64_t id  = time + pthread_self();

    return id;
}

/*



CREATE TABLE `t_user_login_log_0` (
`loginId`  int(10) UNSIGNED NOT NULL AUTO_INCREMENT ,
`userId`  int(10) UNSIGNED NOT NULL COMMENT '用户ID' ,
`ip`  int(10) UNSIGNED NOT NULL DEFAULT 0 COMMENT '登录IP' ,
`addTime`  int(10) UNSIGNED NOT NULL DEFAULT 0 COMMENT '登录时间' ,
`appId`  int(10) UNSIGNED NOT NULL COMMENT '应用ID' ,
`appCode`  varchar(12) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '' COMMENT '应用Code' ,
`loginToken`  char(32) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '登录时候Token' ,
`clientFrom`  tinyint(4) UNSIGNED NOT NULL DEFAULT 0 COMMENT '登录来源 未知 0 Iphone 1 - Android 2' ,
PRIMARY KEY (`loginId`),
INDEX `userId` (`userId`) USING BTREE 
)
ENGINE=InnoDB
DEFAULT CHARACTER SET=utf8 COLLATE=utf8_general_ci
COMMENT='用户模块-用户登录日志【根据用户ID取模】'
AUTO_INCREMENT=309
*/






int DbAgent::generaterOrderID(const WmsPlatform::CreateRoomReq &in, string &order)
{

    try
    {
    uint64_t id  = generaterID();
  
		string sSql = 
                "insert into t_game_room_order (`orderCode`,`useGameCardNum`,`roomCateId`,`gameId`,`orderType`,`subOrderType`,`userId`,`appId`,`appCode`,`addTime`,`expTime`,`updateTime`,`status` ) values " 
                        "('" + TC_Common::tostr(id) + "' ," 
                        " " + in.useNum + " ,"
                        " " + TC_Common::tostr(1) + " ,"
                        "'" + in.gameId + "' ,"
                        "'" + in.orderType + "' ,"  
                        " " + TC_Common::tostr(0) + ","  //subOrderType '子订单类型标记 0 普通，1个人批量开房 2 德州扑克开房'
                        "'" + TC_Common::tostr(in.userId) + "' ," 
                        "'" + in.appId + "' ,"  
                        "'" + in.appCode + "' ,"  
                        " " + TC_Common::tostr(TC_Common::now2ms()/1000)  + ","  
                        " " + TC_Common::tostr(TC_Common::now2ms()/1000 + 24 * 3600) + ","  
                        " " + TC_Common::tostr(TC_Common::now2ms()/1000) + ","  
                        " " + TC_Common::tostr(0) + ")" ;


		

		//sSql += "(" + TC_Common::tostr(id) + ")";
    TLOGDEBUG("DbAgent sSql ok"<< sSql <<endl);
		_mysqlReg.execute(sSql);
		order = TC_Common::tostr(id);
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

    return 0;
}

int DbAgent::updateUserToken(const WmsPlatform::WxoauthReq& req, const string &token)
{
    try
    {
    
      int pos = TC_Common::strto<int>(req.userId) % 10 ;
      ostringstream oss("");
      oss << "replace into t_user_login_log_"<< TC_Common::tostr(pos) << "(userId, ip, addTime, appId, appCode, loginToken, clientFrom) values (" << req.userId << ","
          << "'" << req.ip << "'," << TC_Common::tostr(TC_Common::now2ms()/1000) << ", " << req.appId 
          << ", '" << req.appCode << "', '" << token << "'," << req. clientFrom << ")";


      _mysqlReg.execute(oss.str());

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