#pragma once

#include "ui_queriesdialog.h"
#include "selecttabledialog.hpp"
#include "countdialog.hpp"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <memory>

namespace carop
{

class QueriesDialog : public QDialog
{
  Q_OBJECT

public:
  explicit QueriesDialog(QWidget* parent = nullptr);

public slots:
  void repeat_query();

signals:
  void query_selected(const QSqlQuery& query);

private slots:
  void select_table_confirm(const QString& table_name);
  void count_confirm(int count);

private:
  Ui::QueriesDialog ui_;
  SelectTableDialog dselecttable_;
  CountDialog dcount_;

  QSqlQuery last_query_;

  void exec_with_check(QSqlQuery& query);
};

} // namespace carop