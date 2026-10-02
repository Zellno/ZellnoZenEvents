class ZZEStaticAreaCluster
{
    protected string m_Id;
    protected string m_DisplayName;
    protected vector m_MarkerPosition;
    protected int m_VolumeCount;

    void ZZEStaticAreaCluster(
        string id,
        string displayName,
        vector markerPosition,
        int volumeCount)
    {
        m_Id = id;
        m_DisplayName = displayName;
        m_MarkerPosition = markerPosition;
        m_VolumeCount = volumeCount;
    }

    string GetId()
    {
        return m_Id;
    }

    string GetDisplayName()
    {
        return m_DisplayName;
    }

    vector GetMarkerPosition()
    {
        return m_MarkerPosition;
    }

    int GetVolumeCount()
    {
        return m_VolumeCount;
    }
}
