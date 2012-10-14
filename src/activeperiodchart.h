#ifndef ACTIVEPERIODCHART_H
#define ACTIVEPERIODCHART_H

#include "timelinechart.h"

class LineChart;
class LineChartPicker;

/**
 * @brief Class of chart to show active period.
 */
class ActivePeriodChart : public TimeLineChart
{
public:
  ActivePeriodChart(QWidget *parent=NULL);
  ~ActivePeriodChart();

  /**
   * @brief Set the values.
   */
  void setValues(const int values[24]);

  virtual QSize	sizeHint() const;

private:
  /**
   * The histogram.
   */
  LineChart *chart;

  /**
   * The picker.
   */
  LineChartPicker *picker;
};

#endif // ACTIVEPERIODCHART_H
