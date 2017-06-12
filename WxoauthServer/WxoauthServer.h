#ifndef _WxoauthServer_H_
#define _WxoauthServer_H_

#include <iostream>
#include "servant/Application.h"

using namespace tars;

/**
 *
 **/
class WxoauthServer : public Application
{
public:
    /**
     *
     **/
    virtual ~WxoauthServer() {};

    /**
     *
     **/
    virtual void initialize();

    /**
     *
     **/
    virtual void destroyApp();
};

extern WxoauthServer g_app;

////////////////////////////////////////////
#endif
