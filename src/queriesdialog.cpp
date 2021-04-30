#include "window/queriesdialog.hpp"
#include "storage/query.hpp"

#include <QSqlError>
#include <QMessageBox>

namespace carop
{

QueriesDialog::QueriesDialog(std::shared_ptr<QSqlDatabase> db_sptr, QWidget* parent)
  : QDialog{ parent }
  , ui_{ }
  , db_sptr_{ db_sptr }
  , dselecttable_{ this }
  , last_query_{ }
{
  ui_.setupUi(this);

  connect(ui_.bSelectTable, &QPushButton::clicked,
          &dselecttable_, &SelectTableDialog::open);

  connect(&dselecttable_, &SelectTableDialog::confirmed,
          this, &QueriesDialog::select_table_confirm);
}

void QueriesDialog::repeat_query() { exec_with_check(last_query_); }

void QueriesDialog::select_table_confirm(const QString& table_name)
{
  auto query = query_select_table(table_name, db_sptr_);
  exec_with_check(query);
}

void QueriesDialog::exec_with_check(QSqlQuery& query)
{
  last_query_ = query;

  if (query.exec()) {
    emit query_selected(query);
    close();
  } else {
    QMessageBox::critical(this, "Error", query.lastError().text());
  }
}

} // namespace carop