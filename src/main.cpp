#include <QApplication>
#include <QSystemTrayIcon>
#include <QMessageBox>
#include "mainwindow.h"
#include "tray.h"

// TODO: Comments, mainly the reason to do sth and the way to do it

int main(int argc, char *argv[])
{
//  QApplication::setStyle("cleanlooks");
  QApplication a(argc, argv);
  // Use the argument to judge whether it should be a deamon tray to record
  // or a ui to show the statistic.
  // Here are 2 main reasons:
  //   1. A problem not solved:
  //      On my computer, I install a low level mouse hook and create a widget.
  //      Once I click any of the buttons on the right top part, the ui will
  //      be blocked for a few seconds.
  //   2. Stability:
  //      The ui part may crash because of unknown bugs.
  //      The ui part may lead to memory leak because of unknown bugs.
//  if (argc <= 1)
//  {
//    // Tray and hook
//    if (!QSystemTrayIcon::isSystemTrayAvailable())
//    {
//        QMessageBox::critical(0,
//                              QString::fromUtf8("出错了"),
//                              QString::fromUtf8("未在此系统中发现托盘"));
//        return 1;
//    }

//    Tray *t = new Tray();
//    return a.exec();
//  }
//  else
  {
    // Main window
    MainWindow m;
    m.show();
    return a.exec();
  }
}
