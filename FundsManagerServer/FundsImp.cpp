#include "FundsImp.h"
#include "servant/Application.h"

using namespace std;

//////////////////////////////////////////////////////
void FundsImp::initialize()
{
    //initialize servant here:
    //...
}

//////////////////////////////////////////////////////
void FundsImp::destroy()
{
    //destroy servant here:
    //...
}



int FundsImp::addFunds(const WmsPlatform::FundsNewUserReq& sIn, std::string& sOut, tars::TarsCurrentPtr current)
{

	return 0 ;
}

int FundsImp::getFunds(const WmsPlatform::FundsUserInfoReq& sIn, WmsPlatform::FundsUserInfoRes& sOut, tars::TarsCurrentPtr current)
{
	return 0;
}

int FundsImp::modifyFunds(const WmsPlatform::FundsUserModifyReq& sIn, std::string& sOut, tars::TarsCurrentPtr current)
{
	return 0 ;
}
int FundsImp::modifyFundsOther(const WmsPlatform::FundsUserModifyOtherReq& sIn, WmsPlatform::FundsUserModifyOtherRes& sOut, tars::TarsCurrentPtr current)
{
	return 0 ;
}