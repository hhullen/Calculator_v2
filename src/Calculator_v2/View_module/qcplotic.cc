#include "qcplotic.h"

#include "ui_qcplotic.h"

QCPlotic::QCPlotic(QWidget *parent)
    : QMainWindow(parent), ui_(new Ui::QCPlotic) {
  ui_->setupUi(this);
  set_up_graph_style();

  setup_scale_fields();
  setup_signal_slot_connections();
}

QCPlotic::~QCPlotic() {
  delete ui_;
  delete yellow_line_;
  delete purple_line_;
}

void QCPlotic::draw_graph(QString expression_1, std::vector<double> &x1,
                          std::vector<double> &y1, QString expression_2,
                          std::vector<double> &x2, std::vector<double> &y2) {
  setup_scale();
  build_line(graph1_, expression_1, x1, y1, ui_->expression_1_legend,
             yellow_line_);
  build_line(graph2_, expression_2, x2, y2, ui_->expression_2_legend,
             purple_line_);
  hide();
  show();
}

void QCPlotic::notice_graph_scale_changed() {
  ui_->btn_reset->setStyleSheet("background-color: rgba(200, 40, 30, 0.7;)");
}

void QCPlotic::setup_scale() {
  int buffX = ui_->scale_x_field->text().toInt();
  int buffY = ui_->scale_y_field->text().toInt();
  if (buffX < 0) buffX *= -1;
  if (buffY < 0) buffY *= -1;
  ui_->GraphWidget->xAxis->setRange(-buffX, buffX);
  ui_->GraphWidget->yAxis->setRange(-buffY, buffY);
}

void QCPlotic::build_line(vector<QCPGraph *> &graph, QString &expression,
                          std::vector<double> &x, std::vector<double> &y,
                          QLineEdit *legend, QPen *pen) {
  for (size_t i = 0; i < graph.size(); ++i) {
    ui_->GraphWidget->removeGraph(graph[i]);
  }
  graph.clear();
  ui_->GraphWidget->replot(QCustomPlot::rpQueuedRefresh);
  legend->setText("-");
  graph.push_back(ui_->GraphWidget->addGraph());
  graph.back()->setPen(*pen);
  if (x.size() > 0) {
    graph.back()->addData(x[0], y[0]);
  }
  for (size_t i = 1; i < x.size(); ++i) {
    if (abs(y[i] - y[i - 1]) < kFUNCTION_BREAK) {
      graph.back()->addData(x[i], y[i]);
    } else if (y[i] == y[i] && y[i] < kCONSTRAIN_Y) {
      graph.push_back(ui_->GraphWidget->addGraph());
      graph.back()->setPen(*pen);
    }
  }
  ui_->GraphWidget->replot(QCustomPlot::rpQueuedRefresh);
  legend->setText(expression);
}

void QCPlotic::show_calculator() { emit open_calculator_signal(); }

void QCPlotic::reset_axis_pos() {
  setup_scale();
  ui_->GraphWidget->replot(QCustomPlot::rpQueuedRefresh);
  ui_->btn_reset->setStyleSheet("background-color: rgba(93, 93, 94, 90);");
}

void QCPlotic::closeEvent(QCloseEvent *event) {
  emit graph_view_closed_signal();
}

void QCPlotic::set_up_graph_style() {
  QCustomPlot *graph = ui_->GraphWidget;
  graph->setInteraction(QCP::iRangeZoom, true);
  graph->setInteraction(QCP::iRangeDrag, true);
  graph->setBackground(QBrush(QColor(43, 82, 165, 255)));
  graph->xAxis->grid()->setPen(QPen(QColor(255, 255, 255), 1, Qt::SolidLine));
  graph->yAxis->grid()->setPen(QPen(QColor(255, 255, 255), 1, Qt::SolidLine));
  graph->xAxis->grid()->setSubGridPen(
      QPen(QColor(255, 255, 255), 0.5, Qt ::DotLine));
  graph->yAxis->grid()->setSubGridPen(
      QPen(QColor(255, 255, 255), 0.5, Qt::DotLine));
  graph->xAxis->grid()->setSubGridVisible(true);
  graph->yAxis->grid()->setSubGridVisible(true);
  graph->xAxis->grid()->setZeroLinePen(QPen(QColor(200, 0, 0)));
  graph->yAxis->grid()->setZeroLinePen(QPen(QColor(200, 0, 0)));
  graph->xAxis->setBasePen(QPen(Qt::white, 1));
  graph->yAxis->setBasePen(QPen(Qt::white, 1));
  graph->xAxis->setTickPen(QPen(Qt::white, 1));
  graph->yAxis->setTickPen(QPen(Qt::white, 1));
  graph->xAxis->setSubTickPen(QPen(Qt::white, 1));
  graph->yAxis->setSubTickPen(QPen(Qt::white, 1));
  graph->xAxis->setTickLabelColor(Qt::white);
  graph->yAxis->setTickLabelColor(Qt::white);
  graph->xAxis->setLabel("X axis");
  graph->yAxis->setLabel("Y axis");
  graph->xAxis->setLabelColor(Qt::white);
  graph->yAxis->setLabelColor(Qt::white);
  graph->xAxis->pixelOrientation();
  yellow_line_ = new QPen();
  yellow_line_->setColor(QColor(253, 242, 2));
  yellow_line_->setWidth(5);
  yellow_line_->setStyle(Qt::SolidLine);
  purple_line_ = new QPen();
  purple_line_->setColor(QColor(255, 82, 172));
  purple_line_->setWidth(5);
  purple_line_->setStyle(Qt::SolidLine);
}

void QCPlotic::setup_scale_fields() {
  ui_->scale_x_field->setValidator(new QIntValidator);
  ui_->scale_y_field->setValidator(new QIntValidator);

  ui_->scale_x_field->setText("2000");
  ui_->scale_y_field->setText("20");
}

void QCPlotic::setup_signal_slot_connections() {
  connect(ui_->btn_reset, SIGNAL(clicked()), this, SLOT(reset_axis_pos()));
  connect(ui_->btn_open_calc, SIGNAL(clicked()), this, SLOT(show_calculator()));
  connect(ui_->scale_x_field, SIGNAL(editingFinished()), this,
          SLOT(notice_graph_scale_changed()));
  connect(ui_->scale_y_field, SIGNAL(editingFinished()), this,
          SLOT(notice_graph_scale_changed()));
}
