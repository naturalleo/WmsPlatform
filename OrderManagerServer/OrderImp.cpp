#include "OrderImp.h"
#include "servant/Application.h"

using namespace std;

//////////////////////////////////////////////////////
void OrderImp::initialize()
{
    //initialize servant here:
    //...
	TLOGDEBUG("begin OrderImp initialize"<<endl);
    try
    {

        _db.init();   
        _FundsPrx = Application::getCommunicator()->stringToProxy<FundsPrx>("WmsPlatform.FundsManagerServer.FundsObj");
        //string order = "";
    	//_db.generaterOrderID("hello",order);
   }
    catch(exception &ex)
    {
        cout << ex.what() << endl;
    }

}

//////////////////////////////////////////////////////
void OrderImp::destroy()
{
    //destroy servant here:
    //...
}

int OrderImp::generateOrder(const WmsPlatform::CreateRoomReq &sReq, std::string &sRsp, tars::TarsCurrentPtr current)
{

    TLOGDEBUG("addFunds : " << sReq.userId << endl);
    try
    {
        if (_db.generaterOrderID(sReq, sRsp) == 0)
        {
            FundsUserModifyReq req;
            FundsUserInfoRes res;
            req.userId = sReq.userId;
            req.appId = sReq.appId;
            req.appCode = sReq.appCode;
            req.cards = sReq.useNum;
            req.opcode = "sub";

            if(_FundsPrx->modifyFunds(req, res) == 0 )
                return 0;

        }
    }
    catch(exception &ex)
    {
        cout << ex.what() << endl;
        return -1;
    }
    return -1;
}

/*

*/

int OrderImp::updateUserToken(const WmsPlatform::WxoauthReq& sReq, const std::string& token, tars::TarsCurrentPtr current)
{
    TLOGDEBUG("updateUserToken : " << sReq.userId << endl);

    int _ret;

    try
    {
        _ret = _db.updateUserToken(sReq, token);   
    }
    catch(exception &ex)
    {
        cout << ex.what() << endl;
        return -1;
    }
    return _ret;
}