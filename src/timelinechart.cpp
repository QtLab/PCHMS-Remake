#include "timelinechart.h"

#include <QEvent>
#include <QMouseEvent>
#include <QTime>
#include <QWheelEvent>
#include <qwt_picker_machine.h>
#include <qwt_plot_picker.h>
#include <qwt_scale_draw.h>
#include <qwt_scale_map.h>

/**
 * @brief Class of scale draw to draw the time in a day.
 */
class TimeScaleDraw : public QwtScaleDraw
{
public:
  virtual QwtText label(double v) const;
};

QwtText TimeScaleDraw::label(double v) const
{
  return QTime(0, 0, 0).addSecs(v).toString("hh:mm:ss");
}

XPicker::XPicker(TimeLineChart *plot) :
    QwtPlotPicker(QwtPlot::xBottom,
                  QwtPlot::yLeft,
                  QwtPicker::VLineRubberBand,
                  QwtPicker::AlwaysOn,
                  plot->canvas()),
    p(plot)
{
}

void XPicker::widgetMousePressEvent(QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton)
    pressAtX = invTransform(event->pos()).x();
}

void XPicker::widgetMouseReleaseEvent(QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton)
  {
    int releaseAtX = invTransform(event->pos()).x();
    if (releaseAtX - pressAtX >= 1 ||
        releaseAtX - pressAtX <= -1)
    {
      if (releaseAtX > pressAtX)
        emit zoomIn(pressAtX, releaseAtX);
      else
        emit zoomIn(releaseAtX, pressAtX);
    }
  }
  else if (event->button() == Qt::RightButton)
  {
    emit zoomOut();
  }
}

void XPicker::widgetWheelEvent(QWheelEvent *event)
{
  int delta = (p->getCurrentTo() - p->getCurrentFrom()) / 8;
  if (event->delta() < 0)
    emit zoomIn(p->getCurrentFrom() + delta,
                p->getCurrentTo() + delta);
  else
    emit zoomIn(p->getCurrentFrom() - delta,
                p->getCurrentTo() - delta);
}

QwtText XPicker::trackerTextF(const QPointF &pos) const
{
  int tmp = pos.x();
  int second = tmp % 60;
  tmp = tmp / 60;
  int minute = tmp % 60;
  int hour = tmp / 60;
  return QwtText(QObject::tr("%1%2%3%4%5").
                 arg(hour).
                 arg(':').
                 arg(minute).
                 arg(':').
                 arg(second));
}

const int TimeLineChart::SECONDS_IN_A_DAY = 24 * 3600;

TimeLineChart::TimeLineChart(QWidget *parent) :
    QwtPlot(parent),
    baseFrom(0),
    baseTo(SECONDS_IN_A_DAY),
    currentFrom(0),
    currentTo(SECONDS_IN_A_DAY)
{
  setAxisScaleDraw(QwtPlot::xBottom, new TimeScaleDraw());

  picker = new XPicker(this);
  picker->setEnabled(true);
  picker->setStateMachine(new QwtPickerTrackerMachine());
  connect(picker,
          SIGNAL(zoomIn(int,int)),
          SLOT(zoomIn(int,int)));
  connect(picker,
          SIGNAL(zoomOut()),
          SLOT(zoomOut()));
}

void TimeLineChart::setBase(int from, int to)
{
  if (to > SECONDS_IN_A_DAY)
  {
    baseFrom = SECONDS_IN_A_DAY - (to - from);
    baseTo = SECONDS_IN_A_DAY;
  }
  else if (from < 0)
  {
    baseFrom = 0;
    baseTo = to - from;
  }
  else
  {
    baseFrom = from;
    baseTo = to;
  }
  currentFrom = baseFrom;
  currentTo = baseTo;
  replot();
}

void TimeLineChart::zoomIn(int from, int to)
{
  if (from != currentFrom || to != currentTo)
  {
    int lastFrom = currentFrom;
    int lastTo = currentTo;
    if (to > SECONDS_IN_A_DAY)
    {
      currentFrom = SECONDS_IN_A_DAY - (to - from);
      currentTo = SECONDS_IN_A_DAY;
    }
    else if (from < 0)
    {
      currentFrom = 0;
      currentTo = to - from;
    }
    else
    {
      currentFrom = from;
      currentTo = to;
    }
    replot();
    if (lastFrom != currentFrom || lastTo != currentTo)
      emit timeChanged(currentFrom, currentTo);
  }
}

void TimeLineChart::zoomOut()
{
  if (currentFrom != baseFrom || currentTo != baseTo)
  {
    int lastFrom = currentFrom;
    int lastTo = currentTo;
    currentFrom = baseFrom;
    currentTo = baseTo;
    replot();
    if (lastFrom != currentFrom || lastTo != currentTo)
      emit timeChanged(currentFrom, currentTo);
  }
}

void TimeLineChart::setPickerVisible(bool visible)
{
  picker->setRubberBand(visible ? QwtPicker::VLineRubberBand : QwtPicker::NoRubberBand);
  picker->setTrackerPen(visible ? QPen() : QPen(QColor(0, 0, 0, 0)));
}

TimeLineChart::~TimeLineChart()
{
  delete picker;
}
