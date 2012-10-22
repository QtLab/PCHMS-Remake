#ifndef TOPWIDGET_H
#define TOPWIDGET_H

#include <QWidget>

/**
 * @brief Class of widget which should be on the top of a desktop.
 *
 * Class of widget which should be on the top of a desktop.
 * Whether it's on the top of a desktop can be triggled.
 * Frameless and can't be shown on start menu.
 */
class TopWidget : public QWidget
{
  Q_OBJECT
public:
  TopWidget(QWidget *parent=NULL);

public slots:
  /**
   * @brief Slot to set whether this widget should be on the top.
   */
  void setOnTop(bool top);
};

#endif // TOPWIDGET_H
