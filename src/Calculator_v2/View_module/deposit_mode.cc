#include "deposit_mode.h"

#include "ui_deposit_mode.h"

DepositMode::DepositMode(QWidget *parent)
    : QWidget(parent), ui_(new Ui::DepositMode) {
  ui_->setupUi(this);
  deposit_mode_ = ui_->QW_Deposit;
  replenish = new Replenishment();
  setup_defaul_values();
  setup_regular_expressions();
  setup_replenish_withdrawal_layout();
  setup_signal_slot_connections();
}

DepositMode::~DepositMode() { delete ui_; }

QWidget *DepositMode::get_widget_pointer() { return deposit_mode_; }

void DepositMode::remove_widget(QObject *obj) {
  for (int i = 0; i < replenish_layout_->count(); i++) {
    if (QString::pointer(obj) ==
        QString::pointer(replenish_layout_->itemAt(i)->widget())) {
      replenish_layout_->itemAt(i)->widget()->hide();
      replenish_layout_->removeWidget(replenish_layout_->itemAt(i)->widget());
    }
  }
  for (int i = 0; i < withdrawal_layout_->count(); i++) {
    if (QString::pointer(obj) ==
        QString::pointer(withdrawal_layout_->itemAt(i)->widget())) {
      withdrawal_layout_->itemAt(i)->widget()->hide();
      withdrawal_layout_->removeWidget(withdrawal_layout_->itemAt(i)->widget());
    }
  }
}

void DepositMode::switch_interest_capitalization(int state) {
  if (state == Qt::CheckState::Unchecked) {
    ui_->menu_payments_frequency->setEnabled(false);
  } else {
    ui_->menu_payments_frequency->setEnabled(true);
  }
}

void DepositMode::replenish_button() {
  if (replenish_layout_->count() < 15) {
    new_replenish_form_ = new Replenishment();
    connect(new_replenish_form_, SIGNAL(delete_button_signal(QObject *)), this,
            SLOT(remove_widget(QObject *)));
    replenish_layout_->addWidget(new_replenish_form_);
  }
}

void DepositMode::withdrawal_button() {
  if (withdrawal_layout_->count() < 15) {
    new_withdrawal_form_ = new Withdrawal();
    connect(new_withdrawal_form_, SIGNAL(delete_button_signal(QObject *)), this,
            SLOT(remove_widget(QObject *)));
    withdrawal_layout_->addWidget(new_withdrawal_form_);
  }
}

void DepositMode::check_input_field() {
  matcher_total_ = valid_total_.match(ui_->field_deposit_total->text());
  matcher_term_ = valid_term_.match(ui_->field_deposit_term->text());
  matcher_pec_rate_ = valid_rate_.match(ui_->field_interest_rate->text());
  matcher_tax_rate_ = valid_rate_.match(ui_->field_tax_rate->text());

  if (!matcher_total_.hasMatch())
    ui_->field_deposit_total->setText(ui_->field_deposit_total->text().left(
        ui_->field_deposit_total->text().length() - 1));

  if (!matcher_term_.hasMatch())
    ui_->field_deposit_term->setText(ui_->field_deposit_term->text().left(
        ui_->field_deposit_term->text().length() - 1));

  if (!matcher_pec_rate_.hasMatch())
    ui_->field_interest_rate->setText(ui_->field_interest_rate->text().left(
        ui_->field_interest_rate->text().length() - 1));

  if (!matcher_tax_rate_.hasMatch())
    ui_->field_tax_rate->setText(ui_->field_tax_rate->text().left(
        ui_->field_tax_rate->text().length() - 1));
}

void DepositMode::get_input_data() {
  int yy_now = QDate::currentDate().year();
  int mm_now = QDate::currentDate().month();
  int dd_now = QDate::currentDate().day();

  get_replenish_form_data();
  get_withdraw_form_data();

  get_deposit_fields_data();
  result_interest_ = 0.0;
  result_tax_ = 0.0;
  result_end_deposit_ = 0.0;
  if (ui_->field_deposit_term->text().toInt() > 0 &&
      ui_->field_deposit_total->text().toDouble() > 0) {
    emit calculate_deposit_signal(dd_now, mm_now, yy_now, result_interest_,
                                  result_tax_, result_end_deposit_);
  }
  output_deposit_calculation_result();
  emit clear_deposit_data_signal();
}

void DepositMode::output_deposit_calculation_result() {
  QString temp_str;

  temp_str = QString::number(result_interest_ * 100, 'f', 2);
  temp_str.append("  %");
  ui_->label_result_accured_interest_value->setText(temp_str);
  temp_str = QString::number(result_tax_, 'f', 2);
  temp_str.append("  RUB.");
  ui_->label_result_tax_amount_value->setText(temp_str);
  temp_str = QString::number(result_end_deposit_, 'f', 2);
  temp_str.append("  RUB.");
  ui_->label_result_end_deposit_value->setText(temp_str);
}

void DepositMode::get_deposit_fields_data() {
  double deposit_total = ui_->field_deposit_total->text().toDouble();
  int deposit_term = ui_->field_deposit_term->text().toInt();
  double interest_rate = ui_->field_interest_rate->text().toDouble() / 100;
  double tax_rate = ui_->field_tax_rate->text().toDouble() / 100;
  int payments_frequency = ui_->menu_payments_frequency->currentIndex();
  bool interest_capitalization =
      ui_->check_Interest_capitalization->isChecked();
  if (ui_->menu_deopsit_term_units->currentIndex() == 1) {
    deposit_term *= 12;
  }
  emit set_deposit_fields_data_signal(
      deposit_total, deposit_term, interest_rate, tax_rate, payments_frequency,
      interest_capitalization);
}

void DepositMode::get_replenish_form_data() {
  if (replenish_layout_->count() > 0) {
    int yy = 0, mm = 0, dd = 0;
    for (int i = 0; i < replenish_layout_->count(); i++) {
      tmp_widget_ = replenish_layout_->itemAt(i)->widget();
      form_amount_ = tmp_widget_->findChildren<QLineEdit *>("deposit_amount");
      form_term_ =
          tmp_widget_->findChildren<QDateEdit *>("replenishment_date_edit");
      form_switch_ = tmp_widget_->findChildren<QComboBox *>("periodicity_menu");
      form_term_.at(0)->date().getDate(&yy, &mm, &dd);
      double amount = form_amount_.at(0)->text().toDouble();
      int periodicity = form_switch_.at(0)->currentIndex();
      emit add_replenish_signal(amount, dd, mm, yy, periodicity);
      form_amount_.clear();
      form_term_.clear();
      form_switch_.clear();
    }
  }
}

void DepositMode::get_withdraw_form_data() {
  if (withdrawal_layout_->count() > 0) {
    int yy = 0, mm = 0, dd = 0;
    for (int i = 0; i < withdrawal_layout_->count(); i++) {
      tmp_widget_ = withdrawal_layout_->itemAt(i)->widget();
      form_amount_ = tmp_widget_->findChildren<QLineEdit *>("seizure_amount");
      form_term_ = tmp_widget_->findChildren<QDateEdit *>("seizure_date_edit");
      form_term_.at(0)->date().getDate(&yy, &mm, &dd);
      double amount = form_amount_.at(0)->text().toDouble();
      emit add_withdrawal_signal(amount, dd, mm, yy);
      form_amount_.clear();
      form_term_.clear();
      form_switch_.clear();
    }
  }
}

void DepositMode::setup_regular_expressions() {
  valid_total_.setPattern(
      "(^[0-9]{0,1}$)|(\\.[0-9]{1,2}$)|(^[0-9]*$)|(^[0-9]+\\.$)");
  valid_term_.setPattern("(^[0-9]{0,4}$)");
  valid_rate_.setPattern(
      "(^[0-9]{1,2}$)|(^[0-9]+\\.[0-9]{1,2}$)|(^[0-9]{1,2}$)|(^[0-9]{1,2}\\.$"
      ")");
}

void DepositMode::setup_defaul_values() {
  result_interest_ = 0;
  result_tax_ = 0;
  result_end_deposit_ = 0;
  ui_->menu_payments_frequency->setEnabled(false);
}

void DepositMode::setup_replenish_withdrawal_layout() {
  replenish_layout_ = new QGridLayout();
  replenish_layout_->setAlignment(Qt::AlignTop);
  replenish_layout_->setSpacing(0);
  ui_->replen_widget->setLayout(replenish_layout_);

  withdrawal_layout_ = new QGridLayout();
  withdrawal_layout_->setAlignment(Qt::AlignTop);
  withdrawal_layout_->setSpacing(0);
  ui_->withdraw_widget->setLayout(withdrawal_layout_);
}

void DepositMode::setup_signal_slot_connections() {
  connect(ui_->btn_replan, SIGNAL(clicked()), this, SLOT(replenish_button()));
  connect(ui_->btn_withdrawal, SIGNAL(clicked()), this,
          SLOT(withdrawal_button()));
  connect(ui_->field_deposit_total, SIGNAL(cursorPositionChanged(int, int)),
          this, SLOT(check_input_field()));
  connect(ui_->field_deposit_term, SIGNAL(cursorPositionChanged(int, int)),
          this, SLOT(check_input_field()));
  connect(ui_->field_interest_rate, SIGNAL(cursorPositionChanged(int, int)),
          this, SLOT(check_input_field()));
  connect(ui_->field_tax_rate, SIGNAL(cursorPositionChanged(int, int)), this,
          SLOT(check_input_field()));
  connect(ui_->btn_calculate, SIGNAL(clicked()), this, SLOT(get_input_data()));
  connect(ui_->check_Interest_capitalization, SIGNAL(stateChanged(int)), this,
          SLOT(switch_interest_capitalization(int)));
}
