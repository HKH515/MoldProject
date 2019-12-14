create database walldo owner postgres;

CREATE TABLE room(
    id bigserial PRIMARY KEY,
    name character varying(255) NOT NULL UNIQUE
);

CREATE TABLE device(
    id bigserial PRIMARY KEY,
    name character varying(255) NOT NULL UNIQUE,
    room_id integer REFERENCES room(id)
);

CREATE TABLE measurement(
    id bigserial PRIMARY KEY,
    value double precision NOT NULL,
    device_id integer REFERENCES device(id),
    ts TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);



-- VIEW THAT SHOWS THE LATEST ENTRY FROM EACH DEVICE, USED IN /devices
-- note that this view only shows devices that have at least one entry in measurements
CREATE VIEW deviceOverview AS (
    WITH C AS (
        SELECT device_id, MAX(ts) as ts
        FROM measurement
        GROUP BY device_id
    )
    SELECT device.name as device_name, room.name as room_name, measurement.value, (C.ts > current_timestamp - interval '2 minutes') as connected FROM device
    JOIN C ON C.device_id = device.id
    JOIN room ON device.room_id = room.id
    JOIN measurement ON measurement.device_id = device.id
    WHERE measurement.ts = C.ts
);

-- WARNINGS VIEW, SHOWS EACH DEVICE WHOSE HUMIDITY WAS LAST MEASURED AT Y OR MEASURED HUMIDITY HIGHER THAN 70
CREATE VIEW warningsOverview AS (
    SELECT * FROM deviceOverview
    WHERE value >= 70
    OR connected = False
);

-- SAME AS ABOVE, BUT THIS IS ONLY FOR HUMIDITY, USED FOR THE WARNING ON THE MAIN PAGE
CREATE VIEW warningsOverviewOnlyHumidity AS (
    SELECT * FROM deviceOverview
    WHERE value >= 70
);
