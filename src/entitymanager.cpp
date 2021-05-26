#include "storage/entitymanager.hpp"
#include "storage/query.hpp"

#include <QSqlError>
#include <QDebug>

namespace carop
{

EntityManager::EntityManager(Ui::MainWindow& ui)
  : ui_{ ui }
  , last_error_{ }
{ }

bool EntityManager::load_data()
{
  QSqlQuery query;
  ui_.cbRoute->addItem("", QVariant{ });
  ui_.cbRouteSheet->addItem("", QVariant{ });
  ui_.cbDriver->addItem("", QVariant{ });
  ui_.cbBus->addItem("", QVariant{ });

  // Load routes
  if (query = query_select_table("route"); exec_with_check(query)) {
    while (query.next()) {
      Route route;
      route.id = query.value("id").toUInt();
      route.station_start = query.value("station_start").toString();
      route.station_end = query.value("station_end").toString();
      route.time_start = query.value("time_start").toTime();
      route.time_end = query.value("time_end").toTime();

      ui_.cbRoute->addItem(as_text(route), QVariant::fromValue(route));
    }
  } else {
    return false;
  }

  // Load routsheets
  if (query = query_select_table("routesheet"); exec_with_check(query)) {
    while (query.next()) {
      RouteSheet routesheet;
      routesheet.id = query.value("id").toUInt();
      routesheet.route_id = query.value("route_id").toUInt();
      routesheet.bus_id = query.value("bus_id").toUInt();
      routesheet.status = query.value("status").toString();

      ui_.cbRouteSheet->addItem(as_text(routesheet), QVariant::fromValue(routesheet));
    }
  } else {
    return false;
  }

  // Load buses  
  if (query = query_select_table("bus"); exec_with_check(query)) {
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
  } else {
    return false;
  }

  // Load drivers  
  if (query = query_select_table("driver"); exec_with_check(query)) {
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
  } else {
    return false;
  }

  return true;
}

bool EntityManager::insert_route()
{
  Route route;

  // Get station_start
  if (route.station_start = ui_.leStationStart->text().trimmed();
      route.station_start.isEmpty()) {
    last_error_ = "The station start field must be filled";    
    return false;
  }

  // Get station_end
  if (route.station_end = ui_.leStationEnd->text().trimmed();
      route.station_end.isEmpty()) {
    last_error_ = "The station end filed must be filled";
    return false;
  }

  // Get time_start and time_end
  route.time_start = ui_.teTimeStart->time();
  route.time_end = ui_.teTimeEnd->time();

  // Insert
  if (QSqlQuery query = query_insert(route); exec_with_check(query)) {
    route.id = query.lastInsertId().toUInt();
    ui_.cbRoute->addItem(as_text(route), QVariant::fromValue(route));

    ui_.leStationStart->clear();
    ui_.leStationEnd->clear();
    ui_.teTimeStart->clear();
    ui_.teTimeEnd->clear();

    return true;
  }

  return false;
}

bool EntityManager::insert_routesheet()
{
  RouteSheet routesheet;
  QVariant data;

  // Get route_id
  if (data = ui_.cbRoute->currentData(); data.isNull()) {
    last_error_ = "Route must be selected";
    return false;
  }

  routesheet.route_id = qvariant_cast<Route>(data).id;

  // Get bus_id
  if (data = ui_.cbBus->currentData(); data.isNull()) {
    last_error_ = "Bus must be selected";
    return false;
  }

  routesheet.bus_id = qvariant_cast<Bus>(data).id;

  // Get status
  if (routesheet.status = ui_.leStatus->text().trimmed();
      routesheet.status.isEmpty()) {
    last_error_ = "The status field must be filled";
    return false;
  }

  // Insert
  if (QSqlQuery query = query_insert(routesheet);
      exec_with_check(query)) {
    routesheet.id = query.lastInsertId().toUInt();
    ui_.cbRouteSheet->addItem(as_text(routesheet), QVariant::fromValue(routesheet));

    ui_.cbRoute->clearEditText();
    ui_.cbBus->clearEditText();
    ui_.leStatus->clear();

    return true;
  }

  return false;
}

bool EntityManager::insert_bus()
{
  Bus bus;
  
  // Get gov_number
  if (bus.gov_number = ui_.leGovNumber->text().trimmed();
      bus.gov_number.isEmpty()) {
    last_error_ = "The gov number field must be filled";
    return false;
  }

  // Get driver_id
  QVariant data;

  if (data = ui_.cbDriver->currentData();
      data.isNull()) {
    last_error_ = "Driver must be selected";
    return false;
  }

  bus.driver_id = qvariant_cast<Driver>(data).id;

  // Get model
  if (bus.model = ui_.leModel->text().trimmed();
      bus.model.isEmpty()) {
    last_error_ = "The model field must be filled";
    return false;
  }

  // Get route_number
  if (bus.route_number = ui_.leRouteNumber->text().trimmed();
      bus.route_number.isEmpty()) {
    last_error_ = "The route number field must be filled";
    return false;
  }

  // Get capacity
  if (QString str = ui_.leCapacity->text();
      str.isEmpty()) {
    last_error_ = "The capacity filed must be filled";
    return false;
  } else {
    bus.capacity = str.toUInt();
  }

  if (QSqlQuery query = query_insert(bus);
      exec_with_check(query)) {
    bus.id = query.lastInsertId().toUInt();
    ui_.cbBus->addItem(as_text(bus), QVariant::fromValue(bus));

    ui_.leGovNumber->clear();
    ui_.cbDriver->clearEditText();
    ui_.leModel->clear();
    ui_.leRouteNumber->clear();
    ui_.leCapacity->clear();

    return true;
  }

  return true;
}

bool EntityManager::insert_driver()
{
  Driver driver;

  // Get name
  if (driver.name = ui_.leName->text().trimmed();
      driver.name.isEmpty()) {
    last_error_ = "The name field must be filled";
    return false;
  }

  // Get passport
  if (driver.passport = ui_.lePassport->text().trimmed();
      driver.passport.isEmpty()) {
    last_error_ = "The passport field must be filled";
    return false;
  }

  // Get phone
  if (driver.phone = ui_.lePhone->text().trimmed();
      driver.phone.isEmpty()) {
    last_error_ = "The driver field must be filled";
    return false;
  }

  // Get route_sheet_id
  if (QVariant data = ui_.cbRouteSheet->currentData();
      !data.isNull()) {
    driver.route_sheet_id = qvariant_cast<RouteSheet>(data).id;
  }

  if (QSqlQuery query = query_insert(driver);
      exec_with_check(query)) {
    driver.id = query.lastInsertId().toUInt();
    ui_.cbDriver->addItem(as_text(driver), QVariant::fromValue(driver));

    ui_.leName->clear();
    ui_.lePassport->clear();
    ui_.lePhone->clear();
    ui_.cbRouteSheet->clearEditText();

    return true;
  }

  return false;
}

bool EntityManager::remove_route(id_t route_id)
{
  QSqlQuery query = query_delete_route(route_id);
  return exec_with_check(query);
}

bool EntityManager::remove_routesheet(id_t routesheet_id)
{
  QSqlQuery query = query_delete_routesheet(routesheet_id);
  return exec_with_check(query);
}

bool EntityManager::remove_bus(id_t bus_id)
{
  QSqlQuery query = query_delete_bus(bus_id);
  return exec_with_check(query);
}

bool EntityManager::remove_driver(id_t driver_id)
{
  QSqlQuery query = query_delete_driver(driver_id);
  return exec_with_check(query);
}


QString EntityManager::last_error() const { return last_error_; }

bool EntityManager::exec_with_check(QSqlQuery& query)
{
  if (!query.exec()) {
    last_error_ = query.lastError().text();
    return false;
  }

  return true;
}


QString EntityManager::as_text(const Route& route) const
{
  return QString{ "%1 -> %2 [%3; %4]" }
          .arg(route.station_start)
          .arg(route.station_end)
          .arg(route.time_start.toString())
          .arg(route.time_end.toString());
}

QString EntityManager::as_text(const RouteSheet& routesheet) const
{
  const Route route = find_in_combobox<Route>(ui_.cbRoute, routesheet.route_id);
  const Bus bus = find_in_combobox<Bus>(ui_.cbBus, routesheet.bus_id);

  return QString{ "%1 %2 (%3)" }
          .arg(as_text(route))
          .arg(as_text(bus))
          .arg(routesheet.status);
}

QString EntityManager::as_text(const Bus& bus) const
{
  return QString{ "№%1 (%2)" }
          .arg(bus.route_number)
          .arg(bus.gov_number);
}

QString EntityManager::as_text(const Driver& driver) const
{
  return QString{ "%1 (%2)" }
          .arg(driver.name)
          .arg(driver.phone);
}



} // namespace carop