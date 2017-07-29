#include "GameImp.h"
#include "servant/Application.h"

using namespace std;

//////////////////////////////////////////////////////
void GameImp::initialize()
{
    //initialize servant here:
    //...
    _db.init();  
}

//////////////////////////////////////////////////////
void GameImp::destroy()
{
    //destroy servant here:
    //...
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
    	 return 0;

    }
    catch(exception &ex)
    {
        cout << ex.what() << endl;
        return -1;
    }
}