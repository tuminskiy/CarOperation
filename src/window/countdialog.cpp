#include "window/countdialog.hpp"

#include <QMessageBox>

namespace carop
{

CountDialog::CountDialog(QWidget* parent)
  : QDialog{ parent }
{
  ui_.setupUi(this);

  connect(ui_.bConfirm, &QPushButton::clicked,
          this, &CountDialog::confirm_click);
}

void CountDialog::confirm_click()
{
  const QString count = ui_.leCount->text().trimmed();

  if (count.isEmpty()) {
    QMessageBox::critical(this, "Error", "Field count must be filled");
    return;
  }

  emit confirmed(count.toInt());

  ui_.leCount->clear();

  close();
}

} // namespace carop