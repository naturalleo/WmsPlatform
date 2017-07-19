#include "WxUserinfoImp.h"
#include "servant/Application.h"

using namespace std;


const string WxUserinfoImp:: _basekey1 = "e4b,KyiniuApi>VmZg7J!y";
const string WxUserinfoImp:: _basekey2 = "<zwrUG2^?vN;ixApp";
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
    0 require  string unionId;
    1 require  string appGroupId;
    2 require  string headimgurl;
    3 require  string nickname;
    4 require  string sex;
    5 require  string openid; 
    6 require  string appId;
    7 require  string appCode;  
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


struct FundsUserInfoReq
{
    0 require  string userId;
    1 require  string appId;  
    2 require  string appCode;      
};



struct FundsUserInfoRes
{
    0 require  string userId;
    1 require string  totalcard;
    2 require string  currentcard;

};



*/

int WxUserinfoImp::getWxUserinfo(const WmsPlatform::WxUserinfoReq& sIn, WmsPlatform::WxUserinfoRes& sOut, tars::TarsCurrentPtr current)
{
	TLOGDEBUG("getWxUserinfo : " << sIn.unionId << endl);
    try
    {
    	int iRet = _db.getDbUserinfo(sIn, sOut.userId);
        if (iRet == -1)
        {
            TLOGERROR("WxUserinfoImp getWxUserinfo iRet != 0: " << iRet);
            return -1;
        }
        else if(iRet == 0) 
        {
           sOut.isNew = "1";
        }
        else
        {
            sOut.isNew = "0";
        }

        FundsUserInfoReq req;
        FundsUserInfoRes res;
        req.userId   = sIn.userId ;
        req.appId    = sIn.appId;
        req.appCode  = sIn.appCode;


        iRet = _FundsPrx->getFunds(req, res);
        if (iRet == -1) 
            return -1;


    	sOut.headimgurl  =  sIn.headimgurl ;
    	sOut.nickname    =  sIn.nickname ;
    	sOut.sex 		 =  sIn.sex ;
        sOut.totalcard   = res.totalcard;
        sOut.currentcard = res.currentcard;
 
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

string WxUserinfoImp::getLoginToken(string figure)
{
    string token;
    if (figure == "") 
         figure = TC_Common::now2ms();

    token = tars::TC_MD5::md5str(tars::TC_MD5::md5str(WxoauthImp::_basekey1 + figure) + WxoauthImp::_basekey2) ;

    return token;
}