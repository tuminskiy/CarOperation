#pragma once

#include "ui_startstationdialog.h"

namespace carop
{

class StartStationDialog : public QDialog
{
  Q_OBJECT

public:
  explicit StartStationDialog(QWidget* parent = nullptr);

signals:
  void confirmed(const QString& station_start);

private slots:
  void confirm_click();

private:
  Ui::StartStationDialog ui_;
};

} // namespace carop