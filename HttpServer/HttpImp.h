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
#include "Order.h"
#include "Wxoauth.h"
#include "WxUserinfo.h"
#include "Funds.h"

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
};
/////////////////////////////////////////////////////
#endif


/*
http://10.17.172.34:40001/GameService/updateOrder?status=used&orderCode=userId%3D13%26appId%3D1%26appCode%3Dklmj%26useNum%3D1&useNum=1&roomId=123
http://10.17.172.34:40001/GameService/addOrder?appId=1&appCode=klmj&clientFrom=1&userId=13&token=569553243e028c6711f2a53e6bf6c4d3&gameId=1&useNum=1&orderType=1


*/