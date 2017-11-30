#include "Procedure.h"

Procedure::Procedure()
{
	// Temporary

	std::vector<std::pair<int, int>> p;

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

	m_assembly_id = 24;

	m_CurrentPhase = m_Phases.cbegin();

	m_CurrentStep = m_CurrentPhase->m_Steps.cbegin();

}

bool Procedure::NextStep(Step & step)
{
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
			return false;
		}
	}
}

void Procedure::Reset()
{
	//Reset the pointers
	m_CurrentPhase = m_Phases.cbegin();
	m_CurrentStep = m_CurrentPhase->m_Steps.cbegin();
}

bool Procedure::HasPrevious()
{
	return m_CurrentStep.valid;
}

bool Procedure::HasNext()
{
	return m_CurrentPhase != m_Phases.cend();
}

const Phase & Procedure::CurrentPhase()
{
	return *m_CurrentPhase;
}

int Procedure::GetAssemblyId()
{
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
