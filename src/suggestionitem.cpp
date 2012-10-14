#include "suggestionitem.h"

#include <QPainter>
#include <qwt_plot_canvas.h>
#include <qwt_plot.h>
#include <qwt_scale_map.h>

SuggestionItem::SuggestionItem()
{
}


void SuggestionItem::addSuggestion(QRectF target,
                                   QString suggestion)
{
  SuggestionUnit *unit = new SuggestionUnit();
  unit->target = target;
  unit->suggestion = suggestion;
  QPen pen(QColor(255, 0, 0));
  pen.setWidthF(2);
  unit->pen = pen;
  unit->brush = QBrush(QColor(0, 255, 0, 50));

  suggestions.push_back(unit);
}

void SuggestionItem::addSuggestion(QRectF target,
                                   QString suggestion,
                                   QPen pen,
                                   QBrush brush)
{
  SuggestionUnit *unit = new SuggestionUnit();
  unit->target = target;
  unit->suggestion = suggestion;
  unit->pen = pen;
  unit->brush = brush;

  suggestions.push_back(unit);
}

void SuggestionItem::draw(QPainter *painter,
                          const QwtScaleMap& xMap,
                          const QwtScaleMap& yMap,
                          const QRectF&) const
{
  painter->save();
  SuggestionUnit *unit;
  foreach(unit, suggestions)
  {
    painter->setPen(unit->pen);
    painter->setBrush(unit->brush);
    painter->drawRoundedRect(QwtScaleMap::transform(xMap, yMap, unit->target), 5, 5);
  }
  painter->restore();
}

int SuggestionItem::in(const QPointF& pos) const
{
  SuggestionUnit *unit;
  int i = 0;
  foreach(unit, suggestions)
  {
    if (unit->target.contains(pos))
      return i;
    ++i;
  }
  return -1;
}

void SuggestionItem::clear()
{
  SuggestionUnit *unit;
  foreach(unit, suggestions)
    delete unit;
  suggestions.clear();
}

SuggestionItem::~SuggestionItem()
{
  SuggestionUnit *unit;
  foreach(unit, suggestions)
    delete unit;
}
