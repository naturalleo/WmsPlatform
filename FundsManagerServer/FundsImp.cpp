#include "FundsImp.h"
#include "servant/Application.h"

using namespace std;

//////////////////////////////////////////////////////
void FundsImp::initialize()
{
    //initialize servant here:
    //...

	TLOGDEBUG("begin FundsImp initialize"<<endl);
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
void FundsImp::destroy()
{
    //destroy servant here:
    //...
}


int FundsImp::addFunds(const WmsPlatform::FundsNewUserReq& sIn, WmsPlatform::FundsUserInfoRes& sOut, tars::TarsCurrentPtr current)
{
	TLOGDEBUG("addFunds : " << sIn.userId << endl);
    try
    {
    	 return _db.insertFunds(sIn, sOut);

    }
    catch(exception &ex)
    {
        cout << ex.what() << endl;
        return -1;
    }
}

int FundsImp::getFunds(const WmsPlatform::FundsUserInfoReq& sIn, WmsPlatform::FundsUserInfoRes& sOut, tars::TarsCurrentPtr current)
{
	TLOGDEBUG("getFunds : " << sIn.userId << endl);
    try
    {
    	 return _db.getFunds(sIn, sOut);
    }
    catch(exception &ex)
    {
        cout << ex.what() << endl;
        return -1;
    }
	return 0;
}

int FundsImp::modifyFunds(const WmsPlatform::FundsUserModifyReq& sIn, WmsPlatform::FundsUserInfoRes& sOut, tars::TarsCurrentPtr current)
{
	TLOGDEBUG("modifyFunds : " << sIn.userId << endl);
    try
    {
    	 return _db.modifyFunds(sIn, sOut);
    }
    catch(exception &ex)
    {
        cout << ex.what() << endl;
        return -1;
    }
	return 0;
}
int FundsImp::modifyFundsOther(const WmsPlatform::FundsUserModifyOtherReq& sIn, WmsPlatform::FundsUserModifyOtherRes& sOut, tars::TarsCurrentPtr current)
{



	return 0 ;
}