modded class MissionServer
{
    protected ref ZZERuntimeService m_ZellnoZenEventsRuntime;

    override void OnInit()
    {
        super.OnInit();

        m_ZellnoZenEventsRuntime = new ZZERuntimeService();
        string initializationError;
        if (m_ZellnoZenEventsRuntime.Initialize(initializationError))
        {
            m_ZellnoZenEventsRuntime.Start();
        }
    }

    override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
    {
        super.InvokeOnConnect(player, identity);

        if (m_ZellnoZenEventsRuntime)
        {
            m_ZellnoZenEventsRuntime.HandleConnect(player);
        }
    }

    override void InvokeOnDisconnect(PlayerBase player)
    {
        super.InvokeOnDisconnect(player);

        if (m_ZellnoZenEventsRuntime)
        {
            m_ZellnoZenEventsRuntime.HandleDisconnect(player);
        }
    }

    override void OnMissionFinish()
    {
        if (m_ZellnoZenEventsRuntime)
        {
            m_ZellnoZenEventsRuntime.Stop();
        }

        super.OnMissionFinish();
    }

    ZZERuntimeService GetZellnoZenEventsRuntime()
    {
        return m_ZellnoZenEventsRuntime;
    }
}
