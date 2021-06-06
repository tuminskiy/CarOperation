#pragma once

#include "ui_orderdialog.h"

#include <unordered_map>

namespace carop
{

class OrderDialog : public QDialog
{
  Q_OBJECT

public:
  enum Order {
    Id = 0,
    GovNumber,
    Model,
    RouteNumber,
    Capacity,
    Name,
    Phone,
    Status
  };

  explicit OrderDialog(QWidget* parent = nullptr);

signals:
  void confirmed(const QString& order_by);

private slots:
  void confirm_click();

private:
  Ui::OrderDialog ui_;

  static const std::unordered_map<Order, QString> order_str;
};

} // namespace carop