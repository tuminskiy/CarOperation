#pragma once

#include <QSqlDatabase>
#include <QSqlQuery>
#include <memory>

namespace carop
{

class Route;
class RouteSheet;
class Bus;
class Driver;


QSqlQuery query_select_table(const QString& table_name, std::shared_ptr<QSqlDatabase> db_sptr);


QSqlQuery query_insert(const Route& route, std::shared_ptr<QSqlDatabase> db_sptr);

QSqlQuery query_insert(const RouteSheet& route_sheet, std::shared_ptr<QSqlDatabase> db_sptr);

QSqlQuery query_insert(const Bus& bus, std::shared_ptr<QSqlDatabase> db_sptr);

QSqlQuery query_insert(const Driver& driver, std::shared_ptr<QSqlDatabase> db_sptr);

} // namespace carop