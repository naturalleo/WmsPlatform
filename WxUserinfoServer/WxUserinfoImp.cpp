#include "WxUserinfoImp.h"
#include "servant/Application.h"
#include "util/tc_md5.h"


using namespace std;
extern TC_Config * g_pconf;

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

    	 loadconf();
         _FundsPrx = Application::getCommunicator()->stringToProxy<FundsPrx>("WmsPlatform.FundsManagerServer.FundsObj");
         _OrderPrx = Application::getCommunicator()->stringToProxy<OrderPrx>("WmsPlatform.OrderManagerServer.OrderObj");
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
void WxUserinfoImp::loadconf()
{
    TC_DBConf tcDBConf;
    tcDBConf.loadFromMap(g_pconf->getDomainMap("/tars/db"));
    _db.init(tcDBConf);
}

int WxUserinfoImp::getLoginWxUserinfo(const WmsPlatform::WxLoginUserinfoReq& sIn, WmsPlatform::WxLoginUserinfoRes& sOut, tars::TarsCurrentPtr current)
{
	TLOGDEBUG("getLoginWxUserinfo : " << sIn.unionId << endl);
    try
    {
    	int iRet = _db.getLoginDbUserinfo(sIn, sOut.userId, sOut.activationCode);
        if (iRet == -1)
        {
            TLOGERROR("WxUserinfoImp getWxUserinfo iRet != 0: " << iRet);
            return -1;
        }
        else if(iRet == 1) 
        {
           sOut.isNew = "1";
        }
        else
        {
            sOut.isNew = "0";
        }

        FundsUserInfoReq req;
        FundsUserInfoRes res;
        req.userId   = sOut.userId ;
        req.appId    = sIn.appId;
        req.appCode  = sIn.appCode;

        TLOGDEBUG("getLoginWxUserinfo req.userId : " << req.userId << endl);

        iRet = _FundsPrx->getFunds(req, res);
        if (iRet == -1) 
        {
           TLOGERROR("WxUserinfoImp getWxUserinfo iRet != 0: " << iRet);
           return -1;
        }


    	sOut.headimgurl  =  sIn.headimgurl ;
    	sOut.nickname    =  sIn.nickname ;
    	sOut.sex 		 =  sIn.sex ;
        sOut.totalcard   = res.totalcard;
        sOut.currentcard = res.currentcard;
    }
    catch(exception &ex)
    {
        cout << ex.what() << endl;
        return -1;
    }

    
	return 0;
}


int WxUserinfoImp::getWxUserinfo(const WmsPlatform::WxUserinfoReq& sIn, WmsPlatform::WxUserinfoRes& sOut, tars::TarsCurrentPtr current)
{
    TLOGDEBUG("getWxUserinfo : " << sIn.userId << endl);
    try
    {
        int ret = _db.getDbUserinfo(sIn, sOut);

        if (ret != 0) 
            return ret;
        string ip ; 

        ret = _OrderPrx->checkUserToken(sIn, ip);

        if (ret != 0)
            return ret;

        sOut.ip = ip;

        return  0 ;
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
         string agentType = "0";
         int iRet = _db.isUserAgent(sIn.userId, agentType) ;
         if(iRet == 0) 
         {
            sOut.errorCode = 0 ;
            sOut.userId = sIn.userId;
            sOut.agentType = agentType;
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
        WxUserinfoReq req;
        req.userId = sIn.userId;
        req.appId = sIn.appId;
        req.appCode = sIn.appCode;
        req.token = sIn.token;
        string ip ; 
        if (_OrderPrx->checkUserToken(req, ip) != 0)
        {
            sOut.errorCode = -1;
            TLOGERROR("房卡转让：玩家token校验失败，不能转让房卡! userId="<< sIn.userId << endl);
            //暂时屏蔽token校验
            // return -1;
        }
        if (sIn.userId == sIn.otherId)
        {
            sOut.errorCode = -6 ;
            TLOGERROR("房卡转让：不能转让给自己! userId="<< sIn.userId << endl);
            return 0;
        }
        string agentType = "0";
        int iRet = _db.isUserAgent(sIn.userId, agentType) ;
        if(iRet == 0) 
        {
            sOut.errorCode = 0 ;
            if (agentType == "1" || agentType == "2" ) // 只有1级代理和2级代理才能转让房卡，3级代理（推广员）不能转让房卡，但可以作为激活码
            {

                string otherAgentType = "0";
                int ret = _db.isUserAgent(sIn.otherId, otherAgentType) ;
                if (ret == 0)
                {
                    if (otherAgentType != "0")
                    {
                        if (TC_Common::strto<int>(otherAgentType) <= TC_Common::strto<int>(agentType))
                        {
                            sOut.errorCode = -8 ;
                            TLOGERROR("房卡转让：对方是代理，并且同属一级不能转让，我的代理等级="<< agentType << endl); 
                            TLOGERROR("房卡转让：对方是代理，并且同属一级不能转让，对方代理等级"<< otherAgentType << endl);                              
                        }
                        else
                        {
                            FundsUserModifyOtherReq req;
                            req.userId = sIn.userId;
                            req.otherId = sIn.otherId;
                            req.cards = sIn.cards;
                            req.appId = sIn.appId;
                            req.appCode = sIn.appCode;
                            
                            FundsUserModifyOtherRes res;
                            int ret = _FundsPrx->modifyFundsOther(req, res);
                            if (ret == 0)
                            {
                                sOut.errorCode = 0;
                                sOut.userId = res.userId;
                                sOut.totalcard = res.cards;
                                sOut.currentcard = res.cards;
                                TLOGERROR("房卡转让：转让房卡成功uid="<< sIn.userId << " 对方ID=" <<sIn.otherId<< " 房卡数="<< sIn.cards <<endl); 
                            }
                            else
                            {
                                sOut.errorCode = ret;
                            }                   
                        }
                    }
                    else
                    {
                        FundsUserModifyOtherReq req;
                        req.userId = sIn.userId;
                        req.otherId = sIn.otherId;
                        req.cards = sIn.cards;
                        req.appId = sIn.appId;
                        req.appCode = sIn.appCode;
                        
                        FundsUserModifyOtherRes res;
                        int ret = _FundsPrx->modifyFundsOther(req, res);
                        if (ret == 0)
                        {
                            sOut.errorCode = 0;
                            sOut.userId = res.userId;
                            sOut.totalcard = res.cards;
                            sOut.currentcard = res.cards;
                            TLOGERROR("房卡转让：转让房卡成功uid="<< sIn.userId << " 对方ID=" <<sIn.otherId<< " 房卡数="<< sIn.cards <<endl); 
                        }
                        else
                        {
                            sOut.errorCode = ret;
                        }                    
                    }  
                }
            }
            else
            {
                sOut.errorCode = -5 ;
                TLOGERROR("房卡转让：玩家不是代理，不能转让房卡!"<< sIn.userId << endl);
            }
            return 0;
        }
        else
        {
            sOut.errorCode = -1 ;
            TLOGERROR("房卡转让：异常操作，在数据库中找不到该操作玩家: "<< sIn.userId << endl);
            TLOGERROR("WxUserinfoImp getWxUserIsAgent iRet != 0: " << iRet << endl);
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

    token = tars::TC_MD5::md5str(tars::TC_MD5::md5str(WxUserinfoImp::_basekey1 + figure) + WxUserinfoImp::_basekey2) ;

    return token;
}

int WxUserinfoImp::setActivationCode(const WmsPlatform::WxUserSetActivationCodeReq& sIn, WmsPlatform::WxUserSetActivationCodeRes& sOut, tars::TarsCurrentPtr current)
{
    TLOGDEBUG("setActivationCode : " << sIn.userId << endl);
    try
    { 
        if(sIn.userId == sIn.activationCode)
        {
            sOut.errorCode = -6;
            TLOGERROR("设置激活码：不能输入自己ID!"<< sIn.activationCode << endl);
            return 0;
        }

        string agentType = "0";
        int iRet = _db.isUserAgent(sIn.activationCode, agentType);
        if(iRet == 0) 
        {
            sOut.errorCode = 0 ;
            if (agentType != "0") // 表明是代理身份
            {
                int ret = _db.activationCodeSet(sIn.userId,sIn.activationCode);
                if (ret == 0)
                {
                    sOut.errorCode = 0;
                    sOut.userId = sIn.userId;
                    sOut.activationCode = sIn.activationCode;
                }
                else
                {
                    sOut.errorCode = -3 ;
                    TLOGERROR("设置激活码：设置激活码失败，数据库语句操作错误 "<< sIn.userId << endl);
                    TLOGERROR("WxUserinfoImp setActivationCode iRet != 0: " << iRet << endl);
                    return -1;            
                }
            }
            else
            {
                sOut.errorCode = -2 ;
                TLOGERROR("设置激活码：填入的激活码不是代理!"<< sIn.activationCode << endl);
            }
            return 0;
        }
        else
        {
            sOut.errorCode = -1 ;
            TLOGERROR("设置激活码：是否代理/推广员，在数据库中找不到该操作玩家: "<< sIn.userId << endl);
            TLOGERROR("WxUserinfoImp setActivationCode isUserAgent iRet != 0: " << iRet << endl);
            return -1;
        }        


        // string otherAppId = "";
        // string otherAppCode = "";
        // int ret = _db.selectUserGameType(sIn.activationCode,otherAppId,otherAppCode);
        // if(ret == 1)
        // {
        //     if (otherAppId == sIn.appId && otherAppCode == sIn.appCode)
        //     {
        //         string agentType = "0";
        //         int iRet = _db.isUserAgent(sIn.activationCode, agentType);
        //         if(iRet == 0) 
        //         {
        //             sOut.errorCode = 0 ;
        //             if (agentType != "0") // 表明是代理身份
        //             {
        //                 int ret = _db.activationCodeSet(sIn.userId,sIn.activationCode);
        //                 if (ret == 0)
        //                 {
        //                     sOut.errorCode = 0;
        //                     sOut.userId = sIn.userId;
        //                     sOut.activationCode = sIn.activationCode;
        //                 }
        //                 else
        //                 {
        //                     sOut.errorCode = -3 ;
        //                     TLOGERROR("设置激活码：设置激活码失败，数据库语句操作错误 "<< sIn.userId << endl);
        //                     TLOGERROR("WxUserinfoImp setActivationCode iRet != 0: " << iRet << endl);
        //                     return -1;            
        //                 }
        //             }
        //             else
        //             {
        //                 sOut.errorCode = -2 ;
        //                 TLOGERROR("设置激活码：填入的激活码不是代理!"<< sIn.activationCode << endl);
        //             }
        //             return 0;
        //         }
        //         else
        //         {
        //             sOut.errorCode = -1 ;
        //             TLOGERROR("设置激活码：是否代理/推广员，在数据库中找不到该操作玩家: "<< sIn.userId << endl);
        //             TLOGERROR("WxUserinfoImp setActivationCode isUserAgent iRet != 0: " << iRet << endl);
        //             return -1;
        //         }         
        //     }
        //     else
        //     {
        //         if(sIn.appCode == "hnmj")
        //         {
        //             sOut.errorCode = -4 ;
        //             TLOGERROR("设置激活码：填入的激活码不是湖南麻将的激活码!activationCode="<< sIn.activationCode << endl);
        //         }
        //         else
        //         {
        //             sOut.errorCode = -5 ;
        //             TLOGERROR("设置激活码：填入的激活码不是岭南麻将的激活码!activationCode="<< sIn.activationCode << endl);
        //         }
        //     }  
        // }
        // else
        // {
        //     sOut.errorCode = -5 ;
        //     TLOGERROR("设置激活码：查询玩家，在数据库中找不到该操作玩家: "<< sIn.activationCode << endl);
        //     TLOGERROR("WxUserinfoImp setActivationCode selectUserGameType ret != 0: " << ret << endl);
        //     return -1;
        // }
    }
    catch(exception &ex)
    {
        cout << ex.what() << endl;
        return -1;
    } 
    return 0 ;
}
