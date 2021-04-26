#include "window/mainwindow.hpp"



namespace carop
{

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow{ parent }
  , ui_{}
  , db_{}
{
  ui_.setupUi(this);
}

void MainWindow::set_database(const QSqlDatabase& db)
{
  db_ = std::make_unique<carop::Database>(db);
}

} // namespace carop