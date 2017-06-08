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


