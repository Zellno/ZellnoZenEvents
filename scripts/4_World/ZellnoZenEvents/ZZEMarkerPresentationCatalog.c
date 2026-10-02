class ZZEMarkerPresentationCatalog
{
    static ZZEMarkerPresentation ForCategory(string category)
    {
        if (category == ZZECategory.HELICRASH)
        {
            return new ZZEMarkerPresentation(
                "Helicrash",
                -27136,
                "ZenMap/data/icons/heli.paa");
        }

        if (category == ZZECategory.MILITARY_CONVOY)
        {
            return new ZZEMarkerPresentation(
                "Military Convoy",
                -65374,
                "ZenMap/data/icons/car.paa");
        }

        if (category == ZZECategory.POLICE_CAR)
        {
            return new ZZEMarkerPresentation(
                "Police Car",
                -16760376,
                "ZenMap/data/icons/car.paa");
        }

        if (category == ZZECategory.POLICE_SITUATION)
        {
            return new ZZEMarkerPresentation(
                "Police Situation",
                -16760376,
                "ZenMap/data/icons/warn.paa");
        }

        if (category == ZZECategory.TRAIN)
        {
            return new ZZEMarkerPresentation(
                "Train Wreck",
                -65374,
                "ZenMap/data/icons/cart.paa");
        }

        if (category == ZZECategory.AIRPLANE_CRATE)
        {
            return new ZZEMarkerPresentation(
                "Airplane Crate",
                -3664876,
                "ZenMap/data/icons/flag.paa");
        }

        if (category == ZZECategory.DYNAMIC_CONTAMINATED_AREA)
        {
            return new ZZEMarkerPresentation(
                "Contaminated Area",
                -15165928,
                "ZenMap/data/icons/warn.paa");
        }

        return null;
    }
}
