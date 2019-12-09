#!flask/bin/python
from flask import Flask
from flask import jsonify

from enum import IntEnum


# assigns values to the warning types
class WarningType(IntEnum):
    CONNECTION_LOST = 0
    HUMIDITY = 1

app = Flask(__name__)

@app.route("/devices")
def devices():
    arr = []
    for i in range(5):
        arr.append({
            "name": "Example name %s" % i,
            "location": "Example room %s" % i,
            "id": str(i)
        })
    return jsonify(arr)

@app.route("/devices/<int:device_id>")
def devices_by_id(device_id):
    obj = {
        "name": "Example name",
        "location": "Example room",
        "id": 1
    }
    return jsonify(obj)

@app.route("/warnings")
def warnings():
    arr = []
    for i in range(10):
        arr.append({
            "name": "Example name %s" % i,
            "device_id": i,
            "warning_type": WarningType.CONNECTION_LOST
        })
    return jsonify(arr)



if __name__ == "__main__":
    app.run(debug=True)
