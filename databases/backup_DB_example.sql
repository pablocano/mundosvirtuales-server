DROP TABLE IF EXISTS StockTranslation;

DROP TABLE IF EXISTS PositionModel;

DROP TABLE IF EXISTS Stock;

DROP TABLE IF EXISTS PartsOfModel;

DROP TABLE IF EXISTS PartsOfMachine;

DROP TABLE IF EXISTS MachineTranslation;

DROP TABLE IF EXISTS Machines;

DROP TABLE IF EXISTS Language;

DROP TABLE IF EXISTS Models;

CREATE TABLE Models (
  models_id SERIAL  NOT NULL ,
  path_model VARCHAR(256)   NOT NULL ,
  material VARCHAR(20)    ,
  color VARCHAR(12)    ,
  animated BOOL  DEFAULT False NOT NULL   ,
PRIMARY KEY(models_id));




CREATE TABLE Language (
  language_id SERIAL  NOT NULL ,
  iso_country_code VARCHAR(25)   NOT NULL ,
  iso_language_code VARCHAR(25)   NOT NULL ,
  codepage VARCHAR(25)      ,
PRIMARY KEY(language_id));




CREATE TABLE Machines (
  machines_id SERIAL  NOT NULL ,
  part_number VARCHAR(20)  DEFAULT 'NA' NOT NULL ,
  Models_models_id INTEGER   NOT NULL ,
  canShowInfo BOOL  DEFAULT False NOT NULL ,
  canBeSelected BOOL  DEFAULT False NOT NULL   ,
PRIMARY KEY(machines_id)  ,
  FOREIGN KEY(Models_models_id)
    REFERENCES Models(models_id)
      ON UPDATE CASCADE);


CREATE INDEX Machines_FKIndex1 ON Machines (Models_models_id);


CREATE INDEX IFK_Rel_Models_Machine ON Machines (Models_models_id);


CREATE TABLE MachineTranslation (
  machine_translation_id SERIAL  NOT NULL ,
  Language_language_id INTEGER   NOT NULL ,
  Machines_machines_id INTEGER   NOT NULL ,
  name VARCHAR(25)   NOT NULL ,
  info VARCHAR(256)   NOT NULL ,
  shortInfo VARCHAR(256)   NOT NULL   ,
PRIMARY KEY(machine_translation_id, Language_language_id)    ,
  FOREIGN KEY(Language_language_id)
    REFERENCES Language(language_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE,
  FOREIGN KEY(Machines_machines_id)
    REFERENCES Machines(machines_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE);


CREATE INDEX MachineTranslation_FKIndex1 ON MachineTranslation (Language_language_id);
CREATE INDEX MachineTranslation_FKIndex2 ON MachineTranslation (Machines_machines_id);


CREATE INDEX IFK_Rel_language_translation ON MachineTranslation (Language_language_id);
CREATE INDEX IFK_Rel_machine_translation ON MachineTranslation (Machines_machines_id);


CREATE TABLE PartsOfMachine (
  parts_of_machine_id SERIAL  NOT NULL ,
  Machines_machines_id INTEGER   NOT NULL ,
  Machines_related_machines_id INTEGER   NOT NULL   ,
PRIMARY KEY(parts_of_machine_id, Machines_machines_id, Machines_related_machines_id)    ,
  FOREIGN KEY(Machines_machines_id)
    REFERENCES Machines(machines_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE,
  FOREIGN KEY(Machines_related_machines_id)
    REFERENCES Machines(machines_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE);


CREATE INDEX PartsOfMachine_FKIndex1 ON PartsOfMachine (Machines_machines_id);
CREATE INDEX PartsOfMachine_FKIndex2 ON PartsOfMachine (Machines_related_machines_id);


CREATE INDEX IFK_Rel_Machine ON PartsOfMachine (Machines_machines_id);
CREATE INDEX IFK_Rel_Mechine_Related ON PartsOfMachine (Machines_related_machines_id);


CREATE TABLE PartsOfModel (
  parts_of_model_id SERIAL  NOT NULL ,
  Models_related_models_id INTEGER   NOT NULL ,
  Models_models_id INTEGER   NOT NULL   ,
PRIMARY KEY(parts_of_model_id)    ,
  FOREIGN KEY(Models_models_id)
    REFERENCES Models(models_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE,
  FOREIGN KEY(Models_related_models_id)
    REFERENCES Models(models_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE);


CREATE INDEX PartsOfModel_FKIndex1 ON PartsOfModel (Models_models_id);
CREATE INDEX PartsOfModel_FKIndex2 ON PartsOfModel (Models_related_models_id);


CREATE INDEX IFK_Rel_Models ON PartsOfModel (Models_models_id);
CREATE INDEX IFK_Rel_Related_Models ON PartsOfModel (Models_related_models_id);


CREATE TABLE Stock (
  stock_id SERIAL  NOT NULL ,
  serial_number VARCHAR(20)   NOT NULL ,
  Machines_machines_id INTEGER   NOT NULL   ,
PRIMARY KEY(stock_id)  ,
  FOREIGN KEY(Machines_machines_id)
    REFERENCES Machines(machines_id));


CREATE INDEX Stock_FKIndex1 ON Stock (Machines_machines_id);


CREATE INDEX IFK_Rel_machine_parts_stock ON Stock (Machines_machines_id);


CREATE TABLE PositionModel (
  position_model_id SERIAL  NOT NULL ,
  Stock_stock_id INTEGER   NOT NULL ,
  pos_x FLOAT  DEFAULT 0.0  ,
  pos_y FLOAT  DEFAULT 0.0  ,
  pos_z FLOAT  DEFAULT 0.0  ,
  rot_pitch FLOAT  DEFAULT 0.0  ,
  rot_yaw FLOAT  DEFAULT 0.0    ,
PRIMARY KEY(position_model_id)  ,
  FOREIGN KEY(Stock_stock_id)
    REFERENCES Stock(stock_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE);


CREATE INDEX PositionModel_FKIndex1 ON PositionModel (Stock_stock_id);


CREATE INDEX IFK_Rel_Stock_Position_Model ON PositionModel (Stock_stock_id);


CREATE TABLE StockTranslation (
  stock_translation_id SERIAL  NOT NULL ,
  Stock_stock_id INTEGER   NOT NULL ,
  Language_language_id INTEGER   NOT NULL ,
  info VARCHAR(256)   NOT NULL   ,
PRIMARY KEY(stock_translation_id)    ,
  FOREIGN KEY(Language_language_id)
    REFERENCES Language(language_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE,
  FOREIGN KEY(Stock_stock_id)
    REFERENCES Stock(stock_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE);


CREATE INDEX StockTranslation_FKIndex1 ON StockTranslation (Language_language_id);
CREATE INDEX StockTranslation_FKIndex2 ON StockTranslation (Stock_stock_id);


CREATE INDEX IFK_Rel_stock_language ON StockTranslation (Language_language_id);
CREATE INDEX IFK_Rel_Stock_Translation ON StockTranslation (Stock_stock_id);



CREATE TABLE Language (
  language_id SERIAL  NOT NULL ,
  iso_country_code VARCHAR(25)   NOT NULL ,
  iso_language_code VARCHAR(25)   NOT NULL ,
  codepage VARCHAR(25)

INSERT INTO Language (language_id, iso_country_code, iso_language_code, codepage)
VALUES (1, 'es_CL', 'CL', 'iso-8859-1');


INSERT INTO models (path_model, color, material)
VALUES ('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Accesorios_B4_TRed.Planta-05102017_Bombas_y_tuberias__Accesorios_B4_TRed', 'DarkGray', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Accesorios_B5_codo.Planta-05102017_Bombas_y_tuberias__Accesorios_B5_codo', 'DarkGray', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Accesorios_B6.Planta-05102017_Bombas_y_tuberias__Accesorios_B6', 'Gray', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Accesorios_C1.Planta-05102017_Bombas_y_tuberias__Accesorios_C1', 'Gray', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Accesorios_C2.Planta-05102017_Bombas_y_tuberias__Accesorios_C2', 'Gray', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Accesorios_D1_golilla.Planta-05102017_Bombas_y_tuberias__Accesorios_D1_golilla', 'Gray', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Accesorios_D1_perno.Planta-05102017_Bombas_y_tuberias__Accesorios_D1_perno', 'DarkGray', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Accesorios_D1_tuerca.Planta-05102017_Bombas_y_tuberias__Accesorios_D1_tuerca', 'DarkGray', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Accesorios_E1.Planta-05102017_Bombas_y_tuberias__Accesorios_E1', 'DarkGray', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Accesorios_Union.Planta-05102017_Bombas_y_tuberias__Accesorios_Union', 'DarkGray', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Bomba_10.Planta-05102017_Bombas_y_tuberias__Bomba_10', 'Gray', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Bomba_11.Planta-05102017_Bombas_y_tuberias__Bomba_11', 'DarkGray', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Bomba_12.Planta-05102017_Bombas_y_tuberias__Bomba_12', 'Yellow', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Bomba_13.Planta-05102017_Bombas_y_tuberias__Bomba_13', 'Gray', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Bomba_14.Planta-05102017_Bombas_y_tuberias__Bomba_14', 'Gray', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Bomba_15.Planta-05102017_Bombas_y_tuberias__Bomba_15', 'Gray', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Bomba_01.Planta-05102017_Bombas_y_tuberias__Bomba_01', 'DarkGray', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Bomba_02.Planta-05102017_Bombas_y_tuberias__Bomba_02', 'Yellow', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Bomba_03.Planta-05102017_Bombas_y_tuberias__Bomba_03', 'Gray', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Bomba_04.Planta-05102017_Bombas_y_tuberias__Bomba_04', 'DarkGray', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Bomba_05_1.Planta-05102017_Bombas_y_tuberias__Bomba_05_1', 'Gray', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Bomba_05_2.Planta-05102017_Bombas_y_tuberias__Bomba_05_2', 'Gray', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Bomba_06.Planta-05102017_Bombas_y_tuberias__Bomba_06', 'Gray', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Bomba_07.Planta-05102017_Bombas_y_tuberias__Bomba_07', 'DarkGray', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Bomba_08.Planta-05102017_Bombas_y_tuberias__Bomba_08', 'Gray', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Bomba_09.Planta-05102017_Bombas_y_tuberias__Bomba_09', 'Gray', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Bomba_16_esparrago.Planta-05102017_Bombas_y_tuberias__Bomba_16_esparrago', 'Gray', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Bomba_16_golilla.Planta-05102017_Bombas_y_tuberias__Bomba_16_golilla', 'Gray', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Bomba_16_tuerca.Planta-05102017_Bombas_y_tuberias__Bomba_16_tuerca', 'DarkGray', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Bomba_17_esparrago.Planta-05102017_Bombas_y_tuberias__Bomba_17_esparrago', 'Gray', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Bomba_17_golilla.Planta-05102017_Bombas_y_tuberias__Bomba_17_golilla', 'Gray', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Bomba_17_tuerca.Planta-05102017_Bombas_y_tuberias__Bomba_17_tuerca', 'DarkGray', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Caja_recolectora.Planta-05102017_Bombas_y_tuberias__Caja_recolectora', 'Green', 'Plastic'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Estanque_600_CR_002.Planta-05102017_Bombas_y_tuberias__Estanque_600_CR_002', 'Green', 'Plastic'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Estanque_600_TK_001.Planta-05102017_Bombas_y_tuberias__Estanque_600_TK_001', 'Green', 'Plastic'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Filtro.Planta-05102017_Bombas_y_tuberias__Filtro', 'Cyan', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Filtro_tapa.Planta-05102017_Bombas_y_tuberias__Filtro_tapa', 'Cyan', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Golilla.Planta-05102017_Bombas_y_tuberias__Golilla', 'Gray', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Motor.Planta-05102017_Bombas_y_tuberias__Motor', 'Blue', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Perno.Planta-05102017_Bombas_y_tuberias__Perno', 'Gray', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Reduccion_bomba.Planta-05102017_Bombas_y_tuberias__Reduccion_bomba', 'Gray', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Soplador.Planta-05102017_Bombas_y_tuberias__Soplador', 'Blue', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Soplador_bomba.Planta-05102017_Bombas_y_tuberias__Soplador_bomba', 'Blue', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Soplador_disco.Planta-05102017_Bombas_y_tuberias__Soplador_disco', 'Gray', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Soplador_estanque.Planta-05102017_Bombas_y_tuberias__Soplador_estanque', 'Blue', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Soplador_golilla.Planta-05102017_Bombas_y_tuberias__Soplador_golilla', 'Gray', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Soplador_motor.Planta-05102017_Bombas_y_tuberias__Soplador_motor', 'Blue', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Soplador_perno.Planta-05102017_Bombas_y_tuberias__Soplador_perno', 'Gray', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Soplador_proteccion.Planta-05102017_Bombas_y_tuberias__Soplador_proteccion', 'Orange', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Soplador_tuerca.Planta-05102017_Bombas_y_tuberias__Soplador_tuerca', 'Gray', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Soporte.Planta-05102017_Bombas_y_tuberias__Soporte', 'Gray', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Terreno.Planta-05102017_Bombas_y_tuberias__Terreno', '', 'M_Concrete_Panel'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Tuberia_A1.Planta-05102017_Bombas_y_tuberias__Tuberia_A1', 'DarkGray', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Tuberia_A2.Planta-05102017_Bombas_y_tuberias__Tuberia_A2', 'DarkGray', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Tuberia_A3.Planta-05102017_Bombas_y_tuberias__Tuberia_A3', 'DarkGray', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Tuberia_A6.Planta-05102017_Bombas_y_tuberias__Tuberia_A6', 'DarkGray', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Tuberia_A7.Planta-05102017_Bombas_y_tuberias__Tuberia_A7', 'DarkGray', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Tuerca.Planta-05102017_Bombas_y_tuberias__Tuerca', 'DarkGray', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Valvulas.Planta-05102017_Bombas_y_tuberias__Valvulas', 'Gold', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Valvulas_bola.Planta-05102017_Bombas_y_tuberias__Valvulas_bola', 'Gray', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Valvulas_golillas.Planta-05102017_Bombas_y_tuberias__Valvulas_golillas', 'Gray', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Valvulas_manilla.Planta-05102017_Bombas_y_tuberias__Valvulas_manilla', 'Red', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Valvulas_pernos.Planta-05102017_Bombas_y_tuberias__Valvulas_pernos', 'DarkGray', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Valvulas_placa.Planta-05102017_Bombas_y_tuberias__Valvulas_placa', 'Gray', 'Metal'),
('/Game/Proter/Planta-05102017_Bombas_y_tuberias__Valvulas_tuercas.Planta-05102017_Bombas_y_tuberias__Valvulas_tuercas', 'DarkGray', 'Metal'),
('/Game/Proter/Planta-05102017_Caseta__Barandas_Caseta.Planta-05102017_Caseta__Barandas_Caseta', 'Yellow', 'Metal'),
('/Game/Proter/Planta-05102017_Caseta__Bases.Planta-05102017_Caseta__Bases', '', 'M_Concrete_Tiles'),
('/Game/Proter/Planta-05102017_Caseta__Container.Planta-05102017_Caseta__Container', 'Orange', 'Metal'),
('/Game/Proter/Planta-05102017_Caseta__Cristal.Planta-05102017_Caseta__Cristal', '', 'M_Glass'),
('/Game/Proter/Planta-05102017_Caseta__Estructura_escalera.Planta-05102017_Caseta__Estructura_escalera', 'Blue', 'Metal'),
('/Game/Proter/Planta-05102017_Caseta__Manillas.Planta-05102017_Caseta__Manillas', 'Gray', 'Metal'),
('/Game/Proter/Planta-05102017_Caseta__Marcos.Planta-05102017_Caseta__Marcos', 'Gray', 'Metal'),
('/Game/Proter/Planta-05102017_Caseta__Piso_escalera.Planta-05102017_Caseta__Piso_escalera', 'Yellow', 'Metal'),
('/Game/Proter/Planta-05102017_Caseta__Puertas.Planta-05102017_Caseta__Puertas', 'Gray', 'Metal'),
('/Game/Proter/Planta-05102017_Caseta__Techo.Planta-05102017_Caseta__Techo', 'Gray', 'Metal'),
('/Game/Proter/Planta-05102017_Linea_Electrica__Bases_linea_electrica.Planta-05102017_Linea_Electrica__Bases_linea_electrica', 'Gray', 'Metal'),
('/Game/Proter/Planta-05102017_Linea_Electrica__Canaletas.Planta-05102017_Linea_Electrica__Canaletas', 'Brown', 'Plastic'),
('/Game/Proter/Planta-05102017_Planta__ARRIOSTRAM.Planta-05102017_Planta__ARRIOSTRAM', 'Blue', 'Metal'),
('/Game/Proter/Planta-05102017_Planta__Barandas_Planta.Planta-05102017_Planta__Barandas_Planta', 'Yellow', 'Metal'),
('/Game/Proter/Planta-05102017_Planta__CIELO.Planta-05102017_Planta__CIELO', 'Gray', 'Metal'),
('/Game/Proter/Planta-05102017_Planta__Estructura_Planta.Planta-05102017_Planta__Estructura_Planta', 'Blue', 'Metal'),
('/Game/Proter/Planta-05102017_Planta__PISO.Planta-05102017_Planta__PISO', 'Yellow', 'Metal');


INSERT INTO machines (part_number, Models_models_id, canShowInfo, canBeSelected)
VALUES ('NA', '1', 'False', 'False'),
('NA', '2', 'False', 'False'),
('NA', '3', 'False', 'False'),
('NA', '4', 'False', 'False'),
('NA', '5', 'False', 'False'),
('NA', '6', 'False', 'False'),
('NA', '7', 'False', 'False'),
('NA', '8', 'False', 'False'),
('NA', '9', 'False', 'False'),
('NA', '10', 'False', 'False'),
('PR1-10', '11', 'False', 'False'),
('PR1-11', '12', 'False', 'False'),
('PR1-12', '13', 'False', 'False'),
('PR1-13', '14', 'False', 'False'),
('PR1-14', '15', 'False', 'False'),
('PR1-15', '16', 'False', 'False'),
('PR1-01', '17', 'False', 'False'),
('PR1-02', '18', 'False', 'False'),
('PR1-03', '19', 'False', 'False'),
('PR1-04', '20', 'False', 'False'),
('PR1-05 1', '21', 'False', 'False'),
('PR1-05 2', '22', 'False', 'False'),
('PR1-06', '23', 'False', 'False'),
('PR1-07', '24', 'False', 'False'),
('PR1-08', '25', 'False', 'False'),
('PR1-09', '26', 'False', 'False'),
('PR1-16', '27', 'False', 'False'),
('PR1-17', '28', 'False', 'False'),
('PR1-17', '29', 'False', 'False'),
('PR1-17', '30', 'False', 'False'),
('PR1-17', '31', 'False', 'False'),
('PR1-17', '32', 'False', 'False'),
('NA', '33', 'False', 'False'),
('NA', '34', 'False', 'False'),
('NA', '35', 'False', 'False'),
('NA', '36', 'False', 'False'),
('NA', '37', 'False', 'False'),
('NA', '38', 'False', 'False'),
('PR0101-1', '39', 'False', 'False'),
('NA', '40', 'False', 'False'),
('NA', '41', 'False', 'False'),
('NA', '42', 'False', 'False'),
('NA', '43', 'False', 'False'),
('NA', '44', 'False', 'False'),
('NA', '45', 'False', 'False'),
('NA', '46', 'False', 'False'),
('NA', '47', 'False', 'False'),
('NA', '48', 'False', 'False'),
('NA', '49', 'False', 'False'),
('NA', '50', 'False', 'False'),
('PR1-1', '51', 'False', 'False'),
('NA', '52', 'False', 'False'),
('NA', '53', 'False', 'False'),
('NA', '54', 'False', 'False'),
('NA', '55', 'False', 'False'),
('NA', '56', 'False', 'False'),
('NA', '57', 'False', 'False'),
('NA', '58', 'False', 'False'),
('NA', '59', 'False', 'False'),
('NA', '60', 'False', 'False'),
('NA', '61', 'False', 'False'),
('NA', '62', 'False', 'False'),
('NA', '63', 'False', 'False'),
('NA', '64', 'False', 'False'),
('NA', '65', 'False', 'False'),
('NA', '66', 'False', 'False'),
('NA', '67', 'False', 'False'),
('NA', '68', 'False', 'False'),
('NA', '69', 'False', 'False'),
('NA', '70', 'False', 'False'),
('NA', '71', 'False', 'False'),
('NA', '72', 'False', 'False'),
('NA', '73', 'False', 'False'),
('NA', '74', 'False', 'False'),
('NA', '75', 'False', 'False'),
('NA', '76', 'False', 'False'),
('NA', '77', 'False', 'False'),
('NA', '78', 'False', 'False'),
('NA', '79', 'False', 'False'),
('NA', '80', 'False', 'False'),
('NA', '81', 'False', 'False'),
('NA', '82', 'False', 'False');


INSERT INTO machinetranslation (Language_language_id, Machines_machines_id, name, shortInfo, info)
VALUES ('1', '1', 'Accesorios B4 TRed', 'Accesorios B4 TRed', 'Accesorios B4 TRed'),
('1', '2', 'Accesorios B5 codo', 'Accesorios B5 codo', 'Accesorios B5 codo'),
('1', '3', 'Accesorios B6', 'Accesorios B6', 'Accesorios B6'),
('1', '4', 'Accesorios C1', 'Accesorios C1', 'Accesorios C1'),
('1', '5', 'Accesorios C2', 'Accesorios C2', 'Accesorios C2'),
('1', '6', 'Accesorios D1 golilla', 'Accesorios D1 golilla', 'Accesorios D1 golilla'),
('1', '7', 'Accesorios D1 perno', 'Accesorios D1 perno', 'Accesorios D1 perno'),
('1', '8', 'Accesorios D1 tuerca', 'Accesorios D1 tuerca', 'Accesorios D1 tuerca'),
('1', '9', 'Accesorios E1', 'Accesorios E1', 'Accesorios E1'),
('1', '10', 'Accesorios Union', 'Accesorios Union', 'Accesorios Union'),
('1', '11', 'Flange Giratorio 2', 'Flange Giratorio 2', 'Flange Giratorio 2'),
('1', '12', 'Descarga', 'Descarga', 'Descarga'),
('1', '13', 'Caja Impulsor', 'Caja Impulsor', 'Caja Impulsor'),
('1', '14', 'Flange de Amarre', 'Flange de Amarre', 'Flange de Amarre'),
('1', '15', 'Descarga', 'Descarga', 'Descarga'),
('1', '16', 'Pasador Impulsor', 'Pasador Impulsor', 'Pasador Impulsor'),
('1', '17', 'Soporte Motor', 'Soporte Motor', 'Soporte Motor'),
('1', '18', 'Separador', 'Separador', 'Separador'),
('1', '19', 'Machón de Acople', 'Machón de Acople', 'Machón de Acople'),
('1', '20', 'Seguro de Sello', 'Seguro de Sello', 'Seguro de Sello'),
('1', '21', 'Sello Plano M50 1', 'Sello Plano M50 1', 'Sello Plano M50 1'),
('1', '22', 'Sello Plano M50 2', 'Sello Plano M50 2', 'Sello Plano M50 2'),
('1', '23', 'Tapa Impulsión', 'Tapa Impulsión', 'Tapa Impulsión'),
('1', '24', 'Alma Metálica', 'Alma Metálica', 'Alma Metálica'),
('1', '25', 'Pasador Impulsor', 'Pasador Impulsor', 'Pasador Impulsor'),
('1', '26', 'Impulsor', 'Impulsor', 'Impulsor'),
('1', '27', 'Esparrago 3/8', 'Esparrago 3/8', 'Esparrago - Espárrago 3/8'),
('1', '28', 'Golilla', 'Golilla', 'Golilla'),
('1', '29', 'Tuerca', 'Tuerca', 'Tuerca'),
('1', '30', 'Esparrago 1/2', 'Esparrago 1/2', 'Espárrago 1/2'),
('1', '31', 'Golilla', 'Golilla', 'Golilla'),
('1', '32', 'Tuerca', 'Tuerca', 'Tuerca'),
('1', '33', 'Caja Recolectora', 'Caja Recolectora', 'Caja Recolectora'),
('1', '34', 'Estanque 600 CR 002', 'Estanque 600 CR 002', 'Estanque 600 CR 002'),
('1', '35', 'Estanque 600 TK 001', 'Estanque 600 TK 001', 'Estanque 600 TK 001'),
('1', '36', 'Filtro', 'Filtro', 'Filtro'),
('1', '37', 'Filtro tapa', 'Filtro tapa', 'Filtro tapa'),
('1', '38', 'Golilla', 'Golilla', 'Golilla'),
('1', '39', 'Motor', 'Motor', 'MOTOR ELECTRICO\n Tipo: Trifásico\n clase : IE2 - 225M - 4\n Potencia : 4,1 kw(5.5 Hp)\n RPM : 1470\n Consumo : 8.1 Amp\n Eficiencia : 86.6%\n Factor de Potencia : 0.81'),
('1', '40', 'Perno', 'Perno', 'Perno'),
('1', '41', 'Reduccion Bomba', 'Reduccion Bomba', 'Reduccion Bomba'),
('1', '42', 'Soplador', 'Soplador', 'Soplador'),
('1', '43', 'Soplador Bomba', 'Soplador Bomba', 'Soplador Bomba'),
('1', '44', 'Soplador Disco', 'Soplador Disco', 'Soplador Disco'),
('1', '45', 'Soplador Estanque', 'Soplador Estanque', 'Soplador Estanque'),
('1', '46', 'Soplador Golilla', 'Soplador Golilla', 'Soplador Golilla'),
('1', '47', 'Soplador Motor', 'Soplador Motor', 'Soplador Motor'),
('1', '48', 'Soplador perno', 'Soplador perno', 'Soplador perno'),
('1', '49', 'Soplador proteccion', 'Soplador proteccion', 'Soplador proteccion'),
('1', '50', 'Soplador tuerca', 'Soplador tuerca', 'Soplador tuerca'),
('1', '51', 'Soporte', 'Soporte', 'Soporte'),
('1', '52', 'Terreno', 'Terreno', 'Terreno'),
('1', '53', 'Tuberia A1', 'Tuberia A1', 'Tuberia A1'),
('1', '54', 'Tuberia A2', 'Tuberia A2', 'Tuberia A2'),
('1', '55', 'Tuberia A3', 'Tuberia A3', 'Tuberia A3'),
('1', '56', 'Tuberia A6', 'Tuberia A6', 'Tuberia A6'),
('1', '57', 'Tuberia A7', 'Tuberia A7', 'Tuberia A7'),
('1', '58', 'Tuerca', 'Tuerca', 'Tuerca'),
('1', '59', 'Valvulas', 'Valvulas', 'Valvulas'),
('1', '60', 'Valvulas bola', 'Valvulas bola', 'VALVULA BOLA\n clase: 150\n Bola Acero Inoxidable\n Asientos PTFE\n Para Instalación Vertical u Horizontal\n Asientos recambiables\n Extremos flangeados\n Peso(aprox) 17 kg'),
('1', '61', 'Valvulas golillas', 'Valvulas golillas', 'Valvulas golillas'),
('1', '62', 'Valvulas manilla', 'Valvulas manilla', 'Valvulas manilla'),
('1', '63', 'Valvulas pernos', 'Valvulas pernos', 'Valvulas pernos'),
('1', '64', 'Valvulas placa', 'Valvulas placa', 'Valvulas placa'),
('1', '65', 'Valvulas tuercas', 'Valvulas tuercas', 'Valvulas tuercas'),
('1', '66', 'Barandas Caseta', 'Barandas Caseta', 'Barandas Caseta'),
('1', '67', 'Bases', 'Bases', 'Bases'),
('1', '68', 'Container', 'Container', 'Container'),
('1', '69', 'Cristal', 'Cristal', 'Cristal'),
('1', '70', 'Estructura escalera', 'Estructura escalera', 'Estructura escalera'),
('1', '71', 'Manillas', 'Manillas', 'Manillas'),
('1', '72', 'Marcos', 'Marcos', 'Marcos'),
('1', '73', 'Piso escalera', 'Piso escalera', 'Piso escalera'),
('1', '74', 'Puertas', 'Puertas', 'Puertas'),
('1', '75', 'Techo', 'Techo', 'Techo'),
('1', '76', 'Bases linea electrica', 'Bases linea electrica', 'Bases linea electrica'),
('1', '77', 'Canaletas', 'Canaletas', 'Canaletas'),
('1', '78', 'ARRIOSTRAM', 'ARRIOSTRAM', 'ARRIOSTRAM'),
('1', '79', 'Barandas Planta', 'Barandas Planta', 'Barandas Planta'),
('1', '80', 'CIELO', 'CIELO', 'CIELO'),
('1', '81', 'Estructura Planta', 'Estructura Planta', 'Estructura Planta'),
('1', '82', 'PISO', 'PISO', 'PISO');


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