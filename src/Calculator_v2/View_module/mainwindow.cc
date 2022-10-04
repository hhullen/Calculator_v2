#include "mainwindow.h"

#include "./ui_mainwindow.h"

MainWindow::MainWindow(CalculatorController *controller, QWidget *parent)
    : controller_(controller), QMainWindow(parent), ui_(new Ui::MainWindow) {
  ui_->setupUi(this);
  setWindowFlag(Qt::FramelessWindowHint, Qt::WA_TranslucentBackground);
  set_default_field_values();
  setup_widgets();
  setup_time_date();
  setup_signal_slot_connections();
}

MainWindow::~MainWindow() { delete ui_; }

void MainWindow::update_time() {
  ui_->TimeDateNow->setText(
      p_date_time_->currentDateTime().toString("yyyy MMMM dd hh:mm"));
}

void MainWindow::open_up_window() {
  hide();
  show();
}

void MainWindow::main_menu(int index) {
  if (index == 0) {
    if (p_calculator_widget_->pos() != zero_pos_) {
      calculator_x_pos_ = window_width_;
      animate_widget(p_calc_animation_, &calculator_x_pos_, 0);
    }
    if (p_credit_widget_->pos() == zero_pos_)
      animate_widget(p_credit_animation_, &deposit_x_pos_, -window_width_);
    if (p_deposit_widget_->pos() == zero_pos_)
      animate_widget(p_deposit_animation_, &deposit_x_pos_, -window_width_);
  } else if (index == 1) {
    if (p_credit_widget_->pos() != zero_pos_) {
      deposit_x_pos_ = window_width_;
      animate_widget(p_credit_animation_, &deposit_x_pos_, 0);
    }
    if (p_calculator_widget_->pos() == zero_pos_)
      animate_widget(p_calc_animation_, &calculator_x_pos_, -window_width_);
    if (p_deposit_widget_->pos() == zero_pos_)
      animate_widget(p_deposit_animation_, &deposit_x_pos_, -window_width_);
  } else if (index == 2) {
    if (p_deposit_widget_->pos() != zero_pos_) {
      deposit_x_pos_ = window_width_;
      animate_widget(p_deposit_animation_, &deposit_x_pos_, 0);
    }
    if (p_calculator_widget_->pos() == zero_pos_)
      animate_widget(p_calc_animation_, &calculator_x_pos_, -window_width_);
    if (p_credit_widget_->pos() == zero_pos_)
      animate_widget(p_credit_animation_, &deposit_x_pos_, -window_width_);
  }
}

double MainWindow::calculate_expression(list<string> *expression, double x) {
  double returnable = 0;
  if (expression) {
    returnable = controller_->calculate_expression(expression, x);
  }
  return returnable;
}

void MainWindow::calculate_range(std::list<std::string> *expression, int from,
                                 int to, std::vector<double> &x,
                                 std::vector<double> &y) {
  if (expression) {
    controller_->calculate_range(expression, from, to, x, y);
  }
}

void MainWindow::calculate_credit(double total_in, double term_in,
                                  double rate_in, double *mon_pay_high,
                                  double *mon_pay_low, double *overpayment,
                                  double *total_payment, int mode) {
  if (mode == CreditType::ANNUITY) {
    controller_->calculate_credit(total_in, term_in, rate_in, mon_pay_high,
                                  mon_pay_low, overpayment, total_payment,
                                  s21::CreditMode::ANNUITY);
  } else if (mode == CreditType::DIFFERENTIATED) {
    controller_->calculate_credit(total_in, term_in, rate_in, mon_pay_high,
                                  mon_pay_low, overpayment, total_payment,
                                  s21::CreditMode::DIFFERENTIATED);
  }
}

void MainWindow::add_withdrawal(double deposit, int dd, int mm, int yy) {
  controller_->add_withdrawal(deposit, dd, mm, yy);
}

void MainWindow::add_replenish(double deposit, int dd, int mm, int yy,
                               int periodicity) {
  if (periodicity == Periodicity::AT_ONCE) {
    controller_->add_replenish(deposit, dd, mm, yy,
                               s21::ActPeriodicity::AT_ONCE);
  } else if (periodicity == Periodicity::EVERY_MONTH) {
    controller_->add_replenish(deposit, dd, mm, yy,
                               s21::ActPeriodicity::EVERY_MONTH);
  } else if (periodicity == Periodicity::EVERY_YEAR) {
    controller_->add_replenish(deposit, dd, mm, yy,
                               s21::ActPeriodicity::EVERY_YEAR);
  }
}

void MainWindow::set_deposit_fields_data(double deposit, int term,
                                         double interest_rate, double tax_rate,
                                         int pay_frequency,
                                         bool capitalization) {
  if (pay_frequency == PaymentsFrequency::MONTHLY) {
    controller_->set_deposit_fields_data(deposit, term, interest_rate, tax_rate,
                                         s21::PayFrequency::MONTHLY,
                                         capitalization);
  } else if (pay_frequency == PaymentsFrequency::QUARTERLY) {
    controller_->set_deposit_fields_data(deposit, term, interest_rate, tax_rate,
                                         s21::PayFrequency::QUARTERLY,
                                         capitalization);
  } else if (pay_frequency == PaymentsFrequency::ANNUALY) {
    controller_->set_deposit_fields_data(deposit, term, interest_rate, tax_rate,
                                         s21::PayFrequency::ANNUALY,
                                         capitalization);
  }
}

void MainWindow::calculate_deposit(int dd, int mm, int yy,
                                   double &result_interest, double &result_tax,
                                   double &result_end_deposit) {
  controller_->calculate_deposit(dd, mm, yy, result_interest, result_tax,
                                 result_end_deposit);
}

void MainWindow::clear_deposit_data() { controller_->clear_deposit_data(); }

void MainWindow::animate_widget(QPropertyAnimation *widget, int *current,
                                int target) {
  widget->setStartValue(QRect(*current, 0, 0, 0));
  widget->setEndValue(QRect(target, 0, 0, 0));
  widget->start();
  *current = target;
}

void MainWindow::set_default_field_values() {
  window_width_ = size().width();
  calculator_x_pos_ = 0;
  credit_x_pos_ = 0;
  deposit_x_pos_ = 0;
  mouse_old_x_ = 0;
  mouse_old_y_ = 0;
  dx_ = 0;
  dy_ = 0;
  zero_pos_.setX(0);
  zero_pos_.setY(0);
}

void MainWindow::setup_widgets() {
  p_calculator_mode_ = new CalculatorMode();
  p_credit_mode_ = new CreditMode();
  p_deposit_mode_ = new DepositMode();
  p_calculator_widget_ = p_calculator_mode_->get_widget_pointer();
  p_credit_widget_ = p_credit_mode_->get_widget_pointer();
  p_deposit_widget_ = p_deposit_mode_->get_widget_pointer();
  ui_->layout->addWidget(p_calculator_widget_);
  ui_->layout->addWidget(p_credit_widget_);
  ui_->layout->addWidget(p_deposit_widget_);

  p_calc_animation_ = new QPropertyAnimation(p_calculator_widget_, "geometry");
  p_credit_animation_ = new QPropertyAnimation(p_credit_widget_, "geometry");
  p_deposit_animation_ = new QPropertyAnimation(p_deposit_widget_, "geometry");
  p_calc_animation_->setDuration(100);
  p_credit_animation_->setDuration(100);
  p_deposit_animation_->setDuration(100);
}

void MainWindow::setup_time_date() {
  p_date_time_ = new QDateTime();
  p_timer_ = new QTimer();
  connect(p_timer_, SIGNAL(timeout()), this, SLOT(update_time()));
  p_timer_->start(1000);
  update_time();
}

void MainWindow::setup_signal_slot_connections() {
  connect(ui_->Main_menu, SIGNAL(activated(int)), this, SLOT(main_menu(int)));
  connect(p_calculator_mode_,
          SIGNAL(get_result_signal(list<std::string> *, double)), this,
          SLOT(calculate_expression(list<std::string> *, double)));
  connect(p_calculator_mode_,
          SIGNAL(get_graph_data_signal(list<std::string> *, int, int,
                                       vector<double> &, vector<double> &)),
          this,
          SLOT(calculate_range(list<std::string> *, int, int, vector<double> &,
                               vector<double> &)));
  connect(p_calculator_mode_, SIGNAL(open_the_window_signal()), this,
          SLOT(open_up_window()));
  connect(p_credit_mode_,
          SIGNAL(calculate_credit_singal(double, double, double, double *,
                                         double *, double *, double *, int)),
          this,
          SLOT(calculate_credit(double, double, double, double *, double *,
                                double *, double *, int)));
  connect(p_deposit_mode_, SIGNAL(add_withdrawal_signal(double, int, int, int)),
          this, SLOT(add_withdrawal(double, int, int, int)));
  connect(p_deposit_mode_,
          SIGNAL(add_replenish_signal(double, int, int, int, int)), this,
          SLOT(add_replenish(double, int, int, int, int)));
  connect(
      p_deposit_mode_,
      SIGNAL(set_deposit_fields_data_signal(double, int, double, double, int,
                                            bool)),
      this,
      SLOT(set_deposit_fields_data(double, int, double, double, int, bool)));
  connect(p_deposit_mode_,
          SIGNAL(calculate_deposit_signal(int, int, int, double &, double &,
                                          double &)),
          this,
          SLOT(calculate_deposit(int, int, int, double &, double &, double &)));
  connect(p_deposit_mode_, SIGNAL(clear_deposit_data_signal()), this,
          SLOT(clear_deposit_data()));
}
