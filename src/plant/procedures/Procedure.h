#pragma once

#include "plant/Assembly.h"
#include <vector>

/// <summary>
/// Represent an instruction to be executed
/// </summary>
struct Instruction {

	/// <summary>
	/// The different types of instructions
	/// </summary>
	enum Type
	{
		INS_PROCEDURE,
		INS_INSERT,
		INS_REMOVE,
		INS_CREATE,
		INS_DELETE
	};

	/// <summary>
	/// Constructor of the instruction for a non procedure instruction
	/// </summary>
	/// <param name="type">The type of the instruction</param>
	/// <param name="path">The path to the assembly edited by this instruction</param>
	/// <param name="description">The description of this instruction</param>
	Instruction(Type type, std::vector<std::pair<int, int> > path, std::string description = "") : m_type(type), m_path(path), m_procedure_id(0), m_description(description)
	{}

	/// <summary>
	/// Constructor for a procedure instruction
	/// </summary>
	/// <param name="path">The path to the assembly whose procedure is going to be executed</param>
	/// <param name="procedureId">The id of the procedure</param>
	/// <param name="description">The description of this instruction</param>
	Instruction(std::vector<std::pair<int, int> > path, int procedureId, std::string description = "") : m_type(INS_PROCEDURE), m_path(path), m_procedure_id(procedureId), m_description(description)
	{}

	/// <summary>
	/// The type of the instruction
	/// </summary>
	Type m_type;

	/// <summary>
	/// The path to the assembly related to this instruction
	/// </summary>
	std::vector<std::pair<int, int> > m_path;

	/// <summary>
	/// The id of the procedure to be executed. Only valid for a procedure instruction
	/// </summary>
	int m_procedure_id;

	/// <summary>
	/// Description of this instruction
	/// </summary>
	std::string m_description;
};

typedef std::vector<Instruction> Instructions;

/// <summary>
/// A set of instructions to be executed simultaneusly
/// </summary>
struct Step {

	/// <summary>
	/// The constructor of this step
	/// </summary>
	/// <param name="description">The description of these step</param>
	Step(std::string description = "") : m_description(description)
	{}

	/// <summary>
	/// Get the description of this step
	/// </summary>
	/// <returns></returns>
	std::string getDescription();

	/// <summary>
	/// Get the intructions of this step
	/// </summary>
	/// <returns></returns>
	const Instructions& getInstructions();

private:

	/// <summary>
	/// The description of this step
	/// </summary>
	std::string m_description;

	/// <summary>
	/// All the instructions of this step
	/// </summary>
	Instructions m_Instructions;

	friend class Procedure;
};

typedef std::vector<Step> Steps;

/// <summary>
/// Conceptual stage, used to group several step
/// </summary>
struct Phase {

	Phase(std::string description = "") : m_description(description)
	{}

	/// <summary>
	/// The description of this phase
	/// </summary>
	std::string m_description;

private:
	/// <summary>
	/// All the steps of this phase
	/// </summary>
	Steps m_Steps;

	friend class Procedure;
};

typedef std::vector<Phase> Phases;

class Procedure 
{
public:

	/// <summary>
	/// Constructor
	/// </summary>
	Procedure();

	/// <summary>
	/// Deliver the next step to be executed
	/// </summary>
	/// <param name="step">The next step to be execute, if there is one</param>
	/// <returns>If there is a valid step</returns>
	bool NextStep(Step& step);

	/// <summary>
	/// Return the phase of the step returned by the NextStep function
	/// </summary>
	/// <returns>The current phase</returns>
	const Phase& CurrentPhase();

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	int GetAssemblyId();

private:

	/// <summary>
	/// The id of this procedure
	/// </summary>
	int m_procedure_id;

	/// <summary>
	/// The assembly related with this procedure
	/// </summary>
	int m_assembly_id;

	/// <summary>
	/// All the phases of this procedure
	/// </summary>
	Phases m_Phases;

	/// <summary>
	/// Pointer to the current phase been executed
	/// </summary>
	Phases::const_iterator m_CurrentPhase;

	/// <summary>
	/// Pointer to the next step to be returned
	/// </summary>
	Steps::const_iterator m_CurrentStep;
};