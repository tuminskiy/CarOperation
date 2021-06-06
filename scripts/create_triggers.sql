CREATE OR REPLACE FUNCTION handle_delete_routsheet() RETURNS trigger AS $$
DECLARE
  driver_id INTEGER;
BEGIN
  SELECT id INTO driver_id FROM driver
  WHERE route_sheet_id = OLD.id;

  IF FOUND THEN
      UPDATE driver SET route_sheet_id = NULL
      WHERE id = driver_id;
  END IF;

  RETURN OLD;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER on_delete_routesheet
BEFORE DELETE ON routesheet FOR EACH ROW
EXECUTE PROCEDURE handle_delete_routsheet();


CREATE OR REPLACE FUNCTION handle_delete_route() RETURNS trigger AS $$
DECLARE
    rs_id INTEGER;
BEGIN
    SELECT id INTO rs_id FROM routesheet
    WHERE routesheet.route_id = OLD.id;

    IF FOUND THEN
        PERFORM delete_routesheet(rs_id);
    END IF;

    RETURN OLD;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER on_delete_route
BEFORE DELETE ON route FOR EACH ROW
EXECUTE PROCEDURE handle_delete_route();



CREATE OR REPLACE FUNCTION handle_delete_bus() RETURNS trigger AS $$
DECLARE
  rs_id INTEGER;
  rs_route_id INTEGER;
  rs_status VARCHAR(255);
BEGIN
  SELECT id INTO rs_id, rs_route_id, rs_status FROM routesheet
  WHERE routesheet.bus_id = OLD.id;

  IF FOUND THEN
    PERFORM update_routesheet(rs_id, rs_route_id, NULL, rs_status);
  END IF;

  RETURN OLD;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER on_delete_bus
BEFORE DELETE ON bus FOR EACH ROW
EXECUTE PROCEDURE handle_delete_bus();


CREATE OR REPLACE FUNCTION handle_delete_driver() RETURNS trigger AS $$
DECLARE
  b_id INTEGER;
BEGIN
  SELECT id INTO b_id FROM bus
  WHERE driver_id = OLD.id;

  IF FOUND THEN
    PERFORM delete_bus(b_id);
  END IF;  

  RETURN OLD;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER on_delete_driver
BEFORE DELETE ON driver FOR EACH ROW
EXECUTE PROCEDURE handle_delete_driver();



CREATE OR REPLACE FUNCTION handle_update_routesheet() RETURNS trigger AS $$
DECLARE
  d_id INTEGER;
  driver_cursor CURSOR
    FOR SELECT * FROM driver WHERE id = d_id
    FOR UPDATE;
  driver_record record;
BEGIN
  IF (OLD.bus_id = NEW.bus_id) OR (NEW.bus_id IS NULL) THEN
    RETURN NEW;
  END IF;

  SELECT driver_id INTO d_id FROM (
    SELECT driver_id FROM bus
    WHERE bus.id = OLD.bus_id
  ) odi;

  IF d_id IS NOT NULL THEN
    OPEN driver_cursor;
    FETCH driver_cursor INTO driver_record;

    UPDATE driver SET route_sheet_id = NULL
    WHERE CURRENT OF driver_cursor;

    CLOSE driver_cursor;
  END IF;

  SELECT driver_id INTO d_id FROM (
    SELECT driver_id FROM bus
    WHERE bus.id = NEW.bus_id
  ) ndi; 
  
  IF d_id IS NOT NULL THEN
    OPEN driver_cursor;
    FETCH driver_cursor INTO driver_record;

    UPDATE driver SET route_sheet_id = NEW.id
    WHERE CURRENT OF driver_cursor;

    CLOSE driver_cursor;
  END IF;

  RETURN NEW;
END;
$$ LANGUAGE plpgsql;


CREATE TRIGGER on_update_routesheet
AFTER UPDATE ON routesheet FOR EACH ROW
EXECUTE PROCEDURE handle_update_routesheet();



CREATE OR REPLACE FUNCTION handle_update_bus() RETURNS trigger AS $$
DECLARE
  rs_id INTEGER;
BEGIN
  IF OLD.driver_id = NEW.driver_id THEN
    RETURN NEW;
  END IF;

  SELECT id INTO rs_id FROM routesheet
  WHERE bus_id = NEW.id;

  IF NOT FOUND THEN
    RETURN NEW;
  END IF;

  UPDATE driver SET route_sheet_id = NULL
  WHERE driver.id = OLD.driver_id;

  UPDATE driver SET route_sheet_id = rs_id
  WHERE driver.id = NEW.driver_id;

  return NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER on_update_bus
AFTER UPDATE ON bus FOR EACH ROW
EXECUTE PROCEDURE handle_update_bus();



CREATE OR REPLACE FUNCTION handle_insert_routesheet() RETURNS trigger AS $$
BEGIN
  IF NEW.bus_id IS NULL THEN
    RETURN NEW;
  END IF;
  
  UPDATE driver SET route_sheet_id = NEW.id
  WHERE driver.id = (
    SELECT driver_id FROM bus
    WHERE bus.id = NEW.bus_id
  );

  RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER on_insert_routesheet
AFTER INSERT ON routesheet FOR EACH ROW
EXECUTE PROCEDURE handle_insert_routesheet();



CREATE OR REPLACE FUNCTION handle_update_bus_driver_view() RETURNS trigger AS $$
DECLARE
  d_id INTEGER;
  rs_id INTEGER;
BEGIN
  UPDATE bus SET
    gov_number = COALESCE(NEW.gov_number, OLD.gov_number),
    model = COALESCE(NEW.model, OLD.model),
    route_number = COALESCE(NEW.route_number, OLD.route_number),
    capacity = COALESCE(NEW.capacity, OLD.capacity)
  WHERE id = NEW.id;

  SELECT driver_id INTO d_id FROM bus
  WHERE id = NEW.id;

  UPDATE driver SET
    name = COALESCE(NEW.name, OLD.name),
    phone = COALESCE(NEW.phone, OLD.phone)
  WHERE id = d_id;

  SELECT route_sheet_id INTO rs_id FROM driver
  WHERE id = d_id;

  IF rs_id IS NOT NULL THEN
    UPDATE routesheet SET
      status = COALESCE(NEW.status, OLD.status)
    WHERE id = rs_id;
  END IF;

  RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER on_update_bus_driver_view
INSTEAD OF UPDATE ON bus_driver_view FOR EACH ROW
EXECUTE PROCEDURE handle_update_bus_driver_view();