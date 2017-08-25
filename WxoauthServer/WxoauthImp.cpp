#include "WxoauthImp.h"
#include "servant/Application.h"
#include "util/tc_http.h"
#include "util/tc_md5.h"


using namespace std;

const string WxoauthImp:: _basekey1 = "e4b,KyiniuApi>VmZg7J!y";
const string WxoauthImp:: _basekey2 = "<zwrUG2^?vN;ixApp";
//////////////////////////////////////////////////////
void WxoauthImp::initialize()
{
    //initialize servant here:
    //... 
    _wxuserinfoPrx = Application::getCommunicator()->stringToProxy<WxUserinfoPrx>("WmsPlatform.WxUserinfoServer.WxUserinfoObj");
    _orderPrx   = Application::getCommunicator()->stringToProxy<OrderPrx>("WmsPlatform.OrderManagerServer.OrderObj");
}

//////////////////////////////////////////////////////
void WxoauthImp::destroy()
{
    //destroy servant here:
    //...
}




int WxoauthImp::wxchatLogin(const WmsPlatform::WxoauthReq& sIn, WmsPlatform::WxLoginUserinfoRes& sOut, tars::TarsCurrentPtr current)
{

	try
    {
    	//TLOGDEBUG("WxoauthImp wxchatLogin ok begin " << endl);
		string url = "http://api.weixin.qq.com/sns/userinfo?access_token="
					 + sIn.accessToken + "&openid="
					 +sIn.openId + "&lang=zh_CN";
    	//string url = "https://api.weixin.qq.com/sns/userinfo";
		TLOGDEBUG("WxoauthImp wxchatLogin url : " << url << endl);
		TC_HttpRequest stHttpReq;
	    stHttpReq.setCacheControl("no-cache");

	    stHttpReq.setGetRequest(url);

	    TC_HttpResponse stHttpRep;

	    int iRet = stHttpReq.doRequest(stHttpRep, 3000);
	    if(iRet != 0)
	    {
	        TLOGERROR("WxoauthImp wxchatLogin iRet!=0 : " << iRet << endl);
	        return -1;
	    }


	   // string r = stHttpRep.encode();
	   //TLOGDEBUG("WxoauthImp wxchatLogin ok" << r << endl);
	    TLOGDEBUG("WxoauthImp wxchatLogin content " << stHttpRep.getContent() << endl);
	   // TLOGDEBUG("WxoauthImp headers " << TC_Common::tostr(stHttpRep.getHeaders()) << endl);
        using rapidjson::Document ;
	    rapidjson::Document document;
	    document.Parse(stHttpRep.getContent().c_str());

	    if (document.HasParseError()) // 通过HasParseError()来判断解析是否成功
    	{
    		TLOGERROR("parse error: " << document.GetParseError() << document.GetErrorOffset() );
    		//sOut = "{\"status\":1,\"errCode\":10401,\"error\":\"winxin erro callback\",\"data\":[]}";
    		return -1;
    	}
  		else
    	{
		 if (document.HasMember("errcode") )
		 {
            rapidjson::Value& count_json = document["errcode"];

            int errcode = count_json.GetInt();

            if (errcode == 42001 or errcode == 40001 or errcode == 40014)
            {
                // 后续补充
                //sOut = "{\"status\":1,\"errCode\":10401,\"error\":\"winxin erro callback\",\"data\":[]}";
                /*
                WxLoginUserinfoReq req;
                req.unionId ="oPNdvwN0QlM_lYx1oxTdLVxWIxmM";
                req.headimgurl = "http:\/\/wx.qlogo.cn\/mmhead\/PiajxSqBRaELiahujtxQMlC6R0dFaEzk4elicicr03afBHdRZmS1UL7DFg\/0";
                req.nickname = "YK";
                req.sex = "1";
                req.openId = "oLDB1wGkAE10jbj9TJ9lVbQ_16rs";
                req.appId = "1";
                req.appGroupId = "1";
                req.appCode = "klmgphz";
                req.ip = sIn.ip;
                req.clientFrom = sIn.clientFrom;
                
                if (0 == getLoginUseInfo(req, sIn, sOut))
                    return 0;
                else
                    return -1;
                */
               
               return 1;               
            }

		 }
		 else
		 {


            WxLoginUserinfoReq req;
            req.unionId     = (document["unionid"]).GetString();
            req.headimgurl  = (document["headimgurl"]).GetString();
            req.nickname    = (document["nickname"]).GetString();
            req.sex         = TC_Common::tostr((document["sex"]).GetInt());
            req.openId      = (document["openid"]).GetString();
            req.appCode     = sIn.appCode;
            req.appId       = sIn.appId;
            req.appGroupId  = "1";
            if (0 == getLoginUseInfo(req, sIn, sOut))
            {
                //string token = getLoginToken();
               // if(0 == _orderPrx->updateUserToken(sIn, sIn, token))
               // {
                //    sOut.token = token ;
                    return 0;
               // }
            }
            else
                return -1;
		 }   		
    	}
	}
    catch(exception &ex)
    {
        cout << ex.what() << endl;
    }
    return -1;
}




int WxoauthImp::getLoginUseInfo(const WxLoginUserinfoReq &req, WmsPlatform::WxoauthReq sIn, WxLoginUserinfoRes &res)
{
    try
    {
	    int iRet = _wxuserinfoPrx->getLoginWxUserinfo(req, res);

        if(iRet != 0)
        {
        	TLOGERROR("WxoauthImp getWxUserinfo iRet != 0: " << iRet);
        	return iRet;
        }
        string token = getLoginToken();
        sIn.userId = res.userId ;
        iRet = _orderPrx->updateUserToken(sIn, token);
        res.token = token;
        if(iRet != 0)
        {
            TLOGERROR("WxoauthImp updateUserToken iRet != 0: " << iRet);
            return iRet;
        }
        return iRet;
    }
    catch(exception &ex)
    {
        TLOGERROR("WxoauthImp::getUseInfo exception:" << ex.what() << endl);
    }
    return -1;
}

string WxoauthImp::getLoginToken(string figure)
{
    string token;
    if (figure == "") 
         figure = TC_Common::now2ms();

    token = tars::TC_MD5::md5str(tars::TC_MD5::md5str(WxoauthImp::_basekey1 + figure) + WxoauthImp::_basekey2) ;

    return token;
}