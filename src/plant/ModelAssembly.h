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
	///
	/// </summary>
	/// <returns></returns>
	std::string getPathModel() const;

	/// <summary>
	///
	/// </summary>
	/// <param name="path"></param>
	void setPathModel(std::string path)
	{
		m_path_model = path;
	}

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

	void setVersion(int version)
	{
		m_version = version;
	}

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

	friend void to_json(json& j, const ModelAssembly& m);

	friend void from_json(const json& j, ModelAssembly& m);
};

void to_json(json& j, const ModelAssembly& m);

void from_json(const json& j, ModelAssembly& m);
