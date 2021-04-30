#include "storage/query.hpp"

#include <QSqlQuery>
#include <QVariant>

namespace carop
{

QSqlQuery query_select_table(const QString& table_name, const QSqlDatabase& db)
{
  QSqlQuery query{ db };
  
  const QString str = QString("SELECT * FROM %1;").arg(table_name);
  query.prepare(str);

  return query;
}

} // namespace carop