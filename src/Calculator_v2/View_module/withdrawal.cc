#include "withdrawal.h"

#include "ui_withdrawal.h"

Withdrawal::Withdrawal(QWidget *parent)
    : QWidget(parent), ui_(new Ui::Withdrawal) {
  ui_->setupUi(this);
  valid_.setPattern("(^[0-9]{0,1}$)|(\\.[0-9]{1,2}$)|(^[0-9]*$)|(^[0-9]+\\.$)");
  connect(ui_->btn_close_form, SIGNAL(clicked()), this, SLOT(close_form()));
  connect(ui_->seizure_amount, SIGNAL(cursorPositionChanged(int, int)), this,
          SLOT(check_input()));
}

Withdrawal::~Withdrawal() { delete ui_; }

void Withdrawal::close_form() { emit delete_button_signal(sender()->parent()); }

void Withdrawal::check_input() {
  matcher_valid_ = valid_.match(ui_->seizure_amount->text());
  if (!matcher_valid_.hasMatch()) {
    ui_->seizure_amount->setText(ui_->seizure_amount->text().left(
        ui_->seizure_amount->text().length() - 1));
  }
}
