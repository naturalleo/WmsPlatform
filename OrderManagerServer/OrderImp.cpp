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

    TLOGDEBUG("addFunds : " << sIn.userId << endl);
    try
    {
        if (_db.generaterOrderID(sReq, sRsp) == -1)
        {
            _db.generaterOrderID(sReq, sRsp);
        }
    }
    catch(exception &ex)
    {
        cout << ex.what() << endl;
        return -1;
    }
    return 0;
}

/*

*/

int OrderImp::updateUserToken(const WmsPlatform::WxoauthReq& req)
{
    TLOGDEBUG("updateUserToken : " << sReq.userId << endl);

    int _ret

    try
    {
        _ret = _db.updateUserToken(req);   
    }
    catch(exception &ex)
    {
        cout << ex.what() << endl;
        return -1;
    }
    return _ret;
}