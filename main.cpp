#include "window/connectiondialog.hpp"
#include "window/mainwindow.hpp"

#include <QApplication>
#include <QDebug>
#include <memory>

int main(int argc, char** argv)
{
  QApplication app(argc, argv);

  carop::ConnectionDialog cdialog;
  carop::MainWindow mwindow;

  const auto close_dialog_open_main = [&] {
    mwindow.set_database(cdialog.database());
    cdialog.close();
    mwindow.show();
  };

  cdialog.show();

  QObject::connect(&cdialog, &carop::ConnectionDialog::accepted, close_dialog_open_main);

  return app.exec();
}