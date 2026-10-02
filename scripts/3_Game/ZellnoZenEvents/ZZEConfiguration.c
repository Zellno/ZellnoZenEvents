class ZZEAdministratorPreference
{
    string SteamId64;
    bool MarkersEnabled = true;
}

class ZZEConfiguration
{
    int ConfigVersion = 2;
    bool Enabled = true;
    string VisibilityMode = "AdministratorsOnly";
    bool GlobalMarkersEnabled = false;
    ref array<string> AdministratorSteamIds;
    ref array<ref ZZEAdministratorPreference> AdministratorPreferences;

    void ZZEConfiguration()
    {
        AdministratorSteamIds = new array<string>;
        AdministratorPreferences = new array<ref ZZEAdministratorPreference>;
    }

    static string GetDirectory()
    {
        return "$profile:ZellnoZenEvents";
    }

    static string GetPath()
    {
        return GetDirectory() + "/ZellnoZenEventsConfig.json";
    }

    static ZZEConfiguration LoadOrCreate()
    {
        ZZEConfiguration configuration = new ZZEConfiguration();
        string directory = GetDirectory();
        string path = GetPath();

        if (!FileExist(directory))
        {
            MakeDirectory(directory);
        }

        if (!FileExist(path))
        {
            JsonFileLoader<ZZEConfiguration>.JsonSaveFile(path, configuration);
            return configuration;
        }

        JsonFileLoader<ZZEConfiguration>.JsonLoadFile(path, configuration);
        configuration.Normalize();
        return configuration;
    }

    void Save()
    {
        Normalize();
        JsonFileLoader<ZZEConfiguration>.JsonSaveFile(GetPath(), this);
    }

    void Normalize()
    {
        if (!AdministratorSteamIds)
        {
            AdministratorSteamIds = new array<string>;
        }


        if (!AdministratorPreferences)
        {
            AdministratorPreferences = new array<ref ZZEAdministratorPreference>;
        }

        ConfigVersion = 2;
    }

    bool GetAdministratorMarkersEnabled(string steamId64)
    {
        if (steamId64 == "" || !AdministratorPreferences)
        {
            return true;
        }

        foreach (ZZEAdministratorPreference preference : AdministratorPreferences)
        {
            if (preference && preference.SteamId64 == steamId64)
            {
                return preference.MarkersEnabled;
            }
        }

        return true;
    }

    bool SetAdministratorMarkersEnabled(string steamId64, bool enabled)
    {
        if (steamId64 == "")
        {
            return false;
        }

        Normalize();
        foreach (ZZEAdministratorPreference preference : AdministratorPreferences)
        {
            if (preference && preference.SteamId64 == steamId64)
            {
                preference.MarkersEnabled = enabled;
                Save();
                return true;
            }
        }

        ZZEAdministratorPreference created = new ZZEAdministratorPreference();
        created.SteamId64 = steamId64;
        created.MarkersEnabled = enabled;
        AdministratorPreferences.Insert(created);
        Save();
        return true;
    }

    void SetGlobalMarkersEnabled(bool enabled)
    {
        GlobalMarkersEnabled = enabled;
        Save();
    }
}
