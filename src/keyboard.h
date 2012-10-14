#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QString>

#ifdef WIN32
#include <Windows.h>
#endif // WIN32

/**
 * @brief Namespace to discribe a keyboard.
 *
 * Namespace to discribe a keyboard.
 * Redefined the keys, because may make a hook on linux later(maybe never-.-).
 */
namespace Keyboard
{
  /**
   * Keys.
   */
  enum Key
  {
    KEY_0 = 0,
    KEY_1 = 1,
    KEY_2 = 2,
    KEY_3 = 3,
    KEY_4 = 4,
    KEY_5 = 5,
    KEY_6 = 6,
    KEY_7 = 7,
    KEY_8 = 8,
    KEY_9 = 9,
    KEY_ENTER = 10,
    KEY_PAGE_UP = 11,
    KEY_PAGE_DOWN = 12,
    KEY_HOME = 13,
    KEY_END = 14,
    KEY_DELETE = 15,
    KEY_INSERT = 16,
    KEY_PRINT_SCREEN = 17,
    KEY_BREAK = 18,
    KEY_SCROLL_LOCK = 19,
    KEY_NUM_LOCK = 20,
    KEY_CAPS_LOCK =	21,
    KEY_QUOTE_LEFT = 22,
    KEY_MINUS = 23,
    KEY_EQUALS = 24,
    KEY_BACKSPACE = 25,
    KEY_BRACKET_LEFT = 26,
    KEY_BRACKET_RIGHT = 27,
    KEY_BACK_SLASH = 28,
    KEY_SEMICOLON = 29,
    KEY_APOSTROPHE = 30,
    KEY_COMMA = 31,
    KEY_PERIOD = 32,
    KEY_SLASH = 33,
    KEY_TAB = 34,
    KEY_UP = 35,
    KEY_DOWN = 36,
    KEY_LEFT = 37,
    KEY_RIGHT = 38,
    KEY_NUM_0 = 39,
    KEY_NUM_1 = 40,
    KEY_NUM_2 = 41,
    KEY_NUM_3 = 42,
    KEY_NUM_4 = 43,
    KEY_NUM_5 = 44,
    KEY_NUM_6 = 45,
    KEY_NUM_7 = 46,
    KEY_NUM_8 = 47,
    KEY_NUM_9 = 48,
    KEY_NUM_MINUS = 49,
    KEY_NUM_PLUS = 50,
    KEY_NUM_MULTIPLY = 51,
    KEY_NUM_DEVIDE = 52,
    KEY_NUM_ENTER = 53,
    KEY_NUM_DECIMAL = 54,
    KEY_ESCAPE = 55,
    KEY_WIN_LEFT = 56,
    KEY_WIN_RIGHT = 57,
    KEY_MENU = 58,
    KEY_SHIFT_LEFT = 59,
    KEY_SHIFT_RIGHT = 60,
    KEY_CTRL_LEFT = 61,
    KEY_CTRL_RIGHT = 62,
    KEY_ALT_LEFT = 63,
    KEY_ALT_RIGHT = 64,
    KEY_A = 65,
    KEY_B = 66,
    KEY_C = 67,
    KEY_D = 68,
    KEY_E = 69,
    KEY_F = 70,
    KEY_G = 71,
    KEY_H = 72,
    KEY_I = 73,
    KEY_J = 74,
    KEY_K = 75,
    KEY_L = 76,
    KEY_M = 77,
    KEY_N = 78,
    KEY_O = 79,
    KEY_P = 80,
    KEY_Q = 81,
    KEY_R = 82,
    KEY_S = 83,
    KEY_T = 84,
    KEY_U = 85,
    KEY_V = 86,
    KEY_W = 87,
    KEY_X = 88,
    KEY_Y = 89,
    KEY_Z = 90,
    KEY_SPACE = 91,
    KEY_F1 = 92,
    KEY_F2 = 93,
    KEY_F3 = 94,
    KEY_F4 = 95,
    KEY_F5 = 96,
    KEY_F6 = 97,
    KEY_F7 = 98,
    KEY_F8 = 99,
    KEY_F9 = 100,
    KEY_F10 = 101,
    KEY_F11 = 102,
    KEY_F12 = 103,
    KEY_UNKNOWN = 255
  };

  const int TOTAL_KEY_NUMBER = 104;

  /**
   * Modifiers.
   */
  enum KeyboardModifier
  {
    NoModifier = 0,
    Win = 1,
    Shift = 2,
    Ctrl = 4,
    Alt = 8
  };

  typedef QFlags<KeyboardModifier> KeyboardModifiers;

  /**
   * @brief Struct of combo key.
   */
  struct ComboKey
  {
    KeyboardModifiers modifiers;
    Key key;

    bool operator < (const ComboKey& another) const
    {
      return key < another.key ? true : modifiers < another.modifiers;
    }
  };

  /**
   * Operations.
   */
  enum Operation
  {
    Down = 0,
    Up = 1,
  };

  /**
   * @brief Struct of key operation.
   */
  struct KeyOperation
  {
    Key key;
    Operation operation;
  };

  /**
   * @brief Map key to its name.
   *
   * Map key to its name.
   * Return the raw name, contains both the name on keyboard and its full Chinese name.
   * You'd better use getKeySimpleEnName or getKeyFullZhName to get the name you want.
   *
   * @return The raw name, contains both the name on keyboard and its full Chinese name.
   */
  QString keyToString(Keyboard::Key key);

  /**
   * @brief Map combo key to its name.
   *
   * @return The name for user to read.
   */
  QString comboKeyToString(Keyboard::ComboKey key);

  /**
   * @brief Whether a key is a modifier.
   *
   * Whether a key is a modifier.
   * A modifier can be one of WIN, CTRL, ALT, DELETE.
   */
  bool isModifier(Keyboard::Key key);

  /**
   * @brief Calculate the modifiers of the given key.
   *
   * Calculate the modifiers of the given key.
   * A modifier can be one of WIN, CTRL, ALT, DELETE.
   * If the key isn't a modifier, NoModifier will be returned.
   */
  KeyboardModifiers keyToModifiers(Keyboard::Key key);

  /**
   * @brief Calculate the modifiers from the key status.
   *
   * Calculate the modifiers from the key status.
   * A modifier can be one of WIN, CTRL, ALT, DELETE.
   */
  KeyboardModifiers calculateModifiers(bool keyStatus[TOTAL_KEY_NUMBER]);

  /**
   * @brief Get the simple English name of the key.
   */
  QString getKeySimpleEnName(QString str);

  /**
   * @brief Get the full Chinese name of the key.
   */
  QString getKeyFullZhName(QString str);

#ifdef WIN32
  /**
   * @brief Map vk to key.
   */
  Keyboard::Key vkToKey(DWORD key, LPARAM lParam);

  /**
   * @brief Map key to vk.
   */
  DWORD keyToVk(Keyboard::Key key);
#endif // WIN32
};

#endif // KEYBOARD_H
