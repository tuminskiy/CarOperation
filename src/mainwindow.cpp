#include "window/mainwindow.hpp"
#include "storage/entity.hpp"

#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>

namespace carop
{

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow{ parent }
  , ui_{ }
  , entity_manager_{ ui_ }
  , dqueries_{ this }
  , dselectid_{ this }
  , view_model_{ }
  , widget_func_insert{ }
  , widget_func_show{ }
  , widget_func_remove{ }
{
  ui_.setupUi(this);

  ui_.leStationStart->setMaxLength(constraints::station_start_max);
  ui_.leStationEnd->setMaxLength(constraints::station_end_max);
  ui_.leStatus->setMaxLength(constraints::status_max);
  ui_.leGovNumber->setMaxLength(constraints::gov_number_max);
  ui_.leModel->setMaxLength(constraints::model_max);
  ui_.leRouteNumber->setMaxLength(constraints::route_number_max);
  ui_.leName->setMaxLength(constraints::name_max);
  ui_.lePassport->setMaxLength(constraints::passport_max);
  ui_.lePhone->setMaxLength(constraints::phone_max);

  ui_.tvView->setModel(&view_model_);

  widget_func_insert = {
    { ui_.tabRoute,       std::bind(&EntityManager::insert_route, std::ref(entity_manager_)) },
    { ui_.tabRouteSheet,  std::bind(&EntityManager::insert_routesheet, std::ref(entity_manager_)) },
    { ui_.tabBus,         std::bind(&EntityManager::insert_bus, std::ref(entity_manager_)) },
    { ui_.tabDriver,      std::bind(&EntityManager::insert_driver, std::ref(entity_manager_)) }
  };

  widget_func_show = {
    { ui_.tabRoute,       std::bind(&SelectIdDialog::show_table, std::ref(dselectid_), "Route") },
    { ui_.tabRouteSheet,  std::bind(&SelectIdDialog::show_table, std::ref(dselectid_), "RouteSheet") },
    { ui_.tabBus,         std::bind(&SelectIdDialog::show_table, std::ref(dselectid_), "Bus") },
    { ui_.tabDriver,      std::bind(&SelectIdDialog::show_table, std::ref(dselectid_), "Driver") }
  };

  widget_func_remove = {
    { ui_.tabRoute,       std::bind(&EntityManager::remove_route, std::ref(entity_manager_), std::placeholders::_1) },
    { ui_.tabRouteSheet,  std::bind(&EntityManager::remove_routesheet, std::ref(entity_manager_), std::placeholders::_1) },
    { ui_.tabBus,         std::bind(&EntityManager::remove_bus, std::ref(entity_manager_), std::placeholders::_1) },
    { ui_.tabDriver,      std::bind(&EntityManager::remove_driver, std::ref(entity_manager_), std::placeholders::_1) }
  };

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

  connect(ui_.bRemove, &QPushButton::clicked,
          this, &MainWindow::remove_click);
}

void MainWindow::load_data()
{
  if (!entity_manager_.load_data()) {
    QMessageBox::critical(this, "Error", entity_manager_.last_error());
    close();
  }
}

void MainWindow::submit_click()
{
  auto func_insert = func_for_current_tab(widget_func_insert);

  if (!func_insert) {
    QMessageBox::critical(this, "Error", "Function to insert not found");
    return;
  }

  if (!func_insert()) {
    QMessageBox::critical(this, "Error", entity_manager_.last_error());
    return;
  }

  QMessageBox::information(this, "Inforamtion", "Insert completed successfully");
}

void MainWindow::remove_click()
{
  auto func_show = func_for_current_tab(widget_func_show);

  if (!func_show) {
    QMessageBox::critical(this, "Error", "No window found to display");
    return;
  }

  func_show();

  if (const int result = dselectid_.exec(); result != QDialog::Accepted) {
    return;
  }
  
  auto func_remove = func_for_current_tab(widget_func_remove);

  if (!func_remove) {
    QMessageBox::critical(this, "Error", "Function to remove not found");
    return;
  }

  if (const id_t selected_id = dselectid_.selected_id(); !func_remove(selected_id)) {
    QMessageBox::critical(this, "Error", entity_manager_.last_error());
    return;
  }

  QMessageBox::information(this, "Inforamtion", "Remove completed successfully");
} 


} // namespace carop