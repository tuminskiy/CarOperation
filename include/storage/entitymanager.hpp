#pragma once

#include "ui_mainwindow.h"
#include "entity.hpp"

#include <QSqlDatabase>
#include <QVariant>
#include <memory>

Q_DECLARE_METATYPE(carop::Route)
Q_DECLARE_METATYPE(carop::RouteSheet)
Q_DECLARE_METATYPE(carop::Bus)
Q_DECLARE_METATYPE(carop::Driver)

namespace carop
{

class EntityManager
{
public:
  explicit EntityManager(Ui::MainWindow& ui_);

  bool load_data();

  bool insert_route();
  bool insert_routesheet();
  bool insert_bus();
  bool insert_driver();

  bool remove_route(id_t route_id);
  bool remove_routesheet(id_t routesheet_id);
  bool remove_bus(id_t bus_id);
  bool remove_driver(id_t driver_id);

  QString last_error() const;

private:
  bool exec_with_check(QSqlQuery& query);

  QString as_text(const Route& route) const;
  QString as_text(const RouteSheet& routesheet) const;
  QString as_text(const Bus& bus) const;
  QString as_text(const Driver& driver) const;

  template <class T>
  T find_in_combobox(QComboBox* cb, id_t id) const
  {
    for (int i{ 0 }; i < cb->count(); i++) {
      const QVariant data = cb->itemData(i);

      if (const T value = qvariant_cast<T>(data); value.id == id) {
        return value;
      }
    }

    return { };
  }

private:
  Ui::MainWindow& ui_;
  QString last_error_;
};

} // namespace carop