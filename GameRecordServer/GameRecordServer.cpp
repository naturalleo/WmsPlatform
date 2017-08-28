#include "GameRecordServer.h"
#include "GameImp.h"

using namespace std;

GameRecordServer g_app;
TC_Config * g_pconf;
/////////////////////////////////////////////////////////////////
void
GameRecordServer::initialize()
{
    //initialize application here:
    //...

    addServant<GameImp>(ServerConfig::Application + "." + ServerConfig::ServerName + ".GameObj");
}
/////////////////////////////////////////////////////////////////
void
GameRecordServer::destroyApp()
{
    //destroy application here:
    //...
}
/////////////////////////////////////////////////////////////////
int
main(int argc, char* argv[])
{
    try
    {
        g_pconf =  & g_app.getConfig();
        g_app.main(argc, argv);
        g_app.waitForShutdown();
    }
    catch (std::exception& e)
    {
        cerr << "std::exception:" << e.what() << std::endl;
    }
    catch (...)
    {
        cerr << "unknown exception." << std::endl;
    }
    return -1;
}
/////////////////////////////////////////////////////////////////
