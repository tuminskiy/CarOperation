#include "window/startstationdialog.hpp"

#include <QMessageBox>

namespace carop
{

StartStationDialog::StartStationDialog(QWidget* parent)
  : QDialog{ parent }
{
  ui_.setupUi(this);

  connect(ui_.bCancel, &QPushButton::clicked,
          this, &StartStationDialog::close);
  
  connect(ui_.bConfirm, &QPushButton::clicked,
          this, &StartStationDialog::confirm_click);
}

void StartStationDialog::confirm_click()
{
  const QString station_start = ui_.leStationStart->text().trimmed();

  if (station_start.isEmpty()) {
    QMessageBox::critical(this, "Error", "The field station_start must be filled");
    return;
  }

  emit confirmed(station_start);

  ui_.leStationStart->clear();

  close();
}

} // namespace carop