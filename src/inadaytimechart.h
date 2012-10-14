#ifndef INADAYTIMECHART_H
#define INADAYTIMECHART_H

#include "timelinechart.h"

class Histogram;
class LineChartPicker;
class NameScaleDraw;
class OperationInADay;

/**
 * @brief Class of chart to show active time of programs in a day.
 */
class InADayTimeChart : public TimeLineChart
{
  Q_OBJECT
public:
  InADayTimeChart(QWidget *parent=NULL);
  virtual ~InADayTimeChart();

  /**
   * @brief Set the raw data of the operations.
   */
  void setOperations(OperationInADay *newOperations);

  /**
   * @brief Clear the histograms.
   */
  void clear();

  /**
   * @brief Update the histograms.
   */
  virtual void replot();

  virtual QSize	sizeHint() const;

private:
  /**
   * The operations.
   */
  OperationInADay *operations;

  /**
   * The histograms.
   */
  Histogram **histograms;

  /**
   * The scale draw.
   */
  NameScaleDraw *scaleDraw;

  /**
   * Number of the curves.
   */
  int n;

  /**
   * The programs to watch.
   */
  QVector<QString> programs;

  /**
   * The interval in seconds.
   */
  int interval;

  LineChartPicker *picker;

public slots:
  /**
   * @brief Set the programs.
   */
  void setPrograms(const QVector<QString>& newPrograms);
};

#endif // INADAYTIMECHART_H
