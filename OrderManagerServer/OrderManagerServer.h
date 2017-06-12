#ifndef _OrderManagerServer_H_
#define _OrderManagerServer_H_

#include <iostream>
#include "servant/Application.h"

using namespace tars;

/**
 *
 **/
class OrderManagerServer : public Application
{
public:
    /**
     *
     **/
    virtual ~OrderManagerServer() {};

    /**
     *
     **/
    virtual void initialize();

    /**
     *
     **/
    virtual void destroyApp();
};

extern OrderManagerServer g_app;

////////////////////////////////////////////
#endif
