#include "window/selectiddialog.hpp"
#include "storage/query.hpp"

#include <QMessageBox>
#include <QDialog>
#include <QSqlError>

namespace carop
{

SelectIdDialog::SelectIdDialog(QWidget* parent)
  : QDialog{ parent }
  , ui_{ }
  , model_{ }
  , selected_id_{ }
{
  ui_.setupUi(this);

  ui_.tvData->setModel(&model_);

  connect(ui_.bConfirm, &QPushButton::clicked,
          this, &SelectIdDialog::confirm_click);

  connect(ui_.bCancel, &QPushButton::clicked,
          this, &SelectIdDialog::reject);
}

id_t SelectIdDialog::selected_id() const { return selected_id_; }

void SelectIdDialog::show_table(const QString& table_name)
{
  QSqlQuery query = query_select_table(table_name);

  if (!query.exec()) {
    QMessageBox::critical(this, "Error", query.lastError().text());
    reject();
  }

  model_.setQuery(query);
}

void SelectIdDialog::confirm_click()
{
  auto selected = ui_.tvData->selectionModel()->selectedIndexes();

  auto it = std::find_if(std::begin(selected), std::end(selected),
    [](const QModelIndex& index) {
      return index.column() == 0;
    }
  );

  if (it == std::end(selected)) {
    QMessageBox::critical(this, "Error", "Record not selected");
    return;
  }

  selected_id_ = it->data().toUInt();

  emit accept();
}


} // namespace carop
