class ZZEAdminAccessPolicy
{
    protected ref ZZEConfiguration m_Configuration;
    protected ref array<string> m_AllowedSteamIds;

    void ZZEAdminAccessPolicy()
    {
        m_AllowedSteamIds = new array<string>;
    }

    static ZZEAdminAccessPolicy CreateFromConfiguration(
        ZZEConfiguration configuration)
    {
        ZZEAdminAccessPolicy policy = new ZZEAdminAccessPolicy();

        if (!configuration)
        {
            return policy;
        }

        policy.m_Configuration = configuration;

        if (configuration.AdministratorSteamIds)
        {
            foreach (string steamId64 : configuration.AdministratorSteamIds)
            {
                policy.Allow(steamId64);
            }
        }

        return policy;
    }

    bool Allow(string steamId64)
    {
        if (steamId64 == "" || m_AllowedSteamIds.Find(steamId64) != -1)
        {
            return false;
        }

        m_AllowedSteamIds.Insert(steamId64);
        return true;
    }

    bool IsAuthorized(PlayerBase player)
    {
        if (!m_Configuration || !m_Configuration.Enabled || !player || !player.GetIdentity())
        {
            return false;
        }

        if (m_Configuration.VisibilityMode != "AdministratorsOnly" && m_Configuration.VisibilityMode != "Everyone")
        {
            return false;
        }

        string steamId64 = player.GetIdentity().GetPlainId();
        if (IsAdministrator(player))
        {
            return m_Configuration.GetAdministratorMarkersEnabled(steamId64);
        }

        return m_Configuration.GlobalMarkersEnabled;
    }

    bool IsAdministrator(PlayerBase player)
    {
        if (!m_Configuration || !m_Configuration.Enabled || !player || !player.GetIdentity())
        {
            return false;
        }

        return m_AllowedSteamIds.Find(player.GetIdentity().GetPlainId()) != -1;
    }

    bool AreAdministratorMarkersEnabled(PlayerBase player)
    {
        if (!IsAdministrator(player))
        {
            return false;
        }

        return m_Configuration.GetAdministratorMarkersEnabled(player.GetIdentity().GetPlainId());
    }

    bool SetAdministratorMarkersEnabled(PlayerBase player, bool enabled)
    {
        if (!IsAdministrator(player))
        {
            return false;
        }

        return m_Configuration.SetAdministratorMarkersEnabled(player.GetIdentity().GetPlainId(), enabled);
    }

    bool AreGlobalMarkersEnabled()
    {
        return m_Configuration && m_Configuration.Enabled && m_Configuration.GlobalMarkersEnabled;
    }

    bool SetGlobalMarkersEnabled(bool enabled)
    {
        if (!m_Configuration || !m_Configuration.Enabled)
        {
            return false;
        }

        m_Configuration.SetGlobalMarkersEnabled(enabled);
        return true;
    }
}
