CREATE TABLE Route (
	id SERIAL PRIMARY KEY,
	station_start VARCHAR(255) NOT NULL,
	station_end VARCHAR(255) NOT NULL,
	time_start TIME NOT NULL,
	time_end TIME NOT NULL
);

CREATE TABLE RouteSheet (
	id SERIAL PRIMARY KEY,
	route_id INTEGER NOT NULL,
	bus_id INTEGER NOT NULL,
	status VARCHAR(255) NOT NULL
);

CREATE TABLE Bus (
	id SERIAL PRIMARY KEY,
	gov_number VARCHAR(9) NOT NULL,
	driver_id INTEGER NOT NULL,
	model VARCHAR(255) NOT NULL,
	route_number VARCHAR(5) NOT NULL,
	capacity INTEGER NOT NULL
);

CREATE TABLE Driver (
	id SERIAL PRIMARY KEY,
	name VARCHAR(255) NOT NULL,
	passport CHAR(11) NOT NULL,
	phone CHAR(11) NOT NULL,
	route_sheet_id INTEGER
);

ALTER TABLE RouteSheet
	ADD FOREIGN KEY (route_id) REFERENCES Route(id),
	ADD FOREIGN KEY (bus_id) REFERENCES Bus(id);
	
ALTER TABLE Bus
	ADD FOREIGN KEY (driver_id) REFERENCES Driver(id);
	
ALTER TABLE Driver
	ADD FOREIGN KEY (route_sheet_id) REFERENCES RouteSheet(id);
