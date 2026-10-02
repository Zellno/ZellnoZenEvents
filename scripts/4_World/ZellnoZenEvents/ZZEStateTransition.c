class ZZEStateTransition
{
    protected string m_CandidateId;
    protected string m_PreviousState;
    protected string m_CurrentState;
    protected bool m_ObservationAccepted;
    protected bool m_BecameActive;
    protected bool m_BecameAbsent;

    void ZZEStateTransition(
        string candidateId,
        string previousState,
        string currentState,
        bool observationAccepted,
        bool becameActive,
        bool becameAbsent)
    {
        m_CandidateId = candidateId;
        m_PreviousState = previousState;
        m_CurrentState = currentState;
        m_ObservationAccepted = observationAccepted;
        m_BecameActive = becameActive;
        m_BecameAbsent = becameAbsent;
    }

    string GetCandidateId()
    {
        return m_CandidateId;
    }

    string GetPreviousState()
    {
        return m_PreviousState;
    }

    string GetCurrentState()
    {
        return m_CurrentState;
    }

    bool WasObservationAccepted()
    {
        return m_ObservationAccepted;
    }

    bool DidStateChange()
    {
        return m_PreviousState != m_CurrentState;
    }

    bool BecameActive()
    {
        return m_BecameActive;
    }

    bool BecameAbsent()
    {
        return m_BecameAbsent;
    }
}
