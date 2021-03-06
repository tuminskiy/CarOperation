#include "tabworker/routesheettabworker.hpp"
#include "storage/query.hpp"

#include <QSqlQuery>
#include <QVariant>

namespace carop
{

RouteSheetTabWorker::RouteSheetTabWorker(Ui::MainWindow& ui)
  : BaseTabWorker{ ui }
{ }


void RouteSheetTabWorker::fill_input()
{
  if (!selected_id_.has_value()) {
    throw std::invalid_argument("Not selected id");
  }

  const RouteSheet routesheet = find_in_combobox_by_id<RouteSheet>(ui_.cbRouteSheet, selected_id_.value()).value;

  const int route_index = find_in_combobox_by_id<Route>(ui_.cbRoute, routesheet.route_id).index;
  const int bus_index = find_in_combobox_by_id<Bus>(ui_.cbBus, routesheet.bus_id.value_or(0)).index;

  ui_.cbRoute->setCurrentIndex(route_index);
  ui_.cbBus->setCurrentIndex(bus_index);
  ui_.leStatus->setText(routesheet.status);
}

void RouteSheetTabWorker::clear_input()
{
  selected_id_ = std::nullopt;

  ui_.cbRoute->setCurrentIndex(0);
  ui_.cbBus->setCurrentIndex(0);
  ui_.leStatus->clear();
}


QVariant RouteSheetTabWorker::collect_data() const
{
  RouteSheet routesheet;

  if (selected_id_.has_value()) {
    routesheet.id = selected_id_.value();
  }

  QVariant data;

  // Get route_id
  if (data = ui_.cbRoute->currentData(); data.isNull()) {
    throw std::invalid_argument("Route must be selected");
  }

  routesheet.route_id = qvariant_cast<Route>(data).id;

  // Get bus_id
  if (data = ui_.cbBus->currentData(); !data.isNull()) {
    routesheet.bus_id = qvariant_cast<Bus>(data).id;
  }

  // Get status
  routesheet.status = ui_.leStatus->text().trimmed();

  return QVariant::fromValue(routesheet);
}


QSqlQuery RouteSheetTabWorker::prepare_insert(const QVariant& value) const
{
  const RouteSheet routesheet = qvariant_cast<RouteSheet>(value);

  return carop::query_insert(routesheet);
}

QSqlQuery RouteSheetTabWorker::prepare_remove(id_t id) const
{
  return query_delete_routesheet(id);
}

QSqlQuery RouteSheetTabWorker::prepare_update(const QVariant& value) const
{
  const RouteSheet routesheet = qvariant_cast<RouteSheet>(value);
  
  return carop::query_update(routesheet);
}


} // namespace carop