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

#ifndef _HttpImp_H_
#define _HttpImp_H_

#include "servant/Application.h"
#include "../OrderManagerServer/Order.h"
#include "../WxoauthServer/Wxoauth.h"
#include "../WxUserinfoServer/WxUserinfo.h"
#include "../FundsManagerServer/Funds.h"
#include "../GameRecordServer/Game.h"
#include <vector>
#include "util/tc_encoder.h"

#include "rapidjson/include/rapidjson/document.h"
#include "rapidjson/include/rapidjson/writer.h"
#include "rapidjson/include/rapidjson/stringbuffer.h"

using namespace rapidjson;
using namespace std;
using namespace WmsPlatform;

/**
 *
 *
 */
class HttpImp : public Servant
{
public:
    /**
     *
     */
    virtual ~HttpImp() {}

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
    bool isParamExist(const multimap<string, string> &mmpParams , const string& sName) const;


    string getValue(const multimap<string, string> &mmpParams , const string& sName) const;

    string toDefault(const string &s, const string &sDefault);

    int doRequest(TarsCurrentPtr current, vector<char> &buffer);


    string& trim(string &s);

    void parseNormal(multimap<string, string> &mmpParams, const string& sBuffer);

protected:
    OrderPrx    _orderPrx;
    WxoauthPrx  _wxoauthPrx;
    WxUserinfoPrx  _WxUserinfoPrx;
    FundsPrx  _FundsPrx;
    GamePrx   _GamePrx;
};
/////////////////////////////////////////////////////

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
        string s ;

        if (ret == 0 )
        {
            s = "{\"status\":1,\"errCode\":0,\"error\":\"\",\"data\":"
                    "{"
                    "\"orderCode\" : \""  + sOut +  "\","
                    "\"isNew\" : \"0\" "
                    "}"
                "}";
        }
        else
        {
            s = "{\"status\":-1,\"errCode\":-1,\"error\":\"ret -1\",\"data\":[]}";
        }


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

   virtual void callback_reportSuggestion(tars::Int32 ret)
    {
        //HttpImp::async_response_doRequest(_current, ret, sOut);
        TLOGDEBUG("callback_reportSuggestion : " << ret << endl);
        TC_HttpResponse rsp;
        vector<char> buffer;
        string s ;

        if (ret == 0 )
        {
            s = "{\"status\":1,\"errCode\":0,\"error\":\"\",\"data\": \"\"}";
        }
        else
        {
            s = "{\"status\":-1,\"errCode\":-1,\"error\":\"ret -1\",\"data\":[]}";
        }


        rsp.setResponse(s.c_str(),s.size());
        rsp.encode(buffer);     


        _current->sendResponse(&buffer.at(0),buffer.size());
        TLOGDEBUG("callback_reportSuggestion : " << s << s.size() << endl);
       // _current->sendResponse(tars::TARSSERVERSUCCESS, buffer);    
        //HttpImp::async_response_doRequest(_current, ret, buffer);
    }
    virtual void callback_reportSuggestion_exception(tars::Int32 ret)
    { 
        TLOGERROR("OrderCallback callback_reportSuggestion_exception ret:" << ret << endl); 

        Order::async_response_reportSuggestion(_current, ret);
    }

   virtual void callback_sysNotice(tars::Int32 ret, const SysNoticeRes& res)
    {
        //HttpImp::async_response_doRequest(_current, ret, sOut);
        TLOGDEBUG("callback_sysNotice : " << ret << endl);
        TC_HttpResponse rsp;
        vector<char> buffer;
        string s ;

        if (ret == 0 )
        {
            s = "{\"status\":1,\"errCode\":0,\"error\":\"\",\"data\":"
                    "{"
                       "\"list\": ["
                        "{"
                            "\"aId\": \"" + res.aId + "\","
                            "\"title\": \"" + res.title + "\","
                            "\"desc\": \"" + res.desc + "\""
                       "}]" 
                    "}"
                "}";
        }
        else
        {
            s = "{\"status\":-1,\"errCode\":-1,\"error\":\"ret -1\",\"data\":[]}";
        }


        rsp.setResponse(s.c_str(),s.size());
        rsp.encode(buffer);     


        _current->sendResponse(&buffer.at(0),buffer.size());
        TLOGDEBUG("callback_sysNotice : " << s << s.size() << endl);
       // _current->sendResponse(tars::TARSSERVERSUCCESS, buffer);    
        //HttpImp::async_response_doRequest(_current, ret, buffer);
    }
    virtual void callback_sysNotice_exception(tars::Int32 ret)
    { 
        TLOGERROR("OrderCallback callback_sysNotice_exception ret:" << ret << endl); 
        SysNoticeRes res;
        Order::async_response_sysNotice(_current, ret, res);
    }


    TarsCurrentPtr _current;
};


class WxoauthCallback : public WxoauthPrxCallback
{

public:
    WxoauthCallback(TarsCurrentPtr &current)
    : _current(current)
    {}

    virtual void callback_wxchatLogin(tars::Int32 ret,  const WmsPlatform::WxLoginUserinfoRes& sOut)
    {
        //HttpImp::async_response_doRequest(_current, ret, sOut);
        

        TLOGDEBUG("callback_wxchatLogin : " << ret << endl);
        TC_HttpResponse rsp;
        vector<char> buffer;
        string s ;
        if (0 == ret)
            s = "{\"status\":1,\"errCode\":0,\"error\":\"\",\"data\":"
                    "{ \"mySign \": false,"
                    "\"userId\" : \"" + sOut.userId + "\","
                    "\"token\" : \""  + sOut.token +  "\","
                    "\"nickName\" : \""  + sOut.nickname +  "\","
                    "\"gender\" : \""  + sOut.sex +  "\","
                    "\"ip\" : \""  + _current->getIp() +  "\","
                    "\"avatar\" : \""  + sOut.headimgurl +  "\","
                    "\"totalGameCard\" : \""  + sOut.totalcard +  "\","
                    "\"surplusGameCard\" : \""  + sOut.currentcard +  "\","
                    "\"activationCode\" : \""  + sOut.activationCode +  "\","
                    "\"isNew\" : "  + sOut.isNew +  " "
                    "}"
                "}";
        else if(1 == ret)
            s = "{\"status\":1,\"errCode\":10402,\"error\":\"微信登录失效,请重新登录\",\"data\":[]}";
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
        WmsPlatform::WxLoginUserinfoRes res;
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
        TLOGDEBUG("callback_updateWxUserCards : " << ret  << endl);
        TC_HttpResponse rsp;
        vector<char> buffer;
        string s;
        if (ret == 0 )
        {
            if (sOut.errorCode == 0){
                s = "{\"status\":1,\"errCode\":0,\"error\":\"\",\"data\":"
                    "{"
                    "\"userId\" : \"" + sOut.userId + "\","   
                    "\"totalGameCard\" : \"" + sOut.totalcard + "\","                         
                    "\"surplusGameCard\" : \""  + sOut.currentcard +  "\"" 
                    "}"
                "}";              
            }
            else{
                if (sOut.errorCode == -2){
                    s = "{\"status\":-1,\"errCode\":-2,\"error\":\"非法操作，玩家ID异常!\",\"data\":[]}";
                }else if (sOut.errorCode == -3){
                    s = "{\"status\":-1,\"errCode\":-3,\"error\":\"房卡不足，请检查房卡数!\",\"data\":[]}";
                }else if(sOut.errorCode == -4){
                    s = "{\"status\":-1,\"errCode\":-4,\"error\":\"玩家不存在，请重新输入赠送ID!\",\"data\":[]}";
                }else if(sOut.errorCode == -5){
                    s = "{\"status\":-1,\"errCode\":-5,\"error\":\"只有代理才能转让房卡，请联系官方客服!\",\"data\":[]}";
                }else if(sOut.errorCode == -6){
                    s = "{\"status\":-1,\"errCode\":-6,\"error\":\"不能转让给自己，请重新输入赠送ID!\",\"data\":[]}";
                }else if(sOut.errorCode == -7){
                    s = "{\"status\":-1,\"errCode\":-7,\"error\":\"赠送数量输入有误，请重新输入!\",\"data\":[]}";
                }else if(sOut.errorCode == -8){
                    s = "{\"status\":-1,\"errCode\":-8,\"error\":\"不能向同级代理转让房卡!\",\"data\":[]}";
                }else{
                    s = "{\"status\":-1,\"errCode\":-1,\"error\":\"ret -1\",\"data\":[]}";
                }              
            }
        }
        else
        {
            s = "{\"status\":-1,\"errCode\":-1,\"error\":\"ret -1\",\"data\":[]}";
        }

        rsp.setResponse(s.c_str(),s.size());
        rsp.encode(buffer);     

        _current->sendResponse(&buffer.at(0),buffer.size());
        TLOGDEBUG("callback_updateWxUserCards : " << s << s.size() << endl);
    }
    virtual void callback_updateWxUserCards_exception(tars::Int32 ret)
    { 
        TLOGERROR("WxUserinfoCallback callback_updateWxUserCards_exception ret:" << ret << endl); 
        WmsPlatform::WxUserExchangeRes res;
        WxUserinfo::async_response_updateWxUserCards(_current, ret, res);
    }

    virtual void callback_getWxUserIsAgent(tars::Int32 ret,  const WmsPlatform::WxUserisAgentRes& sOut)
    {
        TLOGDEBUG("callback_getWxUserIsAgent : " << ret << endl);
        TC_HttpResponse rsp;
        vector<char> buffer;
        string s;
        if (ret == 0 )
        {
            s = "{\"status\":1,\"errCode\":0,\"error\":\"\",\"data\":"
                    "{"
                    "\"userId\" : \"" + sOut.userId + "\","       
                    "\"agentType\" : \""  + sOut.agentType +  "\"" 
                    "}"
                "}";
        }
        else
        {
            s = "{\"status\":-1,\"errCode\":-1,\"error\":\"ret -1\",\"data\":[]}";
        }

        rsp.setResponse(s.c_str(),s.size());
        rsp.encode(buffer);     

        _current->sendResponse(&buffer.at(0),buffer.size());
        TLOGDEBUG("callback_getWxUserIsAgent : " << s << s.size() << endl);
    }
    virtual void callback_getWxUserIsAgent_exception(tars::Int32 ret)
    { 
        TLOGERROR("WxUserinfoCallback callback_getWxUserIsAgent_exception ret:" << ret << endl); 
        WmsPlatform::WxUserisAgentRes res;


        WxUserinfo::async_response_getWxUserIsAgent(_current, ret, res);
    }


    virtual void callback_getWxUserinfo(tars::Int32 ret,  const WmsPlatform::WxUserinfoRes& sOut)
    {
        //HttpImp::async_response_doRequest(_current, ret, sOut);
        TLOGDEBUG("callback_getWxUserinfo : " << ret << endl);
        TC_HttpResponse rsp;
        vector<char> buffer;
        string s;
        if (0 == ret)
            s = "{\"status\":1,\"errCode\":0,\"error\":\"\",\"data\":"
                    "{"
                    "\"userId\" : \"" + sOut.userId + "\","
                    "\"nickName\" : \""  + sOut.nickname +  "\","
                    "\"gender\" : \""  + sOut.sex +  "\","
                    "\"avatar\" : \""  + sOut.headimgurl +  "\","  
                    "\"activationCode\" : \""  + sOut.activationCode +  "\","                 
                    "\"ip\" : \""  + sOut.ip +  "\"" 
                    "}"
                "}";

        else
            s = "{\"status\":-1,\"errCode\":-1,\"error\":\"ret -1\",\"data\":[]}";

        rsp.setResponse(s.c_str(),s.size());
        rsp.encode(buffer);     

        _current->sendResponse(&buffer.at(0),buffer.size());
        TLOGDEBUG("callback_getWxUserinfo : " << s << s.size() << endl);
       // _current->sendResponse(tars::TARSSERVERSUCCESS, buffer);    
        //HttpImp::async_response_doRequest(_current, ret, buffer);
    }
    virtual void callback_getWxUserinfo_exception(tars::Int32 ret)
    { 
        TLOGERROR("WxUserinfoCallback callback_getWxUserIsAgent_exception ret:" << ret << endl); 
        WmsPlatform::WxUserinfoRes res;

        WxUserinfo::async_response_getWxUserinfo(_current, ret, res);
    }

    virtual void callback_setActivationCode(tars::Int32 ret,  const WmsPlatform::WxUserSetActivationCodeRes& sOut)
    {
        TLOGDEBUG("callback_setActivationCode : " << ret << endl);
        TC_HttpResponse rsp;
        vector<char> buffer;
        string s;
        if (ret == 0 )
        {
                if (sOut.errorCode == 0)
                {
                    s = "{\"status\":1,\"errCode\":0,\"error\":\"\",\"data\":"
                            "{"
                            "\"userId\" : \"" + sOut.userId + "\","       
                            "\"activationCode\" : \""  + sOut.activationCode +  "\"" 
                            "}"
                        "}";        
                }
                else if (sOut.errorCode == -2)
                {
                    s = "{\"status\":-1,\"errCode\":-2,\"error\":\"激活码无效，请重新输入!\",\"data\":[]}";
                }
                else if(sOut.errorCode == -3)
                {
                     s = "{\"status\":-1,\"errCode\":-3,\"error\":\"激活码无效，请重新输入!\",\"data\":[]}";
                }
                else if(sOut.errorCode == -4)
                {
                     s = "{\"status\":-1,\"errCode\":-4,\"error\":\"请输入湖南麻将激活码!\",\"data\":[]}";
                }
                else if(sOut.errorCode == -5)
                {
                     s = "{\"status\":-1,\"errCode\":-5,\"error\":\"请输入岭南麻将激活码!\",\"data\":[]}";
                }     
                else{
                    s = "{\"status\":-1,\"errCode\":-1,\"error\":\"激活码无效，请重新输入!\",\"data\":[]}";
                }  
        }
        else
        {
            s = "{\"status\":-1,\"errCode\":-1,\"error\":\"ret -1\",\"data\":[]}";
        }

        rsp.setResponse(s.c_str(),s.size());
        rsp.encode(buffer);     

        _current->sendResponse(&buffer.at(0),buffer.size());
        TLOGDEBUG("callback_setActivationCode : " << s << s.size() << endl);
    }
    virtual void callback_setActivationCode_exception(tars::Int32 ret)
    { 
        TLOGERROR("WxUserinfoCallback callback_setActivationCode_exception ret:" << ret << endl); 
        WmsPlatform::WxUserSetActivationCodeRes res;

        WxUserinfo::async_response_setActivationCode(_current, ret, res);
    }



    TarsCurrentPtr _current;
};


class FundsCallback : public FundsPrxCallback
{

public:
    FundsCallback(TarsCurrentPtr &current, string data ="")
    : _current(current),_data(data)
    {}

    virtual void callback_checkFunds(tars::Int32 ret)
    {
        //HttpImp::async_response_doRequest(_current, ret, sOut);
        TLOGDEBUG("callback_checkFunds : " << ret  << endl);
        TC_HttpResponse rsp;
        vector<char> buffer;
        string s;

        if (ret == 0 )
        {
            s = "{\"status\":1,\"errCode\":0,\"error\":\"\",\"data\":"
                    "{"
                    "\"orderCode\" : \""  + _data +  "\","
                    "\"isNew\" : \"0\" "
                    "}"
                "}";
        }
        else
        {
            s = "{\"status\":-1,\"errCode\":-1,\"error\":\"ret -1\",\"data\":[]}";
        }


        rsp.setResponse(s.c_str(),s.size());
        rsp.encode(buffer);     

        _current->sendResponse(&buffer.at(0),buffer.size());
        TLOGDEBUG("callback_checkFunds : " << s << s.size() << endl);
       // _current->sendResponse(tars::TARSSERVERSUCCESS, buffer);    
        //HttpImp::async_response_doRequest(_current, ret, buffer);
    }
    virtual void callback_checkFunds_exception(tars::Int32 ret)
    { 
        TLOGERROR("FundsCallback callback_checkFunds_exception ret:" << ret << endl); 
        Funds::async_response_checkFunds(_current, ret);
    }



    virtual void callback_modifyFunds(tars::Int32 ret, const WmsPlatform::FundsUserInfoRes& sOut)
    {
        //HttpImp::async_response_doRequest(_current, ret, sOut);
        TLOGDEBUG("callback_modifyFunds : " << ret  << endl);
        TC_HttpResponse rsp;
        vector<char> buffer;
        string s;

        if (ret == 0 )
        {
            s = "{\"status\":1,\"errCode\":0,\"error\":\"\",\"data\":"
                    "{"
                    "\"orderCode\" : \""  + _data +  "\"" 
                    "}"
                "}";
        }
        else
        {
            s = "{\"status\":-1,\"errCode\":-1,\"error\":\"ret -1\",\"data\":[]}";
        }


        rsp.setResponse(s.c_str(),s.size());
        rsp.encode(buffer);     

        _current->sendResponse(&buffer.at(0),buffer.size());
        TLOGDEBUG("callback_checkFunds : " << s << s.size() << endl);
       // _current->sendResponse(tars::TARSSERVERSUCCESS, buffer);    
        //HttpImp::async_response_doRequest(_current, ret, buffer);
    }
    virtual void callback_modifyFunds_exception(tars::Int32 ret)
    { 
        TLOGERROR("FundsCallback callback_checkFunds_exception ret:" << ret << endl); 
        WmsPlatform::FundsUserInfoRes res;
        Funds::async_response_modifyFunds(_current, ret, res);
    }

    virtual void callback_getFunds(tars::Int32 ret, const WmsPlatform::FundsUserInfoRes& sOut)
    {
        //HttpImp::async_response_doRequest(_current, ret, sOut);
        TLOGDEBUG("callback_getFunds : " << ret  << "ip : "<<_current->getIp()<< endl);
        TC_HttpResponse rsp;
        vector<char> buffer;
        string s;

        if (ret == 0 )
        {
            s = "{\"status\":1,\"errCode\":0,\"error\":\"\",\"data\":"
                    "{"
                    "\"userId\" : \""  + sOut.userId +  "\","
                    "\"ip\" : \""  + _current->getIp() +  "\","
                    "\"totalGameCard\" : \""  + sOut.totalcard +  "\","
                    "\"surplusGameCard\" : \""  + sOut.currentcard +  "\""
                    "}"
                "}";
        }
        else
        {
            s = "{\"status\":-1,\"errCode\":-1,\"error\":\"ret -1\",\"data\":[]}";
        }


        rsp.setResponse(s.c_str(),s.size());
        rsp.encode(buffer);     

        _current->sendResponse(&buffer.at(0),buffer.size());
        TLOGDEBUG("callback_getFunds : " << s << s.size() << endl);
       // _current->sendResponse(tars::TARSSERVERSUCCESS, buffer);    
        //HttpImp::async_response_doRequest(_current, ret, buffer);
    }
    virtual void callback_getFunds_exception(tars::Int32 ret)
    { 
        TLOGERROR("FundsCallback callback_getFunds_exception ret:" << ret << endl); 
        WmsPlatform::FundsUserInfoRes res;
        Funds::async_response_modifyFunds(_current, ret, res);
    }


    virtual void callback_setWinnerLog(tars::Int32 ret)
    {
        //HttpImp::async_response_doRequest(_current, ret, sOut);
        TLOGDEBUG("callback_setWinnerLog : " << ret << endl);
        TC_HttpResponse rsp;
        vector<char> buffer;
        string s ;

        if (ret == 0 )
        {
            s = "{\"status\":1,\"errCode\":0,\"error\":\"\",\"data\": \"\"}";
        }
        else
        {
            s = "{\"status\":-1,\"errCode\":-1,\"error\":\"ret -1\",\"data\":[]}";
        }


        rsp.setResponse(s.c_str(),s.size());
        rsp.encode(buffer);     


        _current->sendResponse(&buffer.at(0),buffer.size());
        TLOGDEBUG("callback_setWinnerLog : " << s << s.size() << endl);
       // _current->sendResponse(tars::TARSSERVERSUCCESS, buffer);    
        //HttpImp::async_response_doRequest(_current, ret, buffer);
    }
    virtual void callback_setWinnerLog_exception(tars::Int32 ret)
    { 
        TLOGERROR("OrderCallback callback_setWinnerLog_exception ret:" << ret << endl); 

        Funds::async_response_setWinnerLog(_current, ret);
    }


    TarsCurrentPtr _current;
    string _data;
};


class GameCallback : public GamePrxCallback
{

public:
    GameCallback(TarsCurrentPtr &current)
    : _current(current)
    {}

    virtual void callback_getPlayerGameRecord(tars::Int32 ret,  const vector<GameRecordItem>& sOut)
    {
        //HttpImp::async_response_doRequest(_current, ret, sOut);
        TLOGDEBUG("callback_getPlayerGameRecord : " << ret << endl);
        TC_HttpResponse rsp;
        vector<char> buffer;
        string s ;

        if (ret == 0 )
        {
            s = "{\"status\":1,\"errCode\":0,\"error\":\"\",\"data\":[";
            vector<GameRecordItem>::const_iterator itr;
            for(itr = sOut.begin(); itr != sOut.end(); ++itr)
                {
                    s+= (itr == sOut.begin() ? string(""):string(", ")) +
                        "{" 
                            + "\"room_id\": \""        + itr->room_id + "\", "
                            + "\"owner\": \""          + itr->owner + "\", "
                            + "\"end_time\": \""       + itr->end_time + "\", "
                            + "\"game_num\": \""       + itr->game_num + "\", "
                            + "\"shareCode\": \""      + itr->shareCode + "\", "
                            + "\"chair_1_uid\": \""    + itr->chair_1_uid + "\", "
                            + "\"chair_1_name\": \""   + itr->chair_1_name + "\", "
                            + "\"chair_1_point\": \""  + itr->chair_1_point + "\", "
                            + "\"chair_1_avatar\": \"" + itr->chair_1_avatar + "\", "

                            + "\"chair_2_uid\": \""    + itr->chair_2_uid + "\", "
                            + "\"chair_2_name\": \""   + itr->chair_2_name + "\", "
                            + "\"chair_2_point\": \""  + itr->chair_2_point + "\", "
                            + "\"chair_2_avatar\": \"" + itr->chair_2_avatar + "\", "

                            + "\"chair_3_uid\": \""    + itr->chair_3_uid + "\", "
                            + "\"chair_3_name\": \""   + itr->chair_3_name + "\", "
                            + "\"chair_3_point\": \""  + itr->chair_3_point + "\", "
                            + "\"chair_3_avatar\": \"" + itr->chair_3_avatar + "\", "

                            + "\"chair_4_uid\": \""    + itr->chair_4_uid + "\", "
                            + "\"chair_4_name\": \""   + itr->chair_4_name + "\", "
                            + "\"chair_4_point\": \""  + itr->chair_4_point + "\", "
                            + "\"chair_4_avatar\": \"" + itr->chair_4_avatar + "\""
                        "}";
                }
                s += "]}";
        }
        else
        {
            s = "{\"status\":-1,\"errCode\":-1,\"error\":\"ret -1\",\"data\":[]}";
        }


        rsp.setResponse(s.c_str(),s.size());
        rsp.encode(buffer);     


        _current->sendResponse(&buffer.at(0),buffer.size());
        TLOGDEBUG("callback_getPlayerGameRecord : " << s.size() << endl);
       // _current->sendResponse(tars::TARSSERVERSUCCESS, buffer);    
        //HttpImp::async_response_doRequest(_current, ret, buffer);
    }
    virtual void callback_getPlayerGameRecordr_exception(tars::Int32 ret)
    { 
        TLOGERROR("callback_getPlayerGameRecordr_exception ret:" << ret << endl); 
        vector<GameRecordItem> res;
        Game::async_response_getPlayerGameRecord(_current, ret, res);
    }


    virtual void callback_getPlayerGameDetailRecord(tars::Int32 ret,  const vector<GameRecordDetailItem>& sOut)
    {
        //HttpImp::async_response_doRequest(_current, ret, sOut);
        TLOGDEBUG("callback_getPlayerGameDetailRecord : " << ret  << endl);
        TC_HttpResponse rsp;
        vector<char> buffer;
        string s ;

        if (ret == 0 )
        {
           s = "{\"status\":1,\"errCode\":0,\"error\":\"\",\"data\":[";
            for(vector<GameRecordDetailItem>::const_iterator itr = sOut.begin(); itr != sOut.end(); ++itr)
                {
                    s+= (itr == sOut.begin() ? string(""):string(", ")) +
                        "{" 
                            + "\"room_id\": \""        + itr->room_id + "\", "
                            + "\"owner\": \""          + itr->owner + "\", "
                            + "\"end_time\": \""       + itr->end_time + "\", "
                            + "\"game_index\": \""     + itr->game_index + "\", "
                            + "\"shareCode\": \""      + itr->share_code + "\", "
                            + "\"config\": \""         + itr->config + "\", "
                            + "\"game_action\": \""    + itr->game_action + "\", "

                            + "\"chair_1_uid\": \""    + itr->chair_1_uid + "\", "
                            + "\"chair_1_point\": \""  + itr->chair_1_point + "\", "


                            + "\"chair_2_uid\": \""    + itr->chair_2_uid + "\", "
                            + "\"chair_2_point\": \""  + itr->chair_2_point + "\", "

                            + "\"chair_3_uid\": \""    + itr->chair_3_uid + "\", "
                            + "\"chair_3_point\": \""  + itr->chair_3_point + "\", "

                            + "\"chair_4_uid\": \""    + itr->chair_4_uid + "\", "
                            + "\"chair_4_point\": \""  + itr->chair_4_point + "\" "
                        "}";
                }
                s += "]}";
        }
        else
        {
            s = "{\"status\":-1,\"errCode\":-1,\"error\":\"ret -1\",\"data\":[]}";
        }


        rsp.setResponse(s.c_str(),s.size());
        rsp.encode(buffer);     


        _current->sendResponse(&buffer.at(0),buffer.size());
        TLOGDEBUG("callback_getPlayerGameDetailRecord : " << s.size() << endl);
       // _current->sendResponse(tars::TARSSERVERSUCCESS, buffer);    
        //HttpImp::async_response_doRequest(_current, ret, buffer);
    }
    virtual void callback_getPlayerGameDetailRecord_exception(tars::Int32 ret)
    { 
        TLOGERROR("callback_getPlayerGameDetailRecord_exception ret:" << ret << endl); 
        vector<GameRecordDetailItem> res;
        Game::async_response_getPlayerGameDetailRecord(_current, ret, res);
    }

 virtual void callback_getPlayerShareRecord(tars::Int32 ret,  const std::string& sOut)
    {
        //HttpImp::async_response_doRequest(_current, ret, sOut);
        TLOGDEBUG("callback_getPlayerShareRecord : " << ret  << endl);
        TC_HttpResponse rsp;
        vector<char> buffer;
        string s ;

        if (ret == 0 )
        {
           s = sOut;
        }
        else
        {
            s = "{\"status\":-1,\"errCode\":-1,\"error\":\"ret -1\",\"data\":[]}";
        }


        rsp.setResponse(s.c_str(),s.size());
        rsp.encode(buffer);     


        _current->sendResponse(&buffer.at(0),buffer.size());
        TLOGDEBUG("callback_getPlayerShareRecord : " << s.size() << endl);
       // _current->sendResponse(tars::TARSSERVERSUCCESS, buffer);    
        //HttpImp::async_response_doRequest(_current, ret, buffer);
    }
    virtual void callback_getPlayerShareRecord_exception(tars::Int32 ret)
    { 
        TLOGERROR("callback_getPlayerShareRecord_exception ret:" << ret << endl); 
        string res;
        Game::async_response_getPlayerShareRecord(_current, ret, res);
    }





    TarsCurrentPtr _current;
};

#endif

/*

http://10.17.172.34:40001/GameService/updateOrder?status=used&orderCode=userId%3D13%26appId%3D1%26appCode%3Dklmj%26useNum%3D1&useNum=1&roomId=123
http://10.17.172.34:40001/GameService/addOrder?appId=1&appCode=klmj&clientFrom=1&userId=13&token=569553243e028c6711f2a53e6bf6c4d3&gameId=1&useNum=1&orderType=1


*/
