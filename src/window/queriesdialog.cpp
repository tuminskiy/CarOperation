#include "window/queriesdialog.hpp"
#include "storage/query.hpp"

#include <QSqlError>
#include <QMessageBox>

namespace carop
{

QueriesDialog::QueriesDialog(QWidget* parent)
  : QDialog{ parent }
  , ui_{ }
  , dselecttable_{ this }
  , dcount_{ this }
  , last_query_{ }
{
  ui_.setupUi(this);

  connect(ui_.bSelectTable, &QPushButton::clicked,
          &dselecttable_, &SelectTableDialog::open);

  connect(ui_.bHaving, &QPushButton::clicked,
          &dcount_, &CountDialog::open);


  connect(&dselecttable_, &SelectTableDialog::confirmed,
          this, &QueriesDialog::select_table_confirm);

  connect(&dcount_, &CountDialog::confirmed,
          this, &QueriesDialog::count_confirm);
}



void QueriesDialog::repeat_query() { exec_with_check(last_query_); }


void QueriesDialog::select_table_confirm(const QString& table_name)
{
  QSqlQuery query = query_select_table(table_name);
  exec_with_check(query);
}

void QueriesDialog::count_confirm(int count)
{
  QSqlQuery query = query_having(count);
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