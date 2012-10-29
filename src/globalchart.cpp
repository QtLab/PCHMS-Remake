#include "globalchart.h"

#include <QMouseEvent>
#include <QWheelEvent>
#include <qwt_legend.h>
#include <qwt_legend_item.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_layout.h>
#include <qwt_scale_draw.h>
#include "linechart.h"
#include "linechartpicker.h"

const QString names[4] = {QString::fromUtf8("键盘活跃时间"),
                          QString::fromUtf8("键盘活动次数"),
                          QString::fromUtf8("鼠标活跃时间"),
                          QString::fromUtf8("鼠标活动次数")};

const QColor colors[4] = {QColor(255,   0,   0),
                          QColor(255, 126,   0),
                          QColor(  0,   0, 255),
                          QColor(  0, 128, 255)};

XPicker2::XPicker2(QwtPlotCanvas *canvas) :
    QwtPlotPicker(QwtPlot::xBottom,
                  QwtPlot::yLeft,
                  QwtPicker::NoRubberBand,
                  QwtPicker::AlwaysOff,
                  canvas)
{
}

void XPicker2::widgetMousePressEvent(QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton)
  {
    int x = qRound(invTransform(event->pos()).x());
    emit zoomIn(x);
  }
  else if (event->button() == Qt::RightButton)
  {
    emit zoomOut();
  }
}

void XPicker2::widgetWheelEvent(QWheelEvent *event)
{
  if (event->delta() > 0)
    emit last();
  else if (event->delta() < 0)
    emit next();
}

/**
 * @brief Class of scale draw to draw the time.
 */
class TimeWithLevelScaleDraw: public QwtScaleDraw
{
public:
  virtual QwtText label(double v) const
  {
    int value = v;
    switch (l)
    {
    case Statistic::Year:
      {
        QDate date = Statistic::BASE_DATE.addYears(value);
        return date.toString("yyyy");
      }
    case Statistic::Month:
      {
        QDate date = Statistic::BASE_DATE.addYears(value / 12);
        date = date.addMonths(value % 12);
        return date.toString("yyyy-MM");
      }
    case Statistic::Day:
      {
        QDate date = Statistic::BASE_DATE.addDays(value);
        return date.toString("yyyy-MM-dd");
      }
    default:
      return QString("");
    }
  }

  /**
   * @brief Set the names to show.
   */
  void setLevel(Statistic::StatisticLevel level)
  {l = level;}

private:
  Statistic::StatisticLevel l;
};

GlobalChart::GlobalChart(QWidget *parent) :
    QwtPlot(parent)
{
  canvas()->setFrameStyle(QFrame::Box | QFrame::Plain);
  canvas()->setLineWidth(0);
  plotLayout()->setAlignCanvasToScales(true);
  enableAxis(QwtPlot::yRight);

  // Create the legend
  QwtLegend *legend = new QwtLegend;
  legend->setItemMode(QwtLegend::CheckableItem);
  insertLegend(legend, QwtPlot::RightLegend);
  connect(this,
          SIGNAL(legendChecked(QwtPlotItem *, bool)),
          SLOT(showItem(QwtPlotItem *, bool)));

  // Create the grid
  QwtPlotGrid *grid = new QwtPlotGrid();
  grid->enableX(true);
  grid->enableY(true);
  grid->enableXMin(false);
  grid->enableYMin(false);
  grid->setMajPen(QPen(QColor(100, 100, 100), 0, Qt::DotLine));
  grid->attach(this);

  // Things about the axis
  scaleDraw = new TimeWithLevelScaleDraw();
  scaleDraw->setLevel(Statistic::All);
  setAxisScaleDraw(QwtPlot::xBottom, scaleDraw);
  setAxisMaxMinor(QwtPlot::xBottom, 0);

  // Create the charts
  for (int i = 0;i < 4;++i)
  {
    charts[i] = new LineChart(names[i], colors[i]);
    charts[i]->setAxes(xBottom, i % 2 == 0 ? yLeft : yRight);
    charts[i]->attach(this);
    QwtLegendItem *legendItem = (QwtLegendItem *)legend->find(charts[i]);
    if (legendItem != NULL)
        legendItem->setChecked(true);
  }

  // Create the pickers
  picker = new LineChartPicker(this);
  picker2 = new XPicker2(canvas());
  connect(picker2,
          SIGNAL(zoomIn(int)),
          SIGNAL(zoomIn(int)));
  connect(picker2,
          SIGNAL(zoomOut()),
          SIGNAL(zoomOut()));
  connect(picker2,
          SIGNAL(last()),
          SIGNAL(last()));
  connect(picker2,
          SIGNAL(next()),
          SIGNAL(next()));
}

QSize	GlobalChart::sizeHint() const
{
  return QSize(400, 150);
}

void GlobalChart::recalculate(Statistic *statistic)
{
  level = statistic->getLevel();
  scaleDraw->setLevel((Statistic::StatisticLevel) (level + 1));
  const QVector<Statistic *>& statistics = statistic->getSubStatistics();

  // Calculate the data
  QVector<QPointF> v[4];
  QVector<QString> t[4];
  int lMax = 0;
  int rMax = 0;
  int xFrom = -1;
  int xTo = -1;
  for (int i = 0;i < statistics.size();++i)
  {
    const Statistic *subStatistic = statistics[i];
    const Statistic::StatisticUnit *unit = subStatistic->getWholeStatistic();
    QDate date = subStatistic->getEndDate();

    // Calculate things for x axis
    int x = dateToX(date, subStatistic->getLevel());
    if (i == 0)
    {
      xFrom = x;
      xTo = x;
    }
    else
    {
      xTo = qMax(x, xTo);
    }

    // Time
    v[0].push_back(QPointF(x, unit->keyboardActiveTime));
    t[0].push_back(tr("%1").arg(unit->keyboardActiveTime));
    lMax = qMax(lMax, unit->keyboardActiveTime);
    v[2].push_back(QPointF(x, unit->mouseActiveTime));
    t[2].push_back(tr("%1").arg(unit->mouseActiveTime));
    lMax = qMax(lMax, unit->mouseActiveTime);

    // Operations
    int count = 0;
    for (QMap<Keyboard::Key, int>::ConstIterator itr = unit->keyTimes.begin();
         itr != unit->keyTimes.end();
         ++itr)
      count += itr.value();
    v[1].push_back(QPointF(x, count));
    t[1].push_back(tr("%1").arg(count));
    rMax = qMax(rMax, count);

    count = 0;
    for (QMap<Mouse::Button, int>::ConstIterator itr = unit->buttonTimes.begin();
         itr != unit->buttonTimes.end();
         ++itr)
      count += itr.value();
    v[3].push_back(QPointF(x, count));
    t[3].push_back(tr("%1").arg(count));
    rMax = qMax(rMax, count);
  }

  // Set the data
  for (int i = 0;i < 4;++i)
    charts[i]->setValues(v[i], t[i]);
  setAxisMaxMinor(QwtPlot::xBottom, statistics.size());
  setAxisScale(xBottom, xFrom - 0.5, xTo + 0.5, 5.0);
  setAxisScale(yLeft, 0, lMax * 1.2);
  setAxisScale(yRight, 0, rMax * 1.2);
  replot();
}

int GlobalChart::dateToX(QDate date, Statistic::StatisticLevel level)
{
  switch (level)
  {
  case Statistic::Year:
    return (date.year() - Statistic::BASE_DATE.year());
  case Statistic::Month:
    return (date.year() - Statistic::BASE_DATE.year()) * 12 +
           date.month() - Statistic::BASE_DATE.month();
  case Statistic::Day:
    return Statistic::BASE_DATE.daysTo(date);
  default:
    return -1;
  }
}

void GlobalChart::showItem(QwtPlotItem *item, bool visible)
{
  item->setVisible(visible);
  replot();
}

GlobalChart::~GlobalChart()
{
  delete picker;
  delete picker2;
  for (int i = 0;i < 4;++i)
  {
    charts[i]->detach();
    delete charts[i];
  }
}
