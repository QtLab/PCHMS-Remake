#ifndef TRAY_H
#define TRAY_H

#include <QAction>
#include <QDate>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QTimer>
#include "keyboard.h"
#include "recorder.h"

/**
 * @brief Class of the deamon object on the tray to record operations.
 */
class Tray : public QObject
{
  Q_OBJECT
public:
  Tray();
  ~Tray();

private:
  /**
   * The recorder.
   */
  Recorder recorder;

  // Things for tray.
  QAction *installAction;
  QAction *uninstallAction;
  QAction *quitAction;
  QSystemTrayIcon *trayIcon;
  QMenu *trayIconMenu;

  /**
   * @brief Create the tray icon and the menu.
   */
  void createTrayIcon();

  /**
   * @brief Connect the hook.
   */
  void connectHook();

  /**
   * @brief Timer to flush.
   */
  QTimer *timer;

  /**
   * @brief The current date.
   */
  QDate date;

private slots:
  /**
   * @brief Slot used to install the hook.
   */
  void install();

  /**
   * @brief Slot used to uninstall the hook.
   */
  void uninstall();

  /**
   * @brief Slot called when tray is actived.
   *
   * Slot called when tray is actived.
   * May create a new progress to show the charts.
   * May show the menu.
   */
  void trayActivated(QSystemTrayIcon::ActivationReason reason);

  /**
   * @brief Slot called when key pressed.
   */
  void keyPressed(QString programName,
                  Keyboard::Key key,
                  bool *block);

  /**
   * @brief Slot called when key released.
   */
  void keyReleased(QString programName,
                   Keyboard::Key key,
                   bool *block);

  /**
   * @brief Slot called when mouse pressed.
   */
  void mousePressed(QString programName,
                    QPoint point,
                    Mouse::Button button,
                    bool *block);

  /**
   * @brief Slot called when mouse released.
   */
  void mouseReleased(QString programName,
                     QPoint point,
                     Mouse::Button button,
                     bool *block);

  /**
   * @brief Slot to flush the data.
   */
  void flush();

  /**
   * @brief Slot to quit.
   */
  void quit();
};

#endif // TRAY_H
