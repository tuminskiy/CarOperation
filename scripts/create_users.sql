CREATE ROLE worker_role WITH CREATEROLE;
GRANT ALL PRIVILEGES ON DATABASE CarOperation TO worker_role;

CREATE ROLE watcher_role;
GRANT USAGE ON SCHEMA public TO watcher_role;
GRANT SELECT ON ALL TABLES IN SCHEMA public TO watcher_role;
GRANT SELECT ON bus_driver_view TO watcher_role;

CREATE USER user_worker WITH PASSWORD 'zx3021';
GRANT worker_role TO user_worker;

CREATE USER user_watcher WITH PASSWORD '123';
GRANT watcher_role TO user_watcher;