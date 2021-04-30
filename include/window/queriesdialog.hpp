#pragma once

#include "ui_queriesdialog.h"
#include "selecttabledialog.hpp"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <memory>

namespace carop
{

class QueriesDialog : public QDialog
{
  Q_OBJECT

public:
  explicit QueriesDialog(std::shared_ptr<QSqlDatabase> db_sptr, QWidget* parent = nullptr);

public slots:
  void repeat_query();

signals:
  void query_selected(const QSqlQuery& query);

private slots:
  void select_table_confirm(const QString& table_name);

private:
  Ui::QueriesDialog ui_;
  std::shared_ptr<QSqlDatabase> db_sptr_;
  carop::SelectTableDialog dselecttable_;
  QSqlQuery last_query_;

  void exec_with_check(QSqlQuery& query);
};

} // namespace carop