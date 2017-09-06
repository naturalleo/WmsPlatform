#include "DbAgent.h"




using namespace std;
using namespace tars;

int DbAgent::init()
{
	TLOGDEBUG("begin DbAgent init"<<endl);
	try{
        //_mysqlReg.init("10.17.174.171", "linweixiong", "qipai123987", "db_tars_game");
        //_mysqlReg.init("192.168.1.103", "tars", "tars2015", "db_games");
        //_mysqlReg.connect();
        //_mysqlReg.init("192.168.1.103", "tars", "tars2015", "db_funds");
        _mysqlReg.init("10.66.205.133", "root", "QDi9zPVycg", "db_funds");

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

int DbAgent::insertFunds(const WmsPlatform::FundsNewUserReq& sIn, WmsPlatform::FundsUserInfoRes& sOut) 
{
    try
    {
      string sSql = "insert into t_user_funds(`userId`,`surplusGameCard`,`totalUseGameCard`, `addTime`, `updateTime`, `appId`, `appCode`)"
                      "values"
                      "( '" + sIn.userId + "',"
                      " "   + TC_Common::tostr(10) + ","
                      " "   + TC_Common::tostr(10) + ","
                      " "   + TC_Common::tostr(TC_Common::now2ms()/1000) + ","
                      " "   + TC_Common::tostr(TC_Common::now2ms()/1000) + ","
                      "" + sIn.appId + " ,"
                      "'" + sIn.appCode + "')";

      _mysqlReg.execute(sSql);

      sOut.userId = sIn.userId;
      sOut.totalcard = "10";
      sOut.currentcard = "10";

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

int DbAgent::getOtherFunds(const WmsPlatform::FundsUserInfoReq& sIn, WmsPlatform::FundsUserInfoRes& sOut)
{
    try
    {

      string sSql = "select `userId`, `surplusGameCard`,`totalUseGameCard` from `t_user_funds` where userId = '" + sIn.userId + "' and appId = '" + sIn.appId + "and appCode = " + sIn.appCode +  "' limit 0,1";

      tars::TC_Mysql::MysqlData item = _mysqlReg.queryRecord(sSql);


      if (item.size() == 0)
      {
        return -1;
      }
      else
      {
        sOut.userId      = item[0]["userId"];
        sOut.totalcard   = item[0]["totalUseGameCard"];
        sOut.currentcard = item[0]["surplusGameCard"];

        return 0;
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


int DbAgent::getFunds(const WmsPlatform::FundsUserInfoReq& sIn, WmsPlatform::FundsUserInfoRes& sOut)
{
    try
    {

      string sSql = "select `userId`, `surplusGameCard`,`totalUseGameCard` from `t_user_funds` where userId = '" + sIn.userId + "' and appId = '" + sIn.appId + "and appCode = " + sIn.appCode +  "' limit 0,1";

      tars::TC_Mysql::MysqlData item = _mysqlReg.queryRecord(sSql);


      if (item.size() == 0)
      {
        string str;
        FundsNewUserReq req;
        req.userId   = sIn.userId;
        req.appId    = sIn.appId;
        req.appCode  = sIn.appCode;

        insertFunds(req, sOut);
        return 0;
      }
      else
      {
        sOut.userId      = item[0]["userId"];
        sOut.totalcard   = item[0]["totalUseGameCard"];
        sOut.currentcard = item[0]["surplusGameCard"];

        return 0;
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

int DbAgent::modifyFunds(const WmsPlatform::FundsUserModifyReq& sIn, WmsPlatform::FundsUserInfoRes& sOut)
{
    try
    {
      FundsUserInfoReq req;
      req.userId  = sIn.userId;
      req.appId   = sIn.appId;
      req.appCode = sIn.appCode;
      if (getFunds(req, sOut) != 0)   
        return -1;


      if (TC_Common::strto<int>(sIn.cards) <= 0)
        return -1;
      string sSql;
      if (sIn.opcode == "add")
      {
        sSql = "update  t_user_funds " 
                        "set surplusGameCard = surplusGameCard  - "   + sIn.cards + ","
                        "totalUseGameCard = totalUseGameCard - " + sIn.cards + " "
                        " where userId = '" + sIn.userId + "' and appId = " + sIn.appId + " and appCode = \'" + sIn.appCode + "\'";
      }
      else if (sIn.opcode == "sub")
      {
        sSql = "update  t_user_funds " 
                        "set surplusGameCard = surplusGameCard -"  + sIn.cards + ","
                        "totalUseGameCard = totalUseGameCard -" + sIn.cards + " "
                        " where userId = '" + sIn.userId + "' and appId = " + sIn.appId + " and appCode = \'" + sIn.appCode + "\'" ;

        if (TC_Common::strto<int>(sOut.currentcard) - TC_Common::strto<int>(sIn.cards) < 0)
        {
          TLOGDEBUG("DbAgent modifyFunds userId "<< sIn.userId<<"not enough" << sOut.currentcard<< ": " << sIn.cards << endl);
          return -1;
        }
      }
      else
        return -1;

      TLOGDEBUG("modifyFunds sql: " << sSql << endl);
      _mysqlReg.execute(sSql);

      insertUseCardLog(sIn);

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

int DbAgent::modifyFundsOther(const WmsPlatform::FundsUserModifyOtherReq& sIn, WmsPlatform::FundsUserModifyOtherRes& sOut)
{
    try
    {
        FundsUserInfoReq req;
        req.userId  = sIn.userId;
        req.appId   = sIn.appId;
        req.appCode = sIn.appCode;

        FundsUserInfoRes res;

        if (TC_Common::strto<int>(sIn.cards) <= 0)
        {
           TLOGDEBUG("转让房卡：转让房卡数出错，userId=" << sIn.userId <<" appId="<< sIn.appId << " appCode="<< sIn.appCode <<" cards="<< sIn.cards <<endl); 
          return -7;
        }

        if (getFunds(req, res) != 0)
        {
            TLOGDEBUG("转让房卡：找不到操作转让的玩家，userId=" << sIn.userId <<" appId="<< sIn.appId << " appCode="<< sIn.appCode <<endl); 
            return -2;
        }  
        if (TC_Common::strto<int>(res.currentcard) - TC_Common::strto<int>(sIn.cards) < 0)
        {
          TLOGDEBUG("转让房卡：房卡不足，转让失败！userId=" << sIn.userId <<" appId="<< sIn.appId << " appCode="<< sIn.appCode <<" cards="<< sIn.cards << " currentCards="<< res.currentcard << endl); 
          return -3;
        }

        FundsUserInfoReq req1;
        req1.userId  = sIn.otherId;
        req1.appId   = sIn.appId;
        req1.appCode = sIn.appCode;
        FundsUserInfoRes res1;  
        if (getOtherFunds(req1, res1) != 0)
        {
            TLOGDEBUG("转让房卡：找不到被转让的玩家/该玩家不是这省份的游戏，otherId=" << sIn.otherId <<" appId="<< sIn.appId << " appCode="<< sIn.appCode <<endl); 
            return -4;
        }   

        string sSql1 = "";
        sSql1 = "update  t_user_funds " 
                        "set surplusGameCard = surplusGameCard  - "   + sIn.cards + ","
                        "totalUseGameCard = totalUseGameCard - " + sIn.cards + " "
                        " where userId = '" + sIn.userId + "' and appId = " + sIn.appId + " and appCode = \'" + sIn.appCode + "\'";
        _mysqlReg.execute(sSql1);


        string sSql2 = "";
        sSql2 = "update  t_user_funds " 
                        "set surplusGameCard = surplusGameCard  + "   + sIn.cards + ","
                        "totalUseGameCard = totalUseGameCard + " + sIn.cards + " "
                        " where userId = '" + sIn.otherId + "' and appId = " + sIn.appId + " and appCode = \'" + sIn.appCode + "\'";
        _mysqlReg.execute(sSql2);

        sOut.userId = sIn.userId;
        int currentCards = TC_Common::strto<int>(sIn.cards) - TC_Common::strto<int>(res.currentcard);
        sOut.cards = TC_Common::tostr(currentCards);
        

        insertExchangeLog(sIn);

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

int DbAgent::insertUseCardLog(const WmsPlatform::FundsUserModifyReq& sIn) 
{
    try
    {
      string sSql = "insert into t_user_card_log(`userId`, `cards`, `appId`, `appCode`, `createTime`)"
                      "values"
                      "( '" + sIn.userId + "'," 
                      " "   + sIn.cards + ","
                      " " + sIn.appId + " ,"
                      "'" + sIn.appCode + "',"
                      " " + TC_Common::now2str() + ")";

      _mysqlReg.execute(sSql);
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

int DbAgent::insertExchangeLog(const WmsPlatform::FundsUserModifyOtherReq& sIn) 
{
    try
    {
      string sSql = "insert into t_user_exchange_log(`userId`,`otherId`,`cards`, `appId`, `appCode`, `createTime`)"
                      "values"
                      "( '" + sIn.userId + "',"
                      " "   + sIn.otherId + " ,"  
                      " "   + sIn.cards + ","
                      " " + sIn.appId + " ,"
                      "'" + sIn.appCode + "',"
                      " " + TC_Common::now2str() + ")";

      _mysqlReg.execute(sSql);
      // TLOGDEBUG(__FUNCTION__ << pthread_self() <<" affected: " << _mysqlReg.getAffectedRows() << endl);
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
