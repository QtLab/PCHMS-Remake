#include "linechart.h"

#include <qwt_interval_symbol.h>
#include <qwt_legend.h>
#include <qwt_legend_item.h>
#include <qwt_plot.h>
#include <qwt_symbol.h>

#define PEN_WIDTH 2.5

LineChart::LineChart(const QString &title, const QColor &symbolColor):
    QwtPlotCurve(title)

{
  setColor(symbolColor);
}

void LineChart::setColor(const QColor &symbolColor)
{
  setRenderHint(QwtPlotCurve::RenderAntialiased);
  QColor color = symbolColor;
  QPen pen = QPen(color);
  pen.setWidthF(PEN_WIDTH);
  setPen(pen);
  setSymbol(new QwtSymbol(QwtSymbol::Ellipse,
          symbolColor.light(150), symbolColor, QSize(8, 8)));
}

void LineChart::setValues(const QVector<QPointF>& v, const QVector<QString>& t)
{
  setSamples(v);
  if (!t.isEmpty())
    tooltips = t;
  else
    tooltips.clear();
}

void LineChart::setTooltip(QString t)
{
  tooltip = t;
  QwtPlot *p = plot();
  if (p == NULL)
    return;
  QwtLegend *l = p->legend();
  if (l == NULL)
    return;

  QwtLegendItem *legendItem = (QwtLegendItem *)l->find(this);

  if (legendItem != NULL)
    legendItem->setToolTip(tooltip);
}

int LineChart::rtti() const
{
  return RttiLineChart;
}
