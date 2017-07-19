#include "DbAgent.h"
#include <sstream>



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

uint64_t DbAgent::generaterID()
{

    uint64_t time = TC_Common::now2ms();
 
    uint64_t id  = time + pthread_self();

    return id;
}

/*
CREATE TABLE `t_game_room_order` (
`orderId`  int(10) UNSIGNED NOT NULL AUTO_INCREMENT COMMENT '订单ID' ,
`orderCode`  varchar(30) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '' COMMENT '订单编号' ,
`orderType`  tinyint(3) UNSIGNED NOT NULL DEFAULT 1 COMMENT '1 自开 2 代开' ,
`subOrderType`  tinyint(3) UNSIGNED NOT NULL DEFAULT 0 COMMENT '子订单类型标记 0 普通，1个人批量开房 2 德州扑克开房' ,
`userId`  int(11) UNSIGNED NOT NULL COMMENT '用户ID' ,
`nickName`  varchar(32) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL DEFAULT '' COMMENT '用户昵称' ,
`appId`  int(11) UNSIGNED NOT NULL DEFAULT 0 COMMENT '应用ID' ,
`appCode`  varchar(12) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '' COMMENT '应用Code' ,
`gameId`  int(11) UNSIGNED NOT NULL COMMENT '游戏ID' ,
`roomCateId`  int(11) UNSIGNED NOT NULL COMMENT '房间类型ID' ,
`cateName`  float(10,1) UNSIGNED NOT NULL DEFAULT 0.0 COMMENT '房间局数值-开局局数|德州扑克为对应开房时长标识' ,
`useGameCardNum`  tinyint(3) UNSIGNED NOT NULL COMMENT '使用游戏卡数目-续扣游戏以后会累计' ,
`useGameCardNum0`  tinyint(3) UNSIGNED NOT NULL DEFAULT 0 COMMENT '使用游戏卡数目-开局房卡数目不变' ,
`ip`  int(10) UNSIGNED NOT NULL DEFAULT 0 COMMENT 'IP记录' ,
`addTime`  int(11) UNSIGNED NOT NULL DEFAULT 0 COMMENT '添加时间' ,
`expTime`  int(11) UNSIGNED NOT NULL DEFAULT 0 COMMENT '失效时间' ,
`updateTime`  int(11) UNSIGNED NOT NULL DEFAULT 0 COMMENT '更新时间' ,
`continueTime`  int(10) UNSIGNED NOT NULL DEFAULT 0 COMMENT '最后续扣时间' ,
`continueNum`  tinyint(3) UNSIGNED NOT NULL DEFAULT 0 COMMENT '续扣次数-德州扑克会用' ,
`status`  tinyint(3) UNSIGNED NOT NULL DEFAULT 0 COMMENT '状态 0 创建  1 用完关闭  98 超时关闭 99 自动解散关闭' ,
`gameRoomId`  varchar(10) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '' COMMENT '游戏房间ID' ,
`activityId`  int(10) UNSIGNED NOT NULL DEFAULT 0 COMMENT '活动ID' ,
PRIMARY KEY (`orderId`),
INDEX `orderCode` (`orderCode`) USING BTREE ,
INDEX `userId` (`userId`) USING BTREE 
)


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
                        " " + TC_Common::tostr(0) + " ,"
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

int DbAgent::updateUserToken(const WmsPlatform::WxoauthReq& req)
{
    try
    {
    
      int pos = TC_Common::strto<int>(req.userId) % 10 ;
      ostringstream oss("replace into t_user_login_log_");
      oss << TC_Common::tostr(pos) << "(userId, ip, addTime, appId, appCode, loginToken, clientFrom) values (" << req.userId << ","
          << "'" << req.ip << "'," << TC_Common::tostr(TC_Common::now2ms()/1000) << ", " << req.appId 
          << "'" << req.appCode << "', '" << req.token << "'," << req. clientFrom << ")";


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