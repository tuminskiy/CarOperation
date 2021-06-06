#include "tabworker/drivertabworker.hpp"
#include "storage/query.hpp"

#include <QSqlQuery>
#include <QVariant>

namespace carop
{

DriverTabWorker::DriverTabWorker(Ui::MainWindow& ui)
  : BaseTabWorker{ ui }
{ }


void DriverTabWorker::fill_input()
{
  if (!selected_id_.has_value()) {
    throw std::invalid_argument("Not selected id");
  }

  const Driver driver = find_in_combobox_by_id<Driver>(ui_.cbDriver, selected_id_.value()).value;

  ui_.leName->setText(driver.name);
  ui_.lePassport->setText(driver.passport);
  ui_.lePhone->setText(driver.phone);

  if (driver.route_sheet_id.has_value()) {
    const int index = find_in_combobox_by_id<RouteSheet>(ui_.cbRouteSheet, driver.route_sheet_id.value()).index;
    ui_.cbRouteSheet->setCurrentIndex(index);
  }
}

void DriverTabWorker::clear_input()
{
  selected_id_ = std::nullopt;

  ui_.leName->clear();
  ui_.lePassport->clear();
  ui_.lePhone->clear();
  ui_.cbRouteSheet->setCurrentIndex(0);
}


QVariant DriverTabWorker::collect_data() const
{
  Driver driver;

  if (selected_id_) {
    driver.id = selected_id_.value();
  }

  // Get name
  if (driver.name = ui_.leName->text().trimmed(); driver.name.isEmpty()) {
    throw std::invalid_argument("The name field must be filled");
  }

  // Get passport
  if (driver.passport = ui_.lePassport->text().trimmed(); driver.passport.isEmpty()) {
    throw std::invalid_argument("The passport field must be filled");
  }

  // Get phone
  if (driver.phone = ui_.lePhone->text().trimmed(); driver.phone.isEmpty()) {
    throw std::invalid_argument("The driver field must be filled");
  }

  // Get route_sheet_id
  if (QVariant data = ui_.cbRouteSheet->currentData(); !data.isNull()) {
    driver.route_sheet_id = qvariant_cast<RouteSheet>(data).id;
  }

  return QVariant::fromValue(driver);
}


QSqlQuery DriverTabWorker::prepare_insert(const QVariant& value) const
{
  const Driver driver = qvariant_cast<Driver>(value);

  return carop::query_insert(driver);
}

QSqlQuery DriverTabWorker::prepare_remove(id_t id) const
{
  return query_delete_bus(id);
}

QSqlQuery DriverTabWorker::prepare_update(const QVariant& value) const
{
  const Driver driver = qvariant_cast<Driver>(value);
  
  return carop::query_update(driver);
}

} // namespace carop