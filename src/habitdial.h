#ifndef HABITDIAL_H
#define HABITDIAL_H

#include <QDial>

/**
 * @brief Class of dial to show the habit between two things.
 *
 * Class of dial to show the habit between two things.
 * The scale should be -100 to 100.
 */
class HabitDial: public QDial
{
public:
  HabitDial(QWidget *parent=NULL);
  virtual QSize	sizeHint() const;

protected:
  virtual bool event(QEvent *e);
};

#endif // HABITDIAL_H
