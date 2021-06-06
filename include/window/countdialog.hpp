#pragma once

#include "ui_countdialog.h"

namespace carop
{

class CountDialog : public QDialog
{
  Q_OBJECT

public:
  explicit CountDialog(QWidget* parent = nullptr);

signals:
  void confirmed(int count);

private slots:
  void confirm_click();

private:
  Ui::CountDialog ui_;
};

} // namespace carop