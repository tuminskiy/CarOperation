#include "window/selecttabledialog.hpp"

#include <QMessageBox>

namespace carop
{

SelectTableDialog::SelectTableDialog(QWidget* parent)
  : QDialog(parent)
  , ui_{ }
{
  ui_.setupUi(this);

  connect(ui_.bDriver, &QPushButton::clicked,
          this, &SelectTableDialog::confirm_click);
  
  connect(ui_.bBus, &QPushButton::clicked,
          this, &SelectTableDialog::confirm_click);

  connect(ui_.bRoute, &QPushButton::clicked,
          this, &SelectTableDialog::confirm_click);
  
  connect(ui_.bRouteSheet, &QPushButton::clicked,
          this, &SelectTableDialog::confirm_click);
}

void SelectTableDialog::confirm_click()
{
  const auto button = qobject_cast<QPushButton*>(sender());
  const QString table_name = button->text();
  
  emit confirmed(table_name);
  close();
}

} // namespace carop