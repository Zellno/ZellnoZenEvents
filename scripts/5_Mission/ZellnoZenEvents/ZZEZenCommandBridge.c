modded class ZenAdminCommandHandler
{
    override bool HandleRawChatCommand(PlayerBase player, string msg)
    {
        string normalized = msg.Trim();
        string lower = normalized;
        lower.ToLower();

        bool isZellnoZenEventsCommand = lower == "!zze";
        if (!isZellnoZenEventsCommand && lower.Length() > 5)
        {
            isZellnoZenEventsCommand = lower.IndexOf("!zze ") == 0;
        }

        if (!isZellnoZenEventsCommand || !GetGame() || !GetGame().IsServer())
        {
            return super.HandleRawChatCommand(player, msg);
        }

        MissionServer mission = MissionServer.Cast(GetGame().GetMission());
        if (!mission || !mission.GetZellnoZenEventsRuntime())
        {
            return true;
        }

        mission.GetZellnoZenEventsRuntime().HandleChatCommand(player, lower);
        return true;
    }
}
