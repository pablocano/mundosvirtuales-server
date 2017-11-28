#pragma once

#include "plant/Assembly.h"
#include <vector>

class Procedure 
{
public:

	Procedure();

private:

	/// <summary>
	/// Represent an instruction to be executed
	/// </summary>
	struct Instruction {

		/// <summary>
		/// The different types of instructions
		/// </summary>
		enum Type
		{
			PROCEDURE,
			INSERT,
			REMOVE,
			CREATE,
			DELETE
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
		Instruction(std::vector<std::pair<int, int> > path, int procedureId, std::string description = "") : m_type(PROCEDURE), m_path(path), m_procedure_id(procedureId), m_description(description)
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
		/// All the instructions of this step
		/// </summary>
		Instructions m_Instructions;

		/// <summary>
		/// The description of this step
		/// </summary>
		std::string m_description;

	};

	typedef std::vector<Step> Steps;

	/// <summary>
	/// Conceptual stage, used to group several step
	/// </summary>
	struct Phase {

		Phase(std::string description = "") : m_description(description)
		{}
		/// <summary>
		/// All the steps of this phase
		/// </summary>
		Steps m_Steps;

		/// <summary>
		/// The description of this phase
		/// </summary>
		std::string m_description;
	};

	typedef std::vector<Phase> Phases;

	/// <summary>
	/// All the phases of this procedure
	/// </summary>
	Phases m_Phases;

};