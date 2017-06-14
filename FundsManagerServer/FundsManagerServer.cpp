#include "FundsManagerServer.h"
#include "FundsImp.h"

using namespace std;

FundsManagerServer g_app;

/////////////////////////////////////////////////////////////////
void
FundsManagerServer::initialize()
{
    //initialize application here:
    //...

    addServant<FundsImp>(ServerConfig::Application + "." + ServerConfig::ServerName + ".FundsObj");
}
/////////////////////////////////////////////////////////////////
void
FundsManagerServer::destroyApp()
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
