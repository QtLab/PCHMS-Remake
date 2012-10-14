#include "inadayoperationchart.h"

#include <QMap>
#include <qwt_legend.h>
#include <qwt_legend_item.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_layout.h>
#include "colormanagement.h"
#include "operationinaday.h"

#define OTHER_PROGRAM_NAME "其他"

// TODO: suggestion

InADayOperationChart::InADayOperationChart(QWidget *parent) :
    TimeLineChart(parent),
    operations(NULL),
    curves(NULL),
    n(0),
    interval(90)
{
  canvas()->setFrameStyle(QFrame::Box | QFrame::Plain);
  canvas()->setLineWidth(0);
  plotLayout()->setAlignCanvasToScales(true);

  // Create the legend
  QwtLegend *legend = new QwtLegend;
  legend->setItemMode(QwtLegend::CheckableItem);
  insertLegend(legend, QwtPlot::LeftLegend);
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
  setAxisScale(xBottom, 0, SECONDS_IN_A_DAY);
  setBase(0, SECONDS_IN_A_DAY);
}

void InADayOperationChart::setOperations(OperationInADay *newOperations)
{
  operations = newOperations;
}

void InADayOperationChart::clear()
{
  if (curves != NULL)
  {
    for (int i = 0;i < n;++i)
    {
      curves[i]->detach();
      delete curves[i];
    }
    delete [] curves;
    curves = NULL;
    n = 0;
  }
}

void InADayOperationChart::setPrograms(QVector<QString> newPrograms)
{
  // Clear
  clear();

  // Set the programs
  programs = newPrograms;

  // Prepare the curves
  n = programs.size() + 1;
  curves = new QwtPlotCurve *[n];
  for (int i = 0;i < n;++i)
  {
    curves[i] = new QwtPlotCurve(i < n - 1 ?
                                 programs[i] :
                                 QString::fromUtf8(OTHER_PROGRAM_NAME));
    QColor color = ColorManagement::getChartColor(i * 1.0 / n);
    QPen pen(color);
    pen.setWidthF(2.5);
    curves[i]->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    curves[i]->setPen(pen);
    curves[i]->attach(this);

    QwtLegendItem *legendItem = (QwtLegendItem *)legend()->find(curves[i]);
    if (legendItem != NULL)
    {
      legendItem->setChecked(true);
      curves[i]->setVisible(true);
    }
  }

  replot();
}

void InADayOperationChart::replot()
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

  int from = getCurrentFrom();
  int to = getCurrentTo();

  // Auto calculate interval
  interval = qMax(1, (getCurrentTo() - getCurrentFrom()) / 200);

  to = to - (to - from) % interval + interval - 1;
  int maxIndex = (to - from) / interval;

  // Prepare the counts
  int **counts = new int *[n];
  for (int i = 0;i < n;++i)
  {
    counts[i] = new int[maxIndex + 1];
    memset(counts[i], 0, sizeof(int) * (maxIndex + 1));
  }

  // Go through the operations
  for (int i = 0;i < operations->keyOperations.size();++i)
  {
    int slot = (operations->keyOperations[i].ms / 1000 - from) / interval;
    if (slot >= 0 && slot <= maxIndex)
      ++counts[map.value(operations->keyOperations[i].program, n - 1)][slot];
  }

  for (int i = 0;i < operations->mouseOperations.size();++i)
  {
    int slot = (operations->mouseOperations[i].ms / 1000 - from) / interval;
    if (slot >= 0 && slot <= maxIndex)
      ++counts[map.value(operations->mouseOperations[i].program, n - 1)][slot];
  }

  // Set the curves
  for (int i = 0;i < n;++i)
  {
    QVector<QPointF> samples;
    for (int j = 0;j <= maxIndex;++j)
    {
      if (counts[i][j] != 0 ||
          (j > 0 && j < maxIndex && (counts[i][j - 1] != 0 || counts[i][j + 1] != 0)))
        samples.push_back(QPointF(from + interval * j, counts[i][j]));
    }
    curves[i]->setSamples(samples);
  }

  // Release the space
  for (int i = 0;i < n;++i)
    delete [] counts[i];
  delete [] counts;

  setAxisScale(xBottom, from, to - interval);

  QwtPlot::replot();
}

void InADayOperationChart::showItem(QwtPlotItem *item, bool on)
{
  item->setVisible(on);
  QwtPlot::replot();
}

QSize	InADayOperationChart::sizeHint() const
{
  return QSize(180, 80);
}

InADayOperationChart::~InADayOperationChart()
{
  clear();
}
