#!/usr/bin/env python3

print("DSFJDSFKDSJFKDSFJKSJDF0")
import os
print(os.getcwd())
import sys
sys.path.append("code")
import models

from flask import Flask, jsonify, render_template

from enum import IntEnum


# this file is both the API and the webserver



# assigns values to the warning types
class WarningType(IntEnum):
    CONNECTION_LOST = 0
    HUMIDITY = 1

app = Flask(__name__)

def get_env_variable(name):
    try:
        return os.environ[name]
    except KeyError:
        message = "Expected environment variable '{}' not set.".format(name)
        raise Exception(message)

# the values of those depend on your setup
POSTGRES_URL = get_env_variable("POSTGRES_URL")
POSTGRES_URL = "database"
POSTGRES_USER = get_env_variable("POSTGRES_USER")
POSTGRES_PASSWORD = get_env_variable("POSTGRES_PASSWORD")
POSTGRES_DB = get_env_variable("POSTGRES_DB")
print(POSTGRES_URL)
print(POSTGRES_USER)
print(POSTGRES_PASSWORD)
print(POSTGRES_DB)

#DB_URL = 'postgresql+psycopg2://{user}:{pw}@{url}/{db}'.format(user=POSTGRES_USER,pw=POSTGRES_PASSWORD,url=POSTGRES_URL,db=POSTGRES_DB)

#app.config['SQLALCHEMY_DATABASE_URI'] = DB_URL
#app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False # silence the deprecation warning

#db.init_app(app)

import psycopg2

import time

conn = psycopg2.connect("dbname='{db}' user='{user}' host='{url}' password='{pw}'".format(user=POSTGRES_USER,pw=POSTGRES_PASSWORD,url=POSTGRES_URL,db=POSTGRES_DB))
print("connected to db")





# API

@app.route("/api/devices")
def api_devices():
    curr = conn.cursor()
    curr.execute("SELECT * FROM deviceOverview;")
    results = curr.fetchall();
    arr = []
    for row in results:
        arr.append({
            "name": row[0],
            "location": row[1],
            "humidity": row[2],
            "battery": row[3],
            "connected": row[4]
        })
    return arr

@app.route("/api/devices/<int:device_id>")
def api_devices_by_id(device_id):
    obj = {
        "name": "Example name",
        "location": "Example room",
        "id": 1
    }
    return jsonify(obj)

@app.route("/api/warnings")
def api_warnings():
    curr = conn.cursor()
    curr.execute("SELECT * FROM warningsOverview;")
    results = curr.fetchall();
    arr = []
    for row in results:
        arr.append({
            "name": row[0],
            "location": row[1],
            "humidity": row[2],
            "battery": row[3],
            "connected": row[4]
        })
    return arr

# WEBSERVER

@app.route("/")
def index():
    return render_template("index.html") 

@app.route("/devices")
def devices():
    device_list = api_devices()
    return render_template("devices.html", device_list=device_list)

@app.route("/warnings")
def warnings():
    warning_list = api_warnings()
    return render_template("warnings.html", warning_list=warning_list)


if __name__ == "__main__":
    app.run(debug=True)
