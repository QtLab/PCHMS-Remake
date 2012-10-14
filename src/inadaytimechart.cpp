#include "inadaytimechart.h"

#include <qwt_interval.h>
#include <qwt_legend.h>
#include <qwt_legend_item.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_layout.h>
#include <qwt_scale_draw.h>
#include "colormanagement.h"
#include "histogram.h"
#include "linechartpicker.h"
#include "operationinaday.h"
#include "suggestionitem.h"

#define OTHER_PROGRAM_NAME "其他"

// TODO: suggestion

/**
 * @brief Class of scale draw to draw the names of the programs.
 */
class NameScaleDraw: public QwtScaleDraw
{
public:
  virtual QwtText label(double v) const
  {
    int i = qFloor(v) - 1;
    QString name;
    QColor color;
    if (i >= 0 && i < names.size())
    {
      name = names[names.size() - i - 1];
      color = ColorManagement::getChartColor((names.size() - i - 1) * 1.0 / (names.size() + 1));
    }
    else
    {
      name = QString::fromUtf8(OTHER_PROGRAM_NAME);
      color = ColorManagement::getChartColor(names.size() * 1.0 / (names.size() + 1));
    }
    QwtText result(name);
    result.setColor(color);
    return result;
  }

  /**
   * @brief Set the names to show.
   */
  void setNames(const QVector<QString>& newNames)
  {names = newNames;}

private:
  QVector<QString> names;
};

InADayTimeChart::InADayTimeChart(QWidget *parent) :
    TimeLineChart(parent),
    operations(NULL),
    histograms(NULL),
    n(0),
    interval(900)
{
  canvas()->setFrameStyle(QFrame::Box | QFrame::Plain);
  canvas()->setLineWidth(0);
  plotLayout()->setAlignCanvasToScales(true);

  // Create the grid
  QwtPlotGrid *grid = new QwtPlotGrid();
  grid->enableX(true);
  grid->enableY(true);
  grid->enableXMin(false);
  grid->enableYMin(false);
  grid->setMajPen(QPen(QColor(100, 100, 100), 0, Qt::DotLine));
  grid->attach(this);

  // Things about the axis
  scaleDraw = new NameScaleDraw();
  setAxisScaleDraw(QwtPlot::yLeft, scaleDraw);
  setAxisScale(xBottom, 0, SECONDS_IN_A_DAY);
  setAxisMaxMinor(QwtPlot::yLeft, 0);
  setBase(0, SECONDS_IN_A_DAY);

  picker = new LineChartPicker(this);
}

void InADayTimeChart::clear()
{
  if (histograms != NULL)
  {
    for (int i = 0;i < n;++i)
    {
      histograms[i]->detach();
      delete histograms[i];
    }
    delete [] histograms;
    histograms = NULL;
    n = 0;
  }
  scaleDraw->setNames(QVector<QString>());
}

void InADayTimeChart::setOperations(OperationInADay *newOperations)
{
  operations = newOperations;
}

void InADayTimeChart::setPrograms(const QVector<QString>& newPrograms)
{
  // Clear
  clear();

  // Set the programs
  programs = newPrograms;
  scaleDraw->setNames(programs);

  // Prepare the histograms
  n = programs.size() + 1;
  histograms = new Histogram *[n];
  for (int i = 0;i < n;++i)
  {
    QColor color = ColorManagement::getChartColor(i * 1.0 / n);
    histograms[i] = new Histogram(i < n - 1 ?
                                  programs[i] :
                                  QString::fromUtf8(OTHER_PROGRAM_NAME),
                                  color);

    histograms[i]->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    histograms[i]->setBaseline(n - i - 1.4);
    histograms[i]->attach(this);
  }

  replot();
}

void InADayTimeChart::replot()
{
  if (operations == NULL)
  {
    QwtPlot::replot();
    return;
  }

  // Map the name to the index
  QMap<QString, int> map;
  for (int i = 0;i < n - 1;++i)
    map.insert(programs[i], i);

  // Auto calculate interval
  interval = qMax(5, (getCurrentTo() - getCurrentFrom()) / 200);

  // Prepare the samples
  QVector<QwtIntervalSample> *samples = new QVector<QwtIntervalSample>[n];
  int *lastTimes = new int[n];
  int *lastLastTimes = new int[n];
  for (int i = 0;i < n;++i)
  {
    lastTimes[i] = -1;
    lastLastTimes[i] = -1;
  }

  // Go through the operations
  int currentKeyIndex = 0;
  int currentMouseIndex = 0;

  while (currentKeyIndex < operations->keyOperations.size() ||
         currentMouseIndex < operations->mouseOperations.size())
  {
    bool key = currentMouseIndex >= operations->mouseOperations.size() ||
               (currentKeyIndex < operations->keyOperations.size() &&
                operations->keyOperations[currentKeyIndex].id <
                operations->mouseOperations[currentMouseIndex].id);
    QString program;
    int second;
    if (key)
    {
      program = operations->keyOperations[currentKeyIndex].program;
      second = operations->keyOperations[currentKeyIndex].ms / 1000;
    }
    else
    {
      program = operations->mouseOperations[currentMouseIndex].program;
      second = operations->mouseOperations[currentMouseIndex].ms / 1000;
    }
    int index = map.value(program, n - 1);
    int lastTime = lastTimes[index];
    int lastLastTime = lastLastTimes[index];
    if (lastTime == -1)
    {
      // Record the start
      lastLastTimes[index] = second;
      lastTimes[index] = second;
    }
    else if (second - lastTime < interval)
    {
      lastTimes[index] = second;
    }
    else
    {
      // Record a new period of time
      samples[index].push_back(QwtIntervalSample(n - index - 0.6,
                                                 QwtInterval(double(lastLastTime),
                                                             double(lastTime),
                                                             QwtInterval::ExcludeBorders)));
      // Record the start
      lastLastTimes[index] = second;
      lastTimes[index] = second;
    }
    if (key)
      ++currentKeyIndex;
    else
      ++currentMouseIndex;
  }

  // Add the last period of data and set the samples
  for (int i = 0;i < n;++i)
  {
    if (lastLastTimes[i] != -1)
    {
      samples[i].push_back(QwtIntervalSample(n - i - 0.6,
                                             QwtInterval(double(lastLastTimes[i]),
                                                         double(lastTimes[i] + interval),
                                                         QwtInterval::ExcludeBorders)));
    }
    histograms[i]->setData(new QwtIntervalSeriesData(samples[i]));
  }

  // Clear the tmp data
  delete [] samples;
  delete [] lastLastTimes;
  delete [] lastTimes;

  setAxisScale(yLeft, -0.5, n - 0.5, 1);
  setAxisScale(xBottom, getCurrentFrom(), getCurrentTo());
  QwtPlot::replot();
}

QSize	InADayTimeChart::sizeHint() const
{
  return QSize(180, 80);
}

InADayTimeChart::~InADayTimeChart()
{
  delete picker;
}
