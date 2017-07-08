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



    virtual int addFunds(const WmsPlatform::FundsNewUserReq& sIn, std::string& sOut, tars::TarsCurrentPtr current);

    virtual int getFunds(const WmsPlatform::FundsUserInfoReq& sIn, WmsPlatform::FundsUserInfoRes& sOut, tars::TarsCurrentPtr current);

    virtual int modifyFunds(const WmsPlatform::FundsUserModifyReq& sIn, std::string& sOut, tars::TarsCurrentPtr current);

    virtual int modifyFundsOther(const WmsPlatform::FundsUserModifyOtherReq& sIn, WmsPlatform::FundsUserModifyOtherRes& sOut, tars::TarsCurrentPtr current);
        

};
/////////////////////////////////////////////////////
#endif
