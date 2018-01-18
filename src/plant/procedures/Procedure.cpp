#include "Procedure.h"
#include "utils/pugixml/pugixml.hpp"

Procedure::Procedure(std::string filename) : valid(false)
{
	pugi::xml_document doc;
	if (!doc.load_file(filename.c_str())) return;

	pugi::xml_node procedureNode = doc.child("procedure");

	for (pugi::xml_node phaseNode : procedureNode.children("phase"))
	{
		std::string phaseDescription = phaseNode.attribute("description").value();
		Phase phase(phaseDescription);
		for (pugi::xml_node stepNode : phaseNode.children("step"))
		{
			std::string stepDescription = stepNode.attribute("description").value();
			Step step(stepDescription);
			for (pugi::xml_node instructionNode : stepNode.children("instruction"))
			{
				Instruction::Type type = (Instruction::Type)instructionNode.attribute("type").as_int();
				AssemblyPath path = AssemblyPath(instructionNode.attribute("path").value());
				int procedureId = instructionNode.attribute("procedureId").as_int();
				std::string description = instructionNode.attribute("description").value();
				
				if (type == Instruction::INS_PROCEDURE) {
					Instruction ins(path.GetPath(),procedureId, description);
					step.m_Instructions.push_back(ins);
				}
				else {
					Instruction ins(type, path.GetPath(), description);
					step.m_Instructions.push_back(ins);
				}
			}
			phase.m_Steps.push_back(step);
		}
		m_Phases.push_back(phase);
	}

	valid = true;

	m_CurrentPhase = m_Phases.cbegin();

	m_CurrentStep = m_CurrentPhase->m_Steps.cbegin();

	// Temporary

	/*std::vector<std::pair<int, int>> p;

	p.push_back(std::pair<int, int>(11, 1));

	Instruction i(Instruction::INS_REMOVE, p);

	p.clear();
	p.push_back(std::pair<int, int>(11, 2));

	Instruction i2(Instruction::INS_REMOVE, p);

	Step s;

	s.m_Instructions.push_back(i);
	s.m_Instructions.push_back(i2);

	Phase ph;

	ph.m_Steps.push_back(s);

	i = Instruction(Instruction::INS_DELETE,p);

	p.clear();
	p.push_back(std::pair<int, int>(11, 1));

	i2 = Instruction(Instruction::INS_DELETE, p);

	s = Step();
	s.m_Instructions.push_back(i);
	s.m_Instructions.push_back(i2);

	p.clear();
	p.push_back(std::pair<int, int>(10, 1));
	i = Instruction(Instruction::INS_REMOVE, p);
	s.m_Instructions.push_back(i);

	p.clear();
	p.push_back(std::pair<int, int>(10, 2));
	i = Instruction(Instruction::INS_REMOVE, p);
	s.m_Instructions.push_back(i);

	p.clear();
	p.push_back(std::pair<int, int>(10, 3));
	i = Instruction(Instruction::INS_REMOVE, p);
	s.m_Instructions.push_back(i);

	p.clear();
	p.push_back(std::pair<int, int>(10, 4));
	i = Instruction(Instruction::INS_REMOVE, p);
	s.m_Instructions.push_back(i);

	p.clear();
	p.push_back(std::pair<int, int>(10, 5));
	i = Instruction(Instruction::INS_REMOVE, p);
	s.m_Instructions.push_back(i);

	p.clear();
	p.push_back(std::pair<int, int>(10, 6));
	i = Instruction(Instruction::INS_REMOVE, p);
	s.m_Instructions.push_back(i);

	p.clear();
	p.push_back(std::pair<int, int>(10, 7));
	i = Instruction(Instruction::INS_REMOVE, p);
	s.m_Instructions.push_back(i);

	p.clear();
	p.push_back(std::pair<int, int>(10, 8));
	i = Instruction(Instruction::INS_REMOVE, p);
	s.m_Instructions.push_back(i);

	ph.m_Steps.push_back(s);

	m_Phases.push_back(ph);

	ph = Phase();

	s = Step();

	p.clear();
	p.push_back(std::pair<int, int>(7, 1));
	i = Instruction(Instruction::INS_REMOVE, p);
	s.m_Instructions.push_back(i);

	p.clear();
	p.push_back(std::pair<int, int>(8, 1));
	i = Instruction(Instruction::INS_REMOVE, p);
	s.m_Instructions.push_back(i);

	p.clear();
	p.push_back(std::pair<int, int>(9, 1));
	i = Instruction(Instruction::INS_REMOVE, p);
	s.m_Instructions.push_back(i);

	p.clear();
	p.push_back(std::pair<int, int>(12, 1));
	i = Instruction(Instruction::INS_REMOVE, p);
	s.m_Instructions.push_back(i);

	p.clear();
	p.push_back(std::pair<int, int>(10, 1));
	i = Instruction(Instruction::INS_DELETE, p);
	s.m_Instructions.push_back(i);

	p.clear();
	p.push_back(std::pair<int, int>(10, 2));
	i = Instruction(Instruction::INS_DELETE, p);
	s.m_Instructions.push_back(i);

	p.clear();
	p.push_back(std::pair<int, int>(10, 3));
	i = Instruction(Instruction::INS_DELETE, p);
	s.m_Instructions.push_back(i);

	p.clear();
	p.push_back(std::pair<int, int>(10, 4));
	i = Instruction(Instruction::INS_DELETE, p);
	s.m_Instructions.push_back(i);

	p.clear();
	p.push_back(std::pair<int, int>(10, 5));
	i = Instruction(Instruction::INS_DELETE, p);
	s.m_Instructions.push_back(i);

	p.clear();
	p.push_back(std::pair<int, int>(10, 6));
	i = Instruction(Instruction::INS_DELETE, p);
	s.m_Instructions.push_back(i);

	p.clear();
	p.push_back(std::pair<int, int>(10, 7));
	i = Instruction(Instruction::INS_DELETE, p);
	s.m_Instructions.push_back(i);

	p.clear();
	p.push_back(std::pair<int, int>(10, 8));
	i = Instruction(Instruction::INS_DELETE, p);
	s.m_Instructions.push_back(i);

	ph.m_Steps.push_back(s);

	m_Phases.push_back(ph);

	m_assembly_id = 24;*/
}

bool Procedure::NextStep(Step & step)
{
	if (!valid)
		return false;

	// If the top level pointer is in the end of the list, it means that all instructions where passed
	if (m_CurrentPhase == m_Phases.cend())
		return false;

	if (!m_CurrentStep.valid) 
	{
		m_CurrentStep.valid = true;
		// Return the current step
		step = *m_CurrentStep;
		return true;
	}


	// Go one step forward
	m_CurrentStep++;

	// If the current instruction is not valid, go to the next phase
	if (m_CurrentStep != m_CurrentPhase->m_Steps.cend())
	{
		// Return the current step
		step = *m_CurrentStep;
		return true;
	}
	else
	{
		// Change the step
		m_CurrentPhase++;
		if (m_CurrentPhase != m_Phases.cend())
		{
			// Reinitialize the pointer
			m_CurrentStep = m_CurrentPhase->m_Steps.cbegin();

			// Return the current step
			step = *m_CurrentStep;
			return true;
		}
		else
			return false;
	}
}

bool Procedure::PreviousStep(Step & step)
{
	if (!valid)
		return false;

	// If both pointer are in the beginning, there are not previous steps
	if (m_CurrentPhase == m_Phases.cbegin() && m_CurrentStep == m_CurrentPhase->m_Steps.cbegin())
		return false;

	// If the current instruction is not valid, go one step forward
	if (m_CurrentStep != m_CurrentPhase->m_Steps.cbegin())
	{
		// Return the current step and go one step backwards
		m_CurrentStep--;
		step = *m_CurrentStep;
		return true;
	}
	else
	{
		// Return the current step
		step = *m_CurrentStep;

		// If the current phase is not the first, go to the previous phase
		if (m_CurrentPhase != m_Phases.cbegin())
		{
			m_CurrentPhase--;

			// Re initialize the current step
			m_CurrentStep = m_CurrentPhase->m_Steps.cend();
		}
		else 
		{
			m_CurrentStep.valid = false;
		}

		return true;
	}
}

void Procedure::Reset()
{
	if (!valid)
		return;

	//Reset the pointers
	m_CurrentPhase = m_Phases.cbegin();
	m_CurrentStep = m_CurrentPhase->m_Steps.cbegin();
}

bool Procedure::HasPrevious()
{
	if (!valid)
		return false;

	return m_CurrentStep.valid;
}

bool Procedure::HasNext()
{
	if (!valid)
		return false;

	return m_CurrentPhase != m_Phases.cend();
}

const Phase * Procedure::CurrentPhase()
{
	if (!valid)
		return nullptr;

	return &(*m_CurrentPhase);
}

int Procedure::GetAssemblyId()
{
	if (!valid)
		return -1;

	return m_assembly_id;
}

std::string Step::getDescription()
{
	return m_description;
}
const Instructions & Step::getInstructions()
{
	return m_Instructions;
}

AssemblyPath::AssemblyPath(std::string stringPath)
{
	// Set the string path
	m_StringPath = stringPath;

	// Set the position of the first token
	std::size_t firstPos = 0;
	std::size_t tokenPos = m_StringPath.find_first_of(":");

	// Iterate over all the string
	while (tokenPos != std::string::npos)
	{
		// Obtain the first part of the pair
		std::string first = m_StringPath.substr(firstPos, tokenPos - firstPos);

		// Obtain the second part of the pair
		firstPos = tokenPos + 1;
		tokenPos = m_StringPath.find_first_of(",", firstPos);
		std::string second = m_StringPath.substr(firstPos, tokenPos - firstPos);
		
		// Push the new pair into the vector
		m_Path.push_back(std::pair<int, int>(std::stoi(first), std::stoi(second)));

		if (tokenPos == std::string::npos)
			break;

		// Find the next pair
		firstPos = tokenPos + 1;
		tokenPos = m_StringPath.find_first_of(":",tokenPos + 1);
	}

}

AssemblyPath::AssemblyPath(std::vector<std::pair<int, int>> path)
{
	// Initialize the vector path
	m_Path = path;

	// Helper string stream
	std::stringstream ss;

	// Iterate over the vector
	for (std::pair<int, int> p : m_Path)
	{
		// Create the string
		ss << p.first << ":" << p.second << ",";
	}

	// Obtain the string
	std::string m_StringPath = ss.str();

	// Erase the last character
	m_StringPath.erase(m_StringPath.end() - 1, m_StringPath.end());
}