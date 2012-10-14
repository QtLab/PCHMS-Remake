#include "recorder.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QDir>
#include <QRect>
#include "operationinaday.h"

#define ACTIVE_TRIGGLE_SECONDS 60
#define ACTIVE_MIN_SECONDS 20

Recorder::Recorder() :
    lastKeyTime(-1),
    lastMouseTime(-1),
    statistic(Statistic(Statistic::Day)),
    currentModifiers(Keyboard::NoModifier),
    lastIsCombo(false),
    operationStream(NULL)
{
  QDate day = QDate::currentDate();
  statistic.init = true;
  statistic.startDate = day;
  statistic.endDate = day;
  for (int i = 0;i < Keyboard::TOTAL_KEY_NUMBER;++i)
    keyStatus[i] = false;
}

void Recorder::addOperation(QString program, int ms, Keyboard::KeyOperation operation)
{
  // Ignore the repeat keys
  if (keyStatus[operation.key] && operation.operation == Keyboard::Down)
    return;

  // Record the operation
  if (operationStream != NULL)
  {
    recordBasic(program, ms);
    char bytes[2];
    bytes[0] = (char) (operation.operation & 0x1F);
    bytes[1] = (char) operation.key;
    operationStream->writeRawData(bytes, 2);
  }

  int second = ms / 1000;

  // Whether it's a combo key
  bool combo = false;
  Keyboard::ComboKey comboKey;
  if (currentModifiers != Keyboard::NoModifier)
  {
    if (Keyboard::isModifier(operation.key))
    {
      if ((!lastIsCombo) &&
          operation.operation == Keyboard::Up &&
          (currentModifiers & ~Keyboard::keyToModifiers(operation.key)) != Keyboard::NoModifier)
      {
        combo = true;
        lastIsCombo = true;
      }
    }
    else if (operation.operation == Keyboard::Down)
    {
      combo = true;
      lastIsCombo = true;
    }
  }
  if (operation.operation == Keyboard::Down)
    lastIsCombo = combo;
  if (combo)
  {
    comboKey.modifiers = currentModifiers;
    comboKey.key = operation.key;
  }

  // Global
  {
    // Global time
    int lastActiveTime = qMax(lastKeyTime, lastMouseTime);
    int slot = second / 3600;
    if (lastActiveTime == -1)
    {
      statistic.wholeStatistic.activeTime[slot] = 1;
      statistic.wholeStatistic.globalActiveTime = 1;
    }
    else if (second - lastActiveTime < ACTIVE_TRIGGLE_SECONDS)
    {
      statistic.wholeStatistic.activeTime[slot] += second - lastActiveTime;
      statistic.wholeStatistic.globalActiveTime += second - lastActiveTime;
    }
    else
    {
      statistic.wholeStatistic.activeTime[slot] += ACTIVE_MIN_SECONDS;
      statistic.wholeStatistic.globalActiveTime += ACTIVE_MIN_SECONDS;
    }

    // Keyboard time/times
    if (lastKeyTime == -1)
      statistic.wholeStatistic.keyboardActiveTime = 1;
    else if (second - lastKeyTime < ACTIVE_TRIGGLE_SECONDS)
      statistic.wholeStatistic.keyboardActiveTime += second - lastKeyTime;
    else
      statistic.wholeStatistic.keyboardActiveTime += ACTIVE_MIN_SECONDS;
    lastKeyTime = second;
    int v = statistic.wholeStatistic.keyTimes.value(operation.key, 0);
    statistic.wholeStatistic.keyTimes.insert(operation.key, v + 1);
    if (combo)
    {
      v = statistic.wholeStatistic.comboKeyTimes.value(comboKey, 0);
      statistic.wholeStatistic.comboKeyTimes.insert(comboKey, v + 1);
    }
  }

  // Program
  {
    // Check whether their is a statistic for the program.
    Statistic::StatisticUnit *programStatistic = statistic.programStatistic.value(program, NULL);
    if (programStatistic == NULL)
    {
      programStatistic = new Statistic::StatisticUnit();
      statistic.programStatistic.insert(program, programStatistic);
    }

    // Program time
    int lastActiveTime = qMax(lastKeyTimes.value(program, -1),
                              lastMouseTimes.value(program, -1));
    int slot = second / 3600;
    if (lastActiveTime == -1)
    {
      programStatistic->activeTime[slot] = 1;
      programStatistic->globalActiveTime = 1;
    }
    else if (second - lastActiveTime < ACTIVE_TRIGGLE_SECONDS)
    {
      programStatistic->activeTime[slot] += second - lastActiveTime;
      programStatistic->globalActiveTime += second - lastActiveTime;
    }
    else
    {
      programStatistic->activeTime[slot] += ACTIVE_MIN_SECONDS;
      programStatistic->globalActiveTime += ACTIVE_MIN_SECONDS;
    }

    // Program keyboard time/times
    lastActiveTime = lastKeyTimes.value(program, -1);
    if (lastActiveTime == -1)
      programStatistic->keyboardActiveTime = 1;
    else if (second - lastActiveTime < ACTIVE_TRIGGLE_SECONDS)
      programStatistic->keyboardActiveTime += second - lastActiveTime;
    else
      programStatistic->keyboardActiveTime += ACTIVE_MIN_SECONDS;
    lastKeyTimes.insert(program, second);
    int v = programStatistic->keyTimes.value(operation.key, 0);
    programStatistic->keyTimes.insert(operation.key, v + 1);
    if (combo)
    {
      v = programStatistic->comboKeyTimes.value(comboKey, 0);
      programStatistic->comboKeyTimes.insert(comboKey, v + 1);
    }
  }

  // Maintain key status
  keyStatus[operation.key] = operation.operation == Keyboard::Down;

  // Recalculate the modifier.
  currentModifiers = Keyboard::calculateModifiers(keyStatus);
}

void Recorder::addOperation(QString program, int ms, Mouse::MouseOperation operation)
{
  QRect screenRect = QApplication::desktop()->screenGeometry(operation.pos);
  // Record the operation
  if (operationStream != NULL)
  {
    recordBasic(program, ms);
    char bytes[8];
    bytes[0] = (char) ((operation.operation & 0x1F) | 0x20);
    bytes[1] = (char) operation.button;
    int x = qMax(0, (operation.pos.x() - screenRect.left()) * 1000 / screenRect.width());
    int y = qMax(0, (operation.pos.y() - screenRect.top()) * 1000 / screenRect.height());
    bytes[2] = (char) (x >> 16);
    bytes[3] = (char) (x >> 8);
    bytes[4] = (char) x;
    bytes[5] = (char) (y >> 16);
    bytes[6] = (char) (y >> 8);
    bytes[7] = (char) y;
    operationStream->writeRawData(bytes, 8);
  }

  int second = ms / 1000;

  // Whether it's a combo button
  bool combo = false;
  Mouse::ComboButton comboButton;
  if (currentModifiers != Keyboard::NoModifier)
  {
    combo = true;
    comboButton.modifiers = currentModifiers;
    comboButton.button = operation.button;
  }

  // Global
  {
    // Global time
    int lastActiveTime = qMax(lastKeyTime, lastMouseTime);
    int slot = second / 3600;
    if (lastActiveTime == -1)
    {
      statistic.wholeStatistic.activeTime[slot] = 1;
      statistic.wholeStatistic.globalActiveTime = 1;
    }
    else if (second - lastActiveTime < ACTIVE_TRIGGLE_SECONDS)
    {
      statistic.wholeStatistic.activeTime[slot] += second - lastActiveTime;
      statistic.wholeStatistic.globalActiveTime += second - lastActiveTime;
    }
    else
    {
      statistic.wholeStatistic.activeTime[slot] += ACTIVE_MIN_SECONDS;
      statistic.wholeStatistic.globalActiveTime += ACTIVE_MIN_SECONDS;
    }

    // Mouse time/times
    if (lastMouseTime == -1)
      statistic.wholeStatistic.mouseActiveTime = 1;
    else if (second - lastKeyTime < ACTIVE_TRIGGLE_SECONDS)
      statistic.wholeStatistic.mouseActiveTime += second - lastMouseTime;
    else
      statistic.wholeStatistic.mouseActiveTime += ACTIVE_MIN_SECONDS;
    lastMouseTime = second;
    int v = statistic.wholeStatistic.buttonTimes.value(operation.button, 0);
    statistic.wholeStatistic.buttonTimes.insert(operation.button, v + 1);
    if (combo)
    {
      v = statistic.wholeStatistic.comboButtonTimes.value(comboButton, 0);
      statistic.wholeStatistic.comboButtonTimes.insert(comboButton, v + 1);
    }
  }

  // Program
  {
    // Check whether their is a statistic for the program.
    Statistic::StatisticUnit *programStatistic = statistic.programStatistic.value(program, NULL);
    if (programStatistic == NULL)
    {
      programStatistic = new Statistic::StatisticUnit();
      statistic.programStatistic.insert(program, programStatistic);
    }

    // Program time
    int lastActiveTime = qMax(lastKeyTimes.value(program, -1),
                              lastMouseTimes.value(program, -1));
    int slot = second / 3600;
    if (lastActiveTime == -1)
    {
      programStatistic->activeTime[slot] = 1;
      programStatistic->globalActiveTime = 1;
    }
    else if (second - lastActiveTime < ACTIVE_TRIGGLE_SECONDS)
    {
      programStatistic->activeTime[slot] += second - lastActiveTime;
      programStatistic->globalActiveTime += second - lastActiveTime;
    }
    else
    {
      programStatistic->activeTime[slot] += ACTIVE_MIN_SECONDS;
      programStatistic->globalActiveTime += ACTIVE_MIN_SECONDS;
    }

    // Program keyboard time/times
    lastActiveTime = lastMouseTimes.value(program, -1);
    if (lastActiveTime == -1)
      programStatistic->mouseActiveTime = 1;
    else if (second - lastActiveTime < ACTIVE_TRIGGLE_SECONDS)
      programStatistic->mouseActiveTime += second - lastActiveTime;
    else
      programStatistic->mouseActiveTime += ACTIVE_MIN_SECONDS;
    lastMouseTimes.insert(program, second);
    int v = programStatistic->buttonTimes.value(operation.button, 0);
    programStatistic->buttonTimes.insert(operation.button, v + 1);
    if (combo)
    {
      v = programStatistic->comboButtonTimes.value(comboButton, 0);
      programStatistic->comboButtonTimes.insert(comboButton, v + 1);
    }
  }

  // Recalculate the modifier.
  currentModifiers = Keyboard::calculateModifiers(keyStatus);
}

bool Recorder::record() const
{
  if (operationStream != NULL)
    file->flush();
  QString path = QObject::tr("data/%1s").arg(statistic.startDate.toString("yyyyMMdd"));
  return statistic.record(path);
}

void Recorder::init()
{
  lastKeyTime = -1;
  lastMouseTime = -1;
  lastKeyTimes.clear();
  lastMouseTimes.clear();

  statistic.clear();
  statistic.init = true;
  QString path = QObject::tr("data/%1s").arg(QDate::currentDate().toString("yyyyMMdd"));
  if (!statistic.read(path))
  {
    statistic.clear();
    statistic.init = true;
  }
  path = QObject::tr("data/%1d").arg(QDate::currentDate().toString("yyyyMMdd"));
  file = new QFile(path);
  if (file->open(QIODevice::WriteOnly | QIODevice::Append))
    operationStream = new QDataStream(file);
  else
  {
    operationStream = NULL;
    delete file;
  }
}

void Recorder::newDay()
{
  record();
  lastKeyTime = -1;
  lastMouseTime = -1;
  lastKeyTimes.clear();
  lastMouseTimes.clear();

  statistic = Statistic(Statistic::Day);
  QDate day = QDate::currentDate();
  statistic.init = true;
  statistic.startDate = day;
  statistic.endDate = day;

  QString path = QObject::tr("data/%1d").arg(QDate::currentDate().toString("yyyyMMdd"));
  if (file != NULL)
  {
    delete file;
    delete operationStream;
  }
  file = new QFile(path);
  if (file->open(QIODevice::WriteOnly | QIODevice::Append))
    operationStream = new QDataStream(file);
  else
  {
    operationStream = NULL;
    delete file;
  }
}

void Recorder::recordBasic(QString program, int ms)
{
  char bytes[5];
  bytes[0] = (char) (ms >> 24);
  bytes[1] = (char) (ms >> 16);
  bytes[2] = (char) (ms >> 8);
  bytes[3] = (char) ms;
  QByteArray name = program.toLocal8Bit();
  bytes[4] = (char) name.size();
  operationStream->writeRawData(bytes, 5);
  operationStream->writeRawData(name.data(), name.size());
  bytes[0] = (char) currentModifiers;
  operationStream->writeRawData(bytes, 1);
}

OperationInADay *Recorder::readOperationsInADay(QDate date)
{
  QString path = QObject::tr("data/%1d").arg(date.toString("yyyyMMdd"));
  QFile f(path);
  OperationInADay *result = new OperationInADay();
  if (!f.open(QIODevice::ReadOnly))
    return result;
  QDataStream in(&f);
  int ms;
  QString program;
  Keyboard::KeyboardModifiers modifiers;
  char name[128];
  unsigned char tmp[6];
  int i = 0;
  while (!in.atEnd())
  {
    in.readRawData((char *)tmp, 5);
    ms = (((int) tmp[0]) << 24) |
         (((int) tmp[1]) << 16) |
         (((int) tmp[2]) << 8) |
         ((int) tmp[3]);
    int programSize = (int) tmp[4];
    in.readRawData(name, programSize);
    program = QString::fromLocal8Bit(name, programSize);
    in.readRawData((char *)tmp, 3);
    modifiers = (Keyboard::KeyboardModifiers) tmp[0];
    result->counts.insert(program, result->counts.value(program, 0) + 1);
    if ((tmp[1] & 0xE0) == 0x00)
    {
      // Keyboard
      Keyboard::KeyOperation rawKeyOperation;
      rawKeyOperation.operation = (Keyboard::Operation) (tmp[1] & 0x1F);
      rawKeyOperation.key = (Keyboard::Key) tmp[2];
      OperationInADay::KeyOperation keyOperation;
      keyOperation.operation = rawKeyOperation;
      keyOperation.ms = ms;
      keyOperation.program = program;
      keyOperation.modifiers = modifiers;
      keyOperation.id = i;
      result->keyOperations.append(keyOperation);
    }
    else if ((tmp[1] & 0xE0) == 0x20)
    {
      // Mouse
      Mouse::MouseOperation rawMouseOperation;
      rawMouseOperation.operation = (Mouse::Operation) (tmp[1] & 0x1F);
      rawMouseOperation.button = (Mouse::Button) tmp[2];
      in.readRawData((char *)tmp, 6);
      rawMouseOperation.pos.setX((((int) tmp[0]) << 16) | (((int) tmp[1]) << 8) | (int) tmp[2]);
      rawMouseOperation.pos.setY((((int) tmp[3]) << 16) | (((int) tmp[4]) << 8) | (int) tmp[5]);
      OperationInADay::MouseOperation mouseOperation;
      mouseOperation.operation = rawMouseOperation;
      mouseOperation.ms = ms;
      mouseOperation.program = program;
      mouseOperation.modifiers = modifiers;
      mouseOperation.id = i;
      result->mouseOperations.append(mouseOperation);
    }
    ++i;
  }
  return result;
}

Statistic *Recorder::readAllStatistic()
{
  Statistic *result = new Statistic(Statistic::All);
  {
    QString pattern("[0-9]{8}s");
    QRegExp rx(pattern);
    QDir dir("data");
    QStringList filter;
    filter<< "*s";
    dir.setNameFilters(filter);
    QList<QFileInfo> fileInfos = dir.entryInfoList(filter, QDir::Files, QDir::Name);
    QFileInfo fileInfo;
    foreach (fileInfo, fileInfos)
    {
      if (rx.exactMatch(fileInfo.fileName()))
      {
        Statistic *tmp = new Statistic(Statistic::Day);
        tmp->read(QString("data/") + fileInfo.fileName());
        result->addADay(tmp);
      }
    }
  }
  return result;
}
