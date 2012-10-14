#ifndef OPERATIONINADAY_H
#define OPERATIONINADAY_H

#include <QMap>
#include <QVector>
#include "keyboard.h"
#include "mouse.h"

/**
 * @brief Struct to store operation in a day.
 */
struct OperationInADay
{
  /**
   * @brief Struct to store key operation.
   */
  struct KeyOperation
  {
    Keyboard::KeyOperation operation;
    Keyboard::KeyboardModifiers modifiers;
    QString program;
    int ms;
    int id;
  };

  /**
   * @brief Struct to store mouse operation.
   */
  struct MouseOperation
  {
    Mouse::MouseOperation operation;
    Keyboard::KeyboardModifiers modifiers;
    QString program;
    int ms;
    int id;
  };

  /**
   * Count of the operations of program.
   */
  QMap<QString, int> counts;

  /**
   * Key operations.
   */
  QVector<KeyOperation> keyOperations;

  /**
   * Mouse operations.
   */
  QVector<MouseOperation> mouseOperations;
};

#endif // OPERATIONINADAY_H
