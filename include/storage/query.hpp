#pragma once

#include "entity.hpp"

class QSqlQuery;
class QString;

namespace carop
{

QSqlQuery query_select_table(const QString& table_name);


QSqlQuery query_insert(const Route& route);

QSqlQuery query_insert(const RouteSheet& route_sheet);

QSqlQuery query_insert(const Bus& bus);

QSqlQuery query_insert(const Driver& driver);


QSqlQuery query_delete_route(id_t route_id);

QSqlQuery query_delete_routesheet(id_t routesheet_id);

QSqlQuery query_delete_bus(id_t bus_id);

QSqlQuery query_delete_driver(id_t driver_id);


QSqlQuery query_update(const Route& route);

QSqlQuery query_update(const RouteSheet& route_sheet);

QSqlQuery query_update(const Bus& bus);

QSqlQuery query_update(const Driver& driver);


QSqlQuery query_having(int count_more_then);

} // namespace carop