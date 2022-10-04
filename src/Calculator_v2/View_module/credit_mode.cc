#include "credit_mode.h"

CreditMode::CreditMode(QWidget *parent)
    : QWidget(parent), ui_(new Ui::CreditMode) {
  ui_->setupUi(this);
  credit_mode_ = ui_->QW_Credit;
  setup_regular_expressions();
  setup_defaul_values();
  setup_signal_slot_connections();
}

CreditMode::~CreditMode() { delete ui_; }

QWidget *CreditMode::get_widget_pointer() { return credit_mode_; }

void CreditMode::check_input_field() {
  matcher_total_ = valid_total_.match(ui_->field_credit_total->text());
  matcher_term_ = valid_term_.match(ui_->field_credit_term->text());
  matcher_rate_ = valid_rate_.match(ui_->field_Interest_rate->text());

  if (!matcher_total_.hasMatch())
    ui_->field_credit_total->setText(ui_->field_credit_total->text().left(
        ui_->field_credit_total->text().length() - 1));

  if (!matcher_term_.hasMatch())
    ui_->field_credit_term->setText(ui_->field_credit_term->text().left(
        ui_->field_credit_term->text().length() - 1));

  if (!matcher_rate_.hasMatch())
    ui_->field_Interest_rate->setText(ui_->field_Interest_rate->text().left(
        ui_->field_Interest_rate->text().length() - 1));
}

void CreditMode::get_input_data() {
  credit_total_ = ui_->field_credit_total->text().toDouble();
  credit_term_ = ui_->field_credit_term->text().toDouble();
  interest_rate_ = ui_->field_Interest_rate->text().toDouble();

  if (ui_->menu_term->currentIndex() == 1) credit_term_ *= 12;

  if (ui_->menu_pay_type->currentIndex() == 0) {
    emit calculate_credit_singal(credit_total_, credit_term_, interest_rate_,
                                 &mon_pay_high_, &mon_pay_low_, &overpayment_,
                                 &total_payment_, CreditType::ANNUITY);
    write_values_to_fields(mon_pay_high_, mon_pay_low_, overpayment_,
                           total_payment_, CreditType::ANNUITY);
  } else if (ui_->menu_pay_type->currentIndex() == 1) {
    emit calculate_credit_singal(credit_total_, credit_term_, interest_rate_,
                                 &mon_pay_high_, &mon_pay_low_, &overpayment_,
                                 &total_payment_, CreditType::DIFFERENTIATED);
    write_values_to_fields(mon_pay_high_, mon_pay_low_, overpayment_,
                           total_payment_, CreditType::DIFFERENTIATED);
  }
}

void CreditMode::write_values_to_fields(double mon_pay_high, double mon_pay_low,
                                        double overpayment,
                                        double total_payment, int mode) {
  if (mon_pay_high != mon_pay_high) {
    mon_pay_high = 0;
  }
  if (mon_pay_low != mon_pay_low) {
    mon_pay_low = 0;
  }
  if (overpayment != overpayment) {
    overpayment = 0;
  }
  if (total_payment != total_payment) {
    total_payment = 0;
  }

  if (mode == CreditType::ANNUITY) {
    ui_->field_result_monthly_payment->setText(
        QString::number(mon_pay_high, 'f', 2));
    ui_->field_result_overpayment->setText(
        QString::number(overpayment, 'f', 2));
    ui_->field_result_total_payment->setText(
        QString::number(total_payment, 'f', 2));
  } else if (mode == CreditType::DIFFERENTIATED) {
    ui_->field_result_monthly_payment->setText(
        QString::number(mon_pay_high, 'f', 2));
    ui_->field_result_monthly_payment->setText(
        ui_->field_result_monthly_payment->text().append("..."));
    ui_->field_result_monthly_payment->setText(
        ui_->field_result_monthly_payment->text().append(
            QString::number(mon_pay_low, 'f', 2)));
    ui_->field_result_overpayment->setText(
        QString::number(overpayment, 'f', 2));
    ui_->field_result_total_payment->setText(
        QString::number(total_payment, 'f', 2));
  }
}

void CreditMode::setup_regular_expressions() {
  valid_total_.setPattern(
      "(^[0-9]{0,1}$)|(\\.[0-9]{1,2}$)|(^[0-9]*$)|(^[0-9]+\\.$)");
  valid_term_.setPattern("(^[0-9]{0,4}$)");
  valid_rate_.setPattern(
      "(^[0-9]{1,2}$)|(^[0-9]+\\.[0-9]{1,2}$)|(^[0-9]{1,2}$)|(^[0-9]{1,2}\\.$"
      ")");
}

void CreditMode::setup_defaul_values() {
  credit_total_ = 0;
  credit_term_ = 0;
  interest_rate_ = 0;
  mon_pay_high_ = 0;
  mon_pay_low_ = 0;
  overpayment_ = 0;
  total_payment_ = 0;
}

void CreditMode::setup_signal_slot_connections() {
  connect(ui_->field_credit_total, SIGNAL(cursorPositionChanged(int, int)),
          this, SLOT(check_input_field()));
  connect(ui_->field_credit_term, SIGNAL(cursorPositionChanged(int, int)), this,
          SLOT(check_input_field()));
  connect(ui_->field_Interest_rate, SIGNAL(cursorPositionChanged(int, int)),
          this, SLOT(check_input_field()));
  connect(ui_->btn_calculate, SIGNAL(clicked()), this, SLOT(get_input_data()));
}
