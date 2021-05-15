#pragma once

#include <QSqlQuery>
#include <memory>

namespace carop
{

class Route;
class RouteSheet;
class Bus;
class Driver;


QSqlQuery query_select_table(const QString& table_name);


QSqlQuery query_insert(const Route& route);

QSqlQuery query_insert(const RouteSheet& route_sheet);

QSqlQuery query_insert(const Bus& bus);

QSqlQuery query_insert(const Driver& driver);

} // namespace carop