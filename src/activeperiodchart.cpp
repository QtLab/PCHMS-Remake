#include "activeperiodchart.h"

#include "linechart.h"
#include "linechartpicker.h"

ActivePeriodChart::ActivePeriodChart(QWidget *parent) :
    TimeLineChart(parent)
{
  setPickerVisible(false);
  chart = new LineChart(QString::null, QColor(0, 0, 255));
  chart->setRenderHint(QwtPlotItem::RenderAntialiased, true);
  chart->attach(this);
  picker = new LineChartPicker(this);
}

void ActivePeriodChart::setValues(const int values[24])
{
  QVector<QPointF> v;
  QVector<QString> t;
  for (int i = 0;i < 24;++i)
  {
    v.push_back(QPointF(i * TimeLineChart::SECONDS_IN_A_DAY / 24 +
                        TimeLineChart::SECONDS_IN_A_DAY / 48,
                        values[i]));
    t.push_back(tr("%1").arg(values[i]));
  }
  chart->setValues(v, t);

  // 00:00 06:00 12:00 18:00 24:00
  setAxisScale(xBottom,
               0,
               TimeLineChart::SECONDS_IN_A_DAY,
               TimeLineChart::SECONDS_IN_A_DAY / 4);
  replot();
}

QSize	ActivePeriodChart::sizeHint() const
{
  return QSize(400, 100);
}

ActivePeriodChart::~ActivePeriodChart()
{
  delete picker;
  delete chart;
}
