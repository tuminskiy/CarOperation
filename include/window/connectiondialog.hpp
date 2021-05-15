#pragma once

#include "ui_connectiondialog.h"

#include <QSqlDatabase>
#include <memory>

namespace carop 
{

class ConnectionDialog : public QDialog
{
  Q_OBJECT

public:
  explicit ConnectionDialog(QWidget* parent = nullptr);

private slots:
  void connect_click();

private:
  Ui::ConnectionDialog ui_;
};

} // namespace carop 