#include "calculator_mode.h"

#include "ui_calculator_mode.h"

CalculatorMode::CalculatorMode(QWidget *parent)
    : QWidget(parent), ui_(new Ui::CalculatorMode) {
  ui_->setupUi(this);

  calculator_ = ui_->QW_Calculator;
  graph_view = new QCPlotic();
  brackets_ = 0;
  lexem_counter_ = 0;
  expression_lexems_.clear();
  setup_regular_expressions_patterns();
  setup_signal_slot_connections();
}

CalculatorMode::~CalculatorMode() { delete ui_; }

QWidget *CalculatorMode::get_widget_pointer() { return calculator_; }

void CalculatorMode::input_numpad(QAbstractButton *btn) {
  if (is_calculator_widget_active()) {
    QLineEdit *p_editable = get_editable_field(InputType::NUMBER);
    if (is_permitted(btn->text())) {
      is_multiply_needed(p_editable);
      QRegularExpressionMatch change = p_to_change_->match(p_editable->text());
      QRegularExpressionMatch put = p_to_put_->match(p_editable->text());
      QRegularExpressionMatch put_add_zero =
          expression_num_add_zero_.match(p_editable->text());

      if (btn->text() == ".") {
        QRegularExpressionMatch hasnt_point_ =
            expression_no_doublepoint_.match(p_editable->text());
        if (put_add_zero.hasMatch()) {
          put_lexem(p_editable, "0", PutLexemMode::USUAL);
          put_lexem(p_editable, btn->text(), PutLexemMode::USUAL);
        } else if (hasnt_point_.hasMatch()) {
          put_lexem(p_editable, btn->text(), PutLexemMode::USUAL);
        }
      } else if (change.hasMatch()) {
        backspace(p_editable);
        put_lexem(p_editable, btn->text(), PutLexemMode::USUAL);
      } else if (put.hasMatch()) {
        put_lexem(p_editable, btn->text(), PutLexemMode::USUAL);
      }
    }
  }
}

void CalculatorMode::input_operator(QAbstractButton *btn) {
  if (is_calculator_widget_active()) {
    QLineEdit *p_editable = get_editable_field(InputType::OPERATOR);
    if (is_permitted(btn->text())) {
      is_extra_zero_needed(p_editable);
      QRegularExpressionMatch change = p_to_change_->match(p_editable->text());
      QRegularExpressionMatch put = p_to_put_->match(p_editable->text());

      if (btn->text() == "-" && !put.hasMatch()) {
        put = expression_operator_unary_.match(p_editable->text());
        if (put.hasMatch()) {
          put_lexem(p_editable, "−", PutLexemMode::USUAL);
        } else if (change.hasMatch()) {
          backspace(p_editable);
          put_lexem(p_editable, btn->text(), PutLexemMode::USUAL);
        }
      } else if (change.hasMatch()) {
        backspace(p_editable);
        put_lexem(p_editable, btn->text(), PutLexemMode::USUAL);
      } else if (put.hasMatch()) {
        put_lexem(p_editable, btn->text(), PutLexemMode::USUAL);
      }
    }
  }
}

void CalculatorMode::input_variables(QAbstractButton *btn) {
  if (is_calculator_widget_active()) {
    QLineEdit *p_editable = ui_->field_expression;
    p_editable->setFocus();
    QRegularExpressionMatch put =
        expression_variables_put_.match(p_editable->text());
    QRegularExpressionMatch multiply =
        expression_variables_multiply_.match(p_editable->text());
    QRegularExpressionMatch zero_multiply =
        expression_add_zero_.match(p_editable->text());

    if (zero_multiply.hasMatch()) {
      put_lexem(p_editable, "0", PutLexemMode::USUAL);
      put_lexem(p_editable, "×", PutLexemMode::USUAL);
      put_lexem(p_editable, btn->text(), PutLexemMode::USUAL);
    } else if (multiply.hasMatch()) {
      put_lexem(p_editable, "×", PutLexemMode::USUAL);
      put_lexem(p_editable, btn->text(), PutLexemMode::USUAL);
    } else if (put.hasMatch()) {
      put_lexem(p_editable, btn->text(), PutLexemMode::USUAL);
    }
  }
}

void CalculatorMode::input_functions(QAbstractButton *btn) {
  if (is_calculator_widget_active()) {
    QLineEdit *p_editable = ui_->field_expression;
    p_editable->setFocus();
    QRegularExpressionMatch put =
        expression_variables_put_.match(p_editable->text());
    QRegularExpressionMatch multiply =
        expression_variables_multiply_.match(p_editable->text());
    QRegularExpressionMatch zero_multiply =
        expression_add_zero_.match(p_editable->text());

    if (zero_multiply.hasMatch()) {
      put_lexem(p_editable, "0", PutLexemMode::USUAL);
      put_lexem(p_editable, "×", PutLexemMode::USUAL);
      put_lexem(p_editable, btn->text(), PutLexemMode::USUAL);
      put_lexem(p_editable, "(", PutLexemMode::USUAL);
      ++brackets_;
    } else if (multiply.hasMatch()) {
      put_lexem(p_editable, "×", PutLexemMode::USUAL);
      put_lexem(p_editable, btn->text(), PutLexemMode::USUAL);
      put_lexem(p_editable, "(", PutLexemMode::USUAL);
      ++brackets_;
    } else if (put.hasMatch()) {
      put_lexem(p_editable, btn->text(), PutLexemMode::USUAL);
      put_lexem(p_editable, "(", PutLexemMode::USUAL);
      ++brackets_;
    }
  }
}

void CalculatorMode::pressed_backspace() {
  if (is_calculator_widget_active()) {
    QLineEdit *p_editable = get_editable_field(InputType::ERASE);
    backspace(p_editable);
  }
}

void CalculatorMode::pressed_brackets() {
  if (is_calculator_widget_active()) {
    QLineEdit *p_editable = ui_->field_expression;
    p_editable->setFocus();
    QRegularExpressionMatch rbracket =
        expression_rbracket_put_.match(p_editable->text());
    QRegularExpressionMatch lbracket =
        expression_lbracket_put_.match(p_editable->text());
    QRegularExpressionMatch multiply =
        expression_lbracket_multiply_.match(p_editable->text());
    QRegularExpressionMatch zero_multiply =
        expression_add_zero_.match(p_editable->text());

    if (rbracket.hasMatch() && brackets_ > 0) {
      put_lexem(p_editable, ")", PutLexemMode::USUAL);
      --brackets_;
    } else if (zero_multiply.hasMatch()) {
      put_lexem(p_editable, "0", PutLexemMode::USUAL);
      put_lexem(p_editable, "×", PutLexemMode::USUAL);
      put_lexem(p_editable, "(", PutLexemMode::USUAL);
      ++brackets_;
    } else if (multiply.hasMatch()) {
      put_lexem(p_editable, "×", PutLexemMode::USUAL);
      put_lexem(p_editable, "(", PutLexemMode::USUAL);
      ++brackets_;
    } else if (lbracket.hasMatch()) {
      put_lexem(p_editable, "(", PutLexemMode::USUAL);
      ++brackets_;
    }
  }
}

void CalculatorMode::pressed_clean() {
  if (is_calculator_widget_active()) {
    ui_->field_expression->clear();
    brackets_ = 0;
    lexem_counter_ = 0;
    expression_lexems_.clear();
  }
}

void CalculatorMode::pressed_clean_static_x() {
  if (is_calculator_widget_active()) {
    ui_->static_x->clear();
    ui_->static_x->setText("0");
  }
}

void CalculatorMode::pressed_equal() {
  if (is_calculator_widget_active()) {
    consummate_expression();
    double variable_x = ui_->static_x->text().toDouble();
    double result = 0;
    if (!expression_lexems_.empty()) {
      result = emit get_result_signal(&expression_lexems_, variable_x);
    }
    QString result_string = QString::number(result, 'f', 7);
    truncate_result(result_string);
    ui_->field_result->setText(result_string);
  }
}

void CalculatorMode::update_graph_button() {
  ui_->btn_draw_graph->setText("Open graph view");
}

void CalculatorMode::show_calculator_window() { emit open_the_window_signal(); }

void CalculatorMode::edited_range1_high(const QString &str) {
  adjust_range_field_value(ui_->range1_high, kGRAPH_RANGE_HIGH,
                           kGRAPH_RANGE_LOW + 1, kMIN_HIGH, kMAX_HIGH);
  adjust_range(ui_->range1_high, ui_->range1_low, 1, RangeFieldType::HIGH);
}

void CalculatorMode::edited_range1_low(const QString &str) {
  adjust_range_field_value(ui_->range1_low, kGRAPH_RANGE_HIGH - 1,
                           kGRAPH_RANGE_LOW, kMIN_LOW, kMAX_LOW);
  adjust_range(ui_->range1_low, ui_->range1_high, 1, RangeFieldType::LOW);
}

void CalculatorMode::edited_range2_high(const QString &str) {
  adjust_range_field_value(ui_->range2_high, kGRAPH_RANGE_HIGH,
                           kGRAPH_RANGE_LOW + 1, kMIN_HIGH, kMAX_HIGH);
  adjust_range(ui_->range2_high, ui_->range2_low, 1, RangeFieldType::HIGH);
}

void CalculatorMode::edited_range2_low(const QString &str) {
  adjust_range_field_value(ui_->range2_low, kGRAPH_RANGE_HIGH - 1,
                           kGRAPH_RANGE_LOW, kMIN_LOW, kMAX_LOW);
  adjust_range(ui_->range2_low, ui_->range2_high, 1, RangeFieldType::LOW);
}

void CalculatorMode::set_line_1_expression(QAbstractButton *btn) {
  graph_expression_manage(btn, ui_->expression1, &graph_1_expression_);
}

void CalculatorMode::set_line_2_expression(QAbstractButton *btn) {
  graph_expression_manage(btn, ui_->expression2, &graph_2_expression_);
}

void CalculatorMode::graph_expression_manage(QAbstractButton *btn,
                                             QLabel *field,
                                             list<string> *graph_expression) {
  consummate_expression();
  QString expression = ui_->field_expression->text();
  if (!expression.isEmpty() && btn->text() == "get") {
    field->setText(expression);
    *graph_expression = expression_lexems_;
  } else if (btn->text() == "x") {
    field->clear();
    field->setText("-");
    graph_expression->clear();
  }
}

void CalculatorMode::draw_graph() {
  QString line1 = ui_->expression1->text();
  QString line2 = ui_->expression2->text();
  int from_1 = 0, to_1 = 0, from_2 = 0, to_2 = 0;
  get_ranges_data(from_1, to_1, from_2, to_2);
  get_line_data(line1, graph_1_expression_, from_1, to_1, x1, y1);
  get_line_data(line2, graph_2_expression_, from_2, to_2, x2, y2);
  ui_->btn_draw_graph->setText("Update graph view");
  graph_view->draw_graph(line1, x1, y1, line2, x2, y2);
  x1.clear();
  y1.clear();
  x2.clear();
  y2.clear();
}

void CalculatorMode::get_ranges_data(int &from_1, int &to_1, int &from_2,
                                     int &to_2) {
  get_range_field(ui_->range1_low, from_1);
  get_range_field(ui_->range1_high, to_1);
  get_range_field(ui_->range2_low, from_2);
  get_range_field(ui_->range2_high, to_2);
}

bool CalculatorMode::is_empty_range_field(QString value) {
  bool returnable = false;
  if (value.length() == 0 ||
      (value.length() == 1 && value.at(value.length() - 1) == '-')) {
    returnable = true;
  }
  return returnable;
}

void CalculatorMode::get_range_field(QLineEdit *field, int &value) {
  if (is_empty_range_field(field->text())) {
    field->setText("0");
    value = 0;
  } else {
    value = field->text().toInt();
  }
}

void CalculatorMode::get_line_data(QString &line, list<string> &expression,
                                   int from, int to, vector<double> &x,
                                   vector<double> &y) {
  if (!is_empty_range_field(line)) {
    emit get_graph_data_signal(&expression, from, to, x, y);
  }
}

bool CalculatorMode::is_calculator_widget_active() {
  return ui_->QW_Calculator->pos().x() == 0;
}

QLineEdit *CalculatorMode::get_editable_field(InputType type) {
  QLineEdit *returnable;
  if (ui_->static_x->hasFocus()) {
    returnable = ui_->static_x;
    setup_validator(FieldType::DOUBLE, type);
  } else if (ui_->range1_low->hasFocus()) {
    returnable = ui_->range1_low;
    setup_validator(FieldType::RANGE, type);
  } else if (ui_->range1_high->hasFocus()) {
    returnable = ui_->range1_high;
    setup_validator(FieldType::RANGE, type);
  } else if (ui_->range2_low->hasFocus()) {
    returnable = ui_->range2_low;
    setup_validator(FieldType::RANGE, type);
  } else if (ui_->range2_high->hasFocus()) {
    returnable = ui_->range2_high;
    setup_validator(FieldType::RANGE, type);
  } else {
    ui_->field_expression->setFocus();
    returnable = ui_->field_expression;
    setup_validator(FieldType::EXPRESSION, type);
  }
  return returnable;
}

void CalculatorMode::is_multiply_needed(QLineEdit *editable) {
  if (p_to_put_multiply_) {
    QRegularExpressionMatch put_multiply =
        p_to_put_multiply_->match(editable->text());
    if (put_multiply.hasMatch()) {
      editable->setText(editable->text().append("×"));
    }
  }
}

void CalculatorMode::is_extra_zero_needed(QLineEdit *editable) {
  QRegularExpressionMatch put_add_zero =
      expression_add_zero_.match(editable->text());

  if (put_add_zero.hasMatch()) {
    put_lexem(editable, "0", PutLexemMode::USUAL);
  }
}

bool CalculatorMode::is_permitted(QString lexem) {
  QRegularExpressionMatch permitted = p_to_put_permitted_->match(lexem);
  return permitted.hasMatch();
}

void CalculatorMode::setup_validator(FieldType field, InputType type) {
  if (field == FieldType::EXPRESSION) {
    if (type == InputType::NUMBER) {
      setup_expression_num_validator();
    } else if (type == InputType::OPERATOR) {
      setup_expression_operator_validator();
    }
  } else if (field == FieldType::RANGE) {
    if (type == InputType::NUMBER) {
      setup_range_num_validator();
    } else if (type == InputType::OPERATOR) {
      setup_range_operator_validator();
    }
  } else if (field == FieldType::DOUBLE) {
    if (type == InputType::NUMBER) {
      setup_double_num_validator();
    } else if (type == InputType::OPERATOR) {
      setup_double_operator_validator();
    }
  }
}

void CalculatorMode::setup_range_num_validator() {
  p_to_put_ = &range_num_put_;
  p_to_change_ = &expression_num_change_;
  p_to_put_multiply_ = &null_regular_;
  p_to_put_permitted_ = &range_permitted_;
}

void CalculatorMode::setup_range_operator_validator() {
  p_to_put_ = &range_operator_put_;
  p_to_change_ = &range_operator_change_;
  p_to_put_multiply_ = &null_regular_;
  p_to_put_permitted_ = &range_permitted_;
}

void CalculatorMode::setup_expression_num_validator() {
  p_to_put_ = &expression_num_put_;
  p_to_change_ = &expression_num_change_;
  p_to_put_multiply_ = &expression_num_multiply_;
  p_to_put_permitted_ = &expression_permitted_;
}

void CalculatorMode::setup_expression_operator_validator() {
  p_to_put_ = &expression_operator_put_;
  p_to_change_ = &expression_operator_change_;
  p_to_put_multiply_ = &null_regular_;
  p_to_put_permitted_ = &expression_permitted_;
}

void CalculatorMode::setup_double_num_validator() {
  p_to_put_ = &double_num_put_;
  p_to_change_ = &expression_num_change_;
  p_to_put_multiply_ = &null_regular_;
  p_to_put_permitted_ = &double_permitted_;
}

void CalculatorMode::setup_double_operator_validator() {
  p_to_put_ = &range_operator_put_;
  p_to_change_ = &range_operator_change_;
  p_to_put_multiply_ = &null_regular_;
  p_to_put_permitted_ = &double_permitted_;
}

void CalculatorMode::backspace(QLineEdit *editable) {
  bool match = false;
  for (int i = 4; i >= 0 && !match; --i) {
    QRegularExpressionMatch deletion = sym_deletion_[i].match(editable->text());
    if (deletion.hasMatch()) {
      erase_lexem(editable, i + 1);
      remove_expression_lexem(editable, deletion.captured(0));
      match = true;
    }
  }
}

void CalculatorMode::erase_lexem(QLineEdit *editable, int n) {
  int str_length = editable->text().length();
  if (str_length > 0 && editable->text().at(str_length - 1) == ')') {
    ++brackets_;
  } else if (str_length > 0 && editable->text().at(str_length - 1) == '(') {
    --brackets_;
  }
  editable->setText(editable->text().left(editable->text().length() - n));
  if (lexem_counter_ >= 0) {
    --lexem_counter_;
  }
}

void CalculatorMode::put_lexem(QLineEdit *editable, const QString text,
                               PutLexemMode mode) {
  if ((lexem_counter_ <= 255 && mode == PutLexemMode::USUAL) ||
      mode == PutLexemMode::OVERRIDE) {
    editable->setText(editable->text().append(text));
    add_expression_lexem(editable, text);
    ++lexem_counter_;
  }
}

void CalculatorMode::add_expression_lexem(const QLineEdit *editable,
                                          const QString text) {
  string pushable = text.toStdString();
  if (editable == ui_->field_expression) {
    if (pushable == "−") {
      pushable = "_";
    } else if (pushable == "÷") {
      pushable = "/";
    } else if (pushable == "×") {
      pushable = "*";
    } else if (pushable == "π") {
      pushable = "PI";
    } else if (pushable == "√") {
      pushable = "sqrt";
    }
    expression_lexems_.push_back(pushable);
  }
}

void CalculatorMode::remove_expression_lexem(const QLineEdit *editable,
                                             const QString deletable) {
  if (editable == ui_->field_expression) {
    if ((deletable.length() > 2 && deletable.contains("(")) ||
        (deletable.length() > 0 && deletable.contains("√"))) {
      expression_lexems_.pop_back();
      expression_lexems_.pop_back();
    } else {
      expression_lexems_.pop_back();
    }
  }
}

void CalculatorMode::adjust_range_field_value(QLineEdit *field, int max,
                                              int min, QString set_min,
                                              QString set_max) {
  int value = field->text().toInt();
  if (value < min) {
    field->setText(set_min);
  } else if (value > max) {
    field->setText(set_max);
  }
}

void CalculatorMode::adjust_range(const QLineEdit *edited, QLineEdit *editable,
                                  int min_difference, RangeFieldType mode) {
  int checking = edited->text().toInt();
  int verifiable = editable->text().toInt();
  if (mode == RangeFieldType::HIGH) {
    if (checking <= verifiable) {
      editable->setText(QString::number(checking - min_difference));
    }
  } else if (mode == RangeFieldType::LOW) {
    if (checking >= verifiable) {
      editable->setText(QString::number(checking + min_difference));
    }
  }
}

void CalculatorMode::consummate_expression() {
  QLineEdit *p_editable = ui_->field_expression;
  p_editable->setFocus();
  QRegularExpressionMatch operator_end =
      expression_bad_end_.match(p_editable->text());
  while (operator_end.hasMatch()) {
    backspace(p_editable);
    operator_end = expression_bad_end_.match(p_editable->text());
  }
  if (p_editable->text().length() > 0) {
    QChar sym_end = p_editable->text().at(p_editable->text().length() - 1);
    if (sym_end == '.') {
      put_lexem(p_editable, "0", PutLexemMode::OVERRIDE);
    }
  }
  while (brackets_ > 0) {
    put_lexem(p_editable, ")", PutLexemMode::OVERRIDE);
    --brackets_;
  }
}

void CalculatorMode::truncate_result(QString &result_string) {
  while (result_string.length() > 0 &&
         result_string.at(result_string.length() - 1) == '0') {
    result_string = result_string.left(result_string.length() - 1);
  }
  if (result_string.length() > 0 &&
      result_string.at(result_string.length() - 1) == '.') {
    result_string = result_string.left(result_string.length() - 1);
  }
}

void CalculatorMode::setup_regular_expressions_patterns() {
  expression_num_put_.setPattern(
      "(^$)|([0-9\\(\\−\\-\\+\\÷\\×\\.\\^]$)|(mod$)");
  expression_num_change_.setPattern("(^0$)|([^\\d\\.]0$)");
  expression_num_multiply_.setPattern("([\\)\\πX]$)");
  expression_num_add_zero_.setPattern("(^$)|([^\\d\\.]$)");
  expression_no_doublepoint_.setPattern("(^[0-9]*$)|([^\\d\\.][0-9]*$)");

  expression_operator_put_.setPattern("([0-9\\)\\πX]$)");
  expression_operator_change_.setPattern("([\\-\\+\\÷\\×\\^]$)|(mod$)");
  expression_operator_unary_.setPattern("(^$)|([\\(]$)");
  expression_add_zero_.setPattern("([\\.]$)");

  expression_rbracket_put_.setPattern("([0-9X\\π\\)]$)");
  expression_lbracket_put_.setPattern("(^$)|([\\+\\−\\-\\÷\\×\\^\\(]$)|(mod$)");
  expression_lbracket_multiply_.setPattern("([0-9X\\)\\π]$)");

  expression_variables_put_.setPattern(
      "(^$)|([\\+\\−\\-\\÷\\×\\^\\(]$)|(mod$)");
  expression_variables_multiply_.setPattern("([0-9X\\)\\π]$)");

  range_num_put_.setPattern("(^[\\-]*[0-9]{0,7}$)");
  range_operator_put_.setPattern("(^$)");
  range_operator_change_.setPattern("(^0$)");

  double_num_put_.setPattern(
      "(^[\\-]{0,1}[^\\.]*$)|([\\-]{0,1}[0-9]*[\\.][0-9]{0,6}$)");

  expression_permitted_.setPattern(".*");
  range_permitted_.setPattern("([0-9\\-])");
  double_permitted_.setPattern("([0-9\\-\\.])");

  sym_deletion_[0].setPattern("([0-9\\+\\−\\-\\÷\\×\\(\\)\\^\\π\\.X]$)");
  sym_deletion_[1].setPattern("(\\√\\($)");
  sym_deletion_[2].setPattern("(mod$)|(ln\\($)");
  sym_deletion_[3].setPattern("(sin\\($)|(cos\\($)|(tan\\($)|(log\\($)");
  sym_deletion_[4].setPattern("(asin\\($)|(acos\\($)|(atan\\($)");
  null_regular_.setPattern("\\\\");
  expression_bad_end_.setPattern("([\\+\\−\\-\\÷\\×\\^\\()]$)|(mod$)");
}

void CalculatorMode::setup_signal_slot_connections() {
  connect(ui_->nums_button_group, SIGNAL(buttonClicked(QAbstractButton *)),
          this, SLOT(input_numpad(QAbstractButton *)));
  connect(ui_->operators_button_group, SIGNAL(buttonClicked(QAbstractButton *)),
          this, SLOT(input_operator(QAbstractButton *)));
  connect(ui_->btn_brackets, SIGNAL(clicked()), this, SLOT(pressed_brackets()));
  connect(ui_->variables_button_group, SIGNAL(buttonClicked(QAbstractButton *)),
          this, SLOT(input_variables(QAbstractButton *)));
  connect(ui_->functions_button_group, SIGNAL(buttonClicked(QAbstractButton *)),
          this, SLOT(input_functions(QAbstractButton *)));
  connect(ui_->btn_back, SIGNAL(clicked()), this, SLOT(pressed_backspace()));
  connect(ui_->btn_clear, SIGNAL(clicked()), this, SLOT(pressed_clean()));
  connect(ui_->btn_clear_static_x, SIGNAL(clicked()), this,
          SLOT(pressed_clean_static_x()));
  connect(ui_->btn_equal, SIGNAL(clicked()), this, SLOT(pressed_equal()));

  connect(ui_->range1_high, SIGNAL(textChanged(QString)), this,
          SLOT(edited_range1_high(QString)));
  connect(ui_->range1_low, SIGNAL(textChanged(QString)), this,
          SLOT(edited_range1_low(QString)));
  connect(ui_->range2_high, SIGNAL(textChanged(QString)), this,
          SLOT(edited_range2_high(QString)));
  connect(ui_->range2_low, SIGNAL(textChanged(QString)), this,
          SLOT(edited_range2_low(QString)));
  connect(ui_->line1_button_group, SIGNAL(buttonClicked(QAbstractButton *)),
          this, SLOT(set_line_1_expression(QAbstractButton *)));
  connect(ui_->line2_button_group, SIGNAL(buttonClicked(QAbstractButton *)),
          this, SLOT(set_line_2_expression(QAbstractButton *)));
  connect(ui_->btn_draw_graph, SIGNAL(clicked()), this, SLOT(draw_graph()));
  connect(graph_view, SIGNAL(graph_view_closed_signal()), this,
          SLOT(update_graph_button()));
  connect(graph_view, SIGNAL(open_calculator_signal()), this,
          SLOT(show_calculator_window()));
}
