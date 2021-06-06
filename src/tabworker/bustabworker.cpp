#include "tabworker/bustabworker.hpp"
#include "storage/query.hpp"

#include <QSqlQuery>
#include <QVariant>

namespace carop
{

BusTabWorker::BusTabWorker(Ui::MainWindow& ui)
  : BaseTabWorker{ ui }
{ }


void BusTabWorker::fill_input()
{
  if (!selected_id_.has_value()) {
    throw std::invalid_argument("Not selected id");
  }

  const Bus bus = find_in_combobox_by_id<Bus>(ui_.cbBus, selected_id_.value()).value;

  const int driver_index = find_in_combobox_by_id<Driver>(ui_.cbDriver, bus.driver_id).index;

  ui_.leGovNumber->setText(bus.gov_number);
  ui_.cbDriver->setCurrentIndex(driver_index);
  ui_.leModel->setText(bus.model);
  ui_.leRouteNumber->setText(bus.route_number);
  ui_.leCapacity->setText(QString::number(bus.capacity));
}

void BusTabWorker::clear_input()
{
  selected_id_ = std::nullopt;

  ui_.leGovNumber->clear();
  ui_.cbDriver->setCurrentIndex(0);
  ui_.leModel->clear();
  ui_.leRouteNumber->clear();
  ui_.leCapacity->clear();
}


QVariant BusTabWorker::collect_data() const
{
  Bus bus;

  if (selected_id_.has_value()) {
    bus.id = selected_id_.value();
  }

  // Get gov_number
  if (bus.gov_number = ui_.leGovNumber->text().trimmed(); bus.gov_number.isEmpty()) {
    throw std::invalid_argument("The gov number field must be filled");
  }

  // Get driver_id
  QVariant data;

  if (data = ui_.cbDriver->currentData(); data.isNull()) {
    throw std::invalid_argument("Driver must be selected");
  }

  bus.driver_id = qvariant_cast<Driver>(data).id;

  // Get model
  if (bus.model = ui_.leModel->text().trimmed(); bus.model.isEmpty()) {
    throw std::invalid_argument("The model field must be filled");
  }

  // Get route_number
  if (bus.route_number = ui_.leRouteNumber->text().trimmed(); bus.route_number.isEmpty()) {
    throw std::invalid_argument("The route number field must be filled");
  }

  // Get capacity
  if (QString str = ui_.leCapacity->text().trimmed(); str.isEmpty()) {
    throw std::invalid_argument("The capacity filed must be filled");
  } else {
    bus.capacity = str.toUInt();
  }

  return QVariant::fromValue(bus);
}


QSqlQuery BusTabWorker::prepare_insert(const QVariant& value) const
{
  const Bus bus = qvariant_cast<Bus>(value);

  return carop::query_insert(bus);
}

QSqlQuery BusTabWorker::prepare_remove(id_t id) const
{
  return query_delete_bus(id);
}

QSqlQuery BusTabWorker::prepare_update(const QVariant& value) const
{
  const Bus bus = qvariant_cast<Bus>(value);
  
  return carop::query_update(bus);
}


} // namespace carop