#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include "View_module/mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  QTranslator translator;
  const QStringList uiLanguages = QLocale::system().uiLanguages();
  for (const QString &locale : uiLanguages) {
    const QString baseName = "Calculator_v2_" + QLocale(locale).name();
    if (translator.load(":/i18n/" + baseName)) {
      a.installTranslator(&translator);
      break;
    }
  }
  CalculatorModel model;
  CalculatorController controller(&model);
  MainWindow window(&controller);
  window.show();

  return a.exec();
}

void MainWindow::mousePressEvent(QMouseEvent* event) {
  mouse_old_x_ = QCursor::pos().x();
  mouse_old_y_ = QCursor::pos().y();
}

void MainWindow::mouseMoveEvent(QMouseEvent* event) {
  dx_ = QCursor::pos().x() - mouse_old_x_;
  dy_ = QCursor::pos().y() - mouse_old_y_;
  MainWindow::move(MainWindow::pos().x() + dx_, MainWindow::pos().y() + dy_);
  mouse_old_x_ = QCursor::pos().x();
  mouse_old_y_= QCursor::pos().y();
}
