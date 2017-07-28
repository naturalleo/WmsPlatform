#ifndef _GameImp_H_
#define _GameImp_H_

#include "servant/Application.h"
#include "Game.h"


using namespace tars;
/**
 *
 *
 */
class GameImp : public WmsPlatform::Game
{
public:
    /**
     *
     */
    virtual ~GameImp() {}

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
    virtual int test(tars::TarsCurrentPtr current) { return 0;};


    virtual int getPlayerGameRecord(const WmsPlatform::GameRecordReq& sIn, string& sOut, tars::TarsCurrentPtr current);
    virtual int getPlayerGameDetailRecord(const WmsPlatform::GameRecordDetailReq& sIn, string& sOut, tars::TarsCurrentPtr current);
    virtual int getPlayerShareRecord(const WmsPlatform::GameShareRecordReq& sIn, string& sOut, tars::TarsCurrentPtr current);

};
/////////////////////////////////////////////////////
#endif
