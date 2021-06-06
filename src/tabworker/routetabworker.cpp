#include "tabworker/routetabworker.hpp"
#include "storage/query.hpp"

#include <QSqlQuery>
#include <QVariant>

namespace carop
{

RouteTabWorker::RouteTabWorker(Ui::MainWindow& ui)
  : BaseTabWorker{ ui }
{ }

void RouteTabWorker::fill_input()
{
  if (!selected_id_.has_value()) {
    throw std::invalid_argument("Not selected id");
  }

  const Route route = find_in_combobox_by_id<Route>(ui_.cbRoute, selected_id_.value()).value;

  ui_.leStationStart->setText(route.station_start);
  ui_.leStationEnd->setText(route.station_end);
  ui_.teTimeStart->setTime(route.time_start);
  ui_.teTimeEnd->setTime(route.time_end);
}

void RouteTabWorker::clear_input()
{
  selected_id_ = std::nullopt;

  ui_.leStationStart->clear();
  ui_.leStationEnd->clear();
  ui_.teTimeStart->clear();
  ui_.teTimeEnd->clear();
}


QVariant RouteTabWorker::collect_data() const
{
  Route route;

  if (selected_id_) {
    route.id = selected_id_.value();
  }

  // Get station_start
  if (route.station_start = ui_.leStationStart->text().trimmed(); route.station_start.isEmpty()) {
    throw std::invalid_argument("The station start field must be filled");
  }

  // Get station_end
  if (route.station_end = ui_.leStationEnd->text().trimmed(); route.station_end.isEmpty()) {
    throw std::invalid_argument("The station end filed must be filled");
  }

  // Get time_start and time_end
  route.time_start = ui_.teTimeStart->time();
  route.time_end = ui_.teTimeEnd->time();

  return QVariant::fromValue(route);
}


QSqlQuery RouteTabWorker::prepare_insert(const QVariant& value) const
{
  const Route route = qvariant_cast<Route>(value);

  return query_insert(route);
}

QSqlQuery RouteTabWorker::prepare_remove(id_t id) const
{
  return query_delete_route(id);
}

QSqlQuery RouteTabWorker::prepare_update(const QVariant& value) const
{
  const Route route = qvariant_cast<Route>(value);
  
  return query_update(route);
}

} // namespace carop