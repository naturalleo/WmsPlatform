#include "WxoauthImp.h"
#include "servant/Application.h"
#include "util/tc_http.h"



using namespace std;

//////////////////////////////////////////////////////
void WxoauthImp::initialize()
{
    //initialize servant here:
    //...
}

//////////////////////////////////////////////////////
void WxoauthImp::destroy()
{
    //destroy servant here:
    //...
}



int WxoauthImp::wxchatLogin(const WmsPlatform::WxoauthReq& sIn, std::string& sOut, tars::TarsCurrentPtr current)
{

	try
    {
    	//TLOGDEBUG("WxoauthImp wxchatLogin ok begin " << endl);
		//string url = "https://api.weixin.qq.com/sns/userinfo?access_token="
		//			 + sIn.accessToken + "&openid="
		//			 +sIn.openId + "&lang=zh_CN";
    	string url = "https://api.weixin.qq.com/sns/userinfo";
		TC_HttpRequest stHttpReq;
	    stHttpReq.setCacheControl("no-cache");

	    stHttpReq.setGetRequest(url);

	    TC_HttpResponse stHttpRep;

	    int iRet = stHttpReq.doRequest(stHttpRep, 3000);
	    if(iRet != 0)
	    {
	        cout << iRet << endl;
	    }
	    string r = stHttpRep.encode();
	    TLOGDEBUG("WxoauthImp wxchatLogin ok" << r << endl);

	    TLOGDEBUG("WxoauthImp headers " << TC_Common::tostr(stHttpRep.getHeaders()) << endl);


	    sOut = "i come from wxchatLogin callback";

	}
    catch(exception &ex)
    {
        cout << ex.what() << endl;
    }
    return 0;
}