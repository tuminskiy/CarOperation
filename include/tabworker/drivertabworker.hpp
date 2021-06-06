#pragma once

#include "basetabworker.hpp"

namespace carop
{

class DriverTabWorker : public BaseTabWorker
{
public:
  explicit DriverTabWorker(Ui::MainWindow& ui);

  void fill_input() override;
  void clear_input() override;

protected:
  QVariant collect_data() const override;  
  
  QSqlQuery prepare_insert(const QVariant& value) const override;
  QSqlQuery prepare_remove(id_t id) const override;
  QSqlQuery prepare_update(const QVariant& value) const override;
};

} // namespace carop