#ifndef _WxoauthImp_H_
#define _WxoauthImp_H_

#include "servant/Application.h"
#include "Wxoauth.h"
#include "WxUserinfo.h"
#include "rapidjson/include/rapidjson/document.h"
#include "rapidjson/include/rapidjson/writer.h"
#include "rapidjson/include/rapidjson/stringbuffer.h"

using namespace rapidjson;
using namespace std;
using namespace WmsPlatform;

/**
 *
 *
 */
class WxoauthImp : public WmsPlatform::Wxoauth
{
public:
    /**
     *
     */
    virtual ~WxoauthImp() {}

    /**
     *
     */
    virtual void initialize();

    /**
     *
     */
    virtual void destroy();

    /**
     *
     */
    virtual int test(tars::TarsCurrentPtr current) { return 0;};


    int getUseInfo(const WxUserinfoReq &req, WxUserinfoRes & res);

    string getLoginToken(string figure = "");


    virtual int wxchatLogin(const WmsPlatform::WxoauthReq& sIn, WmsPlatform::WxUserinfoRes& sOut, tars::TarsCurrentPtr current);

protected:
     WxUserinfoPrx  _wxuserinfoPrx;


};
/////////////////////////////////////////////////////
#endif
