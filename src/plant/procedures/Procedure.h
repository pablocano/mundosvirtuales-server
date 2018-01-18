#pragma once

#include "plant/Assembly.h"
#include <vector>

/// <summary>
/// Helper class to manage the path to the assemblies
/// </summary>
struct AssemblyPath
{
	/// <summary>
	/// Constructor based of a string that describes the path to an assembly
	/// </summary>
	/// <param name="stringPath">A string representing the path to an assembly</param>
	AssemblyPath(std::string stringPath);

	/// <summary>
	/// Constructor based of a vertor that describes the path to an assembly
	/// </summary>
	/// <param name="path">A vector of pairs assembly-subassembly instance, representing the path to an assembly</param>
	AssemblyPath(std::vector<std::pair<int, int> > path);

	/// <summary>
	/// Get the path in the string format
	/// </summary>
	/// <returns></returns>
	std::string& GetStringPath() { return m_StringPath; }

	/// <summary>
	/// Get the path in a vector format
	/// </summary>
	/// <returns></returns>
	std::vector<std::pair<int, int> >& GetPath() { return m_Path; }

	/// <summary>
	/// The path in form of a string
	/// </summary>
	std::string m_StringPath;

	/// <summary>
	/// The path in form of a vector
	/// </summary>
	std::vector<std::pair<int, int> > m_Path;
};

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
	AssemblyPath m_path;

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

	/// <summary>
	/// Defualt constructor
	/// </summary>
	/// <param name="description">The description of this phase</param>
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

	/// <summary>
	/// Friend class to allow access to all members
	/// </summary>
	friend class Procedure;
};

typedef std::vector<Phase> Phases;

/// <summary>
/// Helper iterator class to test if the first step has been sent
/// </summary>
struct StepIterator : public std::vector<Step>::const_iterator
{
	/// <summary>
	/// The default constructor
	/// </summary>
	StepIterator() : valid(false) {}

	/// <summary>
	/// The copy constructor
	/// </summary>
	/// <param name="it"></param>
	StepIterator(const std::vector<Step>::const_iterator& it) : std::vector<Step>::const_iterator(it), valid(false) {}

	bool valid;
};

class Procedure 
{
public:

	/// <summary>
	/// Constructor
	/// </summary>
	/// <param name="filename">The xml file where this procedure is stored</param>
	Procedure(std::string filename);

	/// <summary>
	/// Deliver the next step to be executed
	/// </summary>
	/// <param name="step">The next step to be execute, if there is one</param>
	/// <returns>If there is a valid step</returns>
	bool NextStep(Step& step);

	/// <summary>
	/// Deliver the previous step
	/// </summary>
	/// <param name="step">The previous step</param>
	/// <returns>If there is a previous step</returns>
	bool PreviousStep(Step& step);

	/// <summary>
	/// Reset the procedure 
	/// </summary>
	void Reset();

	/// <summary>
	/// Ask if there is a previous step
	/// </summary>
	/// <returns>If there is a previous step before the current step </returns>
	bool HasPrevious();

	/// <summary>
	/// Ask if there is a next step
	/// </summary>
	/// <returns>If there is a next step after the current step </returns>
	bool HasNext();

	/// <summary>
	/// Return the phase of the step returned by the NextStep function
	/// </summary>
	/// <returns>The current phase</returns>
	const Phase* CurrentPhase();

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
	StepIterator m_CurrentStep;

	/// <summary>
	/// If this procedure is constructed correctly
	/// </summary>
	bool valid;
};