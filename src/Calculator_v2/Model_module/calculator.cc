#include "calculator.h"

namespace s21 {

/*
  Calculator class
*/
Calculator::Calculator() {
  expression_size_ = 256;
  expression_ = new char[expression_size_];
  expression_iterator_ = 0;
  graph_data_ = new Graph();
  clear_expression();
  operators_.assign("(^[\\+\\-_\\*\\/\\^\\%]$)|(^mod$)");
  variables_.assign("(^[Xe]$)|(^PI$)");
  functions_.assign(
      "(^sin$)|(^cos$)|(^tan$)|(^asin$)|"
      "(^acos$)|(^atan$)|(^sqrt$)|(^ln$)|(^log$)");
}

Calculator::~Calculator() {
  if (expression_) {
    delete[] expression_;
  }
  delete graph_data_;
}

vector<double> Calculator::get_x_values_vector() { return vals_x; }

vector<double> Calculator::get_y_values_vector() { return vals_y; }

char *Calculator::get_expression() { return expression_; }

void Calculator::clear_expression() {
  for (size_t i = 0; i < expression_size_; ++i) {
    expression_[i] = Lexems::END;
  }
  expression_iterator_ = 0;
}

void Calculator::add_expression_lexem(const char lexem) {
  if (!is_valid_lexem(lexem)) {
    throw std::invalid_argument("Invalid lexem");
  }

  if (expression_iterator_ < expression_size_) {
    expression_[expression_iterator_] = lexem;
    ++expression_iterator_;
  }
}

void Calculator::add_expression_lexem(const string &lexem) {
  char addable = Lexems::END;
  if (is_ascii_digit(*lexem.c_str())) {
    addable = *lexem.c_str();
  } else if (*lexem.data() == '.') {
    addable = Lexems::POINT;
  } else if (*lexem.data() == ')') {
    addable = Lexems::RBRACKET;
  } else if (*lexem.data() == '(') {
    addable = Lexems::LBRACKET;
  } else if (regex_match(lexem.c_str(), match_, operators_)) {
    addable = get_operator(match_);
  } else if (regex_match(lexem.c_str(), match_, variables_)) {
    addable = get_variable(match_);
  } else if (regex_match(lexem.c_str(), match_, functions_)) {
    addable = get_function(match_);
  }
  add_expression_lexem(addable);
}

void Calculator::erase_expression_lexem() {
  if (expression_iterator_ > 0) {
    --expression_iterator_;
  }
  expression_[expression_iterator_] = Lexems::END;
}

char Calculator::get_operator(const cmatch &match) {
  char returnable = Lexems::END;
  if (match[0] == "+") {
    returnable = Lexems::PLUS;
  } else if (match[0] == "-") {
    returnable = Lexems::MINUS;
  } else if (match[0] == "_") {
    returnable = Lexems::UMINUS;
  } else if (match[0] == "*") {
    returnable = Lexems::MULTIPLY;
  } else if (match[0] == "/") {
    returnable = Lexems::DIVIDE;
  } else if (match[0] == "%" || match[0] == "mod") {
    returnable = Lexems::MOD;
  } else if (match[0] == "^") {
    returnable = Lexems::EXTENT;
  }
  return returnable;
}

char Calculator::get_variable(const cmatch &match) {
  char returnable = Lexems::END;
  if (match[0] == "e") {
    returnable = Lexems::EXP;
  } else if (match[0] == "X") {
    returnable = Lexems::VARX;
  } else if (match[0] == "PI") {
    returnable = Lexems::PI;
  }
  return returnable;
}

char Calculator::get_function(const cmatch &match) {
  char returnable = Lexems::END;
  if (match[0] == "sin") {
    returnable = Lexems::SIN;
  } else if (match[0] == "cos") {
    returnable = Lexems::COS;
  } else if (match[0] == "tan") {
    returnable = Lexems::TAN;
  } else if (match[0] == "asin") {
    returnable = Lexems::ASIN;
  } else if (match[0] == "acos") {
    returnable = Lexems::ACOS;
  } else if (match[0] == "atan") {
    returnable = Lexems::ATAN;
  } else if (match[0] == "sqrt") {
    returnable = Lexems::SQRT;
  } else if (match[0] == "ln") {
    returnable = Lexems::LN;
  } else if (match[0] == "log") {
    returnable = Lexems::LOG;
  }
  return returnable;
}

void Calculator::calculate_range(char *expression, double from, double to) {
  int threads = 0, range = 0;

  graph_data_->expression = expression;
  graph_data_->from = from;
  graph_data_->to = to;
  range = graph_data_->to - graph_data_->from;
  graph_data_->ppu = (int)pow(log(kMAX_RANGE) - log(range), 2) + 1;
  graph_data_->points = graph_data_->ppu * range;
  graph_data_->dx = 1.0 / graph_data_->ppu;
  threads = graph_data_->points / kLIMIT_PER_THREAD;
  adjust_threads_amount(threads);

  vals_x.resize(graph_data_->points);
  vals_x.shrink_to_fit();
  vals_y.resize(graph_data_->points);
  vals_y.shrink_to_fit();

  run_threads(threads);
}

void Calculator::run_threads(int threads) {
  thread **thread_id = new thread *[threads];
  Graph *thread_data = new Graph[threads];
  int thread_range = graph_data_->points / threads;

  for (int i = 0; i < threads; ++i) {
    thread_data[i].from = thread_range * i + graph_data_->from;
    thread_data[i].to = thread_data[i].from + thread_range;
    thread_data[i].dx = graph_data_->dx;
    thread_data[i].p_start_x = &vals_x.data()[i * thread_range];
    thread_data[i].p_start_y = &vals_y.data()[i * thread_range];
    thread_data[i].points = thread_range;
    thread_data[i].x_value =
        graph_data_->from + thread_range * graph_data_->dx * i;

    thread_id[i] =
        new thread(&Calculator::thread_function, this, &thread_data[i]);
  }

  for (int i = 0; i < threads; ++i) {
    thread_id[i]->join();
    delete thread_id[i];
  }
  delete[] thread_id;
  delete[] thread_data;
}

void Calculator::thread_function(Graph *data) {
  for (int i = 0; data->from < data->to && i < data->points;
       data->from += data->dx, data->x_value += data->dx, ++i) {
    data->p_start_x[i] = data->x_value;
    data->p_start_y[i] =
        calculate_expression(graph_data_->expression, data->x_value);
  }
}

void Calculator::adjust_threads_amount(int &threads) {
  if (threads < 2) {
    threads = 1;
  } else if (hardware_threads_ > 0 && threads > hardware_threads_) {
    threads = hardware_threads_;
  } else if (hardware_threads_ == 0) {
    threads = kAVG_THREADS;
  }
}

double Calculator::calculate_expression(const double &x) {
  return calculate_expression(expression_, x);
}

double Calculator::calculate_expression(char *expression, const double &x) {
  Stack support_stack, main_stack;
  char operand = 0, priority = 0;
  double value = 0;
  int iter = 0;

  for (; expression[iter] != END; ++iter) {
    if (expression[iter] == PI) {
      main_stack.push(M_PI, PRIOR_0, NUM);
    } else if (expression[iter] == EXP) {
      main_stack.push(kEXPONENT, PRIOR_0, NUM);
    } else if (is_ascii_digit(expression[iter])) {
      main_stack.push(make_number(&expression[iter], iter), PRIOR_0, NUM);
    } else if (expression[iter] == VARX) {
      main_stack.push(x, PRIOR_0, VARX);
    } else if (expression[iter] == MINUS || expression[iter] == PLUS) {
      staks_calculation(support_stack, main_stack, PRIOR_1, kLE);
      support_stack.push(0.0, PRIOR_1, expression[iter]);
    } else if (expression[iter] <= MOD) {
      staks_calculation(support_stack, main_stack, PRIOR_2, kLE);
      support_stack.push(0.0, PRIOR_2, expression[iter]);
    } else if (expression[iter] == EXTENT) {
      staks_calculation(support_stack, main_stack, PRIOR_3, kLT);
      support_stack.push(0.0, PRIOR_3, expression[iter]);
    } else if (expression[iter] <= LOG) {
      staks_calculation(support_stack, main_stack, PRIOR_4, kLE);
      support_stack.push(0.0, PRIOR_4, expression[iter]);
    } else if (expression[iter] == LBRACKET) {
      support_stack.push(0.0, PRIOR_5, LBRACKET);
    } else if (expression[iter] == RBRACKET) {
      staks_calculation_Rbracket(support_stack, main_stack, PRIOR_4, 0);
    } else if (expression[iter] == UMINUS) {
      main_stack.push(-1, PRIOR_0, NUM);
      staks_calculation(support_stack, main_stack, PRIOR_2, kLE);
      support_stack.push(0.0, PRIOR_2, MULTIPLY);
    }
  }
  operate_remain_stack(support_stack, main_stack, operand);
  return main_stack.get_value();
}

void Calculator::operate_remain_stack(Stack &support, Stack &main,
                                      char &operand) {
  while (!support.empty()) {
    operand = support.get_operand();
    support.pop();
    if (operand == PLUS || operand == MINUS || operand <= EXTENT) {
      calculate_numbers(main, operand);
    } else if (operand <= LOG) {
      calculate_function(main, operand);
    }
  }
}

void Calculator::staks_calculation(Stack &support, Stack &main, char priority,
                                   char cmp) {
  if (!support.empty()) {
    char picked_priority = support.get_priority();
    char picked_operand = support.get_operand();
    while (cmp_priority(picked_priority, priority) > cmp && !support.empty() &&
           picked_operand != LBRACKET) {
      support.pop();
      if (picked_operand == PLUS || picked_operand == MINUS ||
          picked_operand <= EXTENT) {
        calculate_numbers(main, picked_operand);
      } else if (picked_operand <= LOG) {
        calculate_function(main, picked_operand);
      }
      if (!support.empty()) {
        picked_priority = support.get_priority();
        picked_operand = support.get_operand();
      }
    }
  }
}

void Calculator::calculate_numbers(Stack &stack, const char &operand) {
  double num1 = 0, num2 = 0, result = 0.0;

  num2 = stack.get_value();
  stack.pop();
  num1 = stack.get_value();
  stack.pop();

  if (operand == PLUS) {
    result = num1 + num2;
  } else if (operand == MINUS) {
    result = num1 - num2;
  } else if (operand == MULTIPLY) {
    result = num1 * num2;
  } else if (operand == DIVIDE) {
    result = num1 / num2;
  } else if (operand == MOD) {
    result = fmod(num1, num2);
  } else if (operand == EXTENT) {
    result = pow(num1, num2);
  }
  stack.push(result, PRIOR_0, NUM);
}

void Calculator::calculate_function(Stack &stack, const char &operand) {
  double num = 0, result = 0.0;

  num = stack.get_value();
  stack.pop();
  if (operand == COS) {
    result = cos(num * M_PI / 180);
  } else if (operand == SIN) {
    result = sin(num * M_PI / 180);
  } else if (operand == TAN) {
    result = tan(num * M_PI / 180);
  } else if (operand == ACOS) {
    result = acos(num) * 180 / M_PI;
  } else if (operand == ASIN) {
    result = asin(num) * 180 / M_PI;
  } else if (operand == ATAN) {
    result = atan(num) * 180 / M_PI;
  } else if (operand == SQRT) {
    result = sqrt(num);
  } else if (operand == LN) {
    result = log(num);
  } else if (operand == LOG) {
    result = log10(num);
  }
  stack.push(result, PRIOR_0, NUM);
}

char Calculator::cmp_priority(const char &a, const char &b) {
  return a > b ? 1 : a == b ? 0 : -1;
}

double Calculator::make_number(char *number, int &iter) {
  for (int i = 0;
       (is_ascii_digit(number[i]) || number[i] == POINT || number[i] == EXP);
       ++i) {
    if (number[i] == EXP && (number[i + 1] == '+' || number[i + 1] == '-')) {
      ++iter;
      ++i;
    }
    ++iter;
  }
  --iter;

  return stod(number);
}

bool Calculator::is_ascii_digit(const char &value) {
  return value >= '0' && value <= '9';
}

void Calculator::staks_calculation_Rbracket(Stack &support, Stack &main,
                                            char priority, char cmp) {
  if (!support.empty()) {
    char picked_priority = support.get_priority();
    char picked_operand = support.get_operand();
    while (picked_operand != LBRACKET) {
      support.pop();
      if (picked_operand == PLUS || picked_operand == MINUS ||
          picked_operand <= EXTENT) {
        calculate_numbers(main, picked_operand);
      } else if (picked_operand <= LOG) {
        calculate_function(main, picked_operand);
      }
      if (!support.empty()) {
        picked_priority = support.get_priority();
        picked_operand = support.get_operand();
      }
    }
  }
  support.pop();
}

bool Calculator::is_valid_lexem(const char lexem) {
  bool returnable = false;
  if (is_ascii_digit(lexem) || lexem == '+' || lexem == '-' || lexem == '.' ||
      lexem == 'e' || lexem == Lexems::VARX ||
      (lexem >= Lexems::MULTIPLY && lexem <= Lexems::PI)) {
    returnable = true;
  }
  return returnable;
}

/*
  Stack class
*/

Calculator::Stack::Stack() { top_node_ = nullptr; }

Calculator::Stack::~Stack() {
  while (top_node_) {
    pop();
  }
}

bool Calculator::Stack::empty() { return top_node_ == nullptr; }

double Calculator::Stack::get_value() { return top_node_->data; }

char Calculator::Stack::get_priority() { return top_node_->priority; }

char Calculator::Stack::get_operand() { return top_node_->operand; }

void Calculator::Stack::push(const double &value, const char &priority,
                             const char &operand) {
  Node *buffer = new Node(value, priority, operand, top_node_);
  top_node_ = buffer;
}

void Calculator::Stack::pop() {
  Node *buffer = top_node_->next;
  delete top_node_;
  top_node_ = buffer;
}

}  // namespace s21
