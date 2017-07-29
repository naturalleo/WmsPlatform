#ifndef _GameImp_H_
#define _GameImp_H_

#include "servant/Application.h"
#include "Game.h"
#include "DbAgent.h"

using namespace tars;
using namespace WmsPlatform;
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


    virtual int getPlayerGameRecord(const WmsPlatform::GameRecordReq& sIn, vector<GameRecordItem>& sOut, tars::TarsCurrentPtr current);
    virtual int getPlayerGameDetailRecord(const WmsPlatform::GameRecordDetailReq& sIn, vector<GameRecordDetailItem>& sOut, tars::TarsCurrentPtr current);
    virtual int getPlayerShareRecord(const WmsPlatform::GameShareRecordReq& sIn, string& sOut, tars::TarsCurrentPtr current);
private:
    DbAgent _db;

};
/////////////////////////////////////////////////////
#endif
