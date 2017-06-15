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

/*

struct WxUserinfoReq
{
    0 require  string unionid;
    1 require  string appGroupId;
    2 require  string headimgurl;
    3 require  string nickname;
    4 require  string sex;
    5 require  string openid; 
    6 require  string appId;  
    7 require  string appGroupId; 
};


struct WxUserinfoRes
{
    0 require  string userId;


    1 require  string headimgurl;
    2 require  string nickname;
    3 require  string sex;  

    4 require string isNew;
    5 require string totalcard;
    6 require string currentcard;
    7 require string token;

};

*/

int WxUserinfoImp::getWxUserinfo(const WmsPlatform::WxUserinfoReq& sIn, WmsPlatform::WxUserinfoRes& sOut, tars::TarsCurrentPtr current)
{
	TLOGDEBUG("getWxUserinfo : " << sIn.unionid << endl);
    try
    {
    	 int iRet = _db.getDbUserinfo(sReq, sOut.userId)
    	 sOut.headimgurl =  sIn.headimgurl ;
    	 sOut.nickname   =  sIn.nickname ;
    	 sOut.sex 		 =  sIn.sex ;
    	 sOut.token      = "test"; // 后期MD5串化
    	 if(iRet == 0) 
    	 {
    	 	sOut.isNew = "0";
    	 }
    	 else if (iRet == 1)
    	 {
    	 	sOut.isNew = "1";
    	 }
    	 else
    	 {
    	 	TLOGERROR("WxUserinfoImp getWxUserinfo iRet != 0: " << iRet);
    	 	return -1;
    	 }
    }
    catch(exception &ex)
    {
        cout << ex.what() << endl;
        return -1;
    }

    
	return 0
}
