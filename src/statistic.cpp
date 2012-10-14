#include "statistic.h"

#include <QFile>

const QDate Statistic::BASE_DATE = QDate(1970, 1, 1);

Statistic::StatisticUnit::StatisticUnit() :
    globalActiveTime(0),
    keyboardActiveTime(0),
    mouseActiveTime(0)
{
  memset(activeTime, 0, sizeof(int) * 24);
}

void Statistic::StatisticUnit::combine(const StatisticUnit *another)
{
  globalActiveTime += another->globalActiveTime;
  keyboardActiveTime += another->keyboardActiveTime;
  mouseActiveTime += another->mouseActiveTime;
  for (QMap<Keyboard::ComboKey, int>::ConstIterator itr = another->comboKeyTimes.begin();
       itr != another->comboKeyTimes.end();
       ++itr)
  {
    int v = comboKeyTimes.value(itr.key(), 0);
    comboKeyTimes.insert(itr.key(), v + itr.value());
  }
  for (QMap<Keyboard::Key, int>::ConstIterator itr = another->keyTimes.begin();
       itr != another->keyTimes.end();
       ++itr)
  {
    int v = keyTimes.value(itr.key(), 0);
    keyTimes.insert(itr.key(), v + itr.value());
  }
  for (QMap<Mouse::ComboButton, int>::ConstIterator itr = another->comboButtonTimes.begin();
       itr != another->comboButtonTimes.end();
       ++itr)
  {
    int v = comboButtonTimes.value(itr.key(), 0);
    comboButtonTimes.insert(itr.key(), v + itr.value());
  }
  for (QMap<Mouse::Button, int>::ConstIterator itr = another->buttonTimes.begin();
       itr != another->buttonTimes.end();
       ++itr)
  {
    int v = buttonTimes.value(itr.key(), 0);
    buttonTimes.insert(itr.key(), v + itr.value());
  }
  for (int i = 0;i < 24;++i)
    activeTime[i] += another->activeTime[i];
}

void Statistic::StatisticUnit::clear()
{
  keyboardActiveTime = 0;
  mouseActiveTime = 0;
  comboKeyTimes.clear();
  keyTimes.clear();
  comboButtonTimes.clear();
  buttonTimes.clear();
  memset(activeTime, 0, sizeof(int) * 24);
}

Statistic::Statistic(StatisticLevel l) :
    level(l),
    init(false)
{
}

bool Statistic::addADay(Statistic *statistic)
{
  // Error when the statistic isn't for a day.
  if (statistic->level != Day)
    return false;

  // Error when the order of the date isn't correct.
  if (init && endDate.daysTo(statistic->startDate) <= 0)
    return false;

  if (level != Month)
  {
    Statistic *subStatistic = NULL;
    // The last sub statistic
    Statistic *tmp = subStatistics.isEmpty() ? NULL : subStatistics.last();
    if (tmp != NULL)
    {
      // The last sub statistic contains the day
      if (((tmp->level == Statistic::Year) &&
           (tmp->startDate.year() == statistic->startDate.year())) ||
          ((tmp->level == Statistic::Month) &&
           (tmp->startDate.year() == statistic->startDate.year()) &&
           (tmp->startDate.month() == statistic->startDate.month())))
        subStatistic = tmp;
    }
    if (subStatistic == NULL)
    {
      tmp = NULL;
      subStatistic = new Statistic((StatisticLevel)(level + 1));
    }
    // Add levels
    if (!subStatistic->addADay(statistic))
      return false;
    if (tmp == NULL)
      subStatistics.push_back(subStatistic);
  }
  else
  {
    subStatistics.push_back(statistic);
  }

  // Init if neccessary.
  if (!init)
  {
    startDate = statistic->startDate;
    init = true;
  }

  // Add the statistic of programs
  for (QMap<QString, StatisticUnit *>::ConstIterator itr = statistic->programStatistic.begin();
       itr != statistic->programStatistic.end();
       ++itr)
  {
    QString name = itr.key();
    StatisticUnit *statisticUnit = itr.value();
    StatisticUnit *oldStatisticUnit = programStatistic.value(name, NULL);
    if (oldStatisticUnit == NULL)
    {
      // Add the statistic of this program
      oldStatisticUnit = new StatisticUnit();
      programStatistic.insert(name, oldStatisticUnit);
    }
    // Combine the statistic
    oldStatisticUnit->combine(statisticUnit);
    wholeStatistic.combine(statisticUnit);
  }

  endDate = statistic->startDate;
  return true;
}

bool Statistic::record(QString path) const
{
  // Don't record the empty statistic.
  // Only record the statistic for a day.
  if ((!init) || level != Day)
    return false;

  QFile file(path);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    return false;
  QTextStream out(&file);
  // The level, start date and end date
  out << level << ' ' << BASE_DATE.daysTo(startDate) << ' ' << BASE_DATE.daysTo(endDate) << '\n';

  // The whole statistic
  recordUnit(out, &wholeStatistic);

  // The statistic for programs
  out << programStatistic.size() << '\n';
  for (QMap<QString, StatisticUnit *>::ConstIterator itr = programStatistic.begin();
       itr != programStatistic.end();
       ++itr)
  {
    out << itr.key() << '\n';
    recordUnit(out, itr.value());
  }

  file.close();
  return true;
}

void Statistic::recordUnit(QTextStream& out, const StatisticUnit *unit)
{
  // The keyboard time and mouse time
  out << unit->globalActiveTime << ' '
      << unit->keyboardActiveTime << ' '
      << unit->mouseActiveTime << '\n';

  // The key times
  out << unit->keyTimes.size() << '\n';
  for (QMap<Keyboard::Key, int>::ConstIterator itr = unit->keyTimes.begin();
       itr != unit->keyTimes.end();
       ++itr)
    out << itr.key() << ' ' << itr.value() << '\n';

  // The combo key times
  out << unit->comboKeyTimes.size() << '\n';
  for (QMap<Keyboard::ComboKey, int>::ConstIterator itr = unit->comboKeyTimes.begin();
       itr != unit->comboKeyTimes.end();
       ++itr)
    out << itr.key().modifiers << ' ' << itr.key().key << ' ' << itr.value() << '\n';

  // The button times
  out << unit->buttonTimes.size() << '\n';
  for (QMap<Mouse::Button, int>::ConstIterator itr = unit->buttonTimes.begin();
       itr != unit->buttonTimes.end();
       ++itr)
    out << itr.key() << ' ' << itr.value() << '\n';

  // The combo button times
  out << unit->comboButtonTimes.size() << '\n';
  for (QMap<Mouse::ComboButton, int>::ConstIterator itr = unit->comboButtonTimes.begin();
       itr != unit->comboButtonTimes.end();
       ++itr)
    out << itr.key().modifiers << ' ' << itr.key().button << ' ' << itr.value() << '\n';

  // The statistic of active time in the 24 hours
  for (int i = 0;i < 24;++i)
    out << unit->activeTime[i] << ' ';
  out << '\n';
}

bool Statistic::read(QString path)
{
  int tmp;

  clear();

  QFile file(path);
  if (!file.open(QIODevice::ReadOnly))
    return false;
  QTextStream in(&file);

  // The level, start date and end date
  in >> tmp;
  level = (StatisticLevel) tmp;
  int days;
  in >> days;
  startDate = BASE_DATE.addDays(days);
  in >> days;
  endDate = BASE_DATE.addDays(days);

  // The whole statistic
  if (!readUnit(in, &wholeStatistic))
    return false;

  // The statistic for programs
  int size = 0;
  in >> size;
  for (int i = 0;i < size;++i)
  {
    QString name;
    in >> name;
    StatisticUnit *unit = new StatisticUnit();
    programStatistic.insert(name, unit);
    if (!readUnit(in, unit))
      return false;
  }

  init = true;
  file.close();
  return true;
}

bool Statistic::readUnit(QTextStream& in, StatisticUnit *unit)
{
  // The keyboard time and mouse time
  in >> unit->globalActiveTime >> unit->keyboardActiveTime >> unit->mouseActiveTime;

  // The key times
  int size = 0;
  in >> size;
  for (int i = 0;i < size;++i)
  {
    int tmp;
    Keyboard::Key key;
    int count;
    in >> tmp >> count;
    key = (Keyboard::Key) tmp;
    unit->keyTimes.insert(key, count);
  }

  // The combo key times
  in >> size;
  for (int i = 0;i < size;++i)
  {
    int tmp1, tmp2;
    Keyboard::ComboKey key;
    int count;
    in >> tmp1 >> tmp2 >> count;
    key.modifiers = (Keyboard::KeyboardModifiers) tmp1;
    key.key = (Keyboard::Key) tmp2;
    unit->comboKeyTimes.insert(key, count);
  }

  // The button times
  size = 0;
  in >> size;
  for (int i = 0;i < size;++i)
  {
    int tmp;
    Mouse::Button button;
    int count;
    in >> tmp >> count;
    button = (Mouse::Button) tmp;
    unit->buttonTimes.insert(button, count);
  }

  // The combo button times
  in >> size;
  for (int i = 0;i < size;++i)
  {
    int tmp1, tmp2;
    Mouse::ComboButton key;
    int count;
    in >> tmp1 >> tmp2 >> count;
    key.modifiers = (Keyboard::KeyboardModifiers) tmp1;
    key.button = (Mouse::Button) tmp2;
    unit->comboButtonTimes.insert(key, count);
  }

  // The statistic of active time in the 24 hours
  for (int i = 0;i < 24;++i)
    in >> unit->activeTime[i];

  return true;
}

void Statistic::clear()
{
  // The whole statistic
  wholeStatistic.clear();

  // The statistic for programs
  for (QMap<QString, StatisticUnit *>::Iterator itr = programStatistic.begin();
       itr != programStatistic.end();
       ++itr)
  {
    delete itr.value();
  }
  programStatistic.clear();

  // Sub statistic.
  Statistic *statistic;
  foreach (statistic, subStatistics)
  {
    delete statistic;
  }
  subStatistics.clear();

  init = false;
}
