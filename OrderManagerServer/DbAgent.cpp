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