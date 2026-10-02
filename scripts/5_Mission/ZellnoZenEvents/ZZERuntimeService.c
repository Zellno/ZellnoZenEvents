class ZZERuntimeService
{
    protected ref ZZEConfiguration m_Configuration;
    protected ref ZZEAdminAccessPolicy m_AccessPolicy;
    protected ref ZZEAdminZenMarkerAdapter m_MarkerAdapter;
    protected ref ZZEManualBatchController m_BatchController;
    protected ref ZZETransitionMarkerCoordinator m_TransitionCoordinator;
    protected ref ZZEStaticAreaMarkerCoordinator m_StaticCoordinator;
    protected ref ZZEManifestCatalog m_Catalog;

    protected bool m_Initialized;
    protected bool m_Running;
    protected bool m_TickPending;
    protected bool m_AdminSessionActive;
    protected int m_ProcessedTicks;
    protected int m_CompletedCycles;

    void ZZERuntimeService()
    {
        m_Configuration = ZZEConfiguration.LoadOrCreate();
        m_AccessPolicy = ZZEAdminAccessPolicy.CreateFromConfiguration(m_Configuration);
        m_MarkerAdapter = new ZZEAdminZenMarkerAdapter(m_AccessPolicy);
        m_BatchController = new ZZEManualBatchController();
        m_TransitionCoordinator = new ZZETransitionMarkerCoordinator(m_MarkerAdapter);
        m_StaticCoordinator = new ZZEStaticAreaMarkerCoordinator(m_MarkerAdapter);
        m_Catalog = ZZEManifestCatalog.CreateGenerated();
    }

    bool Initialize(out string errorMessage)
    {
        errorMessage = "";
        m_Initialized = false;
        m_Running = false;
        m_TickPending = false;
        m_AdminSessionActive = false;
        m_ProcessedTicks = 0;
        m_CompletedCycles = 0;

        if (!GetGame() || !GetGame().IsDedicatedServer())
        {
            errorMessage = "Runtime service requires a dedicated server";
            return false;
        }

        if (!m_Catalog.Validate(errorMessage))
        {
            return false;
        }

        if (!m_BatchController.Initialize(errorMessage))
        {
            return false;
        }

        if (!m_TransitionCoordinator.Initialize(m_Catalog, errorMessage))
        {
            return false;
        }

        if (!m_StaticCoordinator.Initialize(m_Catalog, errorMessage))
        {
            return false;
        }

        m_Initialized = true;
        return true;
    }

    bool Start()
    {
        if (!m_Initialized || m_Running)
        {
            return false;
        }

        m_Running = true;
        ScheduleNext(ZZERuntimeSchedule.INITIAL_DELAY_MS);
        return true;
    }

    void Stop()
    {
        m_Running = false;
        m_TickPending = false;
        m_AdminSessionActive = false;
    }

    bool HandleConnect(PlayerBase player)
    {
        if (!m_Initialized || !m_AccessPolicy.IsAuthorized(player))
        {
            return false;
        }

        m_MarkerAdapter.ForgetPlayer(player);

        if (!m_AdminSessionActive)
        {
            string resetError;
            if (!m_BatchController.Initialize(resetError))
            {
                return false;
            }

            m_AdminSessionActive = true;
        }

        bool success = m_StaticCoordinator.AddPrivateMarkers(player, true);

        array<string> activeCandidateIds = new array<string>;
        m_BatchController.GetRegistry().GetActiveCandidateIds(activeCandidateIds);
        if (!m_TransitionCoordinator.RestoreActiveMarkers(player, activeCandidateIds, true))
        {
            success = false;
        }

        if (!m_MarkerAdapter.SyncOwnedMarkers(player))
        {
            success = false;
        }

        return success;
    }

    void HandleDisconnect(PlayerBase player)
    {
        if (!m_Initialized || !player || !player.GetIdentity())
        {
            return;
        }

        m_MarkerAdapter.ForgetPlayer(player);
        m_AdminSessionActive = false;

        string resetError;
        m_BatchController.Initialize(resetError);
    }

    bool HandleChatCommand(PlayerBase player, string command)
    {
        if (!m_Initialized || !player || !player.GetIdentity())
        {
            return true;
        }

        if (!m_AccessPolicy.IsAdministrator(player))
        {
            SendPrivateMessage(player, "Comando indisponivel.");
            return true;
        }

        if (command == "!zze")
        {
            SendCommandHelp(player);
            return true;
        }

        if (command == "!zze admin on")
        {
            return HandleAdministratorToggle(player, "on");
        }

        if (command == "!zze admin off")
        {
            return HandleAdministratorToggle(player, "off");
        }

        if (command == "!zze global on")
        {
            return HandleGlobalToggle(player, "on");
        }

        if (command == "!zze global off")
        {
            return HandleGlobalToggle(player, "off");
        }

        if (command == "!zze status admin")
        {
            return HandleStatus(player, "admin");
        }

        if (command == "!zze status global")
        {
            return HandleStatus(player, "global");
        }

        SendPrivateMessage(player, "Comando invalido. Digite !zze para consultar a ajuda.");
        return true;
    }

    protected bool HandleAdministratorToggle(PlayerBase player, string requestedState)
    {
        bool enabled;
        if (!ParseToggle(requestedState, enabled))
        {
            SendPrivateMessage(player, "Uso: !zze admin on|off");
            return true;
        }

        if (!m_AccessPolicy.SetAdministratorMarkersEnabled(player, enabled))
        {
            SendPrivateMessage(player, "Nao foi possivel salvar sua preferencia.");
            return true;
        }

        RefreshPlayerVisibility(player);
        if (enabled)
        {
            SendPrivateMessage(player, "Marcadores administrativos ativados somente para voce.");
        }
        else
        {
            SendPrivateMessage(player, "Marcadores administrativos desativados somente para voce.");
        }

        return true;
    }

    protected bool HandleGlobalToggle(PlayerBase player, string requestedState)
    {
        bool enabled;
        if (!ParseToggle(requestedState, enabled))
        {
            SendPrivateMessage(player, "Uso: !zze global on|off");
            return true;
        }

        if (!m_AccessPolicy.SetGlobalMarkersEnabled(enabled))
        {
            SendPrivateMessage(player, "Nao foi possivel salvar o estado global.");
            return true;
        }

        RefreshAllOnlinePlayers();
        if (enabled)
        {
            SendPrivateMessage(player, "Marcadores globais ativados para todos os jogadores.");
        }
        else
        {
            SendPrivateMessage(player, "Marcadores globais desativados para os jogadores.");
        }

        return true;
    }

    protected bool HandleStatus(PlayerBase player, string scope)
    {
        if (scope == "admin")
        {
            if (m_AccessPolicy.AreAdministratorMarkersEnabled(player))
            {
                SendPrivateMessage(player, "Estado administrativo pessoal: ON.");
            }
            else
            {
                SendPrivateMessage(player, "Estado administrativo pessoal: OFF.");
            }
            return true;
        }

        if (scope == "global")
        {
            if (m_AccessPolicy.AreGlobalMarkersEnabled())
            {
                SendPrivateMessage(player, "Estado global: ON.");
            }
            else
            {
                SendPrivateMessage(player, "Estado global: OFF.");
            }
            return true;
        }

        SendPrivateMessage(player, "Uso: !zze status admin|global");
        return true;
    }

    protected bool ParseToggle(string value, out bool enabled)
    {
        enabled = false;
        if (value == "on")
        {
            enabled = true;
            return true;
        }

        return value == "off";
    }

    protected void SendCommandHelp(PlayerBase player)
    {
        SendPrivateMessage(player, "Comandos: !zze admin on|off, !zze global on|off, !zze status admin|global");
    }

    protected void SendPrivateMessage(PlayerBase player, string message)
    {
        if (player)
        {
            ZenFunctions.SendPlayerMessage(player, "[ZZE] " + message);
        }
    }

    protected void RefreshAllOnlinePlayers()
    {
        array<Man> onlinePlayers = new array<Man>;
        GetGame().GetPlayers(onlinePlayers);
        foreach (Man man : onlinePlayers)
        {
            RefreshPlayerVisibility(PlayerBase.Cast(man));
        }
    }

    protected bool RefreshPlayerVisibility(PlayerBase player)
    {
        if (!player || !player.GetIdentity())
        {
            return false;
        }

        if (!m_AccessPolicy.IsAuthorized(player))
        {
            return m_MarkerAdapter.RemoveAllOwnedMarkers(player);
        }

        if (!m_AdminSessionActive)
        {
            string resetError;
            if (!m_BatchController.Initialize(resetError))
            {
                return false;
            }
            m_AdminSessionActive = true;
        }

        bool success = m_StaticCoordinator.AddPrivateMarkers(player, true);
        array<string> activeCandidateIds = new array<string>;
        m_BatchController.GetRegistry().GetActiveCandidateIds(activeCandidateIds);
        if (!m_TransitionCoordinator.RestoreActiveMarkers(player, activeCandidateIds, true))
        {
            success = false;
        }

        if (!m_MarkerAdapter.SyncOwnedMarkers(player))
        {
            success = false;
        }

        return success;
    }

    protected void RunScheduledTick()
    {
        m_TickPending = false;
        if (!m_Running)
        {
            return;
        }

        array<PlayerBase> administrators = new array<PlayerBase>;
        CollectOnlineAdministrators(administrators);

        if (m_AdminSessionActive && administrators.Count() > 0)
        {
            ZZEBatchResult batch = m_BatchController.ProcessNextBatch(
                ZZERuntimeSchedule.CANDIDATES_PER_TICK);

            if (batch && batch.GetStatus() == ZZEBatchStatus.OK)
            {
                foreach (PlayerBase administrator : administrators)
                {
                    m_TransitionCoordinator.ApplyBatch(administrator, batch);
                }

                m_ProcessedTicks++;
                if (batch.CompletedCycle())
                {
                    m_CompletedCycles++;
                }
            }
        }

        ScheduleNext(ZZERuntimeSchedule.TICK_INTERVAL_MS);
    }

    protected void ScheduleNext(int delayMilliseconds)
    {
        if (!m_Running || m_TickPending || !GetGame())
        {
            return;
        }

        m_TickPending = true;
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(
            RunScheduledTick,
            delayMilliseconds,
            false);
    }

    protected void CollectOnlineAdministrators(array<PlayerBase> output)
    {
        if (!output)
        {
            return;
        }

        output.Clear();

        array<Man> onlinePlayers = new array<Man>;
        GetGame().GetPlayers(onlinePlayers);

        foreach (Man man : onlinePlayers)
        {
            PlayerBase player = PlayerBase.Cast(man);
            if (player && m_AccessPolicy.IsAuthorized(player))
            {
                output.Insert(player);
            }
        }
    }

    bool IsInitialized()
    {
        return m_Initialized;
    }

    bool IsRunning()
    {
        return m_Running;
    }

    int GetProcessedTicks()
    {
        return m_ProcessedTicks;
    }

    int GetCompletedCycles()
    {
        return m_CompletedCycles;
    }
}
