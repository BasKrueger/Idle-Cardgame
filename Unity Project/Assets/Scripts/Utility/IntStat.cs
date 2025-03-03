using UnityEngine;

[System.Serializable]
public partial class IntStat
{
    public int totalValue { get { return Mathf.RoundToInt(value * (1 + percentBonus)) + flatBonus; } }

    [Header("Settings")]
    public int value;
    
    private bool capped;
    private int maxValue;
    private int minValue;

    [Header("Modifier")]
    public float percentBonus;
    public int flatBonus;

    public IntStat(int value)
    {
        this.value = value;
    }

    public IntStat(IntStat other)
    {
        value = other.value;
        percentBonus = other.percentBonus;
        flatBonus = other.flatBonus;
    }

    public void Reset()
    {
        percentBonus = 0;
        flatBonus = 0;
    }

    public void SetTo(int value)
    {
        flatBonus -= totalValue - value;
    }

    public static implicit operator int(IntStat stat)
    {
        ClampTotalValue(stat);
        return stat.totalValue;
    }

    public static IntStat operator +(IntStat a, int b)
    {
        a.flatBonus += b;
        ClampTotalValue(a);
        return a;
    }

    public static IntStat operator -(IntStat a, int b)
    {
        a.flatBonus -= b;
        ClampTotalValue(a);
        return a;
    }

    public static IntStat operator *(IntStat a, int b)
    {
        a.percentBonus += b;
        ClampTotalValue(a);
        return a;
    }

    public static IntStat operator /(IntStat a, int b)
    {
        a.percentBonus -= b;
        ClampTotalValue(a);
        return a;
    }

    private static void ClampTotalValue(IntStat a)
    {
        if (a.capped)
        {
            if(a.totalValue > a.maxValue)
            {
                var delta = a.totalValue - a.maxValue;
                a.flatBonus -= delta;
            }
            if(a.totalValue < a.minValue)
            {
                var delta = a.minValue - a.totalValue;
                a.flatBonus += delta;
            }
        }
    }
}