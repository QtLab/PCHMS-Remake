#ifndef WIN32SYSTEMCALL_H
#define WIN32SYSTEMCALL_H

#ifdef WIN32

#include <QString>

#include <Windows.h>
#include <tlhelp32.h>

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

#endif // WIN32

#endif // WIN32SYSTEMCALL_H
