#include "OrderImp.h"
#include "servant/Application.h"

using namespace std;
extern TC_Config * g_pconf;
//////////////////////////////////////////////////////
void OrderImp::initialize()
{
    //initialize servant here:
    //...
	TLOGDEBUG("begin OrderImp initialize"<<endl);
    try
    {

        loadconf();   
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


void OrderImp::loadconf()
{
    TC_DBConf tcDBConf;
    tcDBConf.loadFromMap(g_pconf->getDomainMap("/tars/db"));
    _db.init(tcDBConf);
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


int OrderImp::checkUserToken(const WmsPlatform::WxUserinfoReq& sReq, std::string& ip, tars::TarsCurrentPtr current)
{
    TLOGDEBUG("checkUserToken : " << sReq.userId << endl);

    int _ret;
    try
    {
        _ret = _db.checkUserToken(sReq, ip);   
    }
    catch(exception &ex)
    {
        cout << ex.what() << endl;
        return -1;
    }
    return _ret;
}

int OrderImp::reportSuggestion(const ReportSuggestionReq &sReq, tars::TarsCurrentPtr current)
{
    TLOGDEBUG("reportSuggestion : " << sReq.userId << endl);

    int _ret;
    try
    {
        _ret = _db.reportSuggestion(sReq);   
    }
    catch(exception &ex)
    {
        cout << ex.what() << endl;
        return -1;
    }
    return _ret;
}

int OrderImp::reportApplyAgent(const ReportApplyAgentReq &sReq, tars::TarsCurrentPtr current)
{
    TLOGDEBUG("reportApplyAgent : " << sReq.userId << endl);

    int _ret;
    try
    {
        _ret = _db.reportApplyAgent(sReq);   
    }
    catch(exception &ex)
    {
        cout << ex.what() << endl;
        return -1;
    }
    return _ret;
}

int OrderImp::sysNotice(const SysNoticeReq& sReq, SysNoticeRes& res, tars::TarsCurrentPtr current)
{
    TLOGDEBUG("sysNotice : " << endl);

    int _ret;
    try
    {
        _ret = _db.sysNotice(sReq, res);   
    }
    catch(exception &ex)
    {
        cout << ex.what() << endl;
        return -1;
    }
    return _ret;
}