#include "widget.hpp"
#include "application.hpp"
#include "main_window.hpp"

int main(int argc, char **argv)
{
  Application a(argc, argv);
  MainWindow mainWindow;
  return a.exec();
}
