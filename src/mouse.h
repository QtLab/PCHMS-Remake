#ifndef MOUSE_H
#define MOUSE_H

#include <QPoint>
#include "keyboard.h"

/**
 * @brief Namespace to discribe a mouse.
 *
 * Class to discribe a mouse.
 * Redefined the keys, because may make a hook on linux later(maybe never-.-).
 */
namespace Mouse
{
  /**
   * Buttons.
   */
  enum Button
  {
    MainKey = 0,
    SubKey = 1,
    MiddleKey = 2,
    RollUp = 3,
    RollDown = 4,
    Unknown = 255
  };

  const int TOTAL_BUTTON_NUMBER = 5;

  /**
   * @brief Struct of combo button.
   */
  struct ComboButton
  {
    Keyboard::KeyboardModifiers modifiers;
    Button button;

    bool operator < (const ComboButton& another) const
    {
      return button < another.button ? true : modifiers < another.modifiers;
    }
  };

  /**
   * Operations.
   */
  enum Operation
  {
    Down = 0,
    Up = 1
  };

  /**
   * @brief Struct of mouse operation.
   */
  struct MouseOperation
  {
    Button button;
    Operation operation;
    QPoint pos;
  };


  /**
   * Map button to its name.
   */
  QString buttonToString(Button button);
};

#endif // MOUSE_H
