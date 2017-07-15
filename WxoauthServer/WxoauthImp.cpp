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
    _wxuserinfoPrx = Application::getCommunicator()->stringToProxy<WxUserinfoPrx>("WmsPlatform.WxoauthServer.WxUserinfoObj");

}

//////////////////////////////////////////////////////
void WxoauthImp::destroy()
{
    //destroy servant here:
    //...
}


class WxUserinfoCallback : public WxUserinfoPrxCallback
{

public:
    WxUserinfoCallback(TarsCurrentPtr &current)
    : _current(current)
    {}

    virtual void callback_getWxUserinfo(tars::Int32 ret,  const WmsPlatform::WxUserinfoRes& sOut)
    {
        //HttpImp::async_response_doRequest(_current, ret, sOut);
        TLOGDEBUG("callback_getWxUserinfo : " << ret  << endl);
        TC_HttpResponse rsp;
        vector<char> buffer;
        string s = sOut.userId;
        rsp.setResponse(s.c_str(),s.size());
        rsp.encode(buffer);     


        _current->sendResponse(&buffer.at(0),buffer.size());
        TLOGDEBUG("callback_getWxUserinfo : " << s << s.size() << endl);
       // _current->sendResponse(tars::TARSSERVERSUCCESS, buffer);    
        //HttpImp::async_response_doRequest(_current, ret, buffer);
    }
    virtual void callback_getWxUserinfo_exception(tars::Int32 ret)
    { 
        TLOGERROR("WxoauthCallback callback_wxchatLogin_exception ret:" << ret << endl); 
        WmsPlatform::WxUserinfoRes res;

        WxUserinfo::async_response_getWxUserinfo(_current, ret, res);
    }

    TarsCurrentPtr _current;
};


/*
客户端请求
//http://10.17.174.171:8192/user/thirdPartyLogin?nettype=windows+net&appId=40&loginFrom=10&osVer=windows&model=windows&clientFrom=3
//&accessToken=test&openId=2000211&appCode=klmgphz&clientVer=0.1&uuId=windows+uuId&chanId=windows+0&operator=windows+Operator
//&macAddr=windows&imei=UnKnow+IMIE&sign=83054b51ab447abd38a3f49896cf20ce5f080c39
//
后台返回数据
{"status":1,"errCode":0,"error":"","data":{"mySign":"6ddb248cdb99ad9e596b05825543fdd6eb35ff97","userId":"2000211",
"status":"1","token":"46455a42e42b059fe7446fd7dea6c478","gender":"1","nickName":"klgdmj_guest_2000211","avatar":"",
"totalGameCard":"105","surplusGameCard":"62","isNew":0,"ip":"10.17.191.3","shareList":[],"w":true}}




//微信请求数据
//https://api.weixin.qq.com/sns/userinfo?access_token=OezXcEiiBSKSxW0eoylIeAsR0GmYd1awCffdHgb4fhS_KKf2CotGj2cBNUKQQvj-G0ZWEE5
//-uBjBz941EOPqDQy5sS_GCs2z40dnvU99Y5AI1bw2uqN--2jXoBLIM5d6L9RImvm8Vg8cBAiLpWA8Vw&openid=oLVPpjqs9BhvzwPj5A-vTYAX3GLc


微信正常返回数据
{
    "openid": "oLVPpjqs9BhvzwPj5A-vTYAX3GLc",
    "nickname": "刺猬宝宝",
    "sex": 1,
    "language": "简体中文",
    "city": "深圳",
    "province": "广东",
    "country": "中国",
    "headimgurl": "http://wx.qlogo.cn/mmopen/utpKYf69VAbCRDRlbUsPsdQN38DoibCkrU6SAMCSNx558eTaLVM8PyM6jlEGzOrH67hyZibIZPXu4BK1XNWzSXB3Cs4qpBBg18/0",
    "privilege": []
}





*/




int WxoauthImp::wxchatLogin(const WmsPlatform::WxoauthReq& sIn, WmsPlatform::WxUserinfoRes& sOut, tars::TarsCurrentPtr current)
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
	        TLOGERROR("WxoauthImp wxchatLogin iRet!=0 : " << url << endl);
	        return 0;
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
    		sOut = "{\"status\":1,\"errCode\":10401,\"error\":\"winxin erro callback\",\"data\":[]}";
    		return 0;
    	}
  		else
    	{
		 if (!document.HasMember("errcode") )
		 {
		 	TLOGERROR("invalid format: " << stHttpRep.getContent());
    		sOut = "{\"status\":1,\"errCode\":10401,\"error\":\"winxin erro callback\",\"data\":[]}";
    		return 0;
		 }
		 else
		 {
		 	rapidjson::Value& count_json = document["errcode"];

		 	int errcode = count_json.GetInt();

		 	if (errcode == 42001 or errcode == 40001 or errcode == 40014)
		 	{
		 		// 后续补充
	    		sOut = "{\"status\":1,\"errCode\":10401,\"error\":\"winxin erro callback\",\"data\":[]}";
	    		return 0;		 		
		 	}
		 	else
		 	{
                count_json = document["unionid"];
                WxUserinfoReq req;
                req.unionid = (document["unionid"]).GetString()
                req.headimgurl = (document["headimgurl"]).GetString()
                req.nickname = (document["nickname"]).GetString()
                req.sex = (document["sex"]).GetString()
                req.openid = (document["openid"]).GetString()
                if (0 == getUseInfo(req, sOut))
		 		   return 0;
                else
                   return -1;
		 	}
		 }   		
    	}
	}
    catch(exception &ex)
    {
        cout << ex.what() << endl;
    }
    return 0;
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
};

*/




int WxoauthImp::getUseInfo(const WxUserinfoReq &req, WxUserinfoRes &res)
{

    try
    {

	    int iRet = _wxuserinfoPrx->getWxUserinfo(req, res);

        if(iRet != 0)
        {
        	TLOGERROR("WxoauthImp getUseInfo iRet != 0: " << iRet);
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