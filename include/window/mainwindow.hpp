#pragma once

#include "ui_mainwindow.h"
#include "queriesdialog.hpp"
#include "selectiddialog.hpp"
#include "tabworker/basetabworker.hpp"

#include <QSqlQueryModel>
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
  void tab_table_change(int index);

  void submit_click();
  void remove_click();
  void update_click();
  
  void confirm_click();
  void cancel_click();

private:
  void switch_update_mode();

private:
  Ui::MainWindow ui_;

  QueriesDialog dqueries_;
  SelectIdDialog dselectid_;
  
  QSqlQueryModel view_model_;

  std::unique_ptr<BaseTabWorker> tabworker_;

  bool is_update_mode_;
};

} // namespace carop