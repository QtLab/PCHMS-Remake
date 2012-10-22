#include "tray.h"

#include <QApplication>
#include <QProcess>
#include "hook.h"

// TODO: icon

#ifdef WIN32
#include <windows.h>
#endif // WIN32

Tray::Tray() : date(QDate::currentDate())
{
  createTrayIcon();
  install();
  connectHook();
  recorder.init();
  timer = new QTimer();
  timer->setInterval(60000);
  connect(timer, SIGNAL(timeout()), SLOT(flush()));
  timer->start();
}

void Tray::connectHook()
{
  Hook *hook = Hook::getInstance();
  connect(hook,
          SIGNAL(keyPressed(QString,Keyboard::Key,bool*)),
          SLOT(keyPressed(QString,Keyboard::Key,bool*)));
  connect(hook,
          SIGNAL(keyReleased(QString,Keyboard::Key,bool*)),
          SLOT(keyReleased(QString,Keyboard::Key,bool*)));
  connect(hook,
          SIGNAL(mousePressed(QString,QPoint,Mouse::Button,bool*)),
          SLOT(mousePressed(QString,QPoint,Mouse::Button,bool*)));
  connect(hook,
          SIGNAL(mouseReleased(QString,QPoint,Mouse::Button,bool*)),
          SLOT(mouseReleased(QString,QPoint,Mouse::Button,bool*)));
}

void Tray::createTrayIcon()
{
  installAction = new QAction(QString::fromUtf8("开始记录"), this);
  connect(installAction, SIGNAL(triggered()), SLOT(install()));

  uninstallAction = new QAction(QString::fromUtf8("暂停记录"), this);
  connect(uninstallAction, SIGNAL(triggered()), SLOT(uninstall()));

  quitAction = new QAction(QString::fromUtf8("退出"), this);
  connect(quitAction, SIGNAL(triggered()), SLOT(quit()));

  trayIconMenu = new QMenu();
  trayIconMenu->addSeparator();
  trayIconMenu->addAction(quitAction);
  trayIconMenu->addAction(installAction);
  trayIconMenu->addAction(uninstallAction);

  trayIcon = new QSystemTrayIcon();
  trayIcon->setContextMenu(trayIconMenu);
  trayIcon->setToolTip("PCHMS");

  QIcon icon = QIcon(":/PCHMS.icon");
  trayIcon->setIcon(icon);

  trayIcon->show();

  connect(trayIcon,
          SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
          SLOT(trayActivated(QSystemTrayIcon::ActivationReason)));
}

void Tray::install()
{
  installAction->setEnabled(false);
  uninstallAction->setEnabled(true);
  Hook::getInstance()->install();
}

void Tray::uninstall()
{
  installAction->setEnabled(true);
  uninstallAction->setEnabled(false);
  Hook::getInstance()->uninstall();
}

void Tray::trayActivated(QSystemTrayIcon::ActivationReason reason)
{
  if (reason == QSystemTrayIcon::DoubleClick)
  {
    // Created a new progress toshow the chart.
    // Reason is in the main.cpp
    QStringList arg;
    arg.push_back("arg1");
    QProcess::startDetached("PCHMS-Remake.exe", arg);
  }
}

void Tray::keyPressed(QString programName,
                      Keyboard::Key key,
                      bool *block)
{
  if (key == Keyboard::KEY_UNKNOWN)
    return;
  QDate currentDate = QDate::currentDate();
  if (date.daysTo(currentDate) != 0)
    recorder.newDay();
  Keyboard::KeyOperation operation;
  operation.key = key;
  operation.operation = Keyboard::Down;
  recorder.addOperation(programName,
                        QTime(0, 0, 0, 0).msecsTo(QTime::currentTime()),
                        operation);
}

void Tray::keyReleased(QString programName,
                       Keyboard::Key key,
                       bool *block)
{
  if (key == Keyboard::KEY_UNKNOWN)
    return;
  QDate currentDate = QDate::currentDate();
  if (date.daysTo(currentDate) != 0)
    recorder.newDay();
  Keyboard::KeyOperation operation;
  operation.key = key;
  operation.operation = Keyboard::Up;
  recorder.addOperation(programName,
                        QTime(0, 0, 0, 0).msecsTo(QTime::currentTime()),
                        operation);
}

void Tray::mousePressed(QString programName,
                        QPoint point,
                        Mouse::Button button,
                        bool *block)
{
  if (button == Mouse::Unknown)
    return;
  QDate currentDate = QDate::currentDate();
  if (date.daysTo(currentDate) != 0)
    recorder.newDay();
  Mouse::MouseOperation operation;
  operation.button = button;
  operation.pos = point;
  operation.operation = Mouse::Down;
  recorder.addOperation(programName,
                        QTime(0, 0, 0, 0).msecsTo(QTime::currentTime()),
                        operation);
}

void Tray::mouseReleased(QString programName,
                         QPoint point,
                         Mouse::Button button,
                         bool *block)
{
  if (button == Mouse::Unknown)
    return;
  QDate currentDate = QDate::currentDate();
  if (date.daysTo(currentDate) != 0)
    recorder.newDay();
  Mouse::MouseOperation operation;
  operation.button = button;
  operation.pos = point;
  operation.operation = Mouse::Up;
  recorder.addOperation(programName,
                        QTime(0, 0, 0, 0).msecsTo(QTime::currentTime()),
                        operation);
}

void Tray::flush()
{
  recorder.record();
}

void Tray::quit()
{
  delete this;
  qApp->quit();
}

Tray::~Tray()
{
  timer->stop();
  delete timer;
  flush();
  Hook::getInstance()->uninstall();
  trayIcon->hide();
  delete installAction;
  delete uninstallAction;
  delete quitAction;
  delete trayIcon;
  delete trayIconMenu;
}
