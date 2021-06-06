#include "tabworker/viewworker.hpp"
#include "storage/query.hpp"

#include <QSqlQuery>
#include <QSqlError>

namespace carop
{

ViewWorker::ViewWorker(Ui::MainWindow& ui)
  : BaseTabWorker{ ui }
{ }


void ViewWorker::fill_input()
{
  QSqlQuery query = query_select_table("bus_driver_view");

  if (!query.exec()) {
    throw std::runtime_error(query.lastError().text().toStdString());
  }

  while (query.next()) {
    if (selected_id_.value() == query.value("id").toUInt()) {
      ui_.leBdvGovNumber->setText(query.value("gov_number").toString());
      ui_.leBdvModel->setText(query.value("model").toString());
      ui_.leBdvRouteNumber->setText(query.value("route_number").toString());
      ui_.leBdvCapacity->setText(query.value("capacity").toString());
      ui_.leBdvName->setText(query.value("name").toString());
      ui_.leBdvPhone->setText(query.value("phone").toString());
      ui_.leBdvStatus->setText(query.value("status").toString());

      break;
    }
  }
}

void ViewWorker::clear_input()
{
  selected_id_ = std::nullopt;

  ui_.leBdvGovNumber->clear();
  ui_.leBdvModel->clear();
  ui_.leBdvRouteNumber->clear();
  ui_.leBdvCapacity->clear();
  ui_.leBdvName->clear();
  ui_.leBdvPhone->clear();
  ui_.leBdvStatus->clear();
}


QVariant ViewWorker::collect_data() const
{
  BusDriverView bdv;

  if (!selected_id_.has_value()) {
    throw std::runtime_error("It is forbidden to add or remove records for the view");
  }

  bdv.id = selected_id_.value();

  if (bdv.gov_number = ui_.leBdvGovNumber->text().trimmed(); bdv.gov_number.isEmpty()) {
    throw std::invalid_argument("The gov number field must be filled");
  }

  if (bdv.model = ui_.leBdvModel->text().trimmed(); bdv.model.isEmpty()) {
    throw std::invalid_argument("The model field must be filled");
  }

  if (bdv.route_number = ui_.leBdvRouteNumber->text().trimmed(); bdv.route_number.isEmpty()) {
    throw std::invalid_argument("The route number field must be filled");
  }

  if (const QString str = ui_.leBdvCapacity->text().trimmed(); str.isEmpty()) {
    throw std::invalid_argument("The capacity filed must be filled");
  } else {
    bdv.capacity = str.toUInt();
  }

  if (bdv.name = ui_.leBdvName->text().trimmed(); bdv.name.isEmpty()) {
    throw std::invalid_argument("The name field must be filled");
  }

  if (bdv.phone = ui_.leBdvPhone->text().trimmed(); bdv.phone.isEmpty()) {
    throw std::invalid_argument("The phone field must be filled");
  }

  bdv.status = ui_.leBdvStatus->text().trimmed();

  return QVariant::fromValue(bdv);
}


QSqlQuery ViewWorker::prepare_insert(const QVariant& value) const
{
  throw std::runtime_error("It is forbidden to add records to the view");
}

QSqlQuery ViewWorker::prepare_remove(id_t id) const
{
  throw std::runtime_error("It is forbidden to delete records from the view");
}

QSqlQuery ViewWorker::prepare_update(const QVariant& value) const
{
  const BusDriverView bdw = qvariant_cast<BusDriverView>(value);
  
  return query_update(bdw);
}


} // namespace carop