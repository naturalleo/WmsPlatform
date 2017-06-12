#ifndef _WxoauthImp_H_
#define _WxoauthImp_H_

#include "servant/Application.h"
#include "Wxoauth.h"

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


    virtual int wxchatLogin(const WmsPlatform::WxoauthReq& sIn, std::string& sOut, tars::TarsCurrentPtr current);
};
/////////////////////////////////////////////////////
#endif
