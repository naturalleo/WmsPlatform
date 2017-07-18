#ifndef _WxUserinfoImp_H_
#define _WxUserinfoImp_H_

#include "servant/Application.h"
#include "WxUserinfo.h"
#include "DbAgent.h"
#include "Funds.h"
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

    virtual int getWxUserinfo(const WmsPlatform::WxUserinfoReq& sIn, WmsPlatform::WxUserinfoRes& sOut, tars::TarsCurrentPtr current);

    virtual int getWxUserIsAgent(const WmsPlatform::WxUserisAgentReq& sIn, WmsPlatform::WxUserisAgentRes& sOut, tars::TarsCurrentPtr current);

    virtual int updateWxUserCards(const WmsPlatform::WxUserExchangeReq& sIn, WmsPlatform::WxUserExchangeRes& sOut, tars::TarsCurrentPtr current);
protected:
    DbAgent _db;
    FundsPrx  _FundsPrx;

    static const string _basekey1;
    static const string _basekey2;
};
/////////////////////////////////////////////////////
#endif
