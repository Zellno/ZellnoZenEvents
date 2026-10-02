class ZZEStaticAreaMarkerCoordinator
{
    protected ref ZZEAdminZenMarkerAdapter m_Adapter;
    protected ref array<ref ZZEStaticAreaCluster> m_Clusters;
    protected bool m_Initialized;

    void ZZEStaticAreaMarkerCoordinator(ZZEAdminZenMarkerAdapter adapter)
    {
        m_Adapter = adapter;
        m_Clusters = new array<ref ZZEStaticAreaCluster>;
        m_Initialized = false;
    }

    bool Initialize(ZZEManifestCatalog catalog, out string errorMessage)
    {
        m_Initialized = false;

        if (!m_Adapter)
        {
            errorMessage = "Missing marker adapter";
            return false;
        }

        if (!ZZEStaticAreaConsolidator.Build(catalog, m_Clusters, errorMessage))
        {
            return false;
        }

        if (m_Clusters.Count() != 2)
        {
            errorMessage = "Unexpected static contaminated cluster count";
            m_Clusters.Clear();
            return false;
        }

        m_Initialized = true;
        return true;
    }

    bool AddPrivateMarkers(PlayerBase player, bool deferSync = false)
    {
        if (!m_Initialized)
        {
            return false;
        }

        bool success = true;
        bool addedAny = false;

        foreach (ZZEStaticAreaCluster cluster : m_Clusters)
        {
            ZZEMarkerPresentation presentation = new ZZEMarkerPresentation(
                "Permanent Contaminated Area - " + cluster.GetDisplayName(),
                -15165928,
                "ZenMap/data/icons/warn.paa");

            if (m_Adapter.AddOwnedMarkerAt(
                player,
                cluster.GetId(),
                cluster.GetMarkerPosition(),
                presentation,
                true))
            {
                addedAny = true;
            }
            else
            {
                success = false;
            }
        }

        if (addedAny && !deferSync && !m_Adapter.SyncOwnedMarkers(player))
        {
            success = false;
        }

        return success;
    }

    bool RemovePrivateMarkers(PlayerBase player)
    {
        if (!m_Initialized)
        {
            return false;
        }

        bool success = true;
        foreach (ZZEStaticAreaCluster cluster : m_Clusters)
        {
            if (!m_Adapter.RemoveOwnedMarker(player, cluster.GetId()))
            {
                success = false;
            }
        }

        return success;
    }

    array<ref ZZEStaticAreaCluster> GetClusters()
    {
        return m_Clusters;
    }
}
