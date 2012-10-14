#include "mouse.h"

const static char *buttonNames[] = {"主键",
                                    "副键",
                                    "中键",
                                    "向上滚轮",
                                    "向下滚轮",
                                    "未知"};

QString Mouse::buttonToString(Mouse::Button button)
{
  if (button >= 0 && button < TOTAL_BUTTON_NUMBER)
    return QString::fromUtf8(buttonNames[button]);
  return QString::fromUtf8(buttonNames[TOTAL_BUTTON_NUMBER]);
}
