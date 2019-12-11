--create database walldo owner postgres;

CREATE TABLE Location(
    id bigserial PRIMARY KEY,
    name character varying(255) NOT NULL
);

CREATE TABLE Device(
    id bigserial PRIMARY KEY,
    name character varying(255) NOT NULL,
    location_id integer REFERENCES Location(id)
);

CREATE TABLE Measurement(
    id bigserial PRIMARY KEY,
    value double precision NOT NULL,
    battery double precision NOT NULL
);