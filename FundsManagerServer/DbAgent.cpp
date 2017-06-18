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

/*

CREATE TABLE `t_user_funds` (
  `ufId` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT 'ID',
  `userId` int(10) unsigned NOT NULL COMMENT '用户ID',
  `totalMoney` decimal(12,2) unsigned NOT NULL DEFAULT '0.00' COMMENT '总共消费金额',
  `totalGameCard` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '总游戏卡数',
  `totalRechargeGameCard` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '总充值房卡数',
  `extraRewardGameCard` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '额外赠送房卡数目',
  `sysRewardGameCard` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '系统奖励房卡数量累计',
  `returnGameCard` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '返回房卡数量 退款或者超市消耗',
  `totalUseGameCard` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '总使用游戏卡数',
  `totalSelfUseGameCard` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '自己使用游戏卡数',
  `totalGiveUseGameCard` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '代开使用游戏卡数',
  `surplusGameCard` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '剩余游戏卡数',
  `addTime` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '添加时间',
  `updateTime` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '更新时间',
  `appId` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '应用ID',
  `appCode` varchar(12) NOT NULL DEFAULT '' COMMENT '应用Code',
  PRIMARY KEY (`ufId`),
  UNIQUE KEY `userId` (`userId`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=685 DEFAULT CHARSET=utf8 COMMENT='余额模块-用户余额';





*/

int DbAgent::addFunds(const WmsPlatform::FundsNewUserReq& sIn, std::string& sOut) 
{
    try
    {


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

int DbAgent::getFunds(const WmsPlatform::FundsUserInfoReq& sIn, WmsPlatform::FundsUserInfoRes& sOut)
{
    try
    {


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

int DbAgent::modifyFunds(const WmsPlatform::FundsUserModifyReq& sIn, std::string& sOut)
{
    try
    {


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