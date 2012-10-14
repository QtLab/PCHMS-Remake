#ifndef SUGGESTIONITEM_H
#define SUGGESTIONITEM_H

#include <qwt_plot_item.h>

#include <QColor>
#include <QPen>
#include <QVector>

struct SuggestionUnit;

/**
 * @brief Class of item to show suggestion in a plot.
 *
 * Class of item to show suggestion in a plot.
 * An item can contains serveral suggestions.
 */
class SuggestionItem : public QwtPlotItem
{
public:
  /**
   * @brief Struct to store information of a suggestion
   */
  struct SuggestionUnit
  {
    QRectF target;
    QString suggestion;
    QPen pen;
    QBrush brush;
  };

  SuggestionItem();
  ~SuggestionItem();

  static const int RTTI = 1234;

  /**
   * @brief Add a suggestion at given target.
   *
   * The pen will be red and the width will be 2.
   * The brush will be green and the has a low opacity.
   */
  void addSuggestion(QRectF target,
                     QString suggestion);

  /**
   * @brief Add a suggestion at given target with given pen and brush.
   */
  void addSuggestion(QRectF target,
                     QString suggestion,
                     QPen pen,
                     QBrush brush);

  virtual void draw(QPainter *painter,
                    const QwtScaleMap& xMap,
                    const QwtScaleMap& yMap,
                    const QRectF&) const;
  virtual int rtti() const
  {return RTTI;}

  /**
   * @brief Whether the point is in any of the suggestion rectangle.
   *
   * @return The index of the rectangle. -1 if none of the retangles contains the point.
   */
  int in(const QPointF& pos) const;

  /**
   * @brief Clear the suggestions recorded.
   */
  void clear();

  /**
   * @brief Get the suggestion.
   */
  const QVector<SuggestionUnit *>& getSuggestions() const
  {return suggestions;}

private:
  QVector<SuggestionUnit *> suggestions;
};

#endif // SUGGESTIONITEM_H
