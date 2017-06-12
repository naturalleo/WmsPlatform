#ifndef _WxUserinfoImp_H_
#define _WxUserinfoImp_H_

#include "servant/Application.h"
#include "WxUserinfo.h"

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
};
/////////////////////////////////////////////////////
#endif
