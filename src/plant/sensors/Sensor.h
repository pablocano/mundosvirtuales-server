#pragma once

#include <functional>
#include <vector>
#include <memory>

/// <summary>
/// Class encapsulate a sensor.
/// </summary>
class Sensor
{
protected:
	std::vector<std::function<void(Sensor*)>> m_listObs; /* List of observers (callback functions). */

	std::string m_name; /* Name of sensor*/
	std::string m_measurementUnit; /* Measurement unit. */
	std::shared_ptr<void> m_value; /* Current value sensor. */

public:
	
	/// <summary>
	/// Constructor.
	/// </summary>
	/// <param name="name">Name of sensor.</param>
	/// <param name="measurementUnit">Measurement Unit of sensor.</param>
	Sensor(std::string name, std::string measurementUnit) : m_name(name), m_measurementUnit(measurementUnit), m_value(nullptr) {}

	/* Delete copy constructor. */
	Sensor(Sensor const&) = delete;
	Sensor& operator =(Sensor const&) = delete;

	/// <summary>
	/// Update current value of sensor, also notifies all observers. 
	/// </summary>
	/// <param name="value">Value sensor.</param>
	void updateSensor(std::shared_ptr<void> value)
	{
		m_value = value; // Set value
		// Notifies all observers.
		for (std::function<void(Sensor*)> funObs : m_listObs)
		{
			funObs(this);
		}
	}

	/// <summary>
	/// Added observer.
	/// </summary>
	/// <param name="funObs">Callback function.</param>
	void addObserver(std::function<void(Sensor*)> funObs)
	{
		m_listObs.push_back(funObs);
	}

	/// <summary>
	/// Gets value sensor.
	/// </summary>
	/// <returns>Returns current value sensor.</returns>
	template <typename T>
	T getValue() const { return *dynamic_cast<T>(m_value); }

	/// <summary>
	/// Gets name sensor.
	/// </summary>
	/// <returns>Returns sensor name.</returns>
	std::string getName() const { return m_name; }

	/// <summary>
	/// Gets sensor measurement unit.
	/// </summary>
	/// <returns>Returns measurement unit.</returns>
	std::string getMeasurementUnit() const { return m_measurementUnit; }
};
