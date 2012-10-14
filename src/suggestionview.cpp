#include "suggestionview.h"

#include <QHelpEvent>
#include <QToolTip>

SuggestionView::SuggestionView(QWidget *parent) :
    QListView(parent)
{
  model = new QStringListModel();
  setModel(model);
}

void SuggestionView::append(const QVector<QString>& suggestions,
                            const QVector<QString>& details)
{
  if (suggestions.size() != details.size())
  {
    // Not supposed to be reached
    return;
  }
  int row = model->rowCount();
  for (int i = 0;i < suggestions.size();++i)
  {
    model->insertRow(row);
    model->setData(model->index(row), suggestions[i]);

    // Use a vector to store the tooltip because QStringListModel only stores
    // data for DisplayRole/EditRole.
    tooltips.push_back(details[i]);
    ++row;
  }
}

void SuggestionView::append(const QString& suggestion,
                            const QString& detail)
{
  int row = model->rowCount();
  model->insertRow(row);
  model->setData(model->index(row), suggestion);

  // Use a vector to store the tooltip because QStringListModel only stores
  // data for DisplayRole/EditRole.
  tooltips.push_back(detail);
}

void SuggestionView::clear()
{
  model->removeRows(0, model->rowCount());
  tooltips.clear();
}

bool SuggestionView::viewportEvent(QEvent *event)
{
  if (event->type() == QEvent::ToolTip)
  {
    QPoint pos = ((QHelpEvent *) (event))->pos();
    QModelIndex index = indexAt(pos);
    if (index.isValid())
    {
      // Get the tooltip and show it
      QString tooltip = tooltips[index.row()];
      QToolTip::showText(((QHelpEvent *) (event))->globalPos(), tooltip, viewport());
    }
    return false;
  }
  return QListView::viewportEvent(event);
}

SuggestionView::~SuggestionView()
{
  delete model;
}
