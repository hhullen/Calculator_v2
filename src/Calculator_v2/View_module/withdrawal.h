#ifndef WITHDRAWAL_H
#define WITHDRAWAL_H

#include <QRegularExpression>
#include <QWidget>

namespace Ui {
class Withdrawal;
}

class Withdrawal : public QWidget {
  Q_OBJECT

 signals:
  void delete_button_signal(QObject *);

 public:
  explicit Withdrawal(QWidget *parent = nullptr);
  ~Withdrawal();

 private slots:
  void close_form();
  void check_input();

 private:
  Ui::Withdrawal *ui_;
  QRegularExpression valid_;
  QRegularExpressionMatch matcher_valid_;
};

#endif  // WITHDRAWAL_H
