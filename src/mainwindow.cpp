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
  , view_model_{ }
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
  static std::unordered_map<QWidget*, std::function<bool ()>> widget_func = {
    { ui_.tabRoute,       std::bind(&EntityManager::insert_route, std::ref(entity_manager_)) },
    { ui_.tabRouteSheet,  std::bind(&EntityManager::insert_routesheet, std::ref(entity_manager_)) },
    { ui_.tabBus,         std::bind(&EntityManager::insert_bus, std::ref(entity_manager_)) },
    { ui_.tabDriver,      std::bind(&EntityManager::insert_driver, std::ref(entity_manager_)) }
  };

  if (!widget_func[ui_.twTables->currentWidget()]()) {
    QMessageBox::critical(this, "Error", entity_manager_.last_error());
    return;
  }

  QMessageBox::information(this, "Inforamtion", "Insert completed successfully");
}




} // namespace carop