#include "storage/database.hpp"
#include "dialog/connectiondialog.hpp"

#include <QApplication>
#include <QDebug>
#include <memory>

int main(int argc, char** argv)
{
  QApplication app(argc, argv);

  std::shared_ptr<carop::Database> db_ptr_;

  carop::ConnectionDialog cdialog{};
  cdialog.open();

  QObject::connect(&cdialog, &carop::ConnectionDialog::accepted,
    [&] {
      qDebug() << "Success connect to database";
      
      db_ptr_ = std::make_shared<carop::Database>(cdialog.database());

      cdialog.close();
    }
  );

  return app.exec();
}