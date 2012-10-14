#ifndef HOOK_H
#define HOOK_H

#include <QObject>

#include <QPoint>
#include "keyboard.h"
#include "mouse.h"

/**
 * @brief Class of keyboard and mouse hook which can be installed.
 *
 * Class of keyboard and mouse hook which can be installed.
 * Will emit signals when raw receiving keyboard/mouse event.
 * An event can have extra infomation(a DWORD), a raw event's extra information
 * is 0, if you want to map a key to some key sequece, please add an extra
 * information so that this class will ignore the man-made events.
 */
class Hook : public QObject
{
  Q_OBJECT
public:
  /**
   * Get the instance of the hook.
   */
  static Hook *getInstance();

  /**
   * Install the hook.
   */
  void install();

  /**
   * Uninstall the hook.
   */
  void uninstall();

  // These 4 functions will emit a signal,
  // only supposed to be called in hook.cpp.
  inline void pressKey(QString programName,
                       Keyboard::Key key,
                       bool *block)
  {emit keyPressed(programName, key, block);}

  inline void releaseKey(QString programName,
                         Keyboard::Key key,
                         bool *block)
  {emit keyReleased(programName, key, block);}
  inline void pressMouse(QString programName,
                         QPoint point,
                         Mouse::Button button,
                         bool *block)
  {emit mousePressed(programName, point, button, block);}
  inline void releaseMouse(QString programName,
                           QPoint point,
                           Mouse::Button button,
                           bool *block)
  {emit mouseReleased(programName, point, button, block);}


private:
  // Not supposed to be called.
  Hook(){}

  // The only one instance of hook
  static Hook *hook;

  // Whether the hook has been installed
  static bool installed;

signals:
  // Raw signals
  void keyPressed(QString programName,
                  Keyboard::Key key,
                  bool *block);
  void keyReleased(QString programName,
                   Keyboard::Key key,
                   bool *block);
  void mousePressed(QString programName,
                    QPoint point,
                    Mouse::Button button,
                    bool *block);
  void mouseReleased(QString programName,
                     QPoint point,
                     Mouse::Button button,
                     bool *block);
};

#endif // HOOK_H
