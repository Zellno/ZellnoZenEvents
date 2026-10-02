class ZZEManualBatchController
{
    protected ref ZZEManifestCatalog m_Catalog;
    protected ref ZZESpatialDetector m_Detector;
    protected ref ZZEStateRegistry m_Registry;
    protected int m_Cursor;
    protected bool m_Initialized;

    void ZZEManualBatchController()
    {
        m_Catalog = ZZEManifestCatalog.CreateGenerated();
        m_Detector = new ZZESpatialDetector();
        m_Registry = new ZZEStateRegistry();
        m_Cursor = 0;
        m_Initialized = false;
    }

    bool Initialize(out string errorMessage)
    {
        m_Cursor = 0;
        m_Initialized = false;

        if (!m_Catalog.Validate(errorMessage))
        {
            return false;
        }

        if (!m_Registry.Initialize(m_Catalog.GetCandidates(), errorMessage))
        {
            return false;
        }

        m_Initialized = true;
        return true;
    }

    ZZEBatchResult ProcessNextBatch(int requestedCount)
    {
        if (!m_Initialized)
        {
            return new ZZEBatchResult(
                ZZEBatchStatus.NOT_INITIALIZED,
                requestedCount,
                m_Cursor);
        }

        if (requestedCount <= 0)
        {
            return new ZZEBatchResult(
                ZZEBatchStatus.INVALID_REQUEST,
                requestedCount,
                m_Cursor);
        }

        array<ref ZZECandidateDefinition> candidates = m_Catalog.GetCandidates();
        int candidateCount = candidates.Count();
        int processCount = requestedCount;
        if (processCount > candidateCount)
        {
            processCount = candidateCount;
        }

        int startCursor = m_Cursor;
        bool completedCycle = false;
        ZZEBatchResult result = new ZZEBatchResult(
            ZZEBatchStatus.OK,
            requestedCount,
            startCursor);

        for (int index = 0; index < processCount; index++)
        {
            ZZECandidateDefinition candidate = candidates.Get(m_Cursor);
            ZZEDetectionResult detection = m_Detector.Probe(candidate);
            ZZECandidateStateMachine machine = m_Registry.GetMachine(candidate.GetId());
            ZZEStateTransition transition = machine.ApplyObservation(detection);
            result.Add(detection, transition);

            m_Cursor++;
            if (m_Cursor >= candidateCount)
            {
                m_Cursor = 0;
                completedCycle = true;
            }
        }

        result.Complete(m_Cursor, completedCycle);
        return result;
    }

    bool IsInitialized()
    {
        return m_Initialized;
    }

    int GetCursor()
    {
        return m_Cursor;
    }

    ZZEStateRegistry GetRegistry()
    {
        return m_Registry;
    }
}
