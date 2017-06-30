#pragma once

#include "../plant/sensors/Sensor.h"

#include <vector>
#include <string>
#include <memory>

/// <summary>
/// Abstract class of a model for simulation.
/// </summary>
class ModelControl
{
protected:
	std::vector<std::shared_ptr<Sensor>> m_listSensors; /* List of sensors. */
	std::vector<float> m_states; /* List of states. */
	std::vector<std::shared_ptr<ModelControl>> m_inputs; /* List input model. */
	std::vector<std::shared_ptr<ModelControl>> m_outputs; /* List output model. */

public:

	/// <summary>
	/// Construcor.
	/// </summary>
	ModelControl();

	/// <summary>
	/// Update state of model.
	/// </summary>
	virtual void updateState() = 0;

	/// <summary>
	/// Gets output model.
	/// </summary>
	/// <param name="dt">Delta time.</param>
	/// <returns>Returns output model.</returns>
	virtual std::vector<float> getOutput(float dt) = 0;

	/// <summary>
	/// Adds input model.
	/// </summary>
	/// <param name="modelInput">Model.</param>
	void addInput(std::shared_ptr<ModelControl> modelInput);

	/// <summary>
	/// Adds output model.
	/// </summary>
	/// <param name="modelOutput">Model.</param>
	void addOutput(std::shared_ptr<ModelControl> modelOutput);

	/// <summary>
	/// Adds sensor.
	/// </summary>
	/// <param name="sensor">Sensor.</param>
	void addSensor(std::shared_ptr<Sensor> sensor);

	/// <summary>
	/// Adds observer.
	/// </summary>
	/// <param name="funObs">Callback function.</param>
	void addObserver(std::function<void(Sensor*)> funObs);
};
