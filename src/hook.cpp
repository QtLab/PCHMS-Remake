#include "hook.h"

#include "win32systemcall.h"

Hook *Hook::hook = NULL;
bool Hook::installed = false;

Hook *Hook::getInstance()
{
  if (hook == NULL)
    hook = new Hook();
  return hook;
}

#ifdef WIN32
#include <Windows.h>
#include "win32systemcall.h"
#include "keyboard.h"

HHOOK keyHook = NULL;
HHOOK mouseHook = NULL;

// Keyboard hook
LRESULT CALLBACK keyProc(int nCode, WPARAM wParam, LPARAM lParam)
{
  // Get the keyboard hook struct
  KBDLLHOOKSTRUCT *hs = (KBDLLHOOKSTRUCT *) lParam;

  // Do not process it if it has extra info
  if (hs->dwExtraInfo != 0)
    return 0;

  bool block = false;
  HWND w = GetForegroundWindow();
  DWORD pid;
  GetWindowThreadProcessId(w, &pid);
  QString programName = getProcessNameByPid(pid);
  if ((wParam & 1) == 0)
    Hook::getInstance()->pressKey(programName, Keyboard::vkToKey(hs->vkCode, lParam), &block);
  else
    Hook::getInstance()->releaseKey(programName, Keyboard::vkToKey(hs->vkCode, lParam), &block);
  return block ? 1 : 0;
}

#include <QTime>

LRESULT CALLBACK mouseProc(int nCode,WPARAM wParam,LPARAM lParam )
{
  bool block = false;
  MSLLHOOKSTRUCT *hs = (MSLLHOOKSTRUCT *) lParam;
  // Ignore the move event
  if (wParam == 512)
    return 0;
  HWND w = GetForegroundWindow();
  DWORD pid;
  GetWindowThreadProcessId(w, &pid);
  QString programName = getProcessNameByPid(pid);
  /*
  513:main down
  514:main up
  516:sub down
  517:sub up
  519:mid down
  520:mid up
  522:mid roll
      ((int)wParam)>0:up
      ((int)wParam)<0:down
  */
  switch (wParam)
  {
  case 513:
    // Main down
    Hook::getInstance()->pressMouse(programName,
                                    QPoint(hs->pt.x, hs->pt.y),
                                    Mouse::MainKey,
                                    &block);
    break;
  case 514:
    // Main up
    Hook::getInstance()->releaseMouse(programName,
                                      QPoint(hs->pt.x, hs->pt.y),
                                      Mouse::MainKey,
                                      &block);
    break;
  case 516:
    // Sub down
    Hook::getInstance()->pressMouse(programName,
                                    QPoint(hs->pt.x, hs->pt.y),
                                    Mouse::SubKey,
                                    &block);
    break;
  case 517:
    // Sub up
    Hook::getInstance()->releaseMouse(programName,
                                      QPoint(hs->pt.x, hs->pt.y),
                                      Mouse::SubKey,
                                      &block);
    break;
  case 519:
    // Mid down
    Hook::getInstance()->pressMouse(programName,
                                    QPoint(hs->pt.x, hs->pt.y),
                                    Mouse::MiddleKey,
                                    &block);
    break;
  case 520:
    // Mid up
    Hook::getInstance()->releaseMouse(programName,
                                      QPoint(hs->pt.x, hs->pt.y),
                                      Mouse::MiddleKey,
                                      &block);
    break;
  case 522:
    if ((int)hs->mouseData > 0)
      // Up
      Hook::getInstance()->pressMouse(programName,
                                      QPoint(hs->pt.x, hs->pt.y),
                                      Mouse::RollUp,
                                      &block);
    else
      // Down
      Hook::getInstance()->pressMouse(programName,
                                      QPoint(hs->pt.x, hs->pt.y),
                                      Mouse::RollDown,
                                      &block);
    break;
  }
  return block ? 1 : 0;
}

void Hook::install()
{
  if (!installed)
  {
    keyHook = SetWindowsHookEx(WH_KEYBOARD_LL, keyProc, GetModuleHandle(NULL), 0);
    mouseHook = SetWindowsHookEx(WH_MOUSE_LL, mouseProc, GetModuleHandle(NULL), 0);
    installed = true;
  }
}

void Hook::uninstall()
{
  if (installed)
  {
    UnhookWindowsHookEx(keyHook);
    UnhookWindowsHookEx(mouseHook);
    installed = false;
  }
}

#endif // WIN32
