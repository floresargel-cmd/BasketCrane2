#include "application.h"

Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin);

int main(int argc, char* argv[])
{
    basket::BasketCraneApplication application;
    return application.Run(argc, argv);
}