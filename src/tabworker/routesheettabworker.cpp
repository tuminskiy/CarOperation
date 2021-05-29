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
  const int bus_index = find_in_combobox_by_id<Bus>(ui_.cbBus, routesheet.bus_id).index;

  ui_.cbRoute->setCurrentIndex(route_index);
  ui_.cbBus->setCurrentIndex(bus_index);
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
  if (data = ui_.cbBus->currentData(); data.isNull()) {
    throw std::invalid_argument("Bus must be selected");
  }

  routesheet.bus_id = qvariant_cast<Bus>(data).id;

  // Get status
  if (routesheet.status = ui_.leStatus->text().trimmed(); routesheet.status.isEmpty()) {
    throw std::invalid_argument("The status field must be filled");
  }

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


void RouteSheetTabWorker::after_success_insert(id_t id, const QVariant& value)
{
  RouteSheet routesheet = qvariant_cast<RouteSheet>(value);
  routesheet.id = id;

  ui_.cbRouteSheet->addItem(as_text(routesheet), QVariant::fromValue(routesheet));
}

void RouteSheetTabWorker::after_success_remove(id_t id)
{
  const int index = find_in_combobox_by_id<RouteSheet>(ui_.cbRouteSheet, id).index;
  
  ui_.cbRouteSheet->removeItem(index);
}

void RouteSheetTabWorker::after_success_update(const QVariant& value)
{
  const RouteSheet routesheet = qvariant_cast<RouteSheet>(value);

  const int index = find_in_combobox_by_id<RouteSheet>(ui_.cbRouteSheet, routesheet.id).index;

  ui_.cbRouteSheet->setItemData(index, value);
  ui_.cbRouteSheet->setItemText(index, as_text(routesheet));
}


} // namespace carop