#include "storage/query.hpp"
#include "storage/entity.hpp"

#include <QSqlQuery>
#include <QVariant>
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>

namespace carop
{

QSqlQuery query_select_table(const QString& table_name)
{
  QSqlQuery query;
  
  const QString str = QString{ "SELECT * FROM %1;" }.arg(table_name);
  query.prepare(str);

  return query;
}

QSqlQuery query_insert(const Route& route)
{
  QSqlQuery query;

  query.prepare("SELECT insert_route(:station_start, :station_end, :time_start, :time_end);");  
  query.bindValue(":station_start", route.station_start);
  query.bindValue(":station_end", route.station_end);
  query.bindValue(":time_start", route.time_start);
  query.bindValue(":time_end", route.time_end);

  return query;
}

QSqlQuery query_insert(const RouteSheet& route_sheet)
{
  QSqlQuery query;

  query.prepare("SELECT insert_routesheet(:route_id, :bus_id, :status);");
  query.bindValue(":route_id", route_sheet.route_id);
  query.bindValue(":bus_id", route_sheet.bus_id);
  query.bindValue(":status", route_sheet.status);

  return query;
}

QSqlQuery query_insert(const Bus& bus)
{
  QSqlQuery query;

  query.prepare("SELECT insert_bus(:gov_number, :driver_id, :model, :route_number, :capacity);");
  query.bindValue(":gov_number", bus.gov_number);
  query.bindValue(":driver_id", bus.driver_id);
  query.bindValue(":model", bus.model);
  query.bindValue(":route_number", bus.route_number);
  query.bindValue(":capacity", bus.capacity);

  return query;
}

QSqlQuery query_insert(const Driver& driver)
{
  QSqlQuery query;  

  query.prepare("SELECT insert_driver(:name, :passport, :phone, :route_sheet_id);");
  query.bindValue(":name", driver.name);
  query.bindValue(":passport", driver.passport);
  query.bindValue(":phone", driver.phone);
  query.bindValue(":route_sheet_id", 
    driver.route_sheet_id ? QVariant{ driver.route_sheet_id.value() } : QVariant{}
  );

  return query;
}

} // namespace carop