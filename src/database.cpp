#include "storage/database.hpp"

namespace carop
{

Database::Database(const QSqlDatabase& db)
  : db_{ db }
{ }

} // namespace carop