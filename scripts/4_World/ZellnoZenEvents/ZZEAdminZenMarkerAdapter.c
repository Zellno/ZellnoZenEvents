class ZZEAdminZenMarkerAdapter
{
    protected ref ZZEAdminAccessPolicy m_AccessPolicy;
    protected ref map<string, ref ZZEPlayerMarkerSet> m_OwnedMarkers;

    void ZZEAdminZenMarkerAdapter(ZZEAdminAccessPolicy accessPolicy)
    {
        m_AccessPolicy = accessPolicy;
        m_OwnedMarkers = new map<string, ref ZZEPlayerMarkerSet>;
    }

    bool AddOwnedMarker(
        PlayerBase player,
        ZZECandidateDefinition candidate,
        ZZEMarkerPresentation presentation,
        bool deferSync = false)
    {
        if (!candidate)
        {
            return false;
        }

        return AddOwnedMarkerAt(
            player,
            candidate.GetId(),
            candidate.GetMarkerPosition(),
            presentation,
            deferSync);
    }

    bool AddOwnedMarkerAt(
        PlayerBase player,
        string markerId,
        vector markerPosition,
        ZZEMarkerPresentation presentation,
        bool deferSync = false)
    {
        if (!CanOperate(player) || markerId == "" || !presentation || !presentation.IsValid())
        {
            return false;
        }

        ZZEPlayerMarkerSet playerMarkers = GetOrCreatePlayerMarkers(player);
        if (playerMarkers.Contains(markerId))
        {
            return true;
        }

        PluginZenMapMarkers mapPlugin = GetMapPlugin();
        if (!mapPlugin)
        {
            return false;
        }

        int iconIndex = GetZenMapConfig().FileToArrayIndex(presentation.GetIconPath());
        MapMarker marker = new MapMarker(
            markerPosition,
            presentation.GetText(),
            presentation.GetColor(),
            iconIndex);

        if (!mapPlugin.AddMarker(player, marker, !deferSync))
        {
            return false;
        }

        return playerMarkers.Add(markerId, marker);
    }

    bool RemoveOwnedMarker(PlayerBase player, string candidateId)
    {
        if (!CanAddressPlayer(player) || candidateId == "")
        {
            return false;
        }

        ZZEPlayerMarkerSet playerMarkers = GetPlayerMarkers(player);
        if (!playerMarkers || !playerMarkers.Contains(candidateId))
        {
            return true;
        }

        PluginZenMapMarkers mapPlugin = GetMapPlugin();
        if (!mapPlugin)
        {
            return false;
        }

        MapMarker marker = playerMarkers.Get(candidateId);
        if (!mapPlugin.RemoveMarker(player, marker))
        {
            return false;
        }

        playerMarkers.Remove(candidateId);
        return true;
    }

    bool SyncOwnedMarkers(PlayerBase player)
    {
        if (!CanAddressPlayer(player))
        {
            return false;
        }

        PluginZenMapMarkers mapPlugin = GetMapPlugin();
        if (!mapPlugin)
        {
            return false;
        }

        mapPlugin.SyncMarkers(player);
        return true;
    }

    int CountOwnedMarkers(PlayerBase player)
    {
        ZZEPlayerMarkerSet playerMarkers = GetPlayerMarkers(player);
        if (!playerMarkers)
        {
            return 0;
        }

        return playerMarkers.Count();
    }

    bool RemoveAllOwnedMarkers(PlayerBase player)
    {
        if (!CanAddressPlayer(player))
        {
            return false;
        }

        ZZEPlayerMarkerSet playerMarkers = GetPlayerMarkers(player);
        if (!playerMarkers || playerMarkers.Count() == 0)
        {
            return true;
        }

        array<string> candidateIds = new array<string>;
        playerMarkers.GetCandidateIds(candidateIds);

        bool success = true;
        foreach (string candidateId : candidateIds)
        {
            if (!RemoveOwnedMarker(player, candidateId))
            {
                success = false;
            }
        }

        if (!SyncOwnedMarkers(player))
        {
            success = false;
        }

        if (success)
        {
            ForgetPlayer(player);
        }

        return success;
    }

    void ForgetPlayer(PlayerBase player)
    {
        if (!player || !player.GetIdentity())
        {
            return;
        }

        m_OwnedMarkers.Remove(player.GetIdentity().GetPlainId());
    }

    protected bool CanOperate(PlayerBase player)
    {
        if (!GetGame()) return false;
        if (!GetGame().IsDedicatedServer()) return false;
        if (!m_AccessPolicy) return false;
        return m_AccessPolicy.IsAuthorized(player);
    }

    protected bool CanAddressPlayer(PlayerBase player)
    {
        if (!GetGame()) return false;
        if (!GetGame().IsDedicatedServer()) return false;
        return player && player.GetIdentity();
    }

    protected PluginZenMapMarkers GetMapPlugin()
    {
        return PluginZenMapMarkers.Cast(GetPlugin(PluginZenMapMarkers));
    }

    protected ZZEPlayerMarkerSet GetPlayerMarkers(PlayerBase player)
    {
        if (!player || !player.GetIdentity())
        {
            return null;
        }

        return m_OwnedMarkers.Get(player.GetIdentity().GetPlainId());
    }

    protected ZZEPlayerMarkerSet GetOrCreatePlayerMarkers(PlayerBase player)
    {
        string steamId64 = player.GetIdentity().GetPlainId();
        ZZEPlayerMarkerSet playerMarkers = m_OwnedMarkers.Get(steamId64);
        if (!playerMarkers)
        {
            playerMarkers = new ZZEPlayerMarkerSet();
            m_OwnedMarkers.Insert(steamId64, playerMarkers);
        }

        return playerMarkers;
    }
}
