/* INSERT FUNCTIONS */

CREATE OR REPLACE FUNCTION insert_route(
  _station_start VARCHAR(255),
  _station_end VARCHAR(255),
  _time_start TIME,
  _time_end TIME
) 
RETURNS void
AS $$
BEGIN
  INSERT INTO Route(station_start, station_end, time_start, time_end)
  VALUES (_station_start, _station_end, _time_start, _time_end);
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION insert_routesheet(
  _route_id INTEGER,
  _bus_id INTEGER,
  _status VARCHAR(255)
)
RETURNS void
AS $$
BEGIN
  INSERT INTO RouteSheet(route_id, bus_id, status)
  VALUES (_route_id, _bus_id, _status);
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION insert_bus(
  _gov_number VARCHAR(9),
  _driver_id INTEGER,
  _model VARCHAR(255),
  _route_number VARCHAR(5),
  _capacity INTEGER
)
RETURNS void
AS $$
BEGIN
  INSERT INTO Bus(gov_number, driver_id, model, route_number, capacity)
  VALUES (_gov_number, _driver_id, _model, _route_number, _capacity);
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION insert_driver(
  _name VARCHAR(255),
  _passport CHAR(11),
  _phone CHAR(11),
  _route_sheet_id INTEGER
) 
RETURNS void
AS $$
BEGIN
  INSERT INTO Driver(name, passport, phone, route_sheet_id)
  VALUES (_name, _passport, _phone, _route_sheet_id);
END;
$$ LANGUAGE plpgsql;


/* UPDATE FUNCTIONS */

CREATE OR REPLACE FUNCTION update_route(
  _id INTEGER,
  _station_start VARCHAR(255),
  _station_end VARCHAR(255),
  _time_start TIME,
  _time_end TIME
)
RETURNS void
AS $$
BEGIN
  UPDATE Route SET
    station_start = COALESCE(_station_start, Route.station_start),
    station_end   = COALESCE(_station_end, Route.station_end),
    time_start    = COALESCE(_time_start, Route.time_start),
    time_end      = COALESCE(_time_end, Route.time_end)
  WHERE id = _id;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION update_routesheet(
  _id INTEGER,
  _route_id INTEGER,
  _bus_id INTEGER,
  _status VARCHAR(255)
)
RETURNS void
AS $$
BEGIN
  UPDATE RouteSheet SET
    route_id = COALESCE(_route_id, RouteSheet.route_id),
    bus_id   = _bus_id,
    status   = COALESCE(_status, RouteSheet.status)
  WHERE id = _id;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION update_bus(
  _id INTEGER,
  _gov_number VARCHAR(9),
  _driver_id INTEGER,
  _model VARCHAR(255),
  _route_number VARCHAR(5),
  _capacity INTEGER
)
RETURNS void
AS $$
BEGIN
  UPDATE Bus SET
    gov_number    = COALESCE(_gov_number, Bus.gov_number),
    driver_id     = COALESCE(_driver_id, Bus.driver_id),
    model         = COALESCE(_model, Bus.model),
    route_number  = COALESCE(_route_number, Bus.route_number),
    capacity      = COALESCE(_capacity, Bus.capacity)
  WHERE id = _id;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION update_driver(
  _id INTEGER,
  _name VARCHAR(255),
  _passport CHAR(11),
  _phone CHAR(11),
  _route_sheet_id INTEGER
)
RETURNS void
AS $$
BEGIN
  UPDATE Driver SET
    name            = COALESCE(_name, Driver.name),
    passport        = COALESCE(_passport, Driver.passport),
    phone           = COALESCE(_phone, Driver.phone),
    route_sheet_id  = _route_sheet_id
  WHERE id = _id;
END;
$$ LANGUAGE plpgsql;


/* DELETE FUNCTIONS */

CREATE OR REPLACE FUNCTION delete_route(_id INTEGER)
RETURNS void
AS $$
BEGIN
  DELETE FROM Route WHERE id = _id;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION delete_routesheet(_id INTEGER)
RETURNS void
AS $$
BEGIN
  DELETE FROM RouteSheet WHERE id = _id;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION delete_bus(_id INTEGER)
RETURNS void
AS $$
BEGIN
  DELETE FROM Bus WHERE id = _id;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION delete_driver(_id INTEGER)
RETURNS void
AS $$
BEGIN
  DELETE FROM Driver WHERE id = _id;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION route_by_station_start(_station_start VARCHAR(255))
RETURNS TABLE(
  id INTEGER,
  station_start VARCHAR(255),
  station_end VARCHAR(255),
  time_start TIMESTAMP,
  time_end TIMESTAMP
) AS $$
BEGIN
  RETURN QUERY SELECT * FROM route WHERE station_start LIKE _station_start;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION count_driver_on_start_station(_station_start VARCHAR(255))
RETURNS INTEGER
AS $$
DECLARE
  driver_count INTEGER;
BEGIN
  SELECT count(*) INTO driver_count FROM driver d, route r
  WHERE r.id = (
    SELECT route_id FROM routesheet rs
    WHERE rs.id = d.route_sheet_id
  ) AND r.station_start LIKE _station_start;

  RETURN driver_count;
END;
$$ LANGUAGE plpgsql;