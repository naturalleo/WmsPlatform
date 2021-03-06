#ifndef _WxUserinfoImp_H_
#define _WxUserinfoImp_H_

#include "servant/Application.h"
#include "WxUserinfo.h"
#include "DbAgent.h"
#include "../FundsManagerServer/Funds.h"
#include "../OrderManagerServer/Order.h"

/**
 *
 *
 */
class WxUserinfoImp : public WmsPlatform::WxUserinfo
{
public:
    /**
     *
     */
    virtual ~WxUserinfoImp() {}

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

    string getLoginToken(string figure = "");


    virtual int getLoginWxUserinfo(const WmsPlatform::WxLoginUserinfoReq& sIn, WmsPlatform::WxLoginUserinfoRes& sOut, tars::TarsCurrentPtr current);

    virtual int getWxUserinfo(const WmsPlatform::WxUserinfoReq& sIn, WmsPlatform::WxUserinfoRes& sOut, tars::TarsCurrentPtr current);

    virtual int getWxUserIsAgent(const WmsPlatform::WxUserisAgentReq& sIn, WmsPlatform::WxUserisAgentRes& sOut, tars::TarsCurrentPtr current);

    virtual int updateWxUserCards(const WmsPlatform::WxUserExchangeReq& sIn, WmsPlatform::WxUserExchangeRes& sOut, tars::TarsCurrentPtr current);

    virtual int setActivationCode(const WmsPlatform::WxUserSetActivationCodeReq& sIn, WmsPlatform::WxUserSetActivationCodeRes& sOut, tars::TarsCurrentPtr current);
protected:
    DbAgent _db;

    void loadconf();

    FundsPrx  _FundsPrx;
    OrderPrx  _OrderPrx;


    static const string _basekey1;
    static const string _basekey2;
};
/////////////////////////////////////////////////////
#endif
