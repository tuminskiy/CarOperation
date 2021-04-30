#include "window/connectiondialog.hpp"

#include <QMessageBox>
#include <QSqlError>

namespace carop
{

ConnectionDialog::ConnectionDialog(std::shared_ptr<QSqlDatabase> db_sptr, QWidget* parent)
  : QDialog{parent}
  , ui_{}
  , db_sptr_{db_sptr}
{
  ui_.setupUi(this);

  connect(ui_.bConnect, &QPushButton::clicked,
          this, &ConnectionDialog::connect_click);
}

void ConnectionDialog::connect_click()
{
  // Check host
  const QString host{ ui_.leHost->text() };

  if (host.isEmpty()) {
    QMessageBox::critical(this, "Error", "Host must be fill");
    return;
  }

  // Check port
  bool ok{ false };
  const int port{ ui_.lePort->text().toInt(&ok) };

  if (!ok) {
    QMessageBox::critical(this, "Error", "Port is not valid");
    return;
  }

  // Check database name
  const QString db_name{ ui_.leDatabase->text() };

  if (db_name.isEmpty()) {
    QMessageBox::critical(this, "Error", "Database must be fill");
    return;
  }

  // Check username
  const QString username{ ui_.leUsername->text() };

  if (username.isEmpty()) {
    QMessageBox::critical(this, "Error", "Username must be fill");
    return;
  }

  const QString password{ ui_.lePassword->text() };

  // Try connect
  db_sptr_->setHostName(host);
  db_sptr_->setPort(port);
  db_sptr_->setDatabaseName(db_name);
  db_sptr_->setUserName(username);
  db_sptr_->setPassword(password);

  if (!db_sptr_->open()) {
    QMessageBox::critical(this, "Error", "Failed open database.\n" + db_sptr_->lastError().text());
    return;
  }

  emit accepted();
}


} // namespace carop