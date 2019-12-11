#!/usr/bin/env python3

import os
print(os.getcwd())
import sys
sys.path.append("code")
import models

from flask import Flask, jsonify, render_template

from enum import IntEnum



from models import Device, Location, Measurement, db

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
POSTGRES_URL = "127.0.0.1:5432" #get_env_variable("POSTGRES_URL")
POSTGRES_USER = "postgres" #get_env_variable("POSTGRES_USER")
POSTGRES_PW = "BtQ&8DKY9#76" #get_env_variable("POSTGRES_PW")
POSTGRES_DB = "walldo" #get_env_variable("POSTGRES_DB")

DB_URL = 'postgresql+psycopg2://{user}:{pw}@{url}/{db}'.format(user=POSTGRES_USER,pw=POSTGRES_PW,url=POSTGRES_URL,db=POSTGRES_DB)

app.config['SQLALCHEMY_DATABASE_URI'] = DB_URL
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False # silence the deprecation warning

db.init_app(app)


# API

@app.route("/api/devices")
def api_devices():
    arr = []
    for i in range(5):
        arr.append({
            "name": "Example name %s" % i,
            "location": "Example room %s" % i,
            "id": str(i),
            "humidity": "35",
            "battery":"80",
            "connected":True
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
    arr = []
    for i in range(10):
        arr.append({
            "name": "Example name %s" % i,
            "location": "Example room %s" % i,
            "id": str(i),
            "humidity": "35",
            "battery":"80",
            "connected":True
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
