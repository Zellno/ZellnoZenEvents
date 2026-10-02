class ZZETransitionMarkerCoordinator
{
    protected ref ZZEAdminZenMarkerAdapter m_Adapter;
    protected ref map<string, ref ZZECandidateDefinition> m_CandidatesById;
    protected bool m_Initialized;

    void ZZETransitionMarkerCoordinator(ZZEAdminZenMarkerAdapter adapter)
    {
        m_Adapter = adapter;
        m_CandidatesById = new map<string, ref ZZECandidateDefinition>;
        m_Initialized = false;
    }

    bool Initialize(ZZEManifestCatalog catalog, out string errorMessage)
    {
        errorMessage = "";
        m_CandidatesById.Clear();
        m_Initialized = false;

        if (!m_Adapter || !catalog || !catalog.Validate(errorMessage))
        {
            if (errorMessage == "")
            {
                errorMessage = "Missing marker adapter or manifest catalog";
            }
            return false;
        }

        array<ref ZZECandidateDefinition> candidates = catalog.GetCandidates();
        if (!candidates)
        {
            errorMessage = "Missing marker coordinator candidates";
            return false;
        }

        foreach (ZZECandidateDefinition candidate : candidates)
        {
            if (!candidate || m_CandidatesById.Contains(candidate.GetId()))
            {
                errorMessage = "Invalid or duplicate candidate in marker coordinator";
                m_CandidatesById.Clear();
                return false;
            }

            m_CandidatesById.Insert(candidate.GetId(), candidate);
        }

        if (m_CandidatesById.Count() != ZZEConstants.EXPECTED_CANDIDATE_TOTAL)
        {
            errorMessage = "Unexpected marker coordinator candidate count";
            m_CandidatesById.Clear();
            return false;
        }

        m_Initialized = true;
        return true;
    }

    bool ApplyTransition(
        PlayerBase player,
        ZZEStateTransition transition,
        bool deferAddSync = false)
    {
        if (!m_Initialized || !transition || !transition.WasObservationAccepted())
        {
            return false;
        }

        if (!transition.BecameActive() && !transition.BecameAbsent())
        {
            return true;
        }

        ZZECandidateDefinition candidate = m_CandidatesById.Get(transition.GetCandidateId());
        if (!candidate)
        {
            return false;
        }

        if (transition.BecameActive())
        {
            ZZEMarkerPresentation presentation = ZZEMarkerPresentationCatalog.ForCategory(
                candidate.GetCategory());
            if (!presentation)
            {
                return false;
            }

            return m_Adapter.AddOwnedMarker(
                player,
                candidate,
                presentation,
                deferAddSync);
        }

        return m_Adapter.RemoveOwnedMarker(player, candidate.GetId());
    }

    bool ApplyBatch(PlayerBase player, ZZEBatchResult batch)
    {
        if (!m_Initialized || !batch || batch.GetStatus() != ZZEBatchStatus.OK)
        {
            return false;
        }

        bool success = true;
        bool hasActivation = false;

        array<ref ZZEStateTransition> transitions = batch.GetTransitions();
        if (!transitions)
        {
            return false;
        }

        foreach (ZZEStateTransition transition : transitions)
        {
            if (transition && transition.BecameActive())
            {
                hasActivation = true;
            }

            if (!ApplyTransition(player, transition, true))
            {
                success = false;
            }
        }

        if (hasActivation && !m_Adapter.SyncOwnedMarkers(player))
        {
            success = false;
        }

        return success;
    }

    bool RestoreActiveMarkers(
        PlayerBase player,
        array<string> activeCandidateIds,
        bool deferSync = false)
    {
        if (!m_Initialized || !activeCandidateIds)
        {
            return false;
        }

        bool success = true;
        bool addedAny = false;

        foreach (string candidateId : activeCandidateIds)
        {
            ZZECandidateDefinition candidate = m_CandidatesById.Get(candidateId);
            if (!candidate)
            {
                success = false;
                continue;
            }

            ZZEMarkerPresentation presentation = ZZEMarkerPresentationCatalog.ForCategory(
                candidate.GetCategory());
            if (!presentation)
            {
                success = false;
                continue;
            }

            if (m_Adapter.AddOwnedMarker(player, candidate, presentation, true))
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

    bool IsInitialized()
    {
        return m_Initialized;
    }
}
