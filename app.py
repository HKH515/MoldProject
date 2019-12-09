#!flask/bin/python
from flask import Flask, jsonify, render_template

from enum import IntEnum


# this file is both the API and the webserver



# assigns values to the warning types
class WarningType(IntEnum):
    CONNECTION_LOST = 0
    HUMIDITY = 1

app = Flask(__name__)


# API

@app.route("/api/devices")
def api_devices():
    arr = []
    for i in range(5):
        arr.append({
            "name": "Example name %s" % i,
            "location": "Example room %s" % i,
            "id": str(i)
        })
    return jsonify(arr)

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
            "device_id": i,
            "warning_type": WarningType.CONNECTION_LOST
        })
    return jsonify(arr)

# WEBSERVER

@app.route("/")
def index():
    return render_template("index.html") 

@app.route("/devices")
def devices():
    return render_template("devices.html")


if __name__ == "__main__":
    app.run(debug=True)
