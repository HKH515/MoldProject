create database walldo owner postgres;

CREATE TABLE room(
    id bigserial PRIMARY KEY,
    name character varying(255) NOT NULL
);

CREATE TABLE device(
    id bigserial PRIMARY KEY,
    name character varying(255) NOT NULL,
    room_id integer REFERENCES room(id)
);

CREATE TABLE measurement(
    id bigserial PRIMARY KEY,
    value double precision NOT NULL,
    battery double precision NOT NULL,
    device_id integer REFERENCES device(id),
    ts TIMESTAMP
);



-- VIEW THAT SHOWS THE LATEST ENTRY FROM EACH DEVICE, USED IN /devices
CREATE VIEW deviceOverview AS (
    WITH C AS (
        SELECT device_id, MAX(ts) as ts
        FROM measurement
        GROUP BY device_id
    )
    SELECT device.name as device_name, room.name as room_name, measurement.value, measurement.battery, (C.ts > current_timestamp - interval '2 minutes') as connected FROM device
    JOIN C ON C.device_id = device.id
    JOIN room ON device.room_id = room.id
    JOIN measurement ON measurement.device_id = device.id
    WHERE measurement.ts = C.ts
);

-- WARNINGS VIEW, SHOWS EACH DEVICE THAT HAS BATTERY LESS THAN X PERCENT OR HUMIDITY WAS LAST MEASURED AT Y
CREATE VIEW warningsOverview AS (
    SELECT * FROM deviceOverview
    WHERE value >= 70
    OR BATTERY <= 20
    OR connected = False
);