#include "WxUserinfoImp.h"
#include "servant/Application.h"

using namespace std;

//////////////////////////////////////////////////////
void WxUserinfoImp::initialize()
{
    //initialize servant here:
    //...

	TLOGDEBUG("begin WxUserinfoImp initialize"<<endl);
    try
    {

    	 _db.init();
        //string order = "";
    	//_db.generaterOrderID("hello",order);
    }
    catch(exception &ex)
    {
        cout << ex.what() << endl;
    }
}

//////////////////////////////////////////////////////
void WxUserinfoImp::destroy()
{
    //destroy servant here:
    //...
}

int WxUserinfoImp::getWxUserinfo(const WmsPlatform::WxUserinfoReq& sIn, WmsPlatform::WxUserinfoRes& sOut, tars::TarsCurrentPtr current)
{
	TLOGDEBUG("getWxUserinfo : " << sIn.unionid << endl);
    try
    {

    	 _db.getDbUserinfo(sReq, sOut.userId)
    }
    catch(exception &ex)
    {
        cout << ex.what() << endl;
        return -1;
    }

    
	return 0
}
