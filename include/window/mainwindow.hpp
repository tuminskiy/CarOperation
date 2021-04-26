#pragma once

#include "ui_mainwindow.h"
#include "storage/database.hpp"

#include <memory>

namespace carop
{

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget* parent = nullptr);

  void set_database(const QSqlDatabase& db);

private:
  Ui::MainWindow ui_;
  std::unique_ptr<carop::Database> db_;
};

} // namespace carop