#include "replenishment.h"

#include "ui_replenishment.h"

Replenishment::Replenishment(QWidget *parent)
    : QWidget(parent), ui_(new Ui::Replenishment) {
  ui_->setupUi(this);
  valid_.setPattern("(^[0-9]{0,1}$)|(\\.[0-9]{1,2}$)|(^[0-9]*$)|(^[0-9]+\\.$)");
  connect(ui_->btn_close_form, SIGNAL(clicked()), this, SLOT(close_form()));
  connect(ui_->deposit_amount, SIGNAL(cursorPositionChanged(int, int)), this,
          SLOT(check_input()));
}

Replenishment::~Replenishment() { delete ui_; }

void Replenishment::close_form() {
  emit delete_button_signal(sender()->parent());
}

void Replenishment::check_input() {
  matcher_valid_ = valid_.match(ui_->deposit_amount->text());
  if (!matcher_valid_.hasMatch()) {
    ui_->deposit_amount->setText(ui_->deposit_amount->text().left(
        ui_->deposit_amount->text().length() - 1));
  }
}
