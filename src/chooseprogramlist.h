#ifndef CHOOSEPROGRAMLIST_H
#define CHOOSEPROGRAMLIST_H

#include <QWidget>

#include <QCheckBox>
#include <QVBoxLayout>
#include <QVector>

/**
 * @brief Class of widget to list the programs and let the user to select.
 */
class ChooseProgramList : public QWidget
{
  Q_OBJECT
public:
  ChooseProgramList(QWidget *parent=NULL);
  ~ChooseProgramList();

  static const int MAX_AUTO_CHECK;

  /**
   * @brief Set the name of the programs.
   *
   * @param programs The name of the programs.
   */
  void setPrograms(const QVector<QString>& programs);

  /**
   * @brief Get the selected programs.
   */
  QVector<QString> getSelectedPrograms() const;

private:
  /**
   * The check boxes.
   */
  QVector<QCheckBox *> checkBoxes;

  /**
   * The layout.
   */
  QVBoxLayout *layout;

  /**
   * @brief Reset the color of the items.
   */
  void resetColor();

signals:
  /**
   * @brief A signal emitted when the selection is changed.
   */
  void selectionChanged();

private slots:
  /**
   * @brief Called when any of the checked boxes changes the check state.
   */
  void checkChanged(bool checked);
};

#endif // CHOOSEPROGRAMLIST_H
