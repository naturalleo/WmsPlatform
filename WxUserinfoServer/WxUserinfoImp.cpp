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
         _FundsPrx = Application::getCommunicator()->stringToProxy<FundsPrx>("WmsPlatform.FundsManagerServer.FundsObj");
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
	TLOGDEBUG("getWxUserinfo : " << sIn.unionId << endl);
    try
    {
    	 int iRet = _db.getDbUserinfo(sIn, sOut.userId);
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

    
	return 0;
}
int WxUserinfoImp::getWxUserIsAgent(const WmsPlatform::WxUserisAgentReq& sIn, WmsPlatform::WxUserisAgentRes& sOut, tars::TarsCurrentPtr current)
{
    TLOGDEBUG("getWxUserIsAgent : " << sIn.userId << endl);
    try
    {
         int result = 0;
         int iRet = _db.isUserAgent(sIn.userId, result) ;
         if(iRet == 0) 
         {
            sOut.errorCode = 0 ;
            sOut.isAgent = result;
         }
         else
         {
             sOut.errorCode = -1 ;

            TLOGERROR("WxUserinfoImp getWxUserIsAgent iRet != 0: " << iRet);
            return -1;
         }
    }
    catch(exception &ex)
    {
        cout << ex.what() << endl;
        return -1;
    }

    
    return 0  ;
}
/*
struct FundsUserModifyOtherReq
{
    0 require  string userId;
    1 require  string otherId; 
    2 require  string opcode ;
    3 require  string cards;      
};

struct WxUserExchangeReq
{
    0 require  string userId;
    1 require  string otherId;
    2 require  string opcode ;
    3 require  string cards;

};
struct WxUserExchangeRes
{
    0 require  integer errorCode;
    1 require  integer userId;
    2 require  integer cards;
};

struct FundsUserModifyOtherRes
{
    0 require  string userId;
    1 require  string cards ;
 
};
*/

int WxUserinfoImp::updateWxUserCards(const WmsPlatform::WxUserExchangeReq& sIn, WmsPlatform::WxUserExchangeRes& sOut, tars::TarsCurrentPtr current)
{
    TLOGDEBUG("updateWxUserCards : " << sIn.userId << endl);
    try
    {
         int result = 0;
         int iRet = _db.isUserAgent(sIn.userId, result) ;
         if(iRet == 0) 
         {
            sOut.errorCode = 0 ;

            if (result == 0) // 表明是代理身份
            {
                FundsUserModifyOtherReq req;
                req.userId = sIn.userId;
                req.otherId = sIn.otherId;
                req.cards = sIn.cards;
                req.opcode = sIn.opcode;
                FundsUserModifyOtherRes res;
                if (_FundsPrx->modifyFundsOther(req, res) != -1)
                {
                    sOut.errorCode = 0;
                    sOut.userId = sIn.userId;
                    sOut.cards = res.cards;
                }
                else
                {
                    sOut.errorCode = -1;
                }

            }
            else
            {
                sOut.errorCode = -1 ;
            }
            return 0;
         }
         else
         {
             sOut.errorCode = -1 ;

            TLOGERROR("WxUserinfoImp getWxUserIsAgent iRet != 0: " << iRet);
            return -1;
         }
    }
    catch(exception &ex)
    {
        cout << ex.what() << endl;
        return -1;
    }

    
    return 0 ;
}