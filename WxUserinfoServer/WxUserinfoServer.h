#ifndef _WxUserinfoServer_H_
#define _WxUserinfoServer_H_

#include <iostream>
#include "servant/Application.h"

using namespace tars;

/**
 *
 **/
class WxUserinfoServer : public Application
{
public:
    /**
     *
     **/
    virtual ~WxUserinfoServer() {};

    /**
     *
     **/
    virtual void initialize();

    /**
     *
     **/
    virtual void destroyApp();
};

extern WxUserinfoServer g_app;

////////////////////////////////////////////
#endif
