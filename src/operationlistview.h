#ifndef OPERATIONLISTVIEW_H
#define OPERATIONLISTVIEW_H

#include <QTableView>

#include <QStandardItemModel>
#include <QString>
#include <QVector>

class OperationInADay;

/**
 * @brief Class of view to list the detail operations in a table.
 *
 * Class of view to list the detail operations in a table.
 * Can't select program or select time, because it costs lots of time.
 */
class OperationListView : public QTableView
{
public:
  OperationListView(QWidget *parent=NULL);
  ~OperationListView();

  /**
   * @brief Set the raw data of the operations.
   */
  void setOperations(OperationInADay *newOperations);

  /**
   * @brief Clear the histograms.
   */
  void clear();

  /**
   * @brief Recalculate the list.
   */
  void recalculate();

  virtual void setVisible(bool visible);

private:
  /**
   * The operations.
   */
  OperationInADay *operations;

  /**
   * The model.
   */
  QStandardItemModel *model;
};

#endif // OPERATIONLISTVIEW_H
