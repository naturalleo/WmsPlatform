// #include <stdio.h>
// #include <curl/curl.h>
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

// size_t read_data(void *buffer, size_t size, size_t nmemb, void *user_p)
// {
//     TLOGDEBUG("====== : " << buffer << endl); 
//     // return fread(buffer, size, nmemb, (FILE *)user_p);
// }


// int WxoauthImp::testHttps(const WmsPlatform::WxoauthReq& sIn)
// {
//     string url = "https://api.weixin.qq.com/sns/oauth2/refresh_token?appid="
//                          + sIn.wechatAppId + "&grant_type=refresh_token"
//                          + "&refresh_token=" + sIn.refreshToken;    
//     CURL *curl;
//     CURLcode res;

//     curl_global_init(CURL_GLOBAL_DEFAULT);

//     curl = curl_easy_init();
//     if(curl) {
//         curl_easy_setopt(curl, CURLOPT_URL, url);

//         // curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
//         curl_easy_setopt(curl, CURLOPT_READFUNCTION, &read_data);
//         //curl_easy_setopt(curl, CURLOPT_READDATA, fp);
//         // curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, file_size);

//         /* Perform the request, res will get the return code */ 
//         res = curl_easy_perform(curl);
//         /* Check for errors */ 
//         if(res != CURLE_OK)
//             fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));

//         fclose(fp);
//         /* always cleanup */ 
//         curl_easy_cleanup(curl);        
//     }
//     curl_global_cleanup();

//     return 0;    
// }



int WxoauthImp::wxchatLogin(const WmsPlatform::WxoauthReq& sIn, WmsPlatform::WxLoginUserinfoRes& sOut, tars::TarsCurrentPtr current)
{

	try
    {
		string url = "http://api.weixin.qq.com/sns/userinfo?access_token="
					 + sIn.accessToken + "&openid="
					 +sIn.openId + "&lang=zh_CN";
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
	    TLOGDEBUG("WxoauthImp wxchatLogin content " << stHttpRep.getContent() << endl);
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
                    // if(sIn.appId == "2") //岭南麻将测试
                    // {
                    //     string url = "https://api.weixin.qq.com/sns/oauth2/refresh_token?appid="
                    //      + sIn.wechatAppId + "&grant_type=refresh_token"
                    //      + "&refresh_token=" + sIn.refreshToken;
                    //     TLOGDEBUG("WxoauthImp wxchatLogin refresh_token : " << url << endl); 
                    //     TC_HttpRequest stHttpReq;
                    //     stHttpReq.setCacheControl("no-cache");
                    //     stHttpReq.setGetRequest(url,true);
                    //     stHttpReq.encode();
                    //     TC_HttpResponse stHttpRep;
                    //     int iRet = stHttpReq.doRequest(stHttpRep, 3000);
                    //     if(iRet != 0)
                    //     {
                    //         TLOGERROR("WxoauthImp wxchatLogin refresh_token iRet!=0 : " << iRet << endl);
                    //         return -1;
                    //     }
                    //     string isRefreshToken = "";
                    //     string wechatAccessToken = "";
                    //     string wechatRefreshToken = "";

                    //     TLOGDEBUG("WxoauthImp wxchatLogin refresh_token content " << stHttpRep.getContent() << endl);
                    //     using rapidjson::Document ;
                    //     rapidjson::Document document;
                    //     document.Parse(stHttpRep.getContent().c_str());

                    //     if (document.HasParseError()) // 通过HasParseError()来判断解析是否成功
                    //     {
                    //         TLOGERROR("parse error: " << document.GetParseError() << document.GetErrorOffset() );
                    //         return -1;
                    //     }
                    //     else
                    //     {
                    //          if (document.HasMember("errcode") )
                    //          {
                    //             rapidjson::Value& count_json = document["errcode"];
                    //             int errcode = count_json.GetInt();
                    //             if (errcode)
                    //             {
                    //                 //失效，返回错误码，让客户端重新授权登录
                    //                 return 1;  
                    //             }
                    //         }
                    //         else
                    //         {
                    //             wechatAccessToken = (document["access_token"]).GetString();
                    //             wechatRefreshToken = (document["refresh_token"]).GetString();
                    //             isRefreshToken = "true";


                    //             string url = "http://api.weixin.qq.com/sns/userinfo?access_token="
                    //                          + wechatAccessToken + "&openid="
                    //                          +sIn.openId + "&lang=zh_CN";
                    //             TLOGDEBUG("WxoauthImp wxchatLogin again url : " << url << endl);
                    //             TC_HttpRequest stHttpReq;
                    //             stHttpReq.setCacheControl("no-cache");
                    //             stHttpReq.setGetRequest(url);
                    //             TC_HttpResponse stHttpRep;
                    //             int iRet = stHttpReq.doRequest(stHttpRep, 3000);
                    //             if(iRet != 0)
                    //             {
                    //                 TLOGERROR("WxoauthImp wxchatLogin again iRet!=0 : " << iRet << endl);
                    //                 return -1;
                    //             }
                    //             TLOGDEBUG("WxoauthImp wxchatLogin again content " << stHttpRep.getContent() << endl);
                    //             using rapidjson::Document ;
                    //             rapidjson::Document document;
                    //             document.Parse(stHttpRep.getContent().c_str());

                    //             if (document.HasParseError()) // 通过HasParseError()来判断解析是否成功
                    //             {
                    //                 TLOGERROR("parse error: " << document.GetParseError() << document.GetErrorOffset() );
                    //                 //sOut = "{\"status\":1,\"errCode\":10401,\"error\":\"winxin erro callback\",\"data\":[]}";
                    //                 return -1;
                    //             }
                    //             else
                    //             {
                    //                 if (document.HasMember("errcode") )
                    //                 {
                    //                     rapidjson::Value& count_json = document["errcode"];

                    //                     int errcode = count_json.GetInt();

                    //                     if (errcode == 42001 or errcode == 40001 or errcode == 40014)
                    //                     {
                    //                         //失效，返回错误码，让客户端重新授权登录
                    //                         return 1;  
                    //                     }
                    //                 }
                    //                 else
                    //                 {
                    //                     WxLoginUserinfoReq req;
                    //                     req.unionId     = (document["unionid"]).GetString();
                    //                     req.headimgurl  = (document["headimgurl"]).GetString();
                    //                     req.nickname    = (document["nickname"]).GetString();
                    //                     req.sex         = TC_Common::tostr((document["sex"]).GetInt());
                    //                     req.openId      = (document["openid"]).GetString();
                    //                     req.appCode     = sIn.appCode;
                    //                     req.appId       = sIn.appId;
                    //                     req.appGroupId  = "1";
                    //                     req.isRefreshToken = isRefreshToken;
                    //                     req.wechatAccessToken = wechatAccessToken;
                    //                     req.wechatRefreshToken = wechatRefreshToken; 
                    //                     if (0 == getLoginUseInfo(req, sIn, sOut))
                    //                     {
                    //                         TLOGDEBUG("WxoauthImp wxchatLogin again success! " << endl);
                    //                         return 0;
                    //                     }
                    //                     else
                    //                     {
                    //                         return -1;         
                    //                     }
                    //                 }   
                    //             }
                    //         }
                    //     }
                    // }
                    // else
                    // {
                    //     //湖南麻将
                    //     //errcode == 42001 or errcode == 40001 or errcode == 40014
                    //     return 1;   
                    // }    
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
                    return 0;
                }
                else
                {
                    return -1;
                }
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