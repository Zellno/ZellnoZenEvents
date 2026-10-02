class ZZEManifestCatalog
{
    protected ref array<ref ZZECandidateDefinition> m_Candidates;
    protected ref array<ref ZZEStaticAreaDefinition> m_StaticContaminatedVolumes;

    void ZZEManifestCatalog()
    {
        m_Candidates = new array<ref ZZECandidateDefinition>;
        m_StaticContaminatedVolumes = new array<ref ZZEStaticAreaDefinition>;
    }

    static ZZEManifestCatalog CreateGenerated()
    {
        ZZEManifestCatalog catalog = new ZZEManifestCatalog();
        ZZEGeneratedManifestData.InsertCandidates(catalog.m_Candidates);
        ZZEGeneratedManifestData.InsertStaticContaminatedVolumes(catalog.m_StaticContaminatedVolumes);
        return catalog;
    }

    array<ref ZZECandidateDefinition> GetCandidates()
    {
        return m_Candidates;
    }

    array<ref ZZEStaticAreaDefinition> GetStaticContaminatedVolumes()
    {
        return m_StaticContaminatedVolumes;
    }

    bool Validate(out string errorMessage)
    {
        errorMessage = "";

        if (!m_Candidates || m_Candidates.Count() != ZZEConstants.EXPECTED_CANDIDATE_TOTAL)
        {
            errorMessage = "Unexpected candidate count";
            return false;
        }

        if (!m_StaticContaminatedVolumes || m_StaticContaminatedVolumes.Count() != ZZEConstants.EXPECTED_STATIC_VOLUME_TOTAL)
        {
            errorMessage = "Unexpected static contaminated volume count";
            return false;
        }

        ref array<string> candidateIds = new array<string>;
        int helicrashCount;
        int militaryConvoyCount;
        int policeCarCount;
        int policeSituationCount;
        int trainCount;
        int airplaneCrateCount;
        int dynamicContaminatedAreaCount;

        foreach (ZZECandidateDefinition candidate : m_Candidates)
        {
            if (!candidate)
            {
                errorMessage = "Null candidate";
                return false;
            }

            if (candidate.GetId() == "" || candidateIds.Find(candidate.GetId()) != -1)
            {
                errorMessage = "Invalid or duplicate candidate id: " + candidate.GetId();
                return false;
            }

            candidateIds.Insert(candidate.GetId());

            if (!ZZECategory.IsSupported(candidate.GetCategory()))
            {
                errorMessage = "Unsupported category: " + candidate.GetCategory();
                return false;
            }

            if (candidate.GetCEEvent() == "" || candidate.GetSpawnIndex() <= 0)
            {
                errorMessage = "Invalid CE metadata: " + candidate.GetId();
                return false;
            }

            if (!candidate.GetSemanticAnchorClasses() || candidate.GetSemanticAnchorClasses().Count() == 0)
            {
                errorMessage = "Missing semantic anchor class: " + candidate.GetId();
                return false;
            }

            if (candidate.GetProbeRadiusM() <= 0)
            {
                errorMessage = "Invalid probe radius: " + candidate.GetId();
                return false;
            }

            if (candidate.GetCategory() == ZZECategory.HELICRASH)
                helicrashCount++;
            else if (candidate.GetCategory() == ZZECategory.MILITARY_CONVOY)
                militaryConvoyCount++;
            else if (candidate.GetCategory() == ZZECategory.POLICE_CAR)
                policeCarCount++;
            else if (candidate.GetCategory() == ZZECategory.POLICE_SITUATION)
                policeSituationCount++;
            else if (candidate.GetCategory() == ZZECategory.TRAIN)
                trainCount++;
            else if (candidate.GetCategory() == ZZECategory.AIRPLANE_CRATE)
                airplaneCrateCount++;
            else if (candidate.GetCategory() == ZZECategory.DYNAMIC_CONTAMINATED_AREA)
                dynamicContaminatedAreaCount++;
        }

        if (helicrashCount != ZZEConstants.EXPECTED_HELICRASH_TOTAL) { errorMessage = "Unexpected helicrash count"; return false; }
        if (militaryConvoyCount != ZZEConstants.EXPECTED_MILITARY_CONVOY_TOTAL) { errorMessage = "Unexpected military convoy count"; return false; }
        if (policeCarCount != ZZEConstants.EXPECTED_POLICE_CAR_TOTAL) { errorMessage = "Unexpected police car count"; return false; }
        if (policeSituationCount != ZZEConstants.EXPECTED_POLICE_SITUATION_TOTAL) { errorMessage = "Unexpected police situation count"; return false; }
        if (trainCount != ZZEConstants.EXPECTED_TRAIN_TOTAL) { errorMessage = "Unexpected train count"; return false; }
        if (airplaneCrateCount != ZZEConstants.EXPECTED_AIRPLANE_CRATE_TOTAL) { errorMessage = "Unexpected airplane crate count"; return false; }
        if (dynamicContaminatedAreaCount != ZZEConstants.EXPECTED_DYNAMIC_CONTAMINATED_AREA_TOTAL) { errorMessage = "Unexpected dynamic contaminated area count"; return false; }

        ref array<string> staticAreaNames = new array<string>;
        foreach (ZZEStaticAreaDefinition area : m_StaticContaminatedVolumes)
        {
            if (!area || area.GetName() == "" || staticAreaNames.Find(area.GetName()) != -1)
            {
                errorMessage = "Invalid or duplicate static contaminated area";
                return false;
            }

            if (area.GetAreaType() != "ContaminatedArea_Static") { errorMessage = "Invalid static area type: " + area.GetName(); return false; }
            if (area.GetTriggerType() != "ContaminatedTrigger") { errorMessage = "Invalid static trigger type: " + area.GetName(); return false; }
            if (area.GetRadiusM() <= 0) { errorMessage = "Invalid static area radius: " + area.GetName(); return false; }

            staticAreaNames.Insert(area.GetName());
        }

        return true;
    }
}
