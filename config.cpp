class CfgPatches
{
    class ZellnoZenEvents
    {
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] =
        {
            "DZ_Data",
            "DZ_Scripts",
            "ZenModCore",
            "ZenMap"
        };
    };
};

class CfgMods
{
    class ZellnoZenEvents
    {
        author = "Zellno / Noob Open Source";
        type = "mod";
        dependencies[] =
        {
            "Game",
            "World",
            "Mission"
        };

        class defs
        {
            class gameScriptModule
            {
                value = "";
                files[] =
                {
                    "ZellnoZenEvents/scripts/3_Game"
                };
            };

            class worldScriptModule
            {
                value = "";
                files[] =
                {
                    "ZellnoZenEvents/scripts/4_World"
                };
            };

            class missionScriptModule
            {
                value = "";
                files[] =
                {
                    "ZellnoZenEvents/scripts/5_Mission"
                };
            };
        };
    };
};
