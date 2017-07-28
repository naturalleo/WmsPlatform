#ifndef _GameRecordServer_H_
#define _GameRecordServer_H_

#include <iostream>
#include "servant/Application.h"

using namespace tars;

/**
 *
 **/
class GameRecordServer : public Application
{
public:
    /**
     *
     **/
    virtual ~GameRecordServer() {};

    /**
     *
     **/
    virtual void initialize();

    /**
     *
     **/
    virtual void destroyApp();
};

extern GameRecordServer g_app;

////////////////////////////////////////////
#endif
