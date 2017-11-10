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


uint64_t DbAgent::generaterID()
{

    uint64_t time = TC_Common::now2ms();
 
    uint64_t id  = time + pthread_self();

    return id;
}



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

      TLOGDEBUG("updateUserToken  "<< oss.str() <<endl);
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


int DbAgent::checkUserToken(const WxUserinfoReq &req, string &ip)
{
    try
    {
      int pos = TC_Common::strto<int>(req.userId) % 10 ;
      ostringstream oss("");
      oss << "select ip from t_user_login_log_"<< TC_Common::tostr(pos) << " where userId = "<< req.userId << " and "
          << "appId = " << req.appId << " and appCode = '" << req.appCode << "' and loginToken = '" << req.token << "'" << " order by addTime DESC LIMIT 1";

      TLOGDEBUG("checkUserToken  "<< oss.str() <<endl);

      TC_Mysql::MysqlData item = _mysqlReg.queryRecord(oss.str());
      if (item.size() == 0)
      {
          return 1;
      }
      else
      {
        ip = item[0]["ip"];
        return 0 ;
      }

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

int DbAgent::reportSuggestion(const ReportSuggestionReq &req)
{
    try
    {
  
    string sSql = 
                "insert into t_suggestion (`userId`,`appId`,`appCode`,`clientFrom`,`version`,`suggestType`,`title`,`content`,`status`,`contacts`,`addTime`,`updateTime`) values " 
                        "(" + req.userId + " ," 
                        " " + req.appId + " ,"
                        " '" + req.appCode + "' ,"
                        " " + req.clientFrom + " ,"
                        " '" + req.clientVer + "' ,"
                        " " + req.suggestType + " ,"
                        " '" + req.title + "' ,"
                        " '" + req.content + "' ,"
                        " 0 ,"
                        " '" + req.contacts + "' ,"
                        " " + TC_Common::tostr(TC_Common::now2ms()/1000) + " ,"
                        " " + TC_Common::tostr(TC_Common::now2ms()/1000) + " )" ;

    _mysqlReg.execute(sSql);

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

int DbAgent::sysNotice(const SysNoticeReq& req, SysNoticeRes& res)
{
    try
    {
    
      ostringstream oss("");
      oss << "select aId, title, `desc`, content from t_article  where appId = "<< req.appId << " and "
          << " appCode = '" << req.appCode << "' and catId = 2  order by sort asc limit 0,1"  ;


      TC_Mysql::MysqlData item = _mysqlReg.queryRecord(oss.str());
      if (item.size() == 0)
      {
          return 1;
      }
      else
      {
       res.aId = item[0]["aId"];
       res.title = item[0]["title"];
       res.desc = item[0]["desc"];
       res.content = item[0]["content"];
      }
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