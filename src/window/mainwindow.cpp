#include "window/mainwindow.hpp"
#include "storage/entity.hpp"
#include "tabworker/routetabworker.hpp"
#include "tabworker/routesheettabworker.hpp"
#include "tabworker/bustabworker.hpp"
#include "tabworker/drivertabworker.hpp"

#include <QSqlQuery>
#include <QMessageBox>

namespace carop
{

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow{ parent }
  , ui_{ }
  , dqueries_{ this }
  , dselectid_{ this }
  , view_model_{ }
  , tabworker_{  }
  , is_update_mode_{ false }
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

  ui_.bCancel->setVisible(false);
  ui_.bConfirm->setVisible(false);  

  connect(ui_.twTables, &QTabWidget::currentChanged,
          this, &MainWindow::tab_table_change);

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

  connect(ui_.bUpdate, &QPushButton::clicked,
          this, &MainWindow::update_click);

  connect(ui_.bConfirm, QPushButton::clicked,
          this, &MainWindow::confirm_click);

  connect(ui_.bCancel, &QPushButton::clicked,
          this, &MainWindow::cancel_click);
}


void MainWindow::load_data()
{
  try {
    tab_table_change(0);
    tabworker_->load_data();
  } catch (const std::runtime_error& e) {
    QMessageBox::critical(this, "Error", e.what());
  }
}

void MainWindow::tab_table_change(int /*index*/)
{
  const QWidget* const current_tab = ui_.twTables->currentWidget();

  if (current_tab == ui_.tabRoute) {
    tabworker_ = std::make_unique<RouteTabWorker>(ui_);
    dselectid_.set_table("route");
  } else if (current_tab == ui_.tabRouteSheet) {
    tabworker_ = std::make_unique<RouteSheetTabWorker>(ui_);
    dselectid_.set_table("routesheet");
  } else if (current_tab == ui_.tabBus) {
    tabworker_ = std::make_unique<BusTabWorker>(ui_);
    dselectid_.set_table("bus");
  } else if (current_tab == ui_.tabDriver) {
    tabworker_ = std::make_unique<DriverTabWorker>(ui_);
    dselectid_.set_table("driver");
  } else {
    throw std::runtime_error("TabWorker not found");
  }
}


void MainWindow::submit_click()
{
  try {
    tabworker_->insert();
    dselectid_.refresh();

    QMessageBox::information(this, "Inforamtion", "Insert completed successfully");

  } catch (const std::invalid_argument& e) {
    QMessageBox::critical(this, "Error", e.what());
  }
}

void MainWindow::remove_click()
{
  if (const int result = dselectid_.exec(); result != QDialog::Accepted) {
    return;
  }

  tabworker_->set_selected_id(dselectid_.selected_id());

  try {
    tabworker_->remove();
    dselectid_.refresh();

    QMessageBox::information(this, "Inforamtion", "Remove completed successfully");

  } catch (const std::invalid_argument& e) {
    QMessageBox::critical(this, "Error", e.what());
  }
} 

void MainWindow::update_click()
{
  if (const int result = dselectid_.exec(); result != QDialog::Accepted) {
    return;
  }

  tabworker_->set_selected_id(dselectid_.selected_id());
  tabworker_->fill_input();

  switch_update_mode();
}


void MainWindow::confirm_click()
{
  try {
    tabworker_->update();
    dselectid_.refresh();

    switch_update_mode();

    QMessageBox::information(this, "Information", "Update complated successfuly");

  } catch (const std::invalid_argument& e) {
    QMessageBox::critical(this, "Error", e.what());
  }

}

void MainWindow::cancel_click()
{
  switch_update_mode();
}

void MainWindow::switch_update_mode()
{
  if (is_update_mode_) {
    tabworker_->clear_input();
  }

  is_update_mode_ = !is_update_mode_;

  ui_.bCancel->setVisible(is_update_mode_);
  ui_.bConfirm->setVisible(is_update_mode_);

  ui_.bUpdate->setVisible(!is_update_mode_);
  ui_.bRemove->setVisible(!is_update_mode_);
  ui_.bSubmit->setVisible(!is_update_mode_);

  const int current_index = ui_.twTables->currentIndex();

  for (int i = 0; i < ui_.twTables->count(); i++) {
    if (i != current_index) {
      ui_.twTables->setTabEnabled(i, !is_update_mode_);
    }
  }
}


} // namespace carop