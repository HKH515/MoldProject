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

#curr = conn.cursor()
#with open("init.sql") as init_script:
#    curr.execute(init_script.read())
#curr.commit()
#curr.close()
    #curr.execute(init_script.read())




# API

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
            "connected": row[3]
        })
    return arr

@app.route("/api/devices")
def json_api_devices():
    return jsonify(api_devices)

@app.route("/api/devices/<int:device_id>")
def api_devices_by_id(device_id):
    curr = conn.cursor()
    curr.execute("SELECT device.name, room.name FROM device JOIN room ON device.room_id = room.id WHERE device.id = %s;" % device_id)
    results = curr.fetchone();
    obj = {
        "name": results[0],
        "location": results[1],
    }
    return obj


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
            "connected": row[3]
        })
    return arr

@app.route("/api/warnings")
def json_api_warnings():
    return jsonify(api_warnings())

@app.route("/api/index")
def api_index():
    index_info={
            "humidityProblem": False,
            "numberOfDevices": 2,
            "numberOfWarnings": 0
        }
    return index_info

@app.route("/api/chartdata")
def api_chartdata():
    retarr = []
    device1 = []
    device2 = []
    device1.append({"x": "10-12-2019 14:15","y":"23"})
    device1.append({"x": "10-12-2019 15:15","y":"53"})
    device1.append({"x": "10-12-2019 16:15","y":"13"})
    device1.append({"x": "10-12-2019 17:15","y":"63"})
    retarr.append(device1)
    device2.append({"x": "10-12-2019 14:15","y":"99"})
    device2.append({"x": "10-12-2019 15:15","y":"72"})
    device2.append({"x": "10-12-2019 16:15","y":"18"})
    device2.append({"x": "10-12-2019 17:15","y":"12"})
    retarr.append(device2)
    return  jsonify(retarr)

# WEBSERVER

@app.route("/")
def index():
    info = api_index()
    return render_template("index.html", index_info=info) 

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
