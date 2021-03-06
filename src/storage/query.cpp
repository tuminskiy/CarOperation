#include "storage/query.hpp"
#include "storage/entity.hpp"

#include <QSqlQuery>
#include <QVariant>
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>

namespace carop
{

void query_bind(QSqlQuery& query, id_t id) { query.bindValue(":id", id); }

void query_bind(QSqlQuery& query, const Route& route)
{
  query.bindValue(":station_start", route.station_start);
  query.bindValue(":station_end", route.station_end);
  query.bindValue(":time_start", route.time_start);
  query.bindValue(":time_end", route.time_end);
}

void query_bind(QSqlQuery& query, const RouteSheet& route_sheet)
{
  query.bindValue(":route_id", route_sheet.route_id);
  query.bindValue(":bus_id", route_sheet.bus_id.has_value()
    ? QVariant::fromValue(route_sheet.bus_id.value())
    : QVariant{ });
  query.bindValue(":status", route_sheet.status);
}

void query_bind(QSqlQuery& query, const Bus& bus)
{
  query.bindValue(":gov_number", bus.gov_number);
  query.bindValue(":driver_id", bus.driver_id);
  query.bindValue(":model", bus.model);
  query.bindValue(":route_number", bus.route_number);
  query.bindValue(":capacity", bus.capacity);
}

void query_bind(QSqlQuery& query, const Driver& driver)
{
  query.bindValue(":name", driver.name);
  query.bindValue(":passport", driver.passport);
  query.bindValue(":phone", driver.phone);
  query.bindValue(":route_sheet_id", 
    driver.route_sheet_id ? QVariant{ driver.route_sheet_id.value() } : QVariant{}
  );
}


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
  query_bind(query, route);

  return query;
}

QSqlQuery query_insert(const RouteSheet& route_sheet)
{
  QSqlQuery query;

  query.prepare("SELECT insert_routesheet(:route_id, :bus_id, :status);");
  query_bind(query, route_sheet);

  return query;
}

QSqlQuery query_insert(const Bus& bus)
{
  QSqlQuery query;

  query.prepare("SELECT insert_bus(:gov_number, :driver_id, :model, :route_number, :capacity);");
  query_bind(query, bus);

  return query;
}

QSqlQuery query_insert(const Driver& driver)
{
  QSqlQuery query;  

  query.prepare("SELECT insert_driver(:name, :passport, :phone, :route_sheet_id);");
  query_bind(query, driver);

  return query;
}


QSqlQuery query_delete_route(id_t route_id)
{
  QSqlQuery query;

  query.prepare("SELECT delete_route(:id);");
  query_bind(query, route_id);

  return query;
}

QSqlQuery query_delete_routesheet(id_t routesheet_id)
{
  QSqlQuery query;

  query.prepare("SELECT delete_routesheet(:id);");
  query_bind(query, routesheet_id);

  return query;
}

QSqlQuery query_delete_bus(id_t bus_id)
{
  QSqlQuery query;

  query.prepare("SELECT delete_bus(:id);");
  query_bind(query, bus_id);

  return query;
}

QSqlQuery query_delete_driver(id_t driver_id)
{
  QSqlQuery query;

  query.prepare("SELECT delete_driver(:id);");
  query_bind(query, driver_id);

  return query;
}


QSqlQuery query_update(const Route& route)
{
  QSqlQuery query;

  query.prepare("SELECT update_route(:id, :station_start, :station_end, :time_start, :time_end);");
  query_bind(query, route.id);
  query_bind(query, route);
  
  return query;
}

QSqlQuery query_update(const RouteSheet& route_sheet)
{
  QSqlQuery query;

  query.prepare("SELECT update_routesheet(:id, :route_id, :bus_id, :status);");
  query_bind(query, route_sheet.id);
  query_bind(query, route_sheet);

  return query;
}

QSqlQuery query_update(const Bus& bus)
{
  QSqlQuery query;

  query.prepare("SELECT update_bus(:id, :gov_number, :driver_id, :model, :route_number, :capacity);");
  query_bind(query, bus.id);
  query_bind(query, bus);

  return query;
}

QSqlQuery query_update(const Driver& driver)
{
  QSqlQuery query;

  query.prepare("SELECT update_driver(:id, :name, :passport, :phone, :route_sheet_id);");
  query_bind(query, driver.id);
  query_bind(query, driver);

  return query;
}

QSqlQuery query_update(const BusDriverView& bdw)
{
  QSqlQuery query;

  query.prepare(
    "UPDATE bus_driver_view SET "
    "  gov_number = :gov_number, "
    "  model = :model, "
    "  route_number = :route_number, "
    "  capacity = :capacity, "
    "  name = :name, "
    "  phone = :phone, "
    "  status = :status "
    "WHERE id = :id;"
  );

  query.bindValue(":gov_number", bdw.gov_number);
  query.bindValue(":model", bdw.model);
  query.bindValue(":route_number", bdw.route_number);
  query.bindValue(":capacity", bdw.capacity);
  query.bindValue(":name", bdw.name);
  query.bindValue(":phone", bdw.phone);
  query.bindValue(":status", bdw.status);
  query.bindValue(":id", bdw.id);

  return query;
}


QSqlQuery query_having(int count_more_then)
{
  QSqlQuery query;

  query.prepare(
    "SELECT b.model FROM RouteSheet rs "
    "LEFT JOIN bus b ON b.id = rs.bus_id "
    "GROUP BY b.model HAVING COUNT(b.model) > :count_more_then;"
  );

  query.bindValue(":count_more_then", count_more_then);

  return query;
}


QSqlQuery query_view(const QString& order_by)
{
  QSqlQuery query;

  const QString str = QString{
    "SELECT * FROM bus_driver_view ORDER BY %1;"
  }.arg(order_by);

  query.prepare(str);

  return query;
}


QSqlQuery query_any(const QString& station_start)
{
  QSqlQuery query;

  query.prepare(
    "SELECT route.station_start, count_driver_on_start_station(:station_start) driver_count "
    "FROM routesheet, route "
    "WHERE route_id = ANY ( SELECT id FROM route_by_station_start(:station_start) );"
  );

  query.bindValue(":station_start", station_start);

  return query;
}

} // namespace carop