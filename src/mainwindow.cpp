#include "window/mainwindow.hpp"
#include "storage/entity.hpp"
#include "storage/query.hpp"

#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

namespace carop
{

MainWindow::MainWindow(std::shared_ptr<QSqlDatabase> db_sptr, QWidget* parent)
  : QMainWindow{ parent }
  , ui_{ }
  , db_sptr_{ db_sptr }
  , dqueries_{ db_sptr_, this }
  , view_model_{ }
{
  ui_.setupUi(this);

  ui_.tvView->setModel(&view_model_);

  // QueriesDialog connect
  connect(ui_.bQueries, &QPushButton::clicked,
          &dqueries_, &QueriesDialog::open);

  connect(&dqueries_, &QueriesDialog::query_selected,
          &view_model_, qOverload<const QSqlQuery&>(&QSqlQueryModel::setQuery));

  connect(ui_.bRefresh, &QPushButton::clicked,
          &dqueries_, &QueriesDialog::repeat_query);


  // ChangeTab connect
  connect(ui_.bSubmit, &QPushButton::clicked,
          this, &MainWindow::submit_click);

}

void MainWindow::submit_click()
{
  
}




} // namespace carop