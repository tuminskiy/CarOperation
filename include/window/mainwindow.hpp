#pragma once

#include "ui_mainwindow.h"
#include "queriesdialog.hpp"

#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <memory>

namespace carop
{

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(std::shared_ptr<QSqlDatabase> db_sptr, QWidget* parent = nullptr);

private slots:
  void submit_click();

private:
  Ui::MainWindow ui_;

  std::shared_ptr<QSqlDatabase> db_sptr_;
  carop::QueriesDialog dqueries_;
  
  QSqlQueryModel view_model_;
};

} // namespace carop