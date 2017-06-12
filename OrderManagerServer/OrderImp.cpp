#include "OrderImp.h"
#include "servant/Application.h"

using namespace std;

//////////////////////////////////////////////////////
void OrderImp::initialize()
{
    //initialize servant here:
    //...
	TLOGDEBUG("begin OrderImp initialize"<<endl);
    try{
        try{
            _db.init();

        }catch(exception &ex)
        {
                cout << ex.what() << endl;
        }
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
    TLOGDEBUG("generateOrder : " << sReq.userId << endl);

    if (_db.generaterOrderID(sReq, sRsp) == -1)
    {
        _db.generaterOrderID(sReq, sRsp);
    }

    return 0;
}