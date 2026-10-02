class ZZEStaticAreaConsolidator
{
    static bool Build(
        ZZEManifestCatalog catalog,
        array<ref ZZEStaticAreaCluster> output,
        out string errorMessage)
    {
        errorMessage = "";

        if (!output)
        {
            errorMessage = "Missing static contaminated cluster output";
            return false;
        }

        output.Clear();

        if (!catalog || !catalog.Validate(errorMessage))
        {
            if (errorMessage == "")
            {
                errorMessage = "Missing manifest catalog";
            }
            return false;
        }

        float rifyX;
        float rifyZ;
        int rifyCount;
        float pavlovoX;
        float pavlovoZ;
        int pavlovoCount;

        array<ref ZZEStaticAreaDefinition> volumes = catalog.GetStaticContaminatedVolumes();
        if (!volumes)
        {
            errorMessage = "Missing static contaminated volumes";
            output.Clear();
            return false;
        }

        foreach (ZZEStaticAreaDefinition area : volumes)
        {
            if (!area)
            {
                errorMessage = "Null static contaminated volume";
                output.Clear();
                return false;
            }

            vector position = area.GetPosition();

            if (IsRifyVolume(area.GetName()))
            {
                rifyX += position[0];
                rifyZ += position[2];
                rifyCount++;
                continue;
            }

            if (IsPavlovoVolume(area.GetName()))
            {
                pavlovoX += position[0];
                pavlovoZ += position[2];
                pavlovoCount++;
                continue;
            }

            errorMessage = "Unknown static contaminated volume: " + area.GetName();
            output.Clear();
            return false;
        }

        if (rifyCount != 4 || pavlovoCount != 5)
        {
            errorMessage = "Unexpected Rify or Pavlovo volume count";
            output.Clear();
            return false;
        }

        output.Insert(new ZZEStaticAreaCluster(
            "static:rify",
            "Rify",
            Vector(rifyX / rifyCount, 0, rifyZ / rifyCount),
            rifyCount));
        output.Insert(new ZZEStaticAreaCluster(
            "static:pavlovo",
            "Pavlovo",
            Vector(pavlovoX / pavlovoCount, 0, pavlovoZ / pavlovoCount),
            pavlovoCount));

        return true;
    }

    protected static bool IsRifyVolume(string name)
    {
        if (name == "Ship-Bow") return true;
        if (name == "Ship-Center") return true;
        if (name == "Ship-Stern") return true;
        return name == "Ship-East";
    }

    protected static bool IsPavlovoVolume(string name)
    {
        if (name == "Pavlovo-North-Uphill") return true;
        if (name == "Pavlovo-South-Building1") return true;
        if (name == "Pavlovo-South-Building2") return true;
        if (name == "Pavlovo-South-Downhill") return true;
        return name == "Pavlovo-West-Uphill";
    }
}
