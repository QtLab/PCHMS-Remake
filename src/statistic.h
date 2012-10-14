#ifndef STATISTIC_H
#define STATISTIC_H

#include <QDate>
#include <QMap>
#include <QTextStream>
#include <QVector>
#include "keyboard.h"
#include "mouse.h"

class Recorder;

/**
 * @brief Class of statistic.
 *
 * Class of statistic.
 * There are 4 levels of statistic: all, year, month, day.
 */
class Statistic
{
  friend class Recorder;
public:
  /**
   * Level of the statistic.
   */
  enum StatisticLevel{All=0, Year=1, Month=2, Day=3};

  static const int SCREEN_SIZE = 1000;

  static const QDate BASE_DATE;

  /**
   * @brief Struct of a unit of the statistic.
   *
   * Struct of a unit of the statistic.
   * Record the times each key/button pressed.
   */
  struct StatisticUnit
  {
    /**
     * The time in seconds global is active.
     */
    int globalActiveTime;

    /**
     * The time in seconds keyboard is active.
     */
    int keyboardActiveTime;

    /**
     * The time in seconds mouse is active.
     */
    int mouseActiveTime;

    /**
     * The times of combo keys.
     */
    QMap<Keyboard::ComboKey, int> comboKeyTimes;

    /**
     * The times of keys.
     */
    QMap<Keyboard::Key, int> keyTimes;

    /**
     * The times of combo buttons.
     */
    QMap<Mouse::ComboButton, int> comboButtonTimes;

    /**
     * The times of buttons.
     */
    QMap<Mouse::Button, int> buttonTimes;

    /**
     * The active time in the 24 hours.
     */
    int activeTime[24];

    StatisticUnit();

    /**
     * @brief Combine another statistic unit with this one.
     * @param another Another unit.
     */
    void combine(const StatisticUnit *another);

    /**
     * @brief Clear the data.
     */
    void clear();
  };

  Statistic(StatisticLevel level);

  /**
   * @brief Add a statistic of a day.
   *
   * Add a statistic of a day.
   * The day must be after the last day already in the statistic.
   *
   * @param statistic The statistic of the day.
   * @return Whether the operation is successful.
   */
  bool addADay(Statistic *statistic);

  /**
   * @brief Record the statistic to the file.
   *
   * Record the statistic to the file.
   * Only records the record for a day.
   * Won't record the sub statistic.
   *
   * @param path The path of the file.
   * @return Whether the statistic is recorded successfully.
   */
  bool record(QString path) const;

  /**
   * @brief Read the statistic from a file.
   *
   * @param path The path of the file.
   * @return Whether the statistic is readed successfully.
   */
  bool read(QString path);

  /**
   * @brief Clear the records.
   */
  void clear();

  /**
   * @brief Get the statistic of the programs.
   */
  inline const QMap<QString, StatisticUnit *>& getProgramStatistic() const
  {return programStatistic;}

  /**
   * @brief Get the sub statistics.
   */
  inline const QVector<Statistic *>& getSubStatistics() const
  {return subStatistics;}

  /**
   * @brief Get the level of the statistic.
   */
  inline StatisticLevel getLevel() const
  {return level;}

  /**
   * @brief Get the start date.
   */
  inline QDate getStartDate() const
  {return startDate;}

  /**
   * @brief Get the end date.
   */
  inline QDate getEndDate() const
  {return endDate;}

  /**
   * @brief Get the whole statistic.
   */
  inline const StatisticUnit *getWholeStatistic() const
  {return &wholeStatistic;}

private:
  /**
   * Level of this statistic.
   */
  StatisticLevel level;

  /**
   * Start date of this level.
   */
  QDate startDate;

  /**
   * Start date of this level.
   */
  QDate endDate;

  /**
   * Whole tatistic.
   */
  StatisticUnit wholeStatistic;

  /**
   * Statistic of programs.
   */
  QMap<QString, StatisticUnit *> programStatistic;

  /**
   * Sub statistic.
   */
  QVector<Statistic *> subStatistics;

  /**
   * Whether the statistic has been inited.
   */
  bool init;

  /**
   * @brief Record a unit to the output stream.
   *
   * @param out The output stream.
   * @param unit The unit.
   */
  static void recordUnit(QTextStream& out, const StatisticUnit *unit);

  /**
   * @brief Read a unit from a file.
   *
   * @param in The input stream.
   * @param unit The unit.
   * @return Whether the statistic is readed successfully.
   */
  static bool readUnit(QTextStream& in, StatisticUnit *unit);
};

#endif // STATISTIC_H
