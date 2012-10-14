#ifndef SUGGESTIONVIEW_H
#define SUGGESTIONVIEW_H

#include <QListView>

#include <QStringListModel>
#include <QVector>

/**
 * @brief Class of view to show suggestions.
 *
 * Class of view to show suggestions.
 * A piece of suggestion should have a main text to show brief information and
 * a tooltip to describe it.
 */
class SuggestionView : public QListView
{
public:
  SuggestionView(QWidget *parent=NULL);
  ~SuggestionView();

  /**
   * @brief Using given suggestions and details to append the list.
   */
  void append(const QVector<QString>& suggestions,
                   const QVector<QString>& details);

  /**
   * @brief Using given suggestion and detail to append the list.
   */
  void append(const QString& suggestion,
                   const QString& detail);

  /**
   * @brief Clear the list.
   */
  void clear();

protected:
  virtual bool viewportEvent (QEvent *event);

private:
  /**
   * The model to store the display role.
   */
  QStringListModel *model;

  /**
   * The vector to store the tooltips.
   */
  QVector<QString> tooltips;
};

#endif // SUGGESTIONVIEW_H
