class ZZEDetectionStatus
{
    static const string DETECTED = "detected";
    static const string NOT_DETECTED = "not_detected";
    static const string INVALID_CANDIDATE = "invalid_candidate";
    static const string NOT_DEDICATED_SERVER = "not_dedicated_server";
}

class ZZEDetectionResult
{
    protected string m_CandidateId;
    protected string m_Status;
    protected int m_ObjectsScanned;
    protected string m_MatchedClass;
    protected vector m_MatchedPosition;
    protected float m_Distance2D;

    void ZZEDetectionResult(string candidateId, string status)
    {
        m_CandidateId = candidateId;
        m_Status = status;
        m_ObjectsScanned = 0;
        m_MatchedClass = "";
        m_MatchedPosition = "0 0 0";
        m_Distance2D = -1.0;
    }

    void SetObjectsScanned(int objectsScanned)
    {
        m_ObjectsScanned = objectsScanned;
    }

    void SetMatch(string matchedClass, vector matchedPosition, float distance2D)
    {
        m_Status = ZZEDetectionStatus.DETECTED;
        m_MatchedClass = matchedClass;
        m_MatchedPosition = matchedPosition;
        m_Distance2D = distance2D;
    }

    string GetCandidateId()
    {
        return m_CandidateId;
    }

    string GetStatus()
    {
        return m_Status;
    }

    bool IsDetected()
    {
        return m_Status == ZZEDetectionStatus.DETECTED;
    }

    int GetObjectsScanned()
    {
        return m_ObjectsScanned;
    }

    string GetMatchedClass()
    {
        return m_MatchedClass;
    }

    vector GetMatchedPosition()
    {
        return m_MatchedPosition;
    }

    float GetDistance2D()
    {
        return m_Distance2D;
    }
}
