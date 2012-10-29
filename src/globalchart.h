#ifndef GLOBALCHART_H
#define GLOBALCHART_H

#include <qwt_plot.h>

#include <QDate>
#include <qwt_plot_picker.h>
#include "statistic.h"

// zoomin/out

class LineChart;
class LineChartPicker;
class TimeWithLevelScaleDraw;

/**
 * @brief Class of picker only cares the press event in x axis.
 */
class XPicker2 : public QwtPlotPicker
{
  Q_OBJECT
public:
  XPicker2(QwtPlotCanvas *canvas);

protected:
  virtual void widgetMousePressEvent(QMouseEvent *event);
  virtual void widgetWheelEvent(QWheelEvent *event);

signals:
  void zoomIn(int x);
  void zoomOut();
  void last();
  void next();
};

/**
 * @brief Class of chart to draw the operation/time of the keyboard/mouse.
 */
class GlobalChart : public QwtPlot
{
  Q_OBJECT
public:
  GlobalChart(QWidget *parent=NULL);
  ~GlobalChart();

  /**
   * @brief Recalculate the chart with the given statistic.
   */
  void recalculate(Statistic *statistic);

  virtual QSize	sizeHint() const;

private:
  /**
   * Current level of the statistic.
   */
  Statistic::StatisticLevel level;

  /**
   * The charts to draw the operation/time of the keyboard/mouse.
   */
  LineChart *charts[4];

  /**
   * A picker to show tootips.
   */
  LineChartPicker *picker;

  /**
   * A scale draw to draw the operation/time of the keyboard/mouse.
   */
  TimeWithLevelScaleDraw * scaleDraw;

  /**
   * A picker to zoom.
   */
  XPicker2 *picker2;

  /**
   * @brief Map a date to and integer value in x axis.
   */
  static int dateToX(QDate date, Statistic::StatisticLevel level);

signals:
  void zoomOut();
  void zoomIn(int x);
  void last();
  void next();

private slots:
  void showItem(QwtPlotItem *item, bool visible);
};

#endif // GLOBALCHART_H
