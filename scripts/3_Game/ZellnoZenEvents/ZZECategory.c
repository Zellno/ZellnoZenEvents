class ZZECategory
{
    static const string HELICRASH = "helicrash";
    static const string MILITARY_CONVOY = "military_convoy";
    static const string POLICE_CAR = "police_car";
    static const string POLICE_SITUATION = "police_situation";
    static const string TRAIN = "train";
    static const string AIRPLANE_CRATE = "airplane_crate";
    static const string DYNAMIC_CONTAMINATED_AREA = "dynamic_contaminated_area";

    static bool IsSupported(string category)
    {
        if (category == HELICRASH) return true;
        if (category == MILITARY_CONVOY) return true;
        if (category == POLICE_CAR) return true;
        if (category == POLICE_SITUATION) return true;
        if (category == TRAIN) return true;
        if (category == AIRPLANE_CRATE) return true;
        return category == DYNAMIC_CONTAMINATED_AREA;
    }
}
