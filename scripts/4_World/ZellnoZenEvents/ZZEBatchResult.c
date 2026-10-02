class ZZEBatchStatus
{
    static const string OK = "ok";
    static const string NOT_INITIALIZED = "not_initialized";
    static const string INVALID_REQUEST = "invalid_request";
}

class ZZEBatchResult
{
    protected string m_Status;
    protected int m_RequestedCount;
    protected int m_StartCursor;
    protected int m_EndCursor;
    protected bool m_CompletedCycle;
    protected ref array<ref ZZEDetectionResult> m_Detections;
    protected ref array<ref ZZEStateTransition> m_Transitions;

    void ZZEBatchResult(string status, int requestedCount, int startCursor)
    {
        m_Status = status;
        m_RequestedCount = requestedCount;
        m_StartCursor = startCursor;
        m_EndCursor = startCursor;
        m_CompletedCycle = false;
        m_Detections = new array<ref ZZEDetectionResult>;
        m_Transitions = new array<ref ZZEStateTransition>;
    }

    void Add(ZZEDetectionResult detection, ZZEStateTransition transition)
    {
        m_Detections.Insert(detection);
        m_Transitions.Insert(transition);
    }

    void Complete(int endCursor, bool completedCycle)
    {
        m_EndCursor = endCursor;
        m_CompletedCycle = completedCycle;
    }

    string GetStatus()
    {
        return m_Status;
    }

    int GetRequestedCount()
    {
        return m_RequestedCount;
    }

    int GetProcessedCount()
    {
        return m_Transitions.Count();
    }

    int GetStartCursor()
    {
        return m_StartCursor;
    }

    int GetEndCursor()
    {
        return m_EndCursor;
    }

    bool CompletedCycle()
    {
        return m_CompletedCycle;
    }

    array<ref ZZEDetectionResult> GetDetections()
    {
        return m_Detections;
    }

    array<ref ZZEStateTransition> GetTransitions()
    {
        return m_Transitions;
    }
}
