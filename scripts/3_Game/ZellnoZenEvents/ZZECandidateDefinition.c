class ZZECandidateDefinition
{
    protected string m_Id;
    protected string m_Category;
    protected string m_CEEvent;
    protected int m_SpawnIndex;
    protected string m_Group;
    protected vector m_MarkerPosition;
    protected float m_SpawnAngle;
    protected vector m_ProbePosition;
    protected ref array<string> m_SemanticAnchorClasses;
    protected string m_SelectedGroupAnchor;
    protected vector m_GroupAnchorOffset;
    protected float m_ProbeRadiusM;

    void ZZECandidateDefinition(
        string id,
        string category,
        string ceEvent,
        int spawnIndex,
        string group,
        vector markerPosition,
        float spawnAngle,
        vector probePosition,
        array<string> semanticAnchorClasses,
        string selectedGroupAnchor,
        vector groupAnchorOffset,
        float probeRadiusM)
    {
        m_Id = id;
        m_Category = category;
        m_CEEvent = ceEvent;
        m_SpawnIndex = spawnIndex;
        m_Group = group;
        m_MarkerPosition = markerPosition;
        m_SpawnAngle = spawnAngle;
        m_ProbePosition = probePosition;
        m_SelectedGroupAnchor = selectedGroupAnchor;
        m_GroupAnchorOffset = groupAnchorOffset;
        m_ProbeRadiusM = probeRadiusM;

        m_SemanticAnchorClasses = new array<string>;
        foreach (string className : semanticAnchorClasses)
        {
            m_SemanticAnchorClasses.Insert(className);
        }
    }

    string GetId()
    {
        return m_Id;
    }

    string GetCategory()
    {
        return m_Category;
    }

    string GetCEEvent()
    {
        return m_CEEvent;
    }

    int GetSpawnIndex()
    {
        return m_SpawnIndex;
    }

    string GetGroup()
    {
        return m_Group;
    }

    vector GetMarkerPosition()
    {
        return m_MarkerPosition;
    }

    float GetSpawnAngle()
    {
        return m_SpawnAngle;
    }

    vector GetProbePosition()
    {
        return m_ProbePosition;
    }

    array<string> GetSemanticAnchorClasses()
    {
        return m_SemanticAnchorClasses;
    }

    string GetSelectedGroupAnchor()
    {
        return m_SelectedGroupAnchor;
    }

    vector GetGroupAnchorOffset()
    {
        return m_GroupAnchorOffset;
    }

    float GetProbeRadiusM()
    {
        return m_ProbeRadiusM;
    }
}

