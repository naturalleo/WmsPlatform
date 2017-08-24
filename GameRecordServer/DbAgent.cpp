#include "DbAgent.h"
#include <vector>



using namespace std;
using namespace tars;

const map<string, string> DbAgent::_charLib = DbAgent::createLib();

int DbAgent::init()
{
	TLOGDEBUG("begin DbAgent init"<<endl);
	try{
        //_mysqlReg.init("10.17.174.171", "linweixiong", "qipai123987", "db_tars_game");
        //_mysqlReg.init("192.168.1.103", "tars", "tars2015", "db_games");
        //_mysqlReg.connect();
        //_mysqlReg.init("192.168.1.103", "tars", "tars2015", "db_funds");
        _mysqlReg.init("10.66.205.133", "root", "QDi9zPVycg", "db_huhan_stat");

     }catch(exception &ex)
     {
      	TLOGDEBUG("exception "<< ex.what() << endl);
      	return -1;
     }
	 TLOGDEBUG("DbAgent init ok"<<endl);
	 return 0;
}


int DbAgent::getGameRecord(const WmsPlatform::GameRecordReq& sIn, vector<GameRecordItem> &sOut) 
{
    try
    {
      string sSql = "SELECT r.* FROM player_history_log AS p LEFT JOIN room_result_log AS r ON p.room_id_index=r.ID"
                    " WHERE p.uid =" + sIn.userId + " AND r.game_num >0  HAVING r.ID>0 and r.chair_2_uid>0 ORDER BY p.room_id_index DESC LIMIT 10";

      tars::TC_Mysql::MysqlData item = _mysqlReg.queryRecord(sSql);


      if (item.size() == 0)
      {

        return 0;
      }
      else
      {
        for (size_t i = 0 ; i < item.size(); ++i)
        {
          GameRecordItem t;
          t.shareCode        = item[i]["ID"];
          t.room_id          = item[i]["room_id"];
          t.owner            = item[i]["owner"];
          t.end_time         = item[i]["end_time"];
          t.game_num         = item[i]["game_num"];

          t.chair_1_uid      = item[i]["chair_1_uid"];
          t.chair_1_name     = item[i]["chair_1_name"];
          t.chair_1_point    = item[i]["chair_1_point"];     
          t.chair_1_avatar   = item[i]["chair_1_image"]; 

          t.chair_2_uid      = item[i]["chair_2_uid"];
          t.chair_2_name     = item[i]["chair_2_name"];
          t.chair_2_point    = item[i]["chair_2_point"];     
          t.chair_2_avatar   = item[i]["chair_2_image"]; 

          t.chair_3_uid      = item[i]["chair_3_uid"];
          t.chair_3_name     = item[i]["chair_3_name"];
          t.chair_3_point    = item[i]["chair_3_point"];     
          t.chair_3_avatar   = item[i]["chair_3_image"]; 

          t.chair_4_uid      = item[i]["chair_4_uid"];
          t.chair_4_name     = item[i]["chair_4_name"];
          t.chair_4_point    = item[i]["chair_4_point"];     
          t.chair_4_avatar   = item[i]["chair_4_image"];

          sOut.push_back(t);
        }


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

string DbAgent::specialStrChange(string &s)
{
  return tars::TC_Common::replace(s, DbAgent::_charLib);
}


int DbAgent::getGameRecordDetail(const WmsPlatform::GameRecordDetailReq& sIn, vector<GameRecordDetailItem>& sOut)
{
    try
    {

      string sSql = "SELECT t1.*,t2.config FROM game_result_log AS t1 LEFT JOIN build_room_log AS t2 ON t1.room_id=t2.room_id"
                    " WHERE t1.room_id =" + sIn.room_id+ " AND t1.owner = " + sIn.owner + " "
                    "AND (t1.chair_1_uid =" +  sIn.userId + " OR t1.chair_2_uid = " +  sIn.userId + " OR t1.chair_3_uid = " +  sIn.userId + " OR  t1.chair_4_uid = " +  sIn.userId + ") ORDER BY t1.game_index;";

     tars::TC_Mysql::MysqlData item = _mysqlReg.queryRecord(sSql);


      if (item.size() == 0)
      {

        return 0;
      }
      else
      {
        for (size_t i = 0 ; i < item.size(); ++i)
        {
          GameRecordDetailItem t;
          string json_1 = item[i]["config"];
          string json_2 = item[i]["game_action"];
          t.room_id          = item[i]["room_id"];
          t.owner            = item[i]["owner"];
          t.game_index       = item[i]["game_index"];
          t.end_time         = item[i]["end_time"];
          t.config           = specialStrChange(json_1); 
          t.game_action      = specialStrChange(json_2); 

          t.chair_1_uid      = item[i]["chair_1_uid"];
          t.chair_1_point    = item[i]["chair_1_point"];     

          t.chair_2_uid      = item[i]["chair_2_uid"];
          t.chair_2_point    = item[i]["chair_2_point"];     

          t.chair_3_uid      = item[i]["chair_3_uid"];
          t.chair_3_point    = item[i]["chair_3_point"];     

          t.chair_4_uid      = item[i]["chair_4_uid"];
          t.chair_4_point    = item[i]["chair_4_point"]; 

          sOut.push_back(t);
        }

    }
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

int DbAgent::getGameSharesRecord(const WmsPlatform::GameShareRecordReq& sIn, WmsPlatform::GameShareRecordRes& sOut)
{
    try
    {
      //SELECT t1.*,t2.config FROM game_result_log AS t1 LEFT JOIN build_room_log AS t2 ON t1.room_id=t2.room_id  WHERE t1.ID=155703;

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