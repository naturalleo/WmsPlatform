#ifndef _FundsManagerServer_H_
#define _FundsManagerServer_H_

#include <iostream>
#include "servant/Application.h"

using namespace tars;

/**
 *
 **/
class FundsManagerServer : public Application
{
public:
    /**
     *
     **/
    virtual ~FundsManagerServer() {};

    /**
     *
     **/
    virtual void initialize();

    /**
     *
     **/
    virtual void destroyApp();
};

extern FundsManagerServer g_app;

////////////////////////////////////////////
#endif
