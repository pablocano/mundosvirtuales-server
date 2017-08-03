#include "ModelControl.h"

ModelControl::ModelControl() : m_listSensors(), m_states(), m_inputs(), m_outputs()
{
}

void ModelControl::addInput(std::shared_ptr<ModelControl> modelInput)
{
	m_inputs.push_back(modelInput);
}

void ModelControl::addOutput(std::shared_ptr<ModelControl> modelOutput)
{
	m_outputs.push_back(modelOutput);
}

void ModelControl::addSensor(std::shared_ptr<Sensor> sensor)
{
	m_listSensors.push_back(sensor);
}

void ModelControl::addObserver(std::function<void(Sensor*)> funObs)
{
	for (std::shared_ptr<Sensor> sensor : m_listSensors)
	{
		sensor->addObserver(funObs);
	}
}