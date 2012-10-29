#ifndef WIN32SYSTEMCALL_H
#define WIN32SYSTEMCALL_H

#ifdef WIN32

#include <QString>

#include <Windows.h>
#include <tlhelp32.h>

/**
 * @brief Get the name of a process.
 *
 * @param pid The progress id.
 */
QString getProcessNameByPid(DWORD pid)
{
  QString name;
  HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, pid);
  if (INVALID_HANDLE_VALUE != hSnapShot)
  {
    PROCESSENTRY32 processInfo;
    processInfo.dwSize = sizeof(PROCESSENTRY32);
    if (Process32First(hSnapShot, &processInfo))
    {
      while(Process32Next(hSnapShot, &processInfo) != FALSE)
      {
        qint64 currentPid = processInfo.th32ProcessID;
        if (currentPid == pid)
          name = QString::fromWCharArray(processInfo.szExeFile);
      }
    }
  }
  CloseHandle(hSnapShot);
  return name;
}

static DEVMODE devModeBackup;

/**
 * @brief Record the dev mode.
 */
void recordDevMode()
{
  EnumDisplaySettings(NULL, -1, &devModeBackup);
}

/**
 * @brief Resize the screen.
 *
 * @param width The width.
 * @param width The height.
 */
void resize(DWORD width, DWORD height)
{
  DEVMODE devMode;
  EnumDisplaySettings(NULL, -1, &devMode);
  devMode.dmPelsWidth = width;
  devMode.dmPelsHeight = height;
  ChangeDisplaySettings(&devMode, CDS_UPDATEREGISTRY);
}

/**
 * @brief Restore the dev mode.
 */
void restoreDevMode()
{
  ChangeDisplaySettings(&devModeBackup, CDS_UPDATEREGISTRY);
}

#endif // WIN32

#endif // WIN32SYSTEMCALL_H
