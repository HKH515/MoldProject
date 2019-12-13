

# class Location(db.Model):
#     id = db.Column(db.Integer, primary_key=True)
#     name = db.Column(db.String(200), unique=True)

# class Device(db.Model):
#     id = db.Column(db.Integer, primary_key=True)
#     name = db.Column(db.String(200), unique=False, nullable=False)
#     location_id = db.relationship(db.Integer, db.ForeignKey("location.id"))

# class Measurement(db.Model):
#     id = db.Column(db.Integer, primary_key=True)
#     timestamp = db.Column(db.TIMESTAMP, primary_key=False)
#     device_id = db.relationship(db.Integer, db.ForeignKey("device.id"))
#     value = db.Column(db.Float, unique=False, nullable=False)
#     battery = db.Column(db.Float, unique=False, nullable=False)


