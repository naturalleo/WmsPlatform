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


using namespace std;

//////////////////////////////////////////////////////
void HttpImp::initialize()
{
    //initialize servant here:
    //...
    _orderPrx   = Application::getCommunicator()->stringToProxy<OrderPrx>("WmsPlatform.OrderManagerServer.OrderObj");
    _wxoauthPrx = Application::getCommunicator()->stringToProxy<WxoauthPrx>("WmsPlatform.WxoauthServer.WxoauthObj");
    _WxUserinfoPrx = Application::getCommunicator()->stringToProxy<WxUserinfoPrx>("WmsPlatform.WxUserinfoServer.WxUserinfoObj");
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

    virtual void callback_wxchatLogin(tars::Int32 ret,  const WmsPlatform::WxUserinfoRes& sOut)
    {
        //HttpImp::async_response_doRequest(_current, ret, sOut);
        

        TLOGDEBUG("callback_wxchatLogin : " << ret << endl);
/*

    "mySign": false,
    "userId": "32",
    "token": "4856fa1d4e94d488683c1e9655de767e",
    "nickName": "Rocky-江树琪",
    "gender": "1",
    "avatar": "http://wx.qlogo.cn/mmopen/nMLdCrVfKelB8YQPB3wGXibgBHznlibX7mjW4zRcBZ9zQib73ZqXZPmgU9cDO50TsnXnb9yTSm8DFR36jtGshTxL6bQcdtj1icx1/0",
    "totalGameCard": "70",
    "surplusGameCard": "70",
    "isNew": 0,
    "ip": "127.0.0.1",
    "isRefreshToken": true,
    "wechatAccessToken": "4HQbp4Z3lbrU5brjTMrdm_dqsc2vW4_COZSRjQmOzzKAQSiUSmkfDlE_axAHI2AnmnkrPpBAMRXdb4UgYWRgqUu3emgplByniqNE1YN14c0",
    "wechatRefreshToken": "4HQbp4Z3lbrU5brjTMrdm2JZUpPXkbxNAY0A61aU3J_6gdsAIrB9IPdVzg-Et_LEtgZ_LVm7t17Qxc0kYEDlRkAItw386WpQ4mvqU_zhEO4",

struct WxUserinfoRes
{
    0 require  string userId;


    1 require  string headimgurl;
    2 require  string nickname;
    3 require  string sex;  

    4 require string isNew;
    5 require string totalcard;
    6 require string currentcard;
    7 require string token;

};

 */


        TC_HttpResponse rsp;
        vector<char> buffer;
        string s ;
        if (0 == ret)
            s = "{\"status\":1,\"errCode\":0,\"error\":\"\",\"data\":"
                    "{ \"mySign \": false,"
                    "\"userId\" : " + sOut.userId + ","
                    "\"token\" : "  + sOut.token +  ","
                    "\"nickName\" : "  + sOut.nickname +  ","
                    "\"gender\" : "  + sOut.sex +  ","
                    "\"avatar\" : "  + sOut.headimgurl +  ","
                    "\"totalGameCard\" : "  + sOut.totalcard +  ","
                    "\"surplusGameCard\" : "  + sOut.currentcard +  ","
                    "\"isNew\" : "  + sOut.isNew +  ","
                    "}"
                "}";
        else
            s = "{\"status\":-1,\"errCode\":-1,\"error\":\"ret -1\",\"data\":[]}";


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
        WmsPlatform::WxUserinfoRes res;
        Wxoauth::async_response_wxchatLogin(_current, ret, res);
    }

    TarsCurrentPtr _current;
};

class WxUserinfoCallback : public WxUserinfoPrxCallback
{

public:
    WxUserinfoCallback(TarsCurrentPtr &current)
    : _current(current)
    {}

    virtual void callback_updateWxUserCards(tars::Int32 ret,  const WmsPlatform::WxUserExchangeRes& sOut)
    {
        //HttpImp::async_response_doRequest(_current, ret, sOut);
        TLOGDEBUG("callback_updateWxUserCards : " << ret  << endl);
        TC_HttpResponse rsp;
        vector<char> buffer;
        string s="hello world";
        rsp.setResponse(s.c_str(),s.size());
        rsp.encode(buffer);     

        _current->sendResponse(&buffer.at(0),buffer.size());
        TLOGDEBUG("callback_updateWxUserCards : " << s << s.size() << endl);
       // _current->sendResponse(tars::TARSSERVERSUCCESS, buffer);    
        //HttpImp::async_response_doRequest(_current, ret, buffer);
    }
    virtual void callback_updateWxUserCards_exception(tars::Int32 ret)
    { 
        TLOGERROR("WxUserinfoCallback callback_updateWxUserCards_exception ret:" << ret << endl); 
        WmsPlatform::WxUserExchangeRes res;
        WxUserinfo::async_response_updateWxUserCards(_current, ret, res);
    }

    virtual void callback_getWxUserIsAgent(tars::Int32 ret,  const WmsPlatform::WxUserisAgentRes& sOut)
    {
        //HttpImp::async_response_doRequest(_current, ret, sOut);
        TLOGDEBUG("callback_getWxUserIsAgent : " << ret << endl);
        TC_HttpResponse rsp;
        vector<char> buffer;
        string s="hello world";
        rsp.setResponse(s.c_str(),s.size());
        rsp.encode(buffer);     

        _current->sendResponse(&buffer.at(0),buffer.size());
        TLOGDEBUG("callback_getWxUserIsAgent : " << s << s.size() << endl);
       // _current->sendResponse(tars::TARSSERVERSUCCESS, buffer);    
        //HttpImp::async_response_doRequest(_current, ret, buffer);
    }
    virtual void callback_getWxUserIsAgent_exception(tars::Int32 ret)
    { 
        TLOGERROR("WxUserinfoCallback callback_getWxUserIsAgent_exception ret:" << ret << endl); 
        WmsPlatform::WxUserisAgentRes res;


        WxUserinfo::async_response_getWxUserIsAgent(_current, ret, res);
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
            req.openId       = "wxf0862d65306b025a";
            req.accessToken  = getValue(_para,"accessToken"); 
            req.refreshToken = getValue(_para,"refreshToken"); 
            req.wechatAppId  = getValue(_para,"wechatAppId"); 
            req.appCode      = getValue(_para,"appCode"); 
            req.ip           = request.getHost();
            req.appId        = getValue(_para,"appId"); 
            req.clientFrom   = getValue(_para,"loginFrom");
             

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
        else if (request.getRequestUrl() == "/GameService/isAgent")
        {
            current->setResponse(false);
            WxUserisAgentReq req;
            string res;
            req.userId = "123";  
            WmsPlatform::WxUserinfoPrxCallbackPtr cb = new WxUserinfoCallback(current);

            _WxUserinfoPrx->tars_set_timeout(3000)->async_getWxUserIsAgent(cb,req);        
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


/*
要实现以下接口实现
客户端与后台通信API
    App第三方登录注册
    请求示例：
    http://192.168.1.103:40002/User/thirdPartyLogin?nettype=4&loginFrom=10&appId=1&chanId=0&osVer=6.0&model=EVA-AL10&clientFrom=2&wechatAppId=wxb0c1087c2a499493&
    accessToken=iAr1JDqg%5F4Tp%5FekZi8LbJlF3zDhSH7AIMthWANt64n9ZGNTlrhnmjy-Fl9wEzRVlLsPnAen4R3Vyq1uxMCH64d34LUJ77Mll8gUmVEwAq8k&openId=oUJxOv-hvRQlKQn7JzE-l4OsFzFw&
    appCode=klmj&clientVer=0.1&uuId=A2053CBF562A3521863E95C94CDE5FB6&refreshToken=4HQbp4Z3lbrU5brjTMrdm2JZUpPXkbxNAY0A61aU3J%5F6gdsAIrB9IPdVzg-Et%5FLEtgZ%5FLVm7t17Qx
    c0kYEDlRkAItw386WpQ4mvqU%5FzhEO4&operator=1&macAddr=f8%3A23%3Ab2%3Ade%3Aa1%3Ac9&imei=862187033177835&sign=aa2db44c817e92a2befb8ce953de27124528f060
    返回值：
{
  "status": 1,
  "errCode": 0,
  "error": "",
  "data": {
    "mySign": false,
    "userId": "32",
    "token": "4856fa1d4e94d488683c1e9655de767e",
    "nickName": "Rocky-江树琪",
    "gender": "1",
    "avatar": "http://wx.qlogo.cn/mmopen/nMLdCrVfKelB8YQPB3wGXibgBHznlibX7mjW4zRcBZ9zQib73ZqXZPmgU9cDO50TsnXnb9yTSm8DFR36jtGshTxL6bQcdtj1icx1/0",
    "totalGameCard": "70",
    "surplusGameCard": "70",
    "isNew": 0,
    "ip": "127.0.0.1",
    "isRefreshToken": true,
    "wechatAccessToken": "4HQbp4Z3lbrU5brjTMrdm_dqsc2vW4_COZSRjQmOzzKAQSiUSmkfDlE_axAHI2AnmnkrPpBAMRXdb4UgYWRgqUu3emgplByniqNE1YN14c0",
    "wechatRefreshToken": "4HQbp4Z3lbrU5brjTMrdm2JZUpPXkbxNAY0A61aU3J_6gdsAIrB9IPdVzg-Et_LEtgZ_LVm7t17Qxc0kYEDlRkAItw386WpQ4mvqU_zhEO4",
    "shareList": {
      "roomShare": {
        "shareId": "2",
        "shareType": "2",
        "title": "",
        "desc": "",
        "link": "",
        "img": ""
      },
      "mainShare": {
        "shareId": "1",
        "shareType": "1",
        "title": "【快来麻将】",
        "desc": "快来麻将是一款完美还原的\r\n湖南本土麻将，玩法齐全。",
        "link": "http://www.baidu.com/",
        "img": "https://www.baidu.com/img/bd_logo1.png"
      }
    }
  }
}

    获取用户最新余额信息
    请求示例：
    http://10.17.174.171:8192/User/getFunds?appId=1&appCode=klmj&clientFrom=1&userId=12&uuId=2342342343243243&token=oKfGns9KeLBhut0uSp56f9pR-4RE&sign=f1c5614a5abbcc9f6e8fe45f02f6640bab3018c9
    返回值：
    {
      "status": 1,
      "errCode": 0,
      "error": "没有错误",
      "data": {
        "userId": "12",
        "totalGameCard": "3",
        "surplusGameCard": "3",
        "ip": "127.0.0.1"
      }
    }
游戏服务器与后台通信API
    创建游戏房间订单
    请求示例：
    http://10.17.174.171:8192/GameService/addOrder?appId=1&appCode=klmj&clientFrom=1&userId=13&token=569553243e028c6711f2a53e6bf6c4d3&gameId=1&useNum=1&orderType=1
    返回值：
    {
      "status": 1,
      "errCode": 0,
      "error": "没有错误",
      "data": {
        "isNew": "1",
        "userId": "13",
        "nickName": "郑凯斌",
        "avatar": "http://wx.qlogo.cn/mmopen/Q3auHgzwzM4WXv9zEF7tpJlkmXtEbDkJO3LZZjREcy3ic4YN8a3s5jpASmEib79mKAg7xxIdZZmd8EicxPcSRMRfw/0",
        "orderCode": "6177007419",
        "errTxt": "",
        "ip": "10.17.174.110"
      }
    }

    更新游戏房间订单状态
    请求示例：
    http://10.17.174.171:8192/GameService/updateOrder?status=used&orderCode=6177002443&roomId=123
    返回值：
    {
      "status": 1,
      "errCode": 0,
      "error": "没有错误",
      "data": {
        "orderCode": "6177002443"
      }
    }



*/