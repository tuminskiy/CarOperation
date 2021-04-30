#pragma once

#include <QSqlDatabase>

namespace carop
{

QSqlQuery query_select_table(const QString& table_name, const QSqlDatabase& db = QSqlDatabase{});

} // namespace carop