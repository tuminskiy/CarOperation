#pragma once

#include "basetabworker.hpp"

namespace carop
{

class RouteSheetTabWorker : public BaseTabWorker
{
public:
  explicit RouteSheetTabWorker(Ui::MainWindow& ui);

  void fill_input() override;
  void clear_input() override;

protected:
  QVariant collect_data() const override;

  QSqlQuery prepare_insert(const QVariant& value) const override;
  QSqlQuery prepare_remove(id_t id) const override;
  QSqlQuery prepare_update(const QVariant& value) const override;

  void after_success_insert(id_t id, const QVariant& value) override;
  void after_success_remove(id_t id) override;
  void after_success_update(const QVariant& value) override;
};

} // namespace carop