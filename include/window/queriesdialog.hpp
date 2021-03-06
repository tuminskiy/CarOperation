#pragma once

#include "ui_queriesdialog.h"
#include "selecttabledialog.hpp"
#include "countdialog.hpp"
#include "orderdialog.hpp"
#include "startstationdialog.hpp"

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
  void order_confirm(const QString& order_by);
  void station_confirm(const QString& start_station);

private:
  Ui::QueriesDialog ui_;
  SelectTableDialog dselecttable_;
  CountDialog dcount_;
  OrderDialog dorder_;
  StartStationDialog dstation_;

  QSqlQuery last_query_;

  void exec_with_check(QSqlQuery& query);
};

} // namespace carop