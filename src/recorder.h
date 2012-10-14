#ifndef RECORDER_H
#define RECORDER_H

#include <QDataStream>
#include <QFile>
#include <QMap>
#include "keyboard.h"
#include "mouse.h"
#include "statistic.h"

struct OperationInADay;

/**
 * @brief Class of recorder which records the operations one by one.
 *
 * Class of recorder which records the operations one by one.
 * The change of the day won't be jugded here.
 */
class Recorder
{
public:
  Recorder();

  /**
   * @brief Add an operation of keyboard.
   *
   * @param program The program.
   * @param ms The ms in the day.
   * @param operation The operation.
   */
  void addOperation(QString program, int ms, Keyboard::KeyOperation operation);

  /**
   * @brief Add an operation of mouse.
   *
   * @param program The program.
   * @param ms The ms in the day.
   * @param operation The operation.
   */
  void addOperation(QString program, int ms, Mouse::MouseOperation operation);

  /**
   * @brief Record the statistic to the file.
   *
   * @return Whether it's recorded successfully..
   */
  bool record() const;

  /**
   * @brief Read the statistic from a file.
   */
  void init();

  /**
   * Called when a new day is reached.
   */
  void newDay();

  /**
   * @brief Read operations in a day.
   */
  static OperationInADay *readOperationsInADay(QDate date);

  /**
   * @brief Read all the statistics.
   */
  static Statistic *readAllStatistic();

private:
  /**
   * The statistic of the day.
   */
  Statistic statistic;

  /**
   * Last time of the key event.
   */
  int lastKeyTime;

  /**
   * Last time of the mouse event.
   */
  int lastMouseTime;

  /**
   * Last times of the key event.
   */
  QMap<QString, int> lastKeyTimes;

  /**
   * Last times of the mouse event.
   */
  QMap<QString, int> lastMouseTimes;

  /**
   * Last key event.
   */
  Keyboard::KeyOperation lastKeyOperation;

  /**
   * Last mouse event.
   */
  Mouse::MouseOperation lastmouseOperation;

  /**
   * Current modifiers.
   */
  Keyboard::KeyboardModifiers currentModifiers;

  /**
   * The status of the keys.
   */
  bool keyStatus[Keyboard::TOTAL_KEY_NUMBER];

  /**
   * Whether the last key is modifier.
   * If so, release a modifier key on the next step will be recorded as an operation.
   * (Such as Ctrl + Shift)
   * Otherwise, release a modifier will be ignored.
   * (Won't record Ctrl + Shift when using Ctrl + Shift + B)
   */
  bool lastIsCombo;

  /**
   * The file to save the operation.
   */
  QFile *file;

  /**
   * The stream to save the operation.
   */
  QDataStream *operationStream;

  /**
   * @brief Record the basic infomation of the operation
   */
  void recordBasic(QString program, int ms);
};

#endif // RECORDER_H
