#ifndef TIMELINECHART_H
#define TIMELINECHART_H

#include <qwt_plot.h>

#include <qwt_plot_picker.h>

class TimeLineChart;

/**
 * @brief Class of picker only cares the mouse event in x axis.
 */
class XPicker : public QwtPlotPicker
{
  Q_OBJECT
public:
  XPicker(TimeLineChart *plot);

protected:
  virtual void widgetMousePressEvent(QMouseEvent *event);
  virtual void widgetMouseReleaseEvent(QMouseEvent *event);
  virtual void widgetWheelEvent(QWheelEvent *event);
  virtual QwtText trackerTextF(const QPointF &pos) const;

private:
  int pressAtX;
  TimeLineChart *p;

signals:
  void zoomIn(int from, int to);
  void zoomOut();
};

/**
 * @brief A class of plot whose x axis is time line.
 *
 * A class of plot whose x axis is time line.
 * It should have the ability to zoom in/out and pan in the x axis.
 * Doesn't have the ability to save the zoom stack because this may have some
 * conflict with the ability to pan.
 */
class TimeLineChart : public QwtPlot
{
  Q_OBJECT
public:
  TimeLineChart(QWidget *parent=NULL);
  virtual ~TimeLineChart();

  static const int SECONDS_IN_A_DAY;

  /**
   * @brief Set the zoom base.
   */
  void setBase(int from, int to);

  /**
   * @brief Get the base from.
   */
  inline int getBaseFrom() const
  {return baseFrom;}

  /**
   * @brief Get the base to.
   */
  inline int getBaseTo() const
  {return baseTo;}

  /**
   * @brief Get the current from.
   */
  inline int getCurrentFrom() const
  {return currentFrom;}

  /**
   * @brief Get the base to.
   */
  inline int getCurrentTo() const
  {return currentTo;}

  /**
   * @brief Set whether the picker is visible.
   */
  void setPickerVisible(bool visible);

private:
  /**
   * The picker.
   */
  XPicker *picker;

  /**
   * The base from value.
   */
  int baseFrom;

  /**
   * The base To value.
   */
  int baseTo;

  /**
   * From.
   */
  int currentFrom;

  /**
   * To.
   */
  int currentTo;

  int pressAt;

public slots:
  /**
   * @brief A slot to zoom in.
   */
  virtual void zoomIn(int from, int to);

  /**
   * @brief A slot to zoom out.
   */
  virtual void zoomOut();

signals:
  void timeChanged(int from, int to);
};

#endif // TIMELINECHART_H
