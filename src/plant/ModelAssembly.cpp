#include "ModelAssembly.h"



std::string ModelAssembly::getPathModel() const 
{ 
	return m_path_model; 
}

std::string ModelAssembly::getMaterial() const 
{ 
	return m_material;
}

std::string ModelAssembly::getColor() const 
{ 
	return m_color;
}

bool ModelAssembly::isAnimated() const 
{ 
	return m_animated;
}

int ModelAssembly::getVersion() const 
{ 
	return m_version;
}

void ModelAssembly::operator=(const Row& row)
{
	
	ObjectDB::operator=(row);
	this->m_path_model	= row.get<std::string>("path_model");
	this->m_material	= row.get<std::string>("material");
	this->m_color		= row.get<std::string>("color");
	this->m_animated	= row.get<bool>("animated");
	this->m_version		= row.get<int>("version");
}

Row ModelAssembly::getRow() const
{
	Row row;
	std::shared_ptr<Fields> fieldData;

	fieldData->push_back(FieldData(this->getIDFieldName(), TypeData::DB_INTEGER));
	fieldData->push_back(FieldData("path_model", TypeData::DB_STRING));
	fieldData->push_back(FieldData("material", TypeData::DB_STRING));
	fieldData->push_back(FieldData("color", TypeData::DB_STRING));
	fieldData->push_back(FieldData("animated", TypeData::DB_BOOL));
	fieldData->push_back(FieldData("version", TypeData::DB_INTEGER));

	row.setFieldData(fieldData);

	row.addRegisterPerValue<int>(this->getID());
	row.addRegisterPerValue<std::string>(this->getPathModel());
	row.addRegisterPerValue<std::string>(this->getMaterial());
	row.addRegisterPerValue<std::string>(this->getColor());
	row.addRegisterPerValue<bool>(this->isAnimated());
	row.addRegisterPerValue<int>(this->getVersion());

	return row;
}

std::string ModelAssembly::getJoin() const
{
	return "LEFT JOIN model_version ON ((model.model_id = model_version.model_id) AND (model.current_version = model_version.version))";
}

std::string ModelAssembly::getFieldsSelect() const
{
	return "model_id, path_model, material, color, animated, version";
}

void to_json(json& j, const ModelAssembly& m) {
	j = json{ 
		{ "m_path_model",	m.m_path_model },
		{ "m_material",		m.m_material },
		{ "m_color",		m.m_color },
		{ "m_animated",		m.m_animated },
		{ "m_version",		m.m_version } };
}

void from_json(const json& j, ModelAssembly& m) {
	m.m_path_model	= j.at("m_path_model").get<std::string>();
	m.m_material	= j.at("m_material").get<std::string>();
	m.m_color		= j.at("m_color").get<std::string>();
	m.m_animated	= j.at("m_animated").get<bool>();
	m.m_version		= j.at("m_version").get<int>();
}