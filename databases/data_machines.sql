INSERT INTO models (models_id, path_model, color, material)
VALUES (83, '.', 'Gray', 'Metal'),
(84, '.', 'Gray', 'Metal'),
(85, '.', 'Gray', 'Metal'),
(86, '.', 'Gray', 'Metal');

INSERT INTO machines (machines_id, part_number, Models_models_id, canShowInfo, canBeSelected)
VALUES (83, 'NA', 83, 'True', 'False'),
(84, 'PR0501-3', 84, 'True', 'True'),
(85, 'NA', 85, 'True', 'False'),
(86, 'NA', 86, 'True', 'False');

INSERT INTO machinetranslation (Language_language_id, Machines_machines_id, name, shortInfo, info)
VALUES (1, 83, 'Caseta', 'Caseta', 'Caseta'),
(1, 84, 'Bomba', 'Bomba', 'BOMBA\n Fabricante: Reifox\n Tipo : centrifuga horizontal\n Modelo : 1 - 210 - 5.5 hp 1470 RPM\n Q : 55 mt3 / h\n 	H : 12 mca'),
(1, 85, 'Soplador', 'Soplador', 'Soplador'),
(1, 86, 'Planta', 'Planta', 'Planta');

INSERT INTO partsofmachine (Machines_machines_id, Machines_related_machines_id)
VALUES (83, 66),
(83, 67),
(83, 68),
(83, 69),
(83, 70),
(83, 71),
(83, 72),
(83, 73),
(83, 74),
(83, 75),

(84, 11),
(84, 12),
(84, 13),
(84, 14),
(84, 15),
(84, 16),
(84, 17),
(84, 18),
(84, 19),
(84, 20),
(84, 21),
(84, 22),
(84, 23),
(84, 24),
(84, 25),
(84, 26),
(84, 27),
(84, 28),
(84, 29),
(84, 30),
(84, 31),
(84, 32),
(84, 39),
(84, 41),

(85, 42),
(85, 43),
(85, 44),
(85, 45),
(85, 46),
(85, 47),
(85, 48),
(85, 49),
(85, 50),

(86, 78),
(86, 79),
(86, 80),
(86, 81),
(86, 82);