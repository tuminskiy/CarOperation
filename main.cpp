#include "window/connectiondialog.hpp"
#include "window/mainwindow.hpp"

#include <QApplication>
#include <QSqlDatabase>
#include <memory>

int main(int argc, char** argv)
{
  QApplication app(argc, argv);

  auto db_sptr = std::make_shared<QSqlDatabase>(QSqlDatabase::addDatabase("QPSQL"));

  carop::ConnectionDialog cdialog(db_sptr);
  carop::MainWindow mwindow(db_sptr);

  const auto close_dialog_open_main = [&] {
    cdialog.close();
    mwindow.show();
  };

  cdialog.show();

  QObject::connect(&cdialog, &carop::ConnectionDialog::accepted, close_dialog_open_main);

  return app.exec();
}