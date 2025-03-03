using System;
using UnityEngine.Localization.Settings;

public class LegacyLocalizedString
{
    public enum Table
    {
        AdventureLog,
        Characters,
        Cards,
        Menus,
    }
    
    private string key;
    private object[] args;
    private Table table;
    private Guid id = Guid.NewGuid();

    public LegacyLocalizedString(Table table, string localizationKey, params object[] args)
    {
        key = localizationKey;
        this.table = table;
        
        this.args = new object[args.Length];
        for(int i =0;i < args.Length;i++)
        {
            this.args[i] = args[i] is not LegacyLocalizedString ? args[i].ToString() : args[i];
        }
    }
    
    public LegacyLocalizedString(LegacyLocalizedString other)
    {
        this.key = other.key;
        this.args = other.args;
        this.table = other.table;
        this.id = other.id;
    }

    public string Format()
    {
        var raw = LocalizationSettings.StringDatabase.GetLocalizedString($"{table}", $"{key}");
        
        try
        {
            return string.Format(raw, args);
        }
        catch(Exception e)
        {
            UnityEngine.Debug.LogError($"Error while localizing {raw} using the {table} table");
            throw (e);
        }
    }


    public static implicit operator string(LegacyLocalizedString str) => str.Format();

    public override string ToString()
    {
        return Format();
    }

    public override bool Equals(object obj)
    {
        if (obj is not LegacyLocalizedString) return false;

        var other = obj as LegacyLocalizedString;
        
        if (other.key != this.key || other.table != this.table || other.args.Length != this.args.Length) return false;
        
        for(int i =0;i < args.Length;i++)
        {
            if (!this.args[i].Equals(other.args[i])) return false;
        }

        return true;
    }

    public override int GetHashCode()
    {
        return base.GetHashCode();
    }
}