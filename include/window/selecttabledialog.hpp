#pragma once

#include "ui_selecttabledialog.h"

namespace carop
{

class SelectTableDialog : public QDialog
{
  Q_OBJECT

public:
  explicit SelectTableDialog(QWidget* parent = nullptr);

signals:
  void confirmed(const QString& table_name);

private slots:
  void confirm_click();

private:
  Ui::SelectTableDialog ui_;
};

} // namespace carop