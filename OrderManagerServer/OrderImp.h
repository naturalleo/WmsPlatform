#ifndef _OrderImp_H_
#define _OrderImp_H_

#include "servant/Application.h"
#include "Order.h"
#include "DbAgent.h"
#include "Funds.h"
/**
 *
 *
 */
class OrderImp : public WmsPlatform::Order
{
public:
    /**
     *
     */
    virtual ~OrderImp() {}

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

    virtual int generateOrder(const WmsPlatform::CreateRoomReq& sIn, std::string& sOut, tars::TarsCurrentPtr current);

    virtual int updateUserToken(const WmsPlatform::WxoauthReq& req, const std::string& token, tars::TarsCurrentPtr current);
protected:
    DbAgent _db;
    FundsPrx  _FundsPrx;

};
/////////////////////////////////////////////////////
#endif
