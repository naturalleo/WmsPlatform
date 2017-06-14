#ifndef _WxUserinfoImp_H_
#define _WxUserinfoImp_H_

#include "servant/Application.h"
#include "WxUserinfo.h"
#include "DbAgent.h"
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


    virtual int getWxUserinfo(const WmsPlatform::WxUserinfoReq& sIn, WmsPlatform::WxUserinfoRes& sOut, tars::TarsCurrentPtr current);


protected:
    DbAgent _db;
};
/////////////////////////////////////////////////////
#endif
