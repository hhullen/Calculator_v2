#ifndef REPLENISHMENT_H
#define REPLENISHMENT_H

#include <QRegularExpression>
#include <QWidget>

namespace Ui {
class Replenishment;
}

class Replenishment : public QWidget {
  Q_OBJECT

 signals:
  void delete_button_signal(QObject *);

 public:
  explicit Replenishment(QWidget *parent = nullptr);
  ~Replenishment();

 private slots:
  void close_form();
  void check_input();

 private:
  Ui::Replenishment *ui_;
  QRegularExpression valid_;
  QRegularExpressionMatch matcher_valid_;
};

#endif  // REPLENISHMENT_H
