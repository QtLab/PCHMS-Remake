#include "histogram.h"

#include <QPalette>

Histogram::Histogram(const QString &title, const QColor& symbolColor) :
    QwtPlotHistogram(title)
{
  setStyle(QwtPlotHistogram::Columns);

  QColor color = symbolColor;

  setBrush(QBrush(color));
  setPen(QPen(color.darker()));

  QwtColumnSymbol *symbol = new QwtColumnSymbol(QwtColumnSymbol::NoSymbol);
  symbol->setPalette(QPalette(color));
  setSymbol(symbol);
}
