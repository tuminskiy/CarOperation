#pragma once

#include "ui_mainwindow.h"
#include "storage/entity.hpp"

#include <QVariant>
#include <optional>

class QSqlQuery;

namespace carop
{

class BaseTabWorker
{
public:
  explicit BaseTabWorker(Ui::MainWindow& ui);
  virtual ~BaseTabWorker() = default;

  void load_data();

  void set_selected_id(id_t id);

  void insert();
  void remove();
  void update();

  virtual void fill_input() = 0;
  virtual void clear_input() = 0;

protected:
  QString as_text(const Route& route) const;
  QString as_text(const RouteSheet& routesheet) const;
  QString as_text(const Bus& bus) const;
  QString as_text(const Driver& driver) const;

  virtual QVariant collect_data() const = 0;
  
  virtual QSqlQuery prepare_insert(const QVariant& value) const = 0;
  virtual QSqlQuery prepare_remove(id_t id) const = 0;
  virtual QSqlQuery prepare_update(const QVariant& value) const = 0;

  template <class T>
  struct IndexValue { int index; T value; };

  template <class T, class Pred>
  IndexValue<T> find_in_combobox(QComboBox* cb, Pred pred) const
  {
    for (int i = 0; i < cb->count(); i++) {
      const T value = qvariant_cast<T>(cb->itemData(i));

      if (pred(value)) {
        return {i, value};
      }
    }

    return {-1, {}};
  }

  template <class T>
  IndexValue<T> find_in_combobox_by_id(QComboBox* cb, id_t id) const
  {
    const auto pred = [id](const T& value) {
      return value.id == id;
    };

    return find_in_combobox<T>(cb, pred);
  }

protected:
  Ui::MainWindow& ui_;
  std::optional<id_t> selected_id_ = std::nullopt;

private:
  void exec_with_check(QSqlQuery& query);
};

} // namespace carop