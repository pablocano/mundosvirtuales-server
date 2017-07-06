DROP TABLE IF EXISTS OutputModel CASCADE;

DROP TABLE IF EXISTS InputModel CASCADE;

DROP TABLE IF EXISTS ModelControl CASCADE;

DROP TABLE IF EXISTS PartsOfModel CASCADE;

DROP TABLE IF EXISTS PartsOfMachine CASCADE;

DROP TABLE IF EXISTS Permission_user CASCADE;

DROP TABLE IF EXISTS Permission_group CASCADE;

DROP TABLE IF EXISTS Group_user CASCADE;

DROP TABLE IF EXISTS ModelsVersion CASCADE;

DROP TABLE IF EXISTS MachineTranslation CASCADE;

DROP TABLE IF EXISTS Sensor_machine CASCADE;

DROP TABLE IF EXISTS role_user CASCADE;

DROP TABLE IF EXISTS StockTranslation CASCADE;

DROP TABLE IF EXISTS Stock CASCADE;

DROP TABLE IF EXISTS Role_group CASCADE;

DROP TABLE IF EXISTS Logger CASCADE;

DROP TABLE IF EXISTS Machines CASCADE;

DROP TABLE IF EXISTS Translate CASCADE;

DROP TABLE IF EXISTS Groups CASCADE;

DROP TABLE IF EXISTS Users CASCADE;

DROP TABLE IF EXISTS Language CASCADE;

DROP TABLE IF EXISTS roles CASCADE;

DROP TABLE IF EXISTS PositionEntity CASCADE;

DROP TABLE IF EXISTS Permissions CASCADE;

DROP TABLE IF EXISTS Models CASCADE;

DROP TABLE IF EXISTS Sensor CASCADE;


CREATE TABLE Sensor (
  sensor_id SERIAL  NOT NULL ,
  name VARCHAR(50)   NOT NULL ,
  path_sensor VARCHAR(256)   NOT NULL   ,
PRIMARY KEY(sensor_id));




CREATE TABLE Models (
  model_id SERIAL  NOT NULL ,
  current_version INTEGER  DEFAULT 1    ,
PRIMARY KEY(model_id));




CREATE TABLE Permissions (
  permission_id SERIAL  NOT NULL ,
  permission VARCHAR(30)   NOT NULL ,
  created_at TIMESTAMP  DEFAULT now() NOT NULL ,
  deleted_at TIMESTAMP    ,
  updated_at TIMESTAMP      ,
PRIMARY KEY(permission_id));




CREATE TABLE PositionEntity (
  position_entity_id SERIAL  NOT NULL ,
  pos_x FLOAT  DEFAULT 0.0  ,
  pos_y FLOAT  DEFAULT 0.0  ,
  pos_z FLOAT  DEFAULT 0.0  ,
  rot_roll FLOAT  DEFAULT 0.0  ,
  rot_pitch FLOAT  DEFAULT 0.0  ,
  rot_yaw FLOAT  DEFAULT 0.0    ,
PRIMARY KEY(position_entity_id));




CREATE TABLE roles (
  role_id SERIAL  NOT NULL ,
  role VARCHAR(25)   NOT NULL ,
  created_at TIMESTAMP    ,
  deleted_at TIMESTAMP    ,
  updated_at TIMESTAMP      ,
PRIMARY KEY(role_id));




CREATE TABLE Language (
  language_id SERIAL  NOT NULL ,
  iso_country_code VARCHAR(25)   NOT NULL ,
  iso_language_code VARCHAR(25)   NOT NULL ,
  codepage VARCHAR(25)      ,
PRIMARY KEY(language_id));




CREATE TABLE Users (
  user_id SERIAL  NOT NULL ,
  username VARCHAR(15)   NOT NULL ,
  password_user VARCHAR(20)   NOT NULL ,
  first_name VARCHAR(25)   NOT NULL ,
  last_name VARCHAR(25)   NOT NULL ,
  active BOOL  DEFAULT true NOT NULL ,
  created_at TIMESTAMP  DEFAULT now() NOT NULL ,
  deleted_at TIMESTAMP    ,
  updated_at TIMESTAMP      ,
PRIMARY KEY(user_id));




CREATE TABLE Groups (
  group_id SERIAL  NOT NULL ,
  name VARCHAR(25)   NOT NULL ,
  created_at TIMESTAMP  DEFAULT now() NOT NULL ,
  deleted_at TIMESTAMP    ,
  updated_at TIMESTAMP      ,
PRIMARY KEY(group_id));




CREATE TABLE Translate (
  translate_id SERIAL  NOT NULL ,
  Language_language_id INTEGER   NOT NULL ,
  name VARCHAR(20)   NOT NULL ,
  value VARCHAR(256)   NOT NULL   ,
PRIMARY KEY(translate_id, Language_language_id)  ,
  FOREIGN KEY(Language_language_id)
    REFERENCES Language(language_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE);


CREATE INDEX Translate_FKIndex1 ON Translate (Language_language_id);


CREATE INDEX IFK_Rel_Translate ON Translate (Language_language_id);


CREATE TABLE Machines (
  machines_id SERIAL  NOT NULL ,
  Models_model_id INTEGER   NOT NULL ,
  part_number VARCHAR(20)  DEFAULT 'NA' NOT NULL ,
  canShowInfo BOOL  DEFAULT False NOT NULL ,
  canBeSelected BOOL  DEFAULT False NOT NULL   ,
PRIMARY KEY(machines_id)  ,
  FOREIGN KEY(Models_model_id)
    REFERENCES Models(model_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE);


CREATE INDEX Machines_FKIndex1 ON Machines (Models_model_id);


CREATE INDEX IFK_Rel_Machine_Model ON Machines (Models_model_id);


CREATE TABLE Logger (
  log_id SERIAL  NOT NULL ,
  Users_user_id INTEGER   NOT NULL ,
  source VARCHAR(30)    ,
  type_log VARCHAR(20)    ,
  message TEXT    ,
  created_at TIMESTAMP  DEFAULT now()    ,
PRIMARY KEY(log_id)  ,
  FOREIGN KEY(Users_user_id)
    REFERENCES Users(user_id)
      ON UPDATE CASCADE);


CREATE INDEX Logger_FKIndex1 ON Logger (Users_user_id);


CREATE INDEX IFK_Rel_Log ON Logger (Users_user_id);


CREATE TABLE Role_group (
  role_group_id SERIAL  NOT NULL ,
  roles_role_id INTEGER   NOT NULL ,
  Groups_group_id INTEGER   NOT NULL ,
  created_at TIMESTAMP      ,
PRIMARY KEY(role_group_id)    ,
  FOREIGN KEY(Groups_group_id)
    REFERENCES Groups(group_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE,
  FOREIGN KEY(roles_role_id)
    REFERENCES roles(role_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE);


CREATE INDEX Role_group_FKIndex1 ON Role_group (Groups_group_id);
CREATE INDEX Role_group_FKIndex2 ON Role_group (roles_role_id);


CREATE INDEX IFK_Rel_Role_Group_Groups ON Role_group (Groups_group_id);
CREATE INDEX IFK_Rel_Role_Group_Roles ON Role_group (roles_role_id);


CREATE TABLE Stock (
  stock_id SERIAL  NOT NULL ,
  PositionEntity_position_entity_id INTEGER   NOT NULL ,
  serial_number VARCHAR(20)   NOT NULL ,
  Machines_machines_id INTEGER   NOT NULL ,
  created_at TIMESTAMP  DEFAULT now() NOT NULL ,
  deleted_at TIMESTAMP    ,
  updated_at TIMESTAMP      ,
PRIMARY KEY(stock_id)    ,
  FOREIGN KEY(Machines_machines_id)
    REFERENCES Machines(machines_id),
  FOREIGN KEY(PositionEntity_position_entity_id)
    REFERENCES PositionEntity(position_entity_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE);


CREATE INDEX Stock_FKIndex1 ON Stock (Machines_machines_id);
CREATE INDEX Stock_FKIndex2 ON Stock (PositionEntity_position_entity_id);


CREATE INDEX IFK_Rel_machine_parts_stock ON Stock (Machines_machines_id);
CREATE INDEX IFK_Rel_Stock_Position ON Stock (PositionEntity_position_entity_id);


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


CREATE TABLE role_user (
  role_user_id SERIAL  NOT NULL ,
  roles_role_id INTEGER   NOT NULL ,
  Users_user_id INTEGER   NOT NULL ,
  created_at TIMESTAMP  DEFAULT now() NOT NULL   ,
PRIMARY KEY(role_user_id)    ,
  FOREIGN KEY(Users_user_id)
    REFERENCES Users(user_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE,
  FOREIGN KEY(roles_role_id)
    REFERENCES roles(role_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE);


CREATE INDEX role_user_FKIndex1 ON role_user (Users_user_id);
CREATE INDEX role_user_FKIndex2 ON role_user (roles_role_id);


CREATE INDEX IFK_Rel_Role_User_Users ON role_user (Users_user_id);
CREATE INDEX IFK_Rel_Role_User_Roles ON role_user (roles_role_id);


CREATE TABLE Sensor_machine (
  sensor_machine_id SERIAL  NOT NULL ,
  Sensor_sensor_id INTEGER   NOT NULL ,
  Machines_machines_id INTEGER   NOT NULL   ,
PRIMARY KEY(sensor_machine_id)    ,
  FOREIGN KEY(Machines_machines_id)
    REFERENCES Machines(machines_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE,
  FOREIGN KEY(Sensor_sensor_id)
    REFERENCES Sensor(sensor_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE);


CREATE INDEX Sensor_machine_FKIndex1 ON Sensor_machine (Machines_machines_id);
CREATE INDEX Sensor_machine_FKIndex2 ON Sensor_machine (Sensor_sensor_id);


CREATE INDEX IFK_Rel_Machine_Sensor ON Sensor_machine (Machines_machines_id);
CREATE INDEX IFK_Rel_Sensor ON Sensor_machine (Sensor_sensor_id);


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


CREATE TABLE ModelsVersion (
  models_version_id SERIAL  NOT NULL ,
  Models_model_id INTEGER   NOT NULL ,
  PositionEntity_position_entity_id INTEGER   NOT NULL ,
  path_model VARCHAR(256)   NOT NULL ,
  material VARCHAR(20)    ,
  color VARCHAR(12)    ,
  animated BOOL  DEFAULT False NOT NULL ,
  version INTEGER  DEFAULT 1 NOT NULL ,
  created_at TIMESTAMP  DEFAULT now() NOT NULL ,
  deleted_at TIMESTAMP    ,
  updated_at TIMESTAMP      ,
PRIMARY KEY(models_version_id)    ,
  FOREIGN KEY(PositionEntity_position_entity_id)
    REFERENCES PositionEntity(position_entity_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE,
  FOREIGN KEY(Models_model_id)
    REFERENCES Models(model_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE);


CREATE INDEX ModelsVersion_FKIndex1 ON ModelsVersion (PositionEntity_position_entity_id);
CREATE INDEX ModelsVersion_FKIndex2 ON ModelsVersion (Models_model_id);


CREATE INDEX IFK_Rel_Model_Position ON ModelsVersion (PositionEntity_position_entity_id);
CREATE INDEX IFK_Rel_Models_Version ON ModelsVersion (Models_model_id);


CREATE TABLE Group_user (
  group_user_id SERIAL  NOT NULL ,
  Users_user_id INTEGER   NOT NULL ,
  Groups_group_id INTEGER   NOT NULL ,
  created_at TIMESTAMP  DEFAULT now() NOT NULL   ,
PRIMARY KEY(group_user_id)    ,
  FOREIGN KEY(Users_user_id)
    REFERENCES Users(user_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE,
  FOREIGN KEY(Groups_group_id)
    REFERENCES Groups(group_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE);


CREATE INDEX Group_user_FKIndex1 ON Group_user (Users_user_id);
CREATE INDEX Group_user_FKIndex2 ON Group_user (Groups_group_id);


CREATE INDEX IFK_Rel_Group_User_Users ON Group_user (Users_user_id);
CREATE INDEX IFK_Rel_Group_User_Groups ON Group_user (Groups_group_id);


CREATE TABLE Permission_group (
  permission_group_id SERIAL  NOT NULL ,
  Permissions_permission_id INTEGER   NOT NULL ,
  Groups_group_id INTEGER   NOT NULL ,
  created_at TIMESTAMP      ,
PRIMARY KEY(permission_group_id)    ,
  FOREIGN KEY(Groups_group_id)
    REFERENCES Groups(group_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE,
  FOREIGN KEY(Permissions_permission_id)
    REFERENCES Permissions(permission_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE);


CREATE INDEX Permission_group_FKIndex1 ON Permission_group (Groups_group_id);
CREATE INDEX Permission_group_FKIndex2 ON Permission_group (Permissions_permission_id);


CREATE INDEX IFK_Rel_Permission_Group_Group ON Permission_group (Groups_group_id);
CREATE INDEX IFK_Rel_Permission_Group_Permi ON Permission_group (Permissions_permission_id);


CREATE TABLE Permission_user (
  permission_user_id SERIAL  NOT NULL ,
  Permissions_permission_id INTEGER   NOT NULL ,
  Users_user_id INTEGER   NOT NULL ,
  created_at TIMESTAMP  DEFAULT now() NOT NULL   ,
PRIMARY KEY(permission_user_id)    ,
  FOREIGN KEY(Permissions_permission_id)
    REFERENCES Permissions(permission_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE,
  FOREIGN KEY(Users_user_id)
    REFERENCES Users(user_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE);


CREATE INDEX Permission_user_FKIndex1 ON Permission_user (Permissions_permission_id);
CREATE INDEX Permission_user_FKIndex2 ON Permission_user (Users_user_id);


CREATE INDEX IFK_Rel_Permission_User_Permis ON Permission_user (Permissions_permission_id);
CREATE INDEX IFK_Rel_Permission_User_Users ON Permission_user (Users_user_id);


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
CREATE INDEX IFK_Rel_Machine_Related ON PartsOfMachine (Machines_related_machines_id);


CREATE TABLE PartsOfModel (
  parts_of_model_id SERIAL  NOT NULL ,
  ModelsVersion_related_models_version_id INTEGER   NOT NULL ,
  ModelsVersion_models_version_id INTEGER   NOT NULL   ,
PRIMARY KEY(parts_of_model_id)    ,
  FOREIGN KEY(ModelsVersion_models_version_id)
    REFERENCES ModelsVersion(models_version_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE,
  FOREIGN KEY(ModelsVersion_related_models_version_id)
    REFERENCES ModelsVersion(models_version_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE);


CREATE INDEX PartsOfModel_FKIndex1 ON PartsOfModel (ModelsVersion_models_version_id);
CREATE INDEX PartsOfModel_FKIndex2 ON PartsOfModel (ModelsVersion_related_models_version_id);


CREATE INDEX IFK_Rel_PartsOfModel ON PartsOfModel (ModelsVersion_models_version_id);
CREATE INDEX IFK_Rel_PartsOfModel_Related ON PartsOfModel (ModelsVersion_related_models_version_id);


CREATE TABLE ModelControl (
  model_control_id SERIAL  NOT NULL ,
  Stock_stock_id INTEGER   NOT NULL ,
  function_model TEXT      ,
PRIMARY KEY(model_control_id)  ,
  FOREIGN KEY(Stock_stock_id)
    REFERENCES Stock(stock_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE);


CREATE INDEX ModelControl_FKIndex1 ON ModelControl (Stock_stock_id);


CREATE INDEX IFK_Rel_Model_Control ON ModelControl (Stock_stock_id);


CREATE TABLE InputModel (
  input_model_id SERIAL  NOT NULL ,
  ModelControl_model_control_id INTEGER   NOT NULL ,
  position_input INTEGER      ,
PRIMARY KEY(input_model_id)    ,
  FOREIGN KEY(ModelControl_model_control_id)
    REFERENCES ModelControl(model_control_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE,
  FOREIGN KEY(ModelControl_model_control_id)
    REFERENCES ModelControl(model_control_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE);


CREATE INDEX InputModel_FKIndex1 ON InputModel (ModelControl_model_control_id);
CREATE INDEX InputModel_FKIndex2 ON InputModel (ModelControl_model_control_id);


CREATE INDEX IFK_Rel_ModelControl_Input ON InputModel (ModelControl_model_control_id);
CREATE INDEX IFK_Rel_ModelControl_Input_Sou ON InputModel (ModelControl_model_control_id);


CREATE TABLE OutputModel (
  output_model_id SERIAL  NOT NULL ,
  ModelControl_model_control_id INTEGER   NOT NULL ,
  position_output INTEGER   NOT NULL   ,
PRIMARY KEY(output_model_id)    ,
  FOREIGN KEY(ModelControl_model_control_id)
    REFERENCES ModelControl(model_control_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE,
  FOREIGN KEY(ModelControl_model_control_id)
    REFERENCES ModelControl(model_control_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE);


CREATE INDEX OutputModel_FKIndex1 ON OutputModel (ModelControl_model_control_id);
CREATE INDEX OutputModel_FKIndex2 ON OutputModel (ModelControl_model_control_id);


CREATE INDEX IFK_Rel_ModelControl_Output ON OutputModel (ModelControl_model_control_id);
CREATE INDEX IFK_Rel_ModelControl_Output_So ON OutputModel (ModelControl_model_control_id);
