#ifndef SRC_CALCULATOR_V2_MODEL_MODULE_CALCULATOR_H
#define SRC_CALCULATOR_V2_MODEL_MODULE_CALCULATOR_H

#include <cmath>
#include <regex>
#include <string>
#include <thread>
#include <vector>
using std::cmatch;
using std::regex;
using std::regex_match;
using std::stod;
using std::string;
using std::thread;
using std::vector;

namespace s21 {

const char kLT = 1;
const char kLE = -1;

const double kEXPONENT = 2.71828182845904523536028747135266250;
const int kMAX_RANGE = 2000001;
const int kLIMIT_PER_THREAD = 200000;
const int kAVG_THREADS = 4;

enum Lexems {
  VARX = 10,
  NUM = 11,
  PLUS = '+',
  MINUS = '-',
  MULTIPLY = 12,
  DIVIDE = 13,
  MOD = 14,
  EXTENT = 15,
  COS = 16,
  SIN = 17,
  TAN = 18,
  ACOS = 19,
  ASIN = 20,
  ATAN = 21,
  SQRT = 22,
  LN = 23,
  LOG = 24,
  LBRACKET = 25,
  RBRACKET = 26,
  UMINUS = 27,
  POINT = '.',
  PI = 28,
  END = 29,
  EXP = 'e'
};

enum Priorities { PRIOR_0, PRIOR_1, PRIOR_2, PRIOR_3, PRIOR_4, PRIOR_5 };

class Calculator {
 public:
  Calculator();
  ~Calculator();
  double calculate_expression(char *expression, const double &x);
  double calculate_expression(const double &x);

  void calculate_range(char *expression, double from, double to);
  vector<double> get_x_values_vector();
  vector<double> get_y_values_vector();

  void clear_expression();
  void add_expression_lexem(const char lexem);
  void add_expression_lexem(const string &lexem);
  void erase_expression_lexem();
  char *get_expression();

 private:
  class Stack {
   public:
    Stack();
    ~Stack();
    bool empty();
    double get_value();
    char get_priority();
    char get_operand();
    void push(const double &value, const char &priority, const char &operand);
    void pop();

   private:
    class Node {
     public:
      double data;
      char priority;
      char operand;
      Node *next;

      Node(double data, char priority, char operand, Node *next)
          : data(data), priority(priority), operand(operand), next(next) {}
    };

    Node *top_node_;
  };
  size_t expression_size_;
  size_t expression_iterator_;
  char *expression_;

  double make_number(char *number, int &iter);
  bool is_ascii_digit(const char &value);
  char cmp_priority(const char &a, const char &b);
  void calculate_numbers(Stack &stack, const char &operand);
  void calculate_function(Stack &stack, const char &operand);
  void operate_remain_stack(Stack &support, Stack &main, char &operand);
  void staks_calculation(Stack &support, Stack &main, char priority, char cmp);
  void staks_calculation_Rbracket(Stack &support, Stack &main, char priority,
                                  char cmp);

  regex operators_;
  regex variables_;
  regex functions_;
  cmatch match_;

  bool is_valid_lexem(const char lexem);
  char get_operator(const cmatch &match);
  char get_variable(const cmatch &match);
  char get_function(const cmatch &match);

  class Graph {
   public:
    double *p_start_y, *p_start_x;
    double from, to, dx, x_value;
    int points, ppu;
    char *expression;
  };
  Graph *graph_data_;
  vector<double> vals_x, vals_y;
  int hardware_threads_ = std::thread::hardware_concurrency();

  void adjust_threads_amount(int &threads);
  void run_threads(int threads);
  void thread_function(Graph *data);
};

}  // namespace s21

#endif  // SRC_CALCULATOR_V2_MODEL_MODULE_CALCULATOR_H
