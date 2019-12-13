INSERT INTO room (name) VALUES
('Livingroom'),
('Bathroom'),
('Kitchen'),
('Attic'),
('Bedroom');

INSERT INTO device (name, room_id) VALUES
('West wall', 1),
('Under window', 1),
('East wall', 2),
('West wall', 2),
('By shower', 2);

INSERT INTO measurement (value, device_id, ts) VALUES
(3.3, 1, '2019-12-11 19:01:34'),
(1.3, 1, '2019-12-11 19:01:32'),
(99.3, 1, '2019-12-11 19:01:36'),
(65, 1, '2019-12-11 20:01:36'),
(3.3, 2, '2018-12-11 19:01:34'),
(1.3, 2, '2018-12-11 19:01:32'),
(99.3, 2, '2018-12-11 19:01:36');
