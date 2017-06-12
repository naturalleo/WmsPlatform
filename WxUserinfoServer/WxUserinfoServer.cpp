#include "WxUserinfoServer.h"
#include "WxUserinfoImp.h"

using namespace std;

WxUserinfoServer g_app;

/////////////////////////////////////////////////////////////////
void
WxUserinfoServer::initialize()
{
    //initialize application here:
    //...

    addServant<WxUserinfoImp>(ServerConfig::Application + "." + ServerConfig::ServerName + ".WxUserinfoObj");
}
/////////////////////////////////////////////////////////////////
void
WxUserinfoServer::destroyApp()
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
