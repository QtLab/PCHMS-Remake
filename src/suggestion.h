#ifndef SUGGESTION_H
#define SUGGESTION_H

#include <QObject>
#include <QString>
#include <QVector>

/**
 * @brief Namespace to calculate all the suggestions.
 */
namespace Suggestion
{
  /**
   * @brief Append old suggestion with a new one.
   */
  inline void appendSuggestion(QString& oldOne, const QString& newOne)
  {
    if (oldOne.isEmpty())
      oldOne = newOne;
    else
      oldOne = QObject::tr("%1\n%2").arg(oldOne).arg(newOne);
  }

  /**
   * @brief Get tooltip according to suggestions.
   */
  inline QString getTooltip(const QVector<QString>& suggestions)
  {
    QString result;
    QString tmp;
    foreach (tmp, suggestions)
      appendSuggestion(result, tmp);
    return result;
  }

  /**
   * @brief Get the suggestions according to active time in 24 hours.
   */
  void activePeriodSuggestion(const int values[24],
                              QVector<QString>& suggestion,
                              QVector<QString>& detail);

  /**
   * @brief Get the suggestion according to the operation or time of 2 devices.
   */
  void getDeviceTrend(QString nameA,
                      QString nameB,
                      int valueA,
                      int valueB,
                      QVector<QString>& suggestion,
                      QVector<QString>& detail);

  /**
   * @brief Get the suggestion according to the operation and time of 1 device.
   */
  void getBusyTrend(int operation,
                    int time,
                    QString& suggestion,
                    QString& detail);
}

#endif // SUGGESTION_H
