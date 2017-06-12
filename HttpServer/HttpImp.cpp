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

using namespace std;

//////////////////////////////////////////////////////
void HttpImp::initialize()
{
    //initialize servant here:
    //...
    _orderPrx   = Application::getCommunicator()->stringToProxy<OrderPrx>("WmsPlatform.OrderManagerServer.OrderObj");
    _wxoauthPrx = Application::getCommunicator()->stringToProxy<WxoauthPrx>("WmsPlatform.WxoauthServer.WxoauthObj");
}

//////////////////////////////////////////////////////
void HttpImp::destroy()
{
    //destroy servant here:
    //...
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

                mmpParams.insert(multimap<string, string>::value_type(sName, sValue));
            }
            else
            {
                iFlag = 1;
            }
        }
        else
        {
            sValue = TC_Cgi::decodeURL(sTmp);

            mmpParams.insert(multimap<string, string>::value_type(sName, sValue));

            iFlag = 0;
        }

        ++pos;
    }
}

class OrderCallback : public OrderPrxCallback
{

public:
    OrderCallback(TarsCurrentPtr &current)
    : _current(current)
    {}

    virtual void callback_generateOrder(tars::Int32 ret,  const std::string& sOut)
    {
        //HttpImp::async_response_doRequest(_current, ret, sOut);
        TLOGDEBUG("callback_generateOrder : " << ret << sOut << endl);
        TC_HttpResponse rsp;
        vector<char> buffer;
        string s="hello world";
        rsp.setResponse(s.c_str(),s.size());
        rsp.encode(buffer);     


        _current->sendResponse(&buffer.at(0),buffer.size());
        TLOGDEBUG("callback_generateOrder : " << s << s.size() << endl);
       // _current->sendResponse(tars::TARSSERVERSUCCESS, buffer);    
        //HttpImp::async_response_doRequest(_current, ret, buffer);
    }
    virtual void callback_generateOrder_exception(tars::Int32 ret)
    { 
        TLOGERROR("OrderCallback callback_generateOrder_exception ret:" << ret << endl); 

        Order::async_response_generateOrder(_current, ret, "");
    }

    TarsCurrentPtr _current;
};


class WxoauthCallback : public WxoauthPrxCallback
{

public:
    WxoauthCallback(TarsCurrentPtr &current)
    : _current(current)
    {}

    virtual void callback_wxchatLogin(tars::Int32 ret,  const std::string& sOut)
    {
        //HttpImp::async_response_doRequest(_current, ret, sOut);
        TLOGDEBUG("callback_wxchatLogin : " << ret << sOut << endl);
        TC_HttpResponse rsp;
        vector<char> buffer;
        string s = sOut;
        rsp.setResponse(s.c_str(),s.size());
        rsp.encode(buffer);     


        _current->sendResponse(&buffer.at(0),buffer.size());
        TLOGDEBUG("callback_wxchatLogin : " << s << s.size() << endl);
       // _current->sendResponse(tars::TARSSERVERSUCCESS, buffer);    
        //HttpImp::async_response_doRequest(_current, ret, buffer);
    }
    virtual void callback_wxchatLogin_exception(tars::Int32 ret)
    { 
        TLOGERROR("WxoauthCallback callback_wxchatLogin_exception ret:" << ret << endl); 

        Order::async_response_generateOrder(_current, ret, "");
    }

    TarsCurrentPtr _current;
};

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

        if (request.getRequestUrl() == "/GameService/addOrder")
        {
            current->setResponse(false);
            CreateRoomReq req;
            string res;
            req.userId = "123";
            req.appId = "123";
            req.appCode = "123";
            req.token = "123";
            req.gameId = "123";
            req.useNum = "123";
            req.orderType = "123";    
            WmsPlatform::OrderPrxCallbackPtr cb = new OrderCallback(current);

            _orderPrx->tars_set_timeout(3000)->async_generateOrder(cb,req);        
        }
        else if (request.getRequestUrl() == "/user/thirdPartyLogin")
        {
            if (!isParamExist(_para,"openId")  or  !isParamExist(_para,"accessToken"))
               {
                    TC_HttpResponse rsp;
                    string s = "{\"status\":1,\"errCode\":10400,\"error\":\"param lost\",\"data\":[]}";
                    rsp.setResponse(s.c_str(),s.size());
                    rsp.encode(buffer);
                    return 0;
               } 

            current->setResponse(false);
            WxoauthReq req;
            req.openId = getValue(_para,"openId");
            req.accessToken = getValue(_para,"accessToken"); 
            req.refreshToken = getValue(_para,"refreshToken"); 
            req.wechatAppId = getValue(_para,"wechatAppId"); 


            WmsPlatform::WxoauthPrxCallbackPtr cb = new WxoauthCallback(current);
            _wxoauthPrx->tars_set_timeout(3000)->async_wxchatLogin(cb,req);              


            //using rapidjson::Document;
            //Document doc;
            //using rapidjson::Value;
           // Value item(Type::kObjectType);
/*
            rapidjson::StringBuffer jbuffer;
            rapidjson::Writer<rapidjson::StringBuffer> writer(jbuffer);

            writer.StartObject();

            writer.Key("status");
            writer.Int(1);
            writer.EndObject();




            TC_HttpResponse rsp;
            string s = jbuffer.GetString();
            rsp.setResponse(s.c_str(),s.size());
            rsp.encode(buffer);
*/

        }
        else
        {
            TC_HttpResponse rsp;
            string s = "hello come from WmsPlatform HttpServer";
            rsp.setResponse(s.c_str(),s.size());
            rsp.encode(buffer);           
        }

        //_orderPrx->generateOrder(req, res);

        //TLOGDEBUG("res : " << res << endl);

        //TLOGDEBUG("sBuf : " << sBuf << endl);
        //TLOGDEBUG("getRequest : 111111111" << request.getRequest() << endl);
        // TC_HttpResponse rsp;
        // string s="hello";
        //rsp.setResponse(s.c_str(),s.size());
        //rsp.encode(buffer);       



    }
    catch(exception &ex)
    {
        current->setResponse(true);
        cout << ex.what() << endl;
    }
    return 0;
}
