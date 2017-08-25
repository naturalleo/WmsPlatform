#ifndef _WxoauthImp_H_
#define _WxoauthImp_H_

#include "servant/Application.h"
#include "../WxoauthServer/Wxoauth.h"
#include "../WxUserinfoServer/WxUserinfo.h"
#include "../OrderManagerServer/Order.h"

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


    int getLoginUseInfo(const WxLoginUserinfoReq &req, WmsPlatform::WxoauthReq sIn,  WxLoginUserinfoRes & res);

    string getLoginToken(string figure = "");


    virtual int wxchatLogin(const WmsPlatform::WxoauthReq& sIn, WmsPlatform::WxLoginUserinfoRes& sOut, tars::TarsCurrentPtr current);

protected:
    WxUserinfoPrx  _wxuserinfoPrx;
    WxoauthPrx  _wxoauthPrx;
    OrderPrx    _orderPrx;
    static const string _basekey1;
    static const string _basekey2;

};
/////////////////////////////////////////////////////
#endif
