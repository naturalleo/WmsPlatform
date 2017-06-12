#include "OrderManagerServer.h"
#include "OrderImp.h"

using namespace std;

OrderManagerServer g_app;

/////////////////////////////////////////////////////////////////
void
OrderManagerServer::initialize()
{
    //initialize application here:
    //...

    addServant<OrderImp>(ServerConfig::Application + "." + ServerConfig::ServerName + ".OrderObj");
}
/////////////////////////////////////////////////////////////////
void
OrderManagerServer::destroyApp()
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
