/**
 * Tencent is pleased to support the open source community by making Tars available.
 *
 * Copyright (C) 2016THL A29 Limited, a Tencent company. All rights reserved.
 *
 * Licensed under the BSD 3-Clause License (the "License"); you may not use this file except 
 * in compliance with the License. You may obtain a copy of the License at
 *
 * https://opensource.org/licenses/BSD-3-Clause
 *
 * Unless required by applicable law or agreed to in writing, software distributed 
 * under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR 
 * CONDITIONS OF ANY KIND, either express or implied. See the License for the 
 * specific language governing permissions and limitations under the License.
 */

#include "HttpImp.h"
#include "servant/Application.h"
#include "util/tc_common.h"
#include "util/tc_cgi.h"
#include <cctype>
#include <algorithm>
#include <sstream>

using namespace std;

//////////////////////////////////////////////////////
void HttpImp::initialize()
{
    //initialize servant here:
    //...
    _orderPrx       = Application::getCommunicator()->stringToProxy<OrderPrx>("WmsPlatform.OrderManagerServer.OrderObj");
    _wxoauthPrx     = Application::getCommunicator()->stringToProxy<WxoauthPrx>("WmsPlatform.WxoauthServer.WxoauthObj");
    _WxUserinfoPrx  = Application::getCommunicator()->stringToProxy<WxUserinfoPrx>("WmsPlatform.WxUserinfoServer.WxUserinfoObj");
    _FundsPrx       = Application::getCommunicator()->stringToProxy<FundsPrx>("WmsPlatform.FundsManagerServer.FundsObj");
    _GamePrx        = Application::getCommunicator()->stringToProxy<GamePrx>("WmsPlatform.GameRecordServer.GameObj");
}

//////////////////////////////////////////////////////
void HttpImp::destroy()
{
    //destroy servant here:
    //...
}

string& HttpImp::trim(string &s)   
{  
    if (s.empty())   
    {  
        return s;  
    }  
    s.erase(0,s.find_first_not_of(" "));  
    s.erase(s.find_last_not_of(" ") + 1);  
    return s;  
}
void HttpImp::parseNormal(multimap<string, string> &mmpParams, const string& sBuffer)
{
    int iFlag = 0;
    string sName;
    string sValue;
    string sTmp;
    string::size_type len = sBuffer.length();
    string::size_type pos = 0;

    while (pos < len)
    {
        sTmp = "";

        if(iFlag == 0)
        {
            while ( (sBuffer[pos] != '=') && (pos < len) )
            {
                sTmp += (sBuffer[pos] == '+') ? ' ' : sBuffer[pos];

                ++pos;
            }
        }
        else
        {
            while ( (sBuffer[pos] != '&') && (pos < len) )
            {
                sTmp += (sBuffer[pos] == '+') ? ' ' : sBuffer[pos];

                ++pos;
            }
        }

        if (iFlag == 0)                         //param name
        {
            sName = TC_Cgi::decodeURL(sTmp);

            if ( (sBuffer[pos] != '=') || (pos == len - 1) )
            {
                sValue = "";

                mmpParams.insert(multimap<string, string>::value_type(TC_Common::trim(sName), sValue));
            }
            else
            {
                iFlag = 1;
            }
        }
        else
        {
            sValue = TC_Cgi::decodeURL(sTmp);

            mmpParams.insert(multimap<string, string>::value_type(TC_Common::trim(sName), sValue));

            iFlag = 0;
        }

        ++pos;
    }
}




bool HttpImp::isParamExist(const multimap<string, string> &mmpParams , const string& sName) const
{
    return mmpParams.find(sName) != mmpParams.end();
}
string HttpImp::toDefault(const string &s, const string &sDefault)
{
    if(s.empty())
    {
        return sDefault;
    }
    return s;
}
string HttpImp::getValue(const multimap<string, string> &mmpParams , const string& sName) const
{
    multimap<string, string>::const_iterator it;

    if((it = mmpParams.find(sName)) != mmpParams.end())
    {
        return it->second;
    }

    return "";
}



int HttpImp::doRequest(TarsCurrentPtr current, vector<char> &buffer)
{

    try
    {   
        

        TC_HttpRequest request; 
        vector<char> v = current->getRequestBuffer();
        string sBuf;
        sBuf.assign(&v[0],v.size());
        request.decode(sBuf);

        TLOGDEBUG("getRequestParam : " << request.getRequestParam() << endl);
        TLOGDEBUG("getRequestUrl : " << request.getRequestUrl() << endl);
        TLOGDEBUG("getRequest : " << request.getRequest() << endl);


        multimap<string, string> _para;

        parseNormal(_para, request.getRequestParam());

        TLOGDEBUG("doRequest" << TC_Common::tostr(_para) << endl);

        if (request.getRequestUrl() == "/GameService/addOrder" && isParamExist(_para, "userId") && isParamExist(_para, "appId") && isParamExist(_para, "appCode") 
            && isParamExist(_para, "useNum") )
        {
            current->setResponse(false);

            FundsCheckReq req;
            req.userId = getValue(_para,"userId");
            req.appId = getValue(_para,"appId");
            req.appCode = getValue(_para,"appCode");
            req.cards = getValue(_para,"useNum");


            ostringstream oss("");
            oss <<"userId\%3D" << req.userId << "\%26appId\%3D" << req.appId << "\%26appCode\%3D" << req.appCode << "\%26useNum\%3D" << req.cards;


            WmsPlatform::FundsPrxCallbackPtr cb = new FundsCallback(current, oss.str());
            _FundsPrx->tars_set_timeout(3000)->async_checkFunds(cb,req);     
                 
        }
        else if (request.getRequestUrl() == "/GameService/updateOrder" && isParamExist(_para, "orderCode"))
        {
            current->setResponse(false);

            string data = getValue(_para,"orderCode");
            multimap<string, string> __para;

            parseNormal(__para, data);


            FundsUserModifyReq req;
            req.userId = getValue(__para,"userId");
            req.appId = getValue(__para,"appId");
            req.appCode = getValue(__para,"appCode");
            req.cards = getValue(__para,"useNum");

             TLOGDEBUG("updateOrder 2222222222222222222222222" << TC_Common::tostr(__para) << endl);

            if (getValue(_para,"status") == "used")
            {
                req.opcode = "sub" ;


                WmsPlatform::FundsPrxCallbackPtr cb = new FundsCallback(current, data);
                _FundsPrx->tars_set_timeout(3000)->async_modifyFunds(cb,req);  
            }    
        }
        else if (request.getRequestUrl() == "/user/getFunds" && isParamExist(_para, "userId") && isParamExist(_para, "appId") && isParamExist(_para, "appCode") )
        {
            current->setResponse(false);


            FundsUserInfoReq req;
            req.userId = getValue(_para,"userId");
            req.appId = getValue(_para,"appId");
            req.appCode = getValue(_para,"appCode");


            WmsPlatform::FundsPrxCallbackPtr cb = new FundsCallback(current);
            _FundsPrx->tars_set_timeout(3000)->async_getFunds(cb,req);     
                 
        }
        else if (request.getRequestUrl() == "/user/thirdPartyLogin" && isParamExist(_para, "accessToken") && isParamExist(_para, "refreshToken") && isParamExist(_para, "wechatAppId") && isParamExist(_para, "appCode") && isParamExist(_para, "appId") && isParamExist(_para, "loginFrom"))
        {
            if (!isParamExist(_para,"accessToken"))
               {
                    TC_HttpResponse rsp;
                    string s = "{\"status\":1,\"errCode\":10400,\"error\":\"param lost\",\"data\":[]}";
                    rsp.setResponse(s.c_str(),s.size());
                    rsp.encode(buffer);
                    return 0;
               } 

            current->setResponse(false);
            WxoauthReq req;

            TLOGDEBUG("thirdPartyLogin domain : " << current->getIp() << endl);

            req.openId       = "wxf0862d65306b025a";
            req.accessToken  = getValue(_para,"accessToken"); 
            req.refreshToken = getValue(_para,"refreshToken"); 
            req.wechatAppId  = getValue(_para,"wechatAppId"); 
            req.appCode      = getValue(_para,"appCode"); 
            req.ip           = current->getIp();
            req.appId        = getValue(_para,"appId"); 
            req.clientFrom   = getValue(_para,"loginFrom");
             
            WmsPlatform::WxoauthPrxCallbackPtr cb = new WxoauthCallback(current);
            _wxoauthPrx->tars_set_timeout(3000)->async_wxchatLogin(cb,req);              

        }
        else if (request.getRequestUrl() == "/GameService/isAgent")
        {
            current->setResponse(false);
            WxUserisAgentReq req;
            string res;
            req.userId = "123";  

            WmsPlatform::WxUserinfoPrxCallbackPtr cb = new WxUserinfoCallback(current);
            _WxUserinfoPrx->tars_set_timeout(3000)->async_getWxUserIsAgent(cb,req);        
        }
        else if (request.getRequestUrl() == "/GameService/getUserInfo" && isParamExist(_para, "userId") && isParamExist(_para, "appId") && isParamExist(_para, "appCode") && isParamExist(_para, "token") )
        {

            current->setResponse(false);
            WxUserinfoReq req;
            req.userId = getValue(_para,"userId");
            req.appId = getValue(_para,"appId");
            req.appCode = getValue(_para,"appCode");
            req.token = getValue(_para,"token");
            //req.orderCode = getValue(_para,"orderCode");

            WmsPlatform::WxUserinfoPrxCallbackPtr cb = new WxUserinfoCallback(current);
            _WxUserinfoPrx->tars_set_timeout(3000)->async_getWxUserinfo(cb,req);        
        }
        else if (request.getRequestUrl() == "/Game/getResultLst" && isParamExist(_para, "appId") && isParamExist(_para, "appCode") && isParamExist(_para, "userId") && isParamExist(_para, "token"))
        {

            current->setResponse(false);
            GameRecordReq req;
            req.appId = getValue(_para,"appId");
            req.appCode = getValue(_para,"appCode");
            req.userId = getValue(_para,"userId");
            req.token = getValue(_para,"token");

            WmsPlatform::GamePrxCallbackPtr cb = new GameCallback(current);
            _GamePrx->tars_set_timeout(3000)->async_getPlayerGameRecord(cb,req);        
        }
        else if (request.getRequestUrl() == "/Game/getResultDetail" && isParamExist(_para, "appId") && isParamExist(_para, "appCode") && isParamExist(_para, "userId") && isParamExist(_para, "token")
            && isParamExist(_para, "roomId") && isParamExist(_para, "owner") )
        {

            current->setResponse(false);
            GameRecordDetailReq req;
            req.appId = getValue(_para,"appId");
            req.appCode = getValue(_para,"appCode");
            req.userId = getValue(_para,"userId");
            req.token = getValue(_para,"token");
            req.room_id = getValue(_para,"roomId");
            req.owner = getValue(_para,"owner");

            WmsPlatform::GamePrxCallbackPtr cb = new GameCallback(current);
            _GamePrx->tars_set_timeout(3000)->async_getPlayerGameDetailRecord(cb,req);        
        }
        else if (request.getRequestUrl() == "/Suggestion/add")
        {
            current->setResponse(false);
            ReportSuggestionReq req;
            req.appId = getValue(_para,"appId");
            req.appCode = getValue(_para,"appCode");
            req.userId = getValue(_para,"userId");
            req.clientFrom = getValue(_para,"clientFrom");
            req.clientVer = getValue(_para,"clientVer");
            req.contacts = getValue(_para,"contacts");
            req.content = getValue(_para,"content");
            req.suggestType = getValue(_para,"suggestType");
            req.title = getValue(_para,"title");

            WmsPlatform::OrderPrxCallbackPtr cb = new OrderCallback(current);
            _orderPrx->tars_set_timeout(3000)->async_reportSuggestion(cb,req);        
        }
        else if (request.getRequestUrl() == "/Article/lst")
        {
            current->setResponse(false);
            SysNoticeReq req;
            req.appId = getValue(_para,"appId");
            req.appCode = getValue(_para,"appCode");
            req.artCat = getValue(_para,"artCat");


            WmsPlatform::OrderPrxCallbackPtr cb = new OrderCallback(current);
            _orderPrx->tars_set_timeout(3000)->async_sysNotice(cb,req);        
        }
        else if (request.getRequestUrl() == "/GameService/ExchangeCards")
        {
            current->setResponse(false);
            WxUserExchangeReq req;
            req.userId = "123";  
            req.otherId = "123";
            req.opcode = "add";
            req.cards = "1";

            WmsPlatform::WxUserinfoPrxCallbackPtr cb = new WxUserinfoCallback(current);
            _WxUserinfoPrx->tars_set_timeout(3000)->async_updateWxUserCards(cb,req);        
        }
        else
        {
            TC_HttpResponse rsp;
            string s = "{测试}";
            rsp.setResponse(s.c_str(),s.size());
            rsp.encode(buffer);           
        }


    }
    catch(exception &ex)
    {
        current->setResponse(true);
        cout << ex.what() << endl;
    }
    return 0;
}


