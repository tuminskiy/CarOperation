#include "tabworker/basetabworker.hpp"
#include "storage/query.hpp"

#include <QSqlQuery>
#include <QSqlError>

namespace carop
{

void BaseTabWorker::load_data()
{
  ui_.cbRoute->addItem("", QVariant{ });
  ui_.cbRouteSheet->addItem("", QVariant{ });
  ui_.cbDriver->addItem("", QVariant{ });
  ui_.cbBus->addItem("", QVariant{ });

  QSqlQuery query;

  // Load routes
  if (query = query_select_table("route"); !query.exec()) {
    throw std::runtime_error(query.lastError().text().toStdString());
  }

  while (query.next()) {
    Route route;
    route.id = query.value("id").toUInt();
    route.station_start = query.value("station_start").toString();
    route.station_end = query.value("station_end").toString();
    route.time_start = query.value("time_start").toTime();
    route.time_end = query.value("time_end").toTime();

    ui_.cbRoute->addItem(as_text(route), QVariant::fromValue(route));
  }

  // Load drivers  
  if (query = query_select_table("driver"); !query.exec()) {
    throw std::runtime_error(query.lastError().text().toStdString());
  }

  while (query.next()) {
    Driver driver;
    driver.id = query.value("id").toUInt();
    driver.name = query.value("name").toString();
    driver.passport = query.value("passport").toString();
    driver.phone = query.value("phone").toString();

    if (QVariant data = query.value("route_sheet_id"); !data.isNull()) {
      driver.route_sheet_id = data.toUInt();
    }

    ui_.cbDriver->addItem(as_text(driver), QVariant::fromValue(driver));
  }

  // Load buses  
  if (query = query_select_table("bus"); !query.exec()) {
    throw std::runtime_error(query.lastError().text().toStdString());
  }

  while (query.next()) {
    Bus bus;
    bus.id = query.value("id").toUInt();
    bus.gov_number = query.value("gov_number").toString();
    bus.driver_id = query.value("driver_id").toUInt();
    bus.model = query.value("model").toString();
    bus.route_number = query.value("route_number").toString();
    bus.capacity = query.value("capacity").toUInt();

    ui_.cbBus->addItem(as_text(bus), QVariant::fromValue(bus));
  }

  // Load routsheets
  if (query = query_select_table("routesheet"); !query.exec()) {
    throw std::runtime_error(query.lastError().text().toStdString());
  }

  while (query.next()) {
    RouteSheet routesheet;
    routesheet.id = query.value("id").toUInt();
    routesheet.route_id = query.value("route_id").toUInt();
    routesheet.bus_id = query.value("bus_id").toUInt();
    routesheet.status = query.value("status").toString();

    ui_.cbRouteSheet->addItem(as_text(routesheet), QVariant::fromValue(routesheet));
  } 
}


BaseTabWorker::BaseTabWorker(Ui::MainWindow& ui)
  : ui_{ ui }
{ }


void BaseTabWorker::set_selected_id(id_t id)
{
  selected_id_ = id;
}


void BaseTabWorker::insert()
{
  const QVariant value = collect_data();

  QSqlQuery query = prepare_insert(value);

  if (!query.exec()) {
    throw std::invalid_argument(query.lastError().text().toStdString());
  }

  const id_t id = query.lastInsertId().toUInt();

  after_success_insert(id, value);
}

void BaseTabWorker::remove()
{
  if (!selected_id_.has_value()) {
    throw std::invalid_argument("Not selected id");
  }

  QSqlQuery query = prepare_remove(selected_id_.value());

  if (!query.exec()) {
    throw std::invalid_argument(query.lastError().text().toStdString());
  }

  after_success_remove(selected_id_.value());

  selected_id_ = std::nullopt;
}

void BaseTabWorker::update()
{
  if (!selected_id_.has_value()) {
    throw std::invalid_argument("Not selected id");
  }

  const QVariant value = collect_data();

  QSqlQuery query = prepare_update(value);

  if (!query.exec()) {
    throw std::invalid_argument(query.lastError().text().toStdString());
  }

  after_success_update(value);

  clear_input();
}


QString BaseTabWorker::as_text(const Route& route) const
{
  return QString{ "%1 -> %2 [%3; %4]" }
          .arg(route.station_start)
          .arg(route.station_end)
          .arg(route.time_start.toString())
          .arg(route.time_end.toString());
}

QString BaseTabWorker::as_text(const RouteSheet& routesheet) const
{
  const Route route = find_in_combobox_by_id<Route>(ui_.cbRoute, routesheet.route_id).value;
  const Bus bus = find_in_combobox_by_id<Bus>(ui_.cbBus, routesheet.bus_id).value;

  return QString{ "%1 %2 (%3)" }
          .arg(as_text(route))
          .arg(as_text(bus))
          .arg(routesheet.status);
}

QString BaseTabWorker::as_text(const Bus& bus) const
{
  return QString{ "№%1 (%2)" }
          .arg(bus.route_number)
          .arg(bus.gov_number);
}

QString BaseTabWorker::as_text(const Driver& driver) const
{
  return QString{ "%1 (%2)" }
          .arg(driver.name)
          .arg(driver.phone);
}


} // namespace carop