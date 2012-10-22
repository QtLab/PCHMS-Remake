#include "topwidget.h"

#include <QKeyEvent>

TopWidget::TopWidget(QWidget *parent) :
    QWidget(parent)
{
  setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Popup | Qt::Tool);
}

void TopWidget::setOnTop(bool top)
{
  if (top)
  {
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Popup | Qt::Tool);
    show();
  }
  else
  {
    setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
    show();
  }
}

void TopWidget::keyReleaseEvent(QKeyEvent *event)
{
  if (event->key() == Qt::Key_F2)
  {
    bool top = (windowFlags() & Qt::WindowStaysOnTopHint) == Qt::WindowStaysOnTopHint;
    setOnTop(!top);
  }
}
