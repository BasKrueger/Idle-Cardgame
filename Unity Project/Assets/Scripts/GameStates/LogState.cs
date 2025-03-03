using System.Collections.Generic;
using UnityEngine;

[System.Serializable]
public class LogState
{
    public List<LogStateEntry> entries;
}

[System.Serializable]
public class LogStateEntry
{
    public int ID;
    public List<LogStateSubEntry> subEntries;
}

[System.Serializable]
public class LogStateSubEntry
{
    public int ID;
    public string content;
}
