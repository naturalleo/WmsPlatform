#include "WxoauthServer.h"
#include "WxoauthImp.h"

using namespace std;

WxoauthServer g_app;

/////////////////////////////////////////////////////////////////
void
WxoauthServer::initialize()
{
    //initialize application here:
    //...

    addServant<WxoauthImp>(ServerConfig::Application + "." + ServerConfig::ServerName + ".WxoauthObj");
}
/////////////////////////////////////////////////////////////////
void
WxoauthServer::destroyApp()
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
