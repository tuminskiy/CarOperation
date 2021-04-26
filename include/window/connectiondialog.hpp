#pragma once

#include "ui_connectiondialog.h"

#include <QSqlDatabase>

namespace carop 
{

class ConnectionDialog : public QDialog
{
  Q_OBJECT

public:
  explicit ConnectionDialog(QWidget* parent = nullptr);

  QSqlDatabase database() const;

private slots:
  void connect_click();

private:
  Ui::ConnectionDialog ui_;
  QSqlDatabase db_;
};

} // namespace carop 