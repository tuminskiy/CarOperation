#pragma once

#include <QSqlDatabase>

namespace carop
{

class Database
{
public:
  explicit Database(const QSqlDatabase& db);


private:
  QSqlDatabase db_;
};

} // namespace carop

