class ZZEStateRegistry
{
    protected ref map<string, ref ZZECandidateStateMachine> m_Machines;

    void ZZEStateRegistry()
    {
        m_Machines = new map<string, ref ZZECandidateStateMachine>;
    }

    bool Initialize(array<ref ZZECandidateDefinition> candidates, out string errorMessage)
    {
        errorMessage = "";
        m_Machines.Clear();

        if (!candidates || candidates.Count() != ZZEConstants.EXPECTED_CANDIDATE_TOTAL)
        {
            errorMessage = "Unexpected candidate count";
            return false;
        }

        foreach (ZZECandidateDefinition candidate : candidates)
        {
            if (!candidate || candidate.GetId() == "")
            {
                errorMessage = "Invalid candidate while initializing state registry";
                m_Machines.Clear();
                return false;
            }

            if (m_Machines.Contains(candidate.GetId()))
            {
                errorMessage = "Duplicate candidate in state registry: " + candidate.GetId();
                m_Machines.Clear();
                return false;
            }

            m_Machines.Insert(
                candidate.GetId(),
                new ZZECandidateStateMachine(candidate.GetId()));
        }

        return true;
    }

    ZZECandidateStateMachine GetMachine(string candidateId)
    {
        return m_Machines.Get(candidateId);
    }

    int Count()
    {
        return m_Machines.Count();
    }

    void GetActiveCandidateIds(array<string> output)
    {
        if (!output)
        {
            return;
        }

        output.Clear();

        for (int index = 0; index < m_Machines.Count(); index++)
        {
            ZZECandidateStateMachine machine = m_Machines.GetElement(index);
            if (machine && machine.IsActive())
            {
                output.Insert(m_Machines.GetKey(index));
            }
        }

        output.Sort();
    }
}
