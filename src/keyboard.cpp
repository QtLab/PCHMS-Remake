#include "keyboard.h"

#include <QStringList>

#ifdef WIN32

Keyboard::Key Keyboard::vkToKey(DWORD key, LPARAM lParam)
{
  switch (key)
  {
  case 0x30:
    return KEY_0;
  case 0x31:
    return KEY_1;
  case 0x32:
    return KEY_2;
  case 0x33:
    return KEY_3;
  case 0x34:
    return KEY_4;
  case 0x35:
    return KEY_5;
  case 0x36:
    return KEY_6;
  case 0x37:
    return KEY_7;
  case 0x38:
    return KEY_8;
  case 0x39:
    return KEY_9;
  case VK_RETURN:
    return ((int)lParam & 0x1000000) == 0 ? KEY_ENTER : KEY_NUM_ENTER;
  case VK_PRIOR:
    return KEY_PAGE_UP;
  case VK_NEXT:
    return KEY_PAGE_DOWN;
  case VK_HOME:
    return KEY_HOME;
  case VK_END:
    return KEY_END;
  case VK_DELETE:
    return KEY_DELETE;
  case VK_INSERT:
    return KEY_INSERT;
  case VK_SNAPSHOT:
    return KEY_PRINT_SCREEN;
  case VK_PAUSE:
    return KEY_BREAK;
  case VK_SCROLL:
    return KEY_SCROLL_LOCK;
  case VK_NUMLOCK:
    return KEY_NUM_LOCK;
  case VK_CAPITAL:
    return KEY_CAPS_LOCK;
  case 192:
    return KEY_QUOTE_LEFT;
  case 189:
    return KEY_MINUS;
  case 187:
    return KEY_EQUALS;
  case VK_BACK:
    return KEY_BACKSPACE;
  case 219:
    return KEY_BRACKET_LEFT;
  case 221:
    return KEY_BRACKET_RIGHT;
  case 220:
    return KEY_BACK_SLASH;
  case 186:
    return KEY_SEMICOLON;
  case 222:
    return KEY_APOSTROPHE;
  case 188:
    return KEY_COMMA;
  case 190:
    return KEY_PERIOD;
  case 191:
    return KEY_SLASH;
  case VK_TAB:
    return KEY_TAB;
  case VK_UP:
    return KEY_UP;
  case VK_DOWN:
    return KEY_DOWN;
  case VK_LEFT:
    return KEY_LEFT;
  case VK_RIGHT:
    return KEY_RIGHT;
  case VK_NUMPAD0:
    return KEY_NUM_0;
  case VK_NUMPAD1:
    return KEY_NUM_1;
  case VK_NUMPAD2:
    return KEY_NUM_2;
  case VK_NUMPAD3:
    return KEY_NUM_3;
  case VK_NUMPAD4:
    return KEY_NUM_4;
  case VK_NUMPAD5:
    return KEY_NUM_5;
  case VK_NUMPAD6:
    return KEY_NUM_6;
  case VK_NUMPAD7:
    return KEY_NUM_7;
  case VK_NUMPAD8:
    return KEY_NUM_8;
  case VK_NUMPAD9:
    return KEY_NUM_9;
  case VK_SUBTRACT:
    return KEY_NUM_MINUS;
  case VK_ADD:
    return KEY_NUM_PLUS;
  case VK_MULTIPLY:
    return KEY_NUM_MULTIPLY;
  case VK_DIVIDE:
    return KEY_NUM_DEVIDE;
  case VK_DECIMAL:
    return KEY_NUM_DECIMAL;
  case VK_ESCAPE:
    return KEY_ESCAPE;
  case VK_LWIN:
    return KEY_WIN_LEFT;
  case VK_RWIN:
    return KEY_WIN_RIGHT;
  case VK_APPS:
    return KEY_MENU;
  case VK_LSHIFT:
    return KEY_SHIFT_LEFT;
  case VK_RSHIFT:
    return KEY_SHIFT_RIGHT;
  case VK_LCONTROL:
    return KEY_CTRL_LEFT;
  case VK_RCONTROL:
    return KEY_CTRL_RIGHT;
  case VK_LMENU:
    return KEY_ALT_LEFT;
  case VK_RMENU:
    return KEY_ALT_RIGHT;
  case 'A':
  case 'B':
  case 'C':
  case 'D':
  case 'E':
  case 'F':
  case 'G':
  case 'H':
  case 'I':
  case 'J':
  case 'K':
  case 'L':
  case 'M':
  case 'N':
  case 'O':
  case 'P':
  case 'Q':
  case 'R':
  case 'S':
  case 'T':
  case 'U':
  case 'V':
  case 'W':
  case 'X':
  case 'Y':
  case 'Z':
    return (Keyboard::Key) key;
  case VK_SPACE:
    return KEY_SPACE;
  case VK_F1:
    return KEY_F1;
  case VK_F2:
    return KEY_F2;
  case VK_F3:
    return KEY_F3;
  case VK_F4:
    return KEY_F4;
  case VK_F5:
    return KEY_F5;
  case VK_F6:
    return KEY_F6;
  case VK_F7:
    return KEY_F7;
  case VK_F8:
    return KEY_F8;
  case VK_F9:
    return KEY_F9;
  case VK_F10:
    return KEY_F10;
  case VK_F11:
    return KEY_F11;
  case VK_F12:
    return KEY_F12;
  default:
    return KEY_UNKNOWN;
  }
}

DWORD Keyboard::keyToVk(Keyboard::Key key)
{
  switch (key)
  {
  case KEY_0:
    return 0x30;
  case KEY_1:
    return 0x31;
  case KEY_2:
    return 0x32;
  case KEY_3:
    return 0x33;
  case KEY_4:
    return 0x34;
  case KEY_5:
    return 0x35;
  case KEY_6:
    return 0x36;
  case KEY_7:
    return 0x37;
  case KEY_8:
    return 0x38;
  case KEY_9:
    return 0x39;
  case KEY_ENTER:
  case KEY_NUM_ENTER:
    return VK_RETURN;
  case KEY_PAGE_UP:
    return VK_PRIOR;
  case KEY_PAGE_DOWN:
    return VK_NEXT;
  case KEY_HOME:
    return VK_HOME;
  case KEY_END:
    return VK_END;
  case KEY_DELETE:
    return VK_DELETE;
  case KEY_INSERT:
    return VK_INSERT;
  case KEY_PRINT_SCREEN:
    return VK_SNAPSHOT;
  case KEY_BREAK:
    return VK_PAUSE;
  case KEY_SCROLL_LOCK:
    return VK_SCROLL;
  case KEY_NUM_LOCK:
    return VK_NUMLOCK;
  case KEY_CAPS_LOCK:
    return VK_CAPITAL;
  case KEY_QUOTE_LEFT:
    return 192;
  case KEY_MINUS:
    return 189;
  case KEY_EQUALS:
    return 187;
  case KEY_BACKSPACE:
    return VK_BACK;
  case KEY_BRACKET_LEFT:
    return 219;
  case KEY_BRACKET_RIGHT:
    return 221;
  case KEY_BACK_SLASH:
    return 220;
  case KEY_SEMICOLON:
    return 186;
  case KEY_APOSTROPHE:
    return 222;
  case KEY_COMMA:
    return 188;
  case KEY_PERIOD:
    return 190;
  case KEY_SLASH:
    return 191;
  case KEY_TAB:
    return VK_TAB;
  case KEY_UP:
    return VK_UP;
  case KEY_DOWN:
    return VK_DOWN;
  case KEY_LEFT:
    return VK_LEFT;
  case KEY_RIGHT:
    return VK_RIGHT;
  case KEY_NUM_0:
    return VK_NUMPAD0;
  case KEY_NUM_1:
    return VK_NUMPAD1;
  case KEY_NUM_2:
    return VK_NUMPAD2;
  case KEY_NUM_3:
    return VK_NUMPAD3;
  case KEY_NUM_4:
    return VK_NUMPAD4;
  case KEY_NUM_5:
    return VK_NUMPAD5;
  case KEY_NUM_6:
    return VK_NUMPAD6;
  case KEY_NUM_7:
    return VK_NUMPAD7;
  case KEY_NUM_8:
    return VK_NUMPAD8;
  case KEY_NUM_9:
    return VK_NUMPAD9;
  case KEY_NUM_MINUS:
    return VK_SUBTRACT;
  case KEY_NUM_PLUS:
    return VK_ADD;
  case KEY_NUM_MULTIPLY:
    return VK_MULTIPLY;
  case KEY_NUM_DEVIDE:
    return VK_DIVIDE;
  case KEY_NUM_DECIMAL:
    return VK_DECIMAL;
  case KEY_ESCAPE:
    return VK_ESCAPE;
  case KEY_WIN_LEFT:
    return VK_LWIN;
  case KEY_WIN_RIGHT:
    return VK_RWIN;
  case KEY_MENU:
    return VK_APPS;
  case KEY_SHIFT_LEFT:
    return VK_LSHIFT;
  case KEY_SHIFT_RIGHT:
    return VK_RSHIFT;
  case KEY_CTRL_LEFT:
    return VK_LCONTROL;
  case KEY_CTRL_RIGHT:
    return VK_RCONTROL;
  case KEY_ALT_LEFT:
    return VK_LMENU;
  case KEY_ALT_RIGHT:
    return VK_RMENU;
  case 'A':
  case 'B':
  case 'C':
  case 'D':
  case 'E':
  case 'F':
  case 'G':
  case 'H':
  case 'I':
  case 'J':
  case 'K':
  case 'L':
  case 'M':
  case 'N':
  case 'O':
  case 'P':
  case 'Q':
  case 'R':
  case 'S':
  case 'T':
  case 'U':
  case 'V':
  case 'W':
  case 'X':
  case 'Y':
  case 'Z':
    return (DWORD) key;
  case KEY_SPACE:
    return VK_SPACE;
  case KEY_F1:
    return VK_F1;
  case KEY_F2:
    return VK_F2;
  case KEY_F3:
    return VK_F3;
  case KEY_F4:
    return VK_F4;
  case KEY_F5:
    return VK_F5;
  case KEY_F6:
    return VK_F6;
  case KEY_F7:
    return VK_F7;
  case KEY_F8:
    return VK_F8;
  case KEY_F9:
    return VK_F9;
  case KEY_F10:
    return VK_F10;
  case KEY_F11:
    return VK_F11;
  case KEY_F12:
    return VK_F12;
  default:
    return 0xFF;
  }
}

#endif // WIN32

const static char *keyNames[] = {"0",
                                 "1",
                                 "2",
                                 "3",
                                 "4",
                                 "5",
                                 "6",
                                 "7",
                                 "8",
                                 "9",
                                 "<─┘,回车",
                                 "pup,上页",
                                 "pdn,下页",
                                 "hm,起始",
                                 "end,结束",
                                 "del,删除",
                                 "ins,插入",
                                 "psc,印屏幕",
                                 "brk,中断",
                                 "slk,滚动锁定",
                                 "nlk,小键盘锁定",
                                 "Lock,大小写锁定",
                                 "`",
                                 "-",
                                 "=",
                                 "<——,退格",
                                 "[",
                                 "]",
                                 "\\",
                                 ";",
                                 "'",
                                 ",",
                                 ".",
                                 "/",
                                 "TAB",
                                 "↑",
                                 "↓",
                                 "←",
                                 "→",
                                 "0,小键盘0",
                                 "1,小键盘1",
                                 "2,小键盘2",
                                 "3,小键盘3",
                                 "4,小键盘4",
                                 "5,小键盘5",
                                 "6,小键盘6",
                                 "7,小键盘7",
                                 "8,小键盘8",
                                 "9,小键盘9",
                                 "-,小键盘-",
                                 "+,小键盘+",
                                 "*,小键盘*",
                                 "/,小键盘/",
                                 "<┘,小键盘回车",
                                 ".,小键盘.",
                                 "Ese,退出",
                                 "WIN,左WINDOWS",
                                 "WIN,右WINDOWS",
                                 "Menu,菜单",
                                 "SHIFT,左SHIFT",
                                 "SHIFT,右SHIFT",
                                 "CTRL,左CTRL",
                                 "CTRL,右CTRL",
                                 "ALT,左ALT",
                                 "ALT,右ALT",
                                 "A",
                                 "B",
                                 "C",
                                 "D",
                                 "E",
                                 "F",
                                 "G",
                                 "H",
                                 "I",
                                 "J",
                                 "K",
                                 "L",
                                 "M",
                                 "N",
                                 "O",
                                 "P",
                                 "Q",
                                 "R",
                                 "S",
                                 "T",
                                 "U",
                                 "V",
                                 "W",
                                 "X",
                                 "Y",
                                 "Z",
                                 "Space,空格",
                                 "F1",
                                 "F2",
                                 "F3",
                                 "F4",
                                 "F5",
                                 "F6",
                                 "F7",
                                 "F8",
                                 "F9",
                                 "F10",
                                 "F11",
                                 "F12",
                                 "Unknown,未知"};

QString Keyboard::keyToString(Keyboard::Key key)
{
  if (key >= 0 && key < TOTAL_KEY_NUMBER)
    return QString::fromUtf8(keyNames[key]);
  return QString::fromUtf8(keyNames[TOTAL_KEY_NUMBER]);
}

QString Keyboard::comboKeyToString(Keyboard::ComboKey key)
{
  Keyboard::KeyboardModifiers modifiers = keyToModifiers(key.key);
  modifiers = key.modifiers & (~modifiers);
  QString result;
  bool firstModifier = true;
  if (modifiers & Win)
  {
    result += firstModifier ? QString("WIN") : QString("+WIN");
    firstModifier = false;
  }
  if (modifiers & Ctrl)
  {
    result += firstModifier ? QString("CTRL") : QString("+CTRL");
    firstModifier = false;
  }
  if (modifiers & Shift)
  {
    result += firstModifier ? QString("SHIFT") : QString("+SHIFT");
    firstModifier = false;
  }
  if (modifiers & Alt)
  {
    result += firstModifier ? QString("ALT") : QString("+Alt");
    firstModifier = false;
  }
  result += QString("+") + getKeyFullZhName(keyToString(key.key));
  return result;
}

bool Keyboard::isModifier(Keyboard::Key key)
{
  return key == KEY_WIN_LEFT ||
         key == KEY_WIN_LEFT ||
         key == KEY_SHIFT_LEFT ||
         key == KEY_SHIFT_RIGHT ||
         key == KEY_CTRL_LEFT ||
         key == KEY_CTRL_RIGHT ||
         key == KEY_ALT_LEFT ||
         key == KEY_ALT_RIGHT;
}

Keyboard::KeyboardModifiers Keyboard::keyToModifiers(Keyboard::Key key)
{
  if (key == KEY_WIN_LEFT || key == KEY_WIN_LEFT)
    return Win;
  if (key == KEY_SHIFT_LEFT || key == KEY_SHIFT_RIGHT)
    return Shift;
  if (key == KEY_CTRL_LEFT || key == KEY_CTRL_RIGHT)
    return Ctrl;
  if (key == KEY_ALT_LEFT || key == KEY_ALT_RIGHT)
    return Alt;
  return NoModifier;
}

Keyboard::KeyboardModifiers Keyboard::calculateModifiers(bool keyStatus[TOTAL_KEY_NUMBER])
{
  KeyboardModifiers result = NoModifier;
  if (keyStatus[KEY_WIN_LEFT] || keyStatus[KEY_WIN_RIGHT])
    result |= Win;
  if (keyStatus[KEY_SHIFT_LEFT] || keyStatus[KEY_SHIFT_RIGHT])
    result |= Shift;
  if (keyStatus[KEY_CTRL_LEFT] || keyStatus[KEY_CTRL_RIGHT])
    result |= Ctrl;
  if (keyStatus[KEY_ALT_LEFT] || keyStatus[KEY_ALT_RIGHT])
    result |= Alt;
  return result;
}

QString Keyboard::getKeySimpleEnName(QString str)
{
  if (str.length() == 1)
    return str;
  if (str.indexOf(QChar(',')) == -1)
    return str;
  str = str.split(",").first();
  if (str.startsWith(QString("NUM_")) == 0)
    return str;
  return str.split("NUM_").at(1);
}

QString Keyboard::getKeyFullZhName(QString str)
{
  if (str.length() == 1)
    return str;
  if (str.indexOf(QChar(',')) == -1)
    return str;
  return str.split(",").at(1);
}
