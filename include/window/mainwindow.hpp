#pragma once

#include "ui_mainwindow.h"
#include "queriesdialog.hpp"
#include "storage/entitymanager.hpp"

#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <memory>

namespace carop
{

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget* parent = nullptr);

  void load_data();

private slots:
  void submit_click();

private:
  Ui::MainWindow ui_;

  EntityManager entity_manager_;
  QueriesDialog dqueries_;
  
  QSqlQueryModel view_model_;
};

} // namespace carop