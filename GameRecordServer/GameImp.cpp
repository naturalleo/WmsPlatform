#include "GameImp.h"
#include "servant/Application.h"

using namespace std;

extern TC_Config * g_pconf;

//////////////////////////////////////////////////////
void GameImp::initialize()
{
    //initialize servant here:
    //...
    TLOGDEBUG("begin GameImp initialize"<<endl);
    try
    {
        loadconf();
        //string order = "";
        //_db.generaterOrderID("hello",order);
    }
    catch(exception &ex)
    {
        cout << ex.what() << endl;
    }
}

//////////////////////////////////////////////////////
void GameImp::destroy()
{
    //destroy servant here:
    //...
}

void GameImp::loadconf()
{
    TC_DBConf tcDBConf;
    tcDBConf.loadFromMap(g_pconf->getDomainMap("/tars/db"));
    _db.init(tcDBConf);
}

int GameImp::getPlayerGameRecord(const WmsPlatform::GameRecordReq& sIn, vector<GameRecordItem>& sOut, tars::TarsCurrentPtr current)
{
	TLOGDEBUG("getPlayerGameRecord : " <<  endl);
    try
    {
    	 return _db.getGameRecord(sIn, sOut);

    }
    catch(exception &ex)
    {
        cout << ex.what() << endl;
        return -1;
    }

}

int GameImp::getPlayerGameDetailRecord(const WmsPlatform::GameRecordDetailReq& sIn, vector<GameRecordDetailItem>& sOut, tars::TarsCurrentPtr current)
{
	TLOGDEBUG("getPlayerGameDetailRecord : " << endl);
    try
    {
    	 return _db.getGameRecordDetail(sIn, sOut);

    }
    catch(exception &ex)
    {
        cout << ex.what() << endl;
        return -1;
    }
}
int GameImp::getPlayerShareRecord(const WmsPlatform::GameShareRecordReq& sIn, string& sOut, tars::TarsCurrentPtr current)
{
	TLOGDEBUG("getPlayerShareRecord : " << endl);
    try
    {
        
    	 return _db.getGameSharesRecord(sIn, sOut);

    }
    catch(exception &ex)
    {
        cout << ex.what() << endl;
        return -1;
    }
}