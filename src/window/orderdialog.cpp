#include "window/orderdialog.hpp"

namespace carop
{

const std::unordered_map<OrderDialog::Order, QString> OrderDialog::order_str = {
  { Order::Id, "id"},
  { Order::GovNumber, "gov_number" },
  { Order::Model, "model" },
  { Order::RouteNumber, "route_number" },
  { Order::Capacity, "capacity" },
  { Order::Name, "name" },
  { Order::Phone, "phone" },
  { Order::Status, "status" }
};

OrderDialog::OrderDialog(QWidget* parent)
 : QDialog{ parent }
{
  ui_.setupUi(this);

  for (const auto& [order, str] : order_str) {
    ui_.cbOrderBy->addItem(str);
  }

  connect(ui_.bCancel, &QPushButton::clicked,
          this, &OrderDialog::close);

  connect(ui_.bConfirm, &QPushButton::clicked,
          this, &OrderDialog::confirm_click);
}

void OrderDialog::confirm_click()
{
  emit confirmed(ui_.cbOrderBy->currentText());
  close();
}

} // namespace carop