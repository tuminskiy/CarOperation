#include "window/connectiondialog.hpp"
#include "window/mainwindow.hpp"

#include <QApplication>
#include <QSqlDatabase>
#include <memory>

int main(int argc, char** argv)
{
  QApplication app(argc, argv);

  auto db_sptr = std::make_shared<QSqlDatabase>();

  carop::ConnectionDialog cdialog;
  carop::MainWindow mwindow;

  const auto close_dialog_open_main = [&] {
    cdialog.close();
    mwindow.show();
    mwindow.load_data();
  };

  cdialog.show();

  QObject::connect(&cdialog, &carop::ConnectionDialog::accepted, close_dialog_open_main);

  return app.exec();
}