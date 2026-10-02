class ZZEMarkerPresentation
{
    protected string m_Text;
    protected int m_Color;
    protected string m_IconPath;

    void ZZEMarkerPresentation(string text, int color, string iconPath)
    {
        m_Text = text;
        m_Color = color;
        m_IconPath = iconPath;
    }

    bool IsValid()
    {
        return m_Text != "" && m_IconPath != "";
    }

    string GetText()
    {
        return m_Text;
    }

    int GetColor()
    {
        return m_Color;
    }

    string GetIconPath()
    {
        return m_IconPath;
    }
}
