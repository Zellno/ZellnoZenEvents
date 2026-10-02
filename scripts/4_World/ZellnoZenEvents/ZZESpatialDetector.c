class ZZESpatialDetector
{
    ZZEDetectionResult Probe(ZZECandidateDefinition candidate)
    {
        if (!candidate)
        {
            return new ZZEDetectionResult("", ZZEDetectionStatus.INVALID_CANDIDATE);
        }

        ZZEDetectionResult result = new ZZEDetectionResult(
            candidate.GetId(),
            ZZEDetectionStatus.NOT_DETECTED);

        if (!GetGame() || !GetGame().IsDedicatedServer())
        {
            return new ZZEDetectionResult(
                candidate.GetId(),
                ZZEDetectionStatus.NOT_DEDICATED_SERVER);
        }

        array<Object> nearbyObjects = new array<Object>;
        array<CargoBase> proxyCargos = new array<CargoBase>;

        GetGame().GetObjectsAtPosition(
            candidate.GetProbePosition(),
            candidate.GetProbeRadiusM(),
            nearbyObjects,
            proxyCargos);

        result.SetObjectsScanned(nearbyObjects.Count());

        foreach (Object nearbyObject : nearbyObjects)
        {
            if (!nearbyObject)
            {
                continue;
            }

            string objectType = nearbyObject.GetType();
            if (!IsExactSemanticAnchor(objectType, candidate.GetSemanticAnchorClasses()))
            {
                continue;
            }

            vector objectPosition = nearbyObject.GetPosition();
            result.SetMatch(
                objectType,
                objectPosition,
                Distance2D(candidate.GetProbePosition(), objectPosition));
            return result;
        }

        return result;
    }

    protected bool IsExactSemanticAnchor(string objectType, array<string> allowedClasses)
    {
        foreach (string allowedClass : allowedClasses)
        {
            if (objectType == allowedClass)
            {
                return true;
            }
        }

        return false;
    }

    protected float Distance2D(vector first, vector second)
    {
        float deltaX = first[0] - second[0];
        float deltaZ = first[2] - second[2];
        return Math.Sqrt((deltaX * deltaX) + (deltaZ * deltaZ));
    }
}
