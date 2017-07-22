#pragma once

#include "../utils/serialization/json.h"
#include "../utils/db/ObjectDB.h"
#

using json = nlohmann::json;

class ModelAssembly : public ObjectDB
{
protected:

	std::string m_path_model; /* Path where the model is stored. */
	std::string m_material; /* Material of model. */
	std::string m_color; /* Color of model. */
	bool m_animated; /* Flag used to know if this model is animated. */
	int m_version; /* Current version of model. */

public:
	/// <summary>
	/// Constructor.
	/// </summary>
	ModelAssembly() : ObjectDB(0, "model", nullptr), m_path_model(), m_material(), m_color(), m_animated(false), m_version(-1) {}

	/// <summary>
	/// Constructor.
	/// </summary>
	/// <param name="path_model"></param>
	/// <param name="material"></param>
	/// <param name="color"></param>
	/// <param name="animated"></param>
	/// <param name="version"></param>
	ModelAssembly(std::string path_model, std::string material, std::string color, bool animated = 0, int version = -1) :
		ObjectDB(0, "model", nullptr), m_path_model(path_model), m_material(material), m_color(color), m_animated(animated), m_version(version) {}

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	std::string getPathModel() const;

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	std::string getMaterial() const;

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	std::string getColor() const;

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool isAnimated() const;

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	int getVersion() const;

	/// <summary>
	/// Operator equals with Row.
	/// </summary>
	/// <param name="row">Row reference.</param>
	void operator=(const Row& row);

	/// <summary>
	/// Gets a row.
	/// </summary>
	/// <returns>Returns a row constructed from this object's data.</returns>
	Row getRow() const;

	/// <summary>
	/// 
	/// </summary>
	std::string getIDFieldName() const;

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	std::string getJoin() const;

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	std::string getFieldsSelect() const;

	friend void to_json(json& j, const ModelAssembly& m);

	friend void from_json(const json& j, ModelAssembly& m);
};

void to_json(json& j, const ModelAssembly& m);

void from_json(const json& j, ModelAssembly& m);
