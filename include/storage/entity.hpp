#pragma once

#include <QString>
#include <QTime>
#include <optional>

namespace carop
{

using id_t = unsigned int;

struct Route
{
  id_t id;
  QString station_start;
  QString station_end;
  QTime time_start;
  QTime time_end;
};

struct RouteSheet
{
  id_t id;
  id_t route_id;
  id_t bus_id;
  QString status;
};

struct Bus
{
  id_t id;
  QString gov_number;
  id_t driver_id;
  QString model;
  QString route_number;
  unsigned int capacity;
};

struct Driver
{
  id_t id;
  QString name;
  QString passport;
  QString phone;
  std::optional<id_t> route_sheet_id;
};

namespace constraints
{

constexpr int station_start_max = 255;
constexpr int station_end_max   = 255;
constexpr int status_max        = 255;
constexpr int gov_number_max    = 9;
constexpr int model_max         = 255;
constexpr int route_number_max  = 5;
constexpr int name_max          = 255;
constexpr int passport_max      = 11;
constexpr int phone_max         = 11;

} // namespace carop::constraints

} // namespace carop