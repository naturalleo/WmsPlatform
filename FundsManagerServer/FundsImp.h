#ifndef _FundsImp_H_
#define _FundsImp_H_

#include "servant/Application.h"
#include "Funds.h"

/**
 *
 *
 */
class FundsImp : public WmsPlatform::Funds
{
public:
    /**
     *
     */
    virtual ~FundsImp() {}

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
