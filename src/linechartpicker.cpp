#include "linechartpicker.h"

#include <QApplication>
#include <QEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QToolTip>
#include <QWhatsThis>
#include <QPainter>
#include <qwt_plot.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_directpainter.h>
#include <qwt_scale_map.h>
#include <qwt_symbol.h>
#include "linechart.h"
#include "suggestionitem.h"

LineChartPicker::LineChartPicker(QwtPlot *plot) :
    QwtPlotPicker(plot->canvas()),
    p(plot)
{
  QwtPlotCanvas *canvas = plot->canvas();

  canvas->installEventFilter(this);

  canvas->setFocusPolicy(Qt::StrongFocus);
  canvas->setMouseTracking(true);
  canvas->setFocusIndicator(QwtPlotCanvas::ItemFocusIndicator);
  canvas->setFocus();
}

bool LineChartPicker::eventFilter(QObject *object, QEvent *e)
{
  if (object != (QObject *)plot()->canvas())
    return false;

  switch(e->type())
  {
  case QEvent::MouseButtonPress:
    {
      select(((QMouseEvent *)e)->pos());
      return false;
    }
  case QEvent::MouseMove:
    {
      select(((QMouseEvent *)e)->pos());
      return false;
    }
  default:
    break;
  }
  return QObject::eventFilter(object, e);
}

void LineChartPicker::select(const QPoint& pos)
{
  LineChart *curve = NULL;
  SuggestionItem *suggestionItem = NULL;
  int index = -1;
  double dist = 10e10;
  QPointF p = invTransform(pos);

  const QwtPlotItemList& itmList = plot()->itemList();
  if (!itmList.isEmpty())
  {
    QwtPlotItemIterator itr = itmList.end();
    while (true)
    {
      --itr;
      if ((*itr)->rtti() == LineChart::RttiLineChart && (*itr)->isVisible())
      {
        LineChart *c = (LineChart*)(*itr);

        double d;
        int idx = c->closestPoint(pos, &d);
        // Find the nearest chart
        if (d < dist)
        {
          curve = c;
          index = idx;
          dist = d;
        }
      }
      else if ((*itr)->rtti() == SuggestionItem::RTTI && (*itr)->isVisible())
      {
        // Find the first suggestion item covers the point
        int idx = ((SuggestionItem *) *itr)->in(p);
        if (idx >= 0)
        {
          index = idx;
          suggestionItem = (SuggestionItem *) (*itr);
        }
        break;
      }
      if (itr == itmList.begin())
        break;
    }
  }

  if (suggestionItem != NULL)
  {
    // Show the suggestion
    const SuggestionItem::SuggestionUnit *item = suggestionItem->getSuggestions()[index];
    QToolTip::showText(plot()->canvas()->mapToGlobal(pos),
                       item->suggestion);
  }
  else if (curve != NULL && dist < 5)
  {
    // Show the tooltip
    QToolTip::showText(plot()->canvas()->mapToGlobal(pos),
                       tr("%1").arg(curve->getTooptipAt(index)));
  }
}
