DROP TABLE IF EXISTS output_model CASCADE;

DROP TABLE IF EXISTS input_model CASCADE;

DROP TABLE IF EXISTS stock_translation CASCADE;

DROP TABLE IF EXISTS model_control CASCADE;

DROP TABLE IF EXISTS substock CASCADE;

DROP TABLE IF EXISTS permission_user CASCADE;

DROP TABLE IF EXISTS permission_group CASCADE;

DROP TABLE IF EXISTS model_version CASCADE;

DROP TABLE IF EXISTS assembly_translation CASCADE;

DROP TABLE IF EXISTS group_user CASCADE;

DROP TABLE IF EXISTS role_group CASCADE;

DROP TABLE IF EXISTS sensor_stock CASCADE;

DROP TABLE IF EXISTS stock CASCADE;

DROP TABLE IF EXISTS role_user CASCADE;

DROP TABLE IF EXISTS assembly CASCADE;

DROP TABLE IF EXISTS translate CASCADE;

DROP TABLE IF EXISTS logger CASCADE;

DROP TABLE IF EXISTS groups CASCADE;

DROP TABLE IF EXISTS users CASCADE;

DROP TABLE IF EXISTS language CASCADE;

DROP TABLE IF EXISTS position_entity CASCADE;

DROP TABLE IF EXISTS roles CASCADE;

DROP TABLE IF EXISTS Sensor CASCADE;

DROP TABLE IF EXISTS model CASCADE;

DROP TABLE IF EXISTS Permissions CASCADE;

CREATE TABLE language (
  language_id SERIAL  NOT NULL ,
  iso_country_code VARCHAR(25)   NOT NULL ,
  iso_language_code VARCHAR(25)   NOT NULL ,
  codepage VARCHAR(25)      ,
PRIMARY KEY(language_id));




CREATE TABLE position_entity (
  position_entity_id SERIAL  NOT NULL ,
  pos_x FLOAT  DEFAULT 0.0  ,
  pos_y FLOAT  DEFAULT 0.0  ,
  pos_z FLOAT  DEFAULT 0.0  ,
  rot_roll FLOAT  DEFAULT 0.0  ,
  rot_pitch FLOAT  DEFAULT 0.0  ,
  rot_yaw FLOAT  DEFAULT 0.0    ,
PRIMARY KEY(position_entity_id));




CREATE TABLE model (
  model_id SERIAL  NOT NULL ,
  current_version INTEGER  DEFAULT 1    ,
PRIMARY KEY(model_id));




CREATE TABLE Permissions (
  permission_id SERIAL  NOT NULL ,
  permission VARCHAR(30)   NOT NULL ,
  created_at TIMESTAMP  DEFAULT now() NOT NULL ,
  deleted_at TIMESTAMP    ,
  updated_at TIMESTAMP  DEFAULT now()    ,
PRIMARY KEY(permission_id));




CREATE TABLE users (
  users_id SERIAL  NOT NULL ,
  username VARCHAR(15)   NOT NULL ,
  password_user VARCHAR(20)   NOT NULL ,
  first_name VARCHAR(25)   NOT NULL ,
  last_name VARCHAR(25)   NOT NULL ,
  active BOOL  DEFAULT true NOT NULL ,
  created_at TIMESTAMP  DEFAULT now() NOT NULL ,
  deleted_at TIMESTAMP    ,
  updated_at TIMESTAMP  DEFAULT now()    ,
PRIMARY KEY(users_id));




CREATE TABLE Sensor (
  sensor_id SERIAL  NOT NULL ,
  name VARCHAR(50)   NOT NULL ,
  path_sensor VARCHAR(256)   NOT NULL   ,
PRIMARY KEY(sensor_id));




CREATE TABLE roles (
  roles_id SERIAL  NOT NULL ,
  role VARCHAR(25)   NOT NULL ,
  created_at TIMESTAMP    ,
  deleted_at TIMESTAMP    ,
  updated_at TIMESTAMP      ,
PRIMARY KEY(roles_id));




CREATE TABLE groups (
  groups_id SERIAL  NOT NULL ,
  name VARCHAR(25)   NOT NULL ,
  created_at TIMESTAMP  DEFAULT now() NOT NULL ,
  deleted_at TIMESTAMP    ,
  updated_at TIMESTAMP  DEFAULT now()    ,
PRIMARY KEY(groups_id));




CREATE TABLE model_version (
  model_version_id SERIAL  NOT NULL ,
  model_id INTEGER   NOT NULL ,
  path_model VARCHAR(256)   NOT NULL ,
  material VARCHAR(20)    ,
  color VARCHAR(12)    ,
  animated BOOL  DEFAULT False NOT NULL ,
  version INTEGER  DEFAULT 1 NOT NULL ,
  created_at TIMESTAMP  DEFAULT now() NOT NULL ,
  updated_at TIMESTAMP  DEFAULT now()    ,
PRIMARY KEY(model_version_id)  ,
  FOREIGN KEY(model_id)
    REFERENCES model(model_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE);


CREATE INDEX ModelsVersion_FKIndex2 ON model_version (model_id);


CREATE INDEX IFK_Rel_models_version ON model_version (model_id);


CREATE TABLE logger (
  logger_id SERIAL  NOT NULL ,
  users_id INTEGER   NOT NULL ,
  source VARCHAR(30)    ,
  type_log VARCHAR(20)    ,
  message TEXT    ,
  created_at TIMESTAMP  DEFAULT now()    ,
PRIMARY KEY(logger_id)  ,
  FOREIGN KEY(users_id)
    REFERENCES users(users_id)
      ON UPDATE CASCADE);


CREATE INDEX logger_FKIndex1 ON logger (users_id);


CREATE INDEX IFK_Rel_Log ON logger (users_id);


CREATE TABLE translate (
  translate_id SERIAL  NOT NULL ,
  language_id INTEGER   NOT NULL ,
  name VARCHAR(20)   NOT NULL ,
  value VARCHAR(256)   NOT NULL   ,
PRIMARY KEY(translate_id, language_id)  ,
  FOREIGN KEY(language_id)
    REFERENCES language(language_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE);


CREATE INDEX translate_FKIndex1 ON translate (language_id);


CREATE INDEX IFK_Rel_translate ON translate (language_id);


CREATE TABLE assembly (
  assembly_id SERIAL  NOT NULL ,
  model_id INTEGER   NOT NULL ,
  part_number VARCHAR(20)  DEFAULT 'NA' NOT NULL   ,
PRIMARY KEY(assembly_id)  ,
  FOREIGN KEY(model_id)
    REFERENCES model(model_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE);


CREATE INDEX assembly_FKIndex1 ON assembly (model_id);


CREATE INDEX IFK_Rel_assembly_model ON assembly (model_id);


CREATE TABLE stock (
  stock_id SERIAL  NOT NULL ,
  position_entity_id INTEGER   NOT NULL ,
  assembly_id INTEGER   NOT NULL ,
  serial_number VARCHAR(20)   NOT NULL ,
  canShowInfo BOOL  DEFAULT false NOT NULL ,
  canBeSelected BOOL  DEFAULT false NOT NULL ,
  enable BOOL  DEFAULT true NOT NULL ,
  created_at TIMESTAMP  DEFAULT now() NOT NULL ,
  updated_at TIMESTAMP  DEFAULT now()    ,
PRIMARY KEY(stock_id)    ,
  FOREIGN KEY(assembly_id)
    REFERENCES assembly(assembly_id),
  FOREIGN KEY(position_entity_id)
    REFERENCES position_entity(position_entity_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE);


CREATE INDEX stock_FKIndex1 ON stock (assembly_id);
CREATE INDEX stock_FKIndex2 ON stock (position_entity_id);


CREATE INDEX IFK_Rel_assembly_stock ON stock (assembly_id);
CREATE INDEX IFK_Rel_stock_position ON stock (position_entity_id);


CREATE TABLE sensor_stock (
  sensor_stock_id SERIAL  NOT NULL ,
  sensor_id INTEGER   NOT NULL ,
  stock_id INTEGER   NOT NULL   ,
PRIMARY KEY(sensor_stock_id)    ,
  FOREIGN KEY(sensor_id)
    REFERENCES Sensor(sensor_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE,
  FOREIGN KEY(stock_id)
    REFERENCES stock(stock_id));


CREATE INDEX sensor_stock_FKIndex2 ON sensor_stock (sensor_id);
CREATE INDEX sensor_stock_stock_FKIndex2 ON sensor_stock (stock_id);


CREATE INDEX IFK_Rel_sensor ON sensor_stock (sensor_id);
CREATE INDEX IFK_Rel_stock_sensor ON sensor_stock (stock_id);


CREATE TABLE role_user (
  role_user_id SERIAL  NOT NULL ,
  roles_id INTEGER   NOT NULL ,
  users_id INTEGER   NOT NULL ,
  created_at TIMESTAMP  DEFAULT now() NOT NULL   ,
PRIMARY KEY(role_user_id)    ,
  FOREIGN KEY(users_id)
    REFERENCES users(users_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE,
  FOREIGN KEY(roles_id)
    REFERENCES roles(roles_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE);


CREATE INDEX role_user_FKIndex1 ON role_user (users_id);
CREATE INDEX role_user_FKIndex2 ON role_user (roles_id);


CREATE INDEX IFK_Rel_role_user_users ON role_user (users_id);
CREATE INDEX IFK_Rel_role_user_roles ON role_user (roles_id);


CREATE TABLE group_user (
  group_user_id SERIAL  NOT NULL ,
  users_id INTEGER   NOT NULL ,
  groups_id INTEGER   NOT NULL ,
  created_at TIMESTAMP  DEFAULT now() NOT NULL   ,
PRIMARY KEY(group_user_id)    ,
  FOREIGN KEY(users_id)
    REFERENCES users(users_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE,
  FOREIGN KEY(groups_id)
    REFERENCES groups(groups_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE);


CREATE INDEX group_user_FKIndex1 ON group_user (groups_id);
CREATE INDEX group_user_FKIndex2 ON group_user (users_id);


CREATE INDEX IFK_Rel_groups_user_users ON group_user (users_id);
CREATE INDEX IFK_Rel_groups_user_groups ON group_user (groups_id);


CREATE TABLE assembly_translation (
  assembly_translation_id SERIAL  NOT NULL ,
  language_id INTEGER   NOT NULL ,
  assembly_id INTEGER   NOT NULL ,
  name VARCHAR(25)   NOT NULL ,
  info VARCHAR(256)   NOT NULL ,
  shortInfo VARCHAR(256)   NOT NULL   ,
PRIMARY KEY(assembly_translation_id)    ,
  FOREIGN KEY(language_id)
    REFERENCES language(language_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE,
  FOREIGN KEY(assembly_id)
    REFERENCES assembly(assembly_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE);


CREATE INDEX assembly_translation_FKIndex1 ON assembly_translation (language_id);
CREATE INDEX assembly_translation_FKIndex2 ON assembly_translation (assembly_id);


CREATE INDEX IFK_Rel_language_translation ON assembly_translation (language_id);
CREATE INDEX IFK_Rel_assembly_translation ON assembly_translation (assembly_id);


CREATE TABLE role_group (
  role_group_id SERIAL  NOT NULL ,
  roles_id INTEGER   NOT NULL ,
  groups_id INTEGER   NOT NULL ,
  created_at TIMESTAMP      ,
PRIMARY KEY(role_group_id)    ,
  FOREIGN KEY(groups_id)
    REFERENCES groups(groups_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE,
  FOREIGN KEY(roles_id)
    REFERENCES roles(roles_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE);


CREATE INDEX role_group_FKIndex1 ON role_group (groups_id);
CREATE INDEX role_group_FKIndex2 ON role_group (roles_id);


CREATE INDEX IFK_Rel_Role_Group_Groups ON role_group (groups_id);
CREATE INDEX IFK_Rel_role_group_roles ON role_group (roles_id);


CREATE TABLE permission_user (
  permission_user_id SERIAL  NOT NULL ,
  permission_id INTEGER   NOT NULL ,
  users_id INTEGER   NOT NULL ,
  created_at TIMESTAMP  DEFAULT now() NOT NULL   ,
PRIMARY KEY(permission_user_id)    ,
  FOREIGN KEY(permission_id)
    REFERENCES Permissions(permission_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE,
  FOREIGN KEY(users_id)
    REFERENCES users(users_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE);


CREATE INDEX permission_user_FKIndex1 ON permission_user (permission_id);
CREATE INDEX permission_user_FKIndex2 ON permission_user (users_id);


CREATE INDEX IFK_Rel_permission_user_permis ON permission_user (permission_id);
CREATE INDEX IFK_Rel_permission_user_users ON permission_user (users_id);


CREATE TABLE permission_group (
  permission_group_id SERIAL  NOT NULL ,
  permission_id INTEGER   NOT NULL ,
  groups_id INTEGER   NOT NULL ,
  created_at TIMESTAMP      ,
PRIMARY KEY(permission_group_id)    ,
  FOREIGN KEY(groups_id)
    REFERENCES groups(groups_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE,
  FOREIGN KEY(permission_id)
    REFERENCES Permissions(permission_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE);


CREATE INDEX permission_group_FKIndex1 ON permission_group (groups_id);
CREATE INDEX permission_group_FKIndex2 ON permission_group (permission_id);


CREATE INDEX IFK_Rel_permission_group_group ON permission_group (groups_id);
CREATE INDEX IFK_Rel_permission_group_permi ON permission_group (permission_id);


CREATE TABLE model_control (
  model_control_id SERIAL  NOT NULL ,
  stock_id INTEGER   NOT NULL ,
  function_model TEXT      ,
PRIMARY KEY(model_control_id)  ,
  FOREIGN KEY(stock_id)
    REFERENCES stock(stock_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE);


CREATE INDEX model_control_FKIndex1 ON model_control (stock_id);


CREATE INDEX IFK_Rel_model_control ON model_control (stock_id);


CREATE TABLE substock (
  substock_id SERIAL  NOT NULL ,
  parent_stock_id INTEGER   NOT NULL ,
  child_stock_id INTEGER   NOT NULL   ,
PRIMARY KEY(substock_id)    ,
  FOREIGN KEY(child_stock_id)
    REFERENCES stock(stock_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE,
  FOREIGN KEY(parent_stock_id)
    REFERENCES stock(stock_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE);


CREATE INDEX substock_FKIndex2 ON substock (child_stock_id);
CREATE INDEX substock_FKIndex3 ON substock (parent_stock_id);


CREATE INDEX IFK_Rel_substock_child ON substock (child_stock_id);
CREATE INDEX IFK_Rel_substock_parent ON substock (parent_stock_id);


CREATE TABLE stock_translation (
  stock_translation_id SERIAL  NOT NULL ,
  stock_id INTEGER   NOT NULL ,
  language_id INTEGER   NOT NULL ,
  info VARCHAR(256)   NOT NULL   ,
PRIMARY KEY(stock_translation_id)    ,
  FOREIGN KEY(language_id)
    REFERENCES language(language_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE,
  FOREIGN KEY(stock_id)
    REFERENCES stock(stock_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE);


CREATE INDEX stock_translation_FKIndex1 ON stock_translation (language_id);
CREATE INDEX stock_translation_FKIndex2 ON stock_translation (stock_id);


CREATE INDEX IFK_Rel_stock_language ON stock_translation (language_id);
CREATE INDEX IFK_Rel_stock_translation ON stock_translation (stock_id);


CREATE TABLE input_model (
  input_model_id SERIAL  NOT NULL ,
  dst_model_control_id INTEGER   NOT NULL ,
  src_model_control_id INTEGER   NOT NULL ,
  position_input INTEGER      ,
PRIMARY KEY(input_model_id)    ,
  FOREIGN KEY(dst_model_control_id)
    REFERENCES model_control(model_control_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE,
  FOREIGN KEY(src_model_control_id)
    REFERENCES model_control(model_control_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE);


CREATE INDEX input_model_FKIndex1 ON input_model (dst_model_control_id);
CREATE INDEX input_model_FKIndex2 ON input_model (src_model_control_id);


CREATE INDEX IFK_Rel_model_control_input_ds ON input_model (dst_model_control_id);
CREATE INDEX IFK_Rel_model_control_input_sr ON input_model (src_model_control_id);


CREATE TABLE output_model (
  output_model_id SERIAL  NOT NULL ,
  dst_model_control_id INTEGER   NOT NULL ,
  src_model_control_id INTEGER   NOT NULL ,
  position_output INTEGER   NOT NULL   ,
PRIMARY KEY(output_model_id)    ,
  FOREIGN KEY(dst_model_control_id)
    REFERENCES model_control(model_control_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE,
  FOREIGN KEY(src_model_control_id)
    REFERENCES model_control(model_control_id)
      ON DELETE CASCADE
      ON UPDATE CASCADE);


CREATE INDEX output_model_FKIndex1 ON output_model (dst_model_control_id);
CREATE INDEX output_model_FKIndex2 ON output_model (src_model_control_id);


CREATE INDEX IFK_Rel_model_control_output_d ON output_model (dst_model_control_id);
CREATE INDEX IFK_Rel_model_control_output_s ON output_model (src_model_control_id);
