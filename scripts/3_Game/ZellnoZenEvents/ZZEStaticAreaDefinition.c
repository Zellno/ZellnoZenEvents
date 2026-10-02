class ZZEStaticAreaDefinition
{
    protected string m_Name;
    protected string m_Type;
    protected string m_TriggerType;
    protected vector m_Position;
    protected float m_RadiusM;
    protected float m_PositiveHeightM;
    protected float m_NegativeHeightM;

    void ZZEStaticAreaDefinition(
        string name,
        string areaType,
        string triggerType,
        vector position,
        float radiusM,
        float positiveHeightM,
        float negativeHeightM)
    {
        m_Name = name;
        m_Type = areaType;
        m_TriggerType = triggerType;
        m_Position = position;
        m_RadiusM = radiusM;
        m_PositiveHeightM = positiveHeightM;
        m_NegativeHeightM = negativeHeightM;
    }

    string GetName()
    {
        return m_Name;
    }

    string GetAreaType()
    {
        return m_Type;
    }

    string GetTriggerType()
    {
        return m_TriggerType;
    }

    vector GetPosition()
    {
        return m_Position;
    }

    float GetRadiusM()
    {
        return m_RadiusM;
    }

    float GetPositiveHeightM()
    {
        return m_PositiveHeightM;
    }

    float GetNegativeHeightM()
    {
        return m_NegativeHeightM;
    }
}

