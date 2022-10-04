#ifndef CALCULATOR_MODE_H
#define CALCULATOR_MODE_H

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRegularExpression>
#include <QWidget>
#include <list>
#include <string>

#include "qcplotic.h"

using std::list;
using std::string;

enum class InputType { NUMBER, OPERATOR, ERASE };
enum class FieldType { EXPRESSION, RANGE, DOUBLE };
enum class RangeFieldType { HIGH, LOW };
enum class PutLexemMode { USUAL, OVERRIDE };

namespace Ui {
class CalculatorMode;
}

class CalculatorMode : public QWidget {
  Q_OBJECT

 signals:
  double get_result_signal(list<std::string> *expression, double x);
  void get_graph_data_signal(list<std::string> *expression, int from, int to,
                             vector<double> &x, vector<double> &y);
  void open_the_window_signal();

 public:
  explicit CalculatorMode(QWidget *parent = nullptr);
  ~CalculatorMode();
  QWidget *get_widget_pointer();

 private slots:
  void input_numpad(QAbstractButton *btn);
  void input_operator(QAbstractButton *btn);
  void input_variables(QAbstractButton *btn);
  void input_functions(QAbstractButton *btn);
  void pressed_backspace();
  void pressed_brackets();
  void pressed_clean();
  void pressed_clean_static_x();
  void pressed_equal();
  void update_graph_button();
  void show_calculator_window();

  void edited_range1_high(const QString &str);
  void edited_range1_low(const QString &str);
  void edited_range2_high(const QString &str);
  void edited_range2_low(const QString &str);
  void set_line_1_expression(QAbstractButton *btn);
  void set_line_2_expression(QAbstractButton *btn);
  void draw_graph();

 private:
  const int kGRAPH_RANGE_LOW = -1000000;
  const int kGRAPH_RANGE_HIGH = 1000000;
  const QString kMIN_LOW = "-1000000";
  const QString kMIN_HIGH = "-999999";
  const QString kMAX_LOW = "999999";
  const QString kMAX_HIGH = "1000000";

  Ui::CalculatorMode *ui_;
  QWidget *calculator_ = new QWidget(this);
  QCPlotic *graph_view;
  int brackets_;
  int lexem_counter_;
  list<string> expression_lexems_;
  list<string> graph_1_expression_;
  list<string> graph_2_expression_;
  vector<double> x1, y1;
  vector<double> x2, y2;

  void get_ranges_data(int &from_1, int &to_1, int &from_2, int &to_2);
  bool is_empty_range_field(QString value);
  void get_range_field(QLineEdit *field, int &value);
  void graph_expression_manage(QAbstractButton *btn, QLabel *field,
                               list<string> *graph_expression);
  void get_line_data(QString &line, list<string> &expression, int from, int to,
                     vector<double> &x, vector<double> &y);
  void setup_regular_expressions_patterns();
  void setup_signal_slot_connections();
  bool hasFocus() const;
  bool is_calculator_widget_active();
  QLineEdit *get_editable_field(InputType type);
  void is_multiply_needed(QLineEdit *editable);
  void is_extra_zero_needed(QLineEdit *editable);
  bool is_permitted(QString lexem);
  void setup_validator(FieldType field, InputType type);
  void setup_range_num_validator();
  void setup_range_operator_validator();
  void setup_expression_num_validator();
  void setup_expression_operator_validator();
  void setup_double_num_validator();
  void setup_double_operator_validator();
  void backspace(QLineEdit *editable);
  void erase_lexem(QLineEdit *editable, int n);
  void put_lexem(QLineEdit *editable, const QString text, PutLexemMode mode);
  void add_expression_lexem(const QLineEdit *editable, const QString text);
  void remove_expression_lexem(const QLineEdit *editable,
                               const QString deletable);

  void adjust_range_field_value(QLineEdit *field, int max, int min,
                                QString set_min, QString set_max);
  void adjust_range(const QLineEdit *edited, QLineEdit *editable,
                    int min_difference, RangeFieldType mode);
  void consummate_expression();
  void truncate_result(QString &result_string);

  QRegularExpression *p_to_put_;
  QRegularExpression *p_to_change_;
  QRegularExpression *p_to_put_multiply_;
  QRegularExpression *p_to_put_permitted_;

  QRegularExpression expression_num_put_;
  QRegularExpression expression_num_change_;
  QRegularExpression expression_num_multiply_;
  QRegularExpression expression_num_add_zero_;
  QRegularExpression expression_no_doublepoint_;
  QRegularExpression expression_operator_put_;
  QRegularExpression expression_operator_change_;
  QRegularExpression expression_operator_unary_;
  QRegularExpression expression_add_zero_;
  QRegularExpression expression_rbracket_put_;
  QRegularExpression expression_lbracket_put_;
  QRegularExpression expression_lbracket_multiply_;
  QRegularExpression expression_variables_put_;
  QRegularExpression expression_variables_multiply_;
  QRegularExpression expression_variables_zero_multiply_;

  QRegularExpression range_num_put_;
  QRegularExpression range_operator_put_;
  QRegularExpression range_operator_change_;

  QRegularExpression double_num_put_;
  QRegularExpression double_num_change_;

  QRegularExpression expression_permitted_;
  QRegularExpression range_permitted_;
  QRegularExpression double_permitted_;

  QRegularExpression sym_deletion_[5];
  QRegularExpression null_regular_;
  QRegularExpression expression_bad_end_;
};

#endif  // CALCULATOR_MODE_H
