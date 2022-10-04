#ifndef QCPLOTIC_H
#define QCPLOTIC_H

#include <QMainWindow>
#include <QVector>
#include <vector>

#include "QCustomPlot-library-master/lib/qcustomplot.h"

const int kFUNCTION_BREAK = 1000000;
const int kCONSTRAIN_Y = 2000;

using std::vector;

namespace Ui {
class QCPlotic;
}

class QCPlotic : public QMainWindow {
  Q_OBJECT

 public:
  explicit QCPlotic(QWidget *parent = nullptr);
  ~QCPlotic();
  void draw_graph(QString expression_1, vector<double> &x1, vector<double> &y1,
                  QString expression_2, vector<double> &x2, vector<double> &y2);

 private slots:
  void closeEvent(QCloseEvent *event);
  void reset_axis_pos();
  void show_calculator();
  void notice_graph_scale_changed();

 signals:
  void graph_view_closed_signal();
  void open_calculator_signal();

 private:
  Ui::QCPlotic *ui_;
  QPen *yellow_line_;
  QPen *purple_line_;
  vector<QCPGraph *> graph1_;
  vector<QCPGraph *> graph2_;

  void setup_scale();
  void build_line(vector<QCPGraph *> &graph, QString &expression,
                  vector<double> &x, vector<double> &y, QLineEdit *legend,
                  QPen *pen);
  void set_up_graph_style();
  void setup_scale_fields();
  void setup_signal_slot_connections();
};

#endif  // QCPLOTIC_H
