class ZZECandidateStateMachine
{
    protected string m_CandidateId;
    protected string m_State;

    void ZZECandidateStateMachine(string candidateId)
    {
        m_CandidateId = candidateId;
        m_State = ZZEEventState.UNKNOWN;
    }

    ZZEStateTransition ApplyObservation(ZZEDetectionResult observation)
    {
        string previousState = m_State;

        if (!IsAcceptedObservation(observation))
        {
            return BuildTransition(previousState, false);
        }

        if (observation.IsDetected())
        {
            ApplyPresence();
        }
        else
        {
            ApplyAbsence();
        }

        return BuildTransition(previousState, true);
    }

    string GetCandidateId()
    {
        return m_CandidateId;
    }

    string GetState()
    {
        return m_State;
    }

    bool IsActive()
    {
        if (m_State == ZZEEventState.ACTIVE) return true;
        return m_State == ZZEEventState.ABSENT_PENDING;
    }

    protected bool IsAcceptedObservation(ZZEDetectionResult observation)
    {
        if (!observation || observation.GetCandidateId() != m_CandidateId)
        {
            return false;
        }

        string status = observation.GetStatus();
        if (status == ZZEDetectionStatus.DETECTED) return true;
        return status == ZZEDetectionStatus.NOT_DETECTED;
    }

    protected void ApplyPresence()
    {
        if (m_State == ZZEEventState.UNKNOWN || m_State == ZZEEventState.ABSENT)
        {
            m_State = ZZEEventState.PRESENT_PENDING;
            return;
        }

        if (m_State == ZZEEventState.PRESENT_PENDING || m_State == ZZEEventState.ABSENT_PENDING)
        {
            m_State = ZZEEventState.ACTIVE;
        }
    }

    protected void ApplyAbsence()
    {
        if (m_State == ZZEEventState.UNKNOWN || m_State == ZZEEventState.PRESENT_PENDING)
        {
            m_State = ZZEEventState.ABSENT;
            return;
        }

        if (m_State == ZZEEventState.ACTIVE)
        {
            m_State = ZZEEventState.ABSENT_PENDING;
            return;
        }

        if (m_State == ZZEEventState.ABSENT_PENDING)
        {
            m_State = ZZEEventState.ABSENT;
        }
    }

    protected ZZEStateTransition BuildTransition(string previousState, bool accepted)
    {
        bool becameActive = false;
        if (accepted && previousState == ZZEEventState.PRESENT_PENDING && m_State == ZZEEventState.ACTIVE)
        {
            becameActive = true;
        }

        bool becameAbsent = false;
        if (accepted && m_State == ZZEEventState.ABSENT)
        {
            becameAbsent = previousState == ZZEEventState.ACTIVE || previousState == ZZEEventState.ABSENT_PENDING;
        }

        return new ZZEStateTransition(
            m_CandidateId,
            previousState,
            m_State,
            accepted,
            becameActive,
            becameAbsent);
    }
}
