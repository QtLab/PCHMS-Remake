#include "suggestion.h"

// Here are some magic numbers.
// Most of them are set according to my feeling
// If you have a more proper way to give user suggestion, please tell me.
// Thanks in advance.

void Suggestion::activePeriodSuggestion(const int values[24],
                                        QVector<QString>& suggestion,
                                        QVector<QString>& detail)
{
  suggestion.clear();
  detail.clear();

  // Get the max active value
  int max = 0;
  for (int i = 0;i < 24;++i)
    max = qMax(max, values[i]);

  // Sleep time
  {
    bool maySleep[24];
    // Calculate the time a person be in a sleep state.
    {
      memset(maySleep, 0, sizeof(bool) * 24);
      for (int i = 0;i < 24;++i)
      {
        // The magic number may be tested
        maySleep[i] = values[i] <= max / 20;
      }
    }
    // Contineous sleep time
    {
      // Calculate the max contineous hours a person may sleep
      int maxContineousHour = 0;
      {
        int currentContineousHour = 0;
        // 2 rounds because its normal to sleep from one day night to another day morning.
        for (int i = 0;i < 24 * 2;++i)
        {
          currentContineousHour = maySleep[i % 24] ?
                                  currentContineousHour + 1 :
                                  0;
          maxContineousHour = qMax(maxContineousHour, currentContineousHour);
        }
      }
      // 7 and 9 because one should have other things to do besides using computer and sleep.
      // Please also be aware that one may not use computer in a fixed period of time.
      if (maxContineousHour <= 7)
      {
        suggestion.push_back(QString::fromUtf8("极度缺少睡眠或者生活不规律！"));
        detail.push_back(QString::fromUtf8("连续的不活跃时间极短\n"
                                           "可能极度缺少睡眠\n"
                                           "也可能生活不规律，睡眠时间段总是变化\n"
                                           "好好睡觉哦"));
      }
      else if (maxContineousHour <= 9)
      {
        suggestion.push_back(QString::fromUtf8("比较缺少睡眠或者使用电脑时间不规律"));
        detail.push_back(QString::fromUtf8("连续的不活跃时间较短\n"
                                           "可能略微缺少睡眠\n"
                                           "也可能生活不规律，睡眠时间段经常变化\n"
                                           "好好睡觉哦"));
      }
    }
    // Longest contineous period between 9pm to 6am
    int startHour = -1;
    int maxContineousHour = 0;
    {
      int currentContineousHour = 0;
      // 2 rounds because its normal to sleep from one day night to another day morning.
      for (int i = 30;i >= 20;--i)
      {
        currentContineousHour = maySleep[i % 24] ?
                                currentContineousHour + 1 :
                                0;
        if (currentContineousHour >= maxContineousHour)
        {
          maxContineousHour = currentContineousHour;
          startHour = i % 24;
        }
      }
    }
    if (startHour == -1 || maxContineousHour <= 3)
    {
      suggestion.push_back(QString::fromUtf8("常常熬夜"));
      detail.push_back(QString::fromUtf8("晚上9点至次日6点之间没有足够的时间用于睡眠\n"
                                         "好好睡觉哦"));
    }
    else if (startHour > 22 || startHour < 1)
    {
      suggestion.push_back(QString::fromUtf8("晚上10点过后才能睡"));
      detail.push_back(QString::fromUtf8("最早的可能睡下的时间在晚上10点以后\n"
                                         "好好睡觉哦"));
    }
    else if (startHour < 3)
    {
      suggestion.push_back(QString::fromUtf8("次日凌晨才睡"));
      detail.push_back(QString::fromUtf8("最早的可能睡下的时间在次日凌晨\n"
                                         "好好睡觉哦"));
    }

  }
  // Rest time
  {
    bool mayActive[24];
    // Calculate the time a person be quite active
    {
      memset(mayActive, 0, sizeof(bool) * 24);
      for (int i = 0;i < 24;++i)
      {
        // The magic number may be tested
        mayActive[i] = values[i] >= max / 2;
      }
    }
    // Contineous active time
    int maxContineousHour = 0;
    // Calculate the max contineous hours a person may be quite active
    {
      {
        int currentContineousHour = 0;
        for (int i = 0;i < 24;++i)
        {
          currentContineousHour = mayActive[i] ?
                                  currentContineousHour + 1 :
                                  0;
          maxContineousHour = qMax(maxContineousHour, currentContineousHour);
        }
      }
    }
    // In fact, 1 or 2 may be proper for children
    if (maxContineousHour >= 7)
    {
      suggestion.push_back(QString::fromUtf8("连续活跃时间严重过长！"));
      detail.push_back(QString::fromUtf8("连续的活跃时间极长\n"
                                         "可能是使用电脑过程中没有或缺少休息\n"
                                         "久坐不利于健康哦，每连续用上1、2小时电脑后，活动活动吧"));
    }
    else if (maxContineousHour >= 4)
    {
      suggestion.push_back(QString::fromUtf8("连续活跃时间较长"));
      detail.push_back(QString::fromUtf8("连续的活跃时间过长\n"
                                         "可能是使用电脑过程中没有或缺少休息\n"
                                         "久坐不利于健康哦，每连续用上1、2小时电脑后，活动活动吧"));
    }
  }
}

void Suggestion::getDeviceTrend(QString nameA,
                                QString nameB,
                                int valueA,
                                int valueB,
                                QVector<QString>& suggestion,
                                QVector<QString>& detail)
{
  suggestion.clear();
  detail.clear();

  double tmp = 1.0 * valueA / valueB;
  bool preferA = tmp >= 1.0;
  if (!preferA)
    tmp = 1.0 / tmp;
  QString prefer = preferA ? nameA : nameB;
  if (tmp < 1.2)
  {
    suggestion.push_back(QObject::tr("%1%2%3").
                         arg(nameA).
                         arg(nameB).
                         arg(QString::fromUtf8("使用比例相当")));
    detail.push_back("");
  }
  else if (tmp < 1.8)
  {
    suggestion.push_back(QObject::tr("%1%2%3").
                         arg(QString::fromUtf8("使用")).
                         arg(prefer).
                         arg(QString::fromUtf8("较多")));
    detail.push_back("");
  }
  else if (tmp < 4.1)
  {
    suggestion.push_back(QObject::tr("%1%2%3").
                         arg(QString::fromUtf8("使用")).
                         arg(prefer).
                         arg(QString::fromUtf8("很多")));
    detail.push_back("");
  }
  else
  {
    suggestion.push_back(QObject::tr("%1%2").
                         arg(QString::fromUtf8("极倾向于使用")).
                         arg(prefer));
    detail.push_back("");
  }
}

void Suggestion::getBusyTrend(int operation,
                              int time,
                              QString& suggestion,
                              QString& detail)
{
  suggestion = "";
  detail = "";

  if (operation == 0 || time == 0)
    return;
  double tmp = 1.0 * operation / time;

  if (tmp < 1.5)
  {
    suggestion = "";
    detail = "";
  }
  else if (tmp < 2.0)
  {
    suggestion = QString::fromUtf8("比较忙");
    detail = QString::fromUtf8("单位时间内的操作数较多\n"
                               "鼠标键盘的使用强度不宜过高，易患手部疾病\n"
                               "如果进一步增加操作强度，可能影响健康哦");
  }
  else
  {
    suggestion = QString::fromUtf8("很忙");
    detail = QString::fromUtf8("单位时间内的操作数过多\n"
                               "鼠标键盘的使用强度不宜过高，易患手部疾病\n"
                               "闲暇时多做些手部的放松性活动吧");
  }
}
