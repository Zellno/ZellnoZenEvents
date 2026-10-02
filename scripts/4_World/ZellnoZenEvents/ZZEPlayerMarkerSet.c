class ZZEPlayerMarkerSet
{
    protected ref map<string, ref MapMarker> m_ByCandidateId;

    void ZZEPlayerMarkerSet()
    {
        m_ByCandidateId = new map<string, ref MapMarker>;
    }

    bool Contains(string candidateId)
    {
        return m_ByCandidateId.Contains(candidateId);
    }

    MapMarker Get(string candidateId)
    {
        return m_ByCandidateId.Get(candidateId);
    }

    bool Add(string candidateId, MapMarker marker)
    {
        if (candidateId == "" || !marker || m_ByCandidateId.Contains(candidateId))
        {
            return false;
        }

        m_ByCandidateId.Insert(candidateId, marker);
        return true;
    }

    void Remove(string candidateId)
    {
        m_ByCandidateId.Remove(candidateId);
    }

    int Count()
    {
        return m_ByCandidateId.Count();
    }

    void GetCandidateIds(array<string> output)
    {
        if (!output)
        {
            return;
        }

        output.Clear();
        foreach (string candidateId, MapMarker marker : m_ByCandidateId)
        {
            output.Insert(candidateId);
        }
    }
}
