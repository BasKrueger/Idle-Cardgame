using System;
using System.Collections.Generic;
using System.Linq;

public partial class LogModel 
{
    private const int MAX_ENTRIES = 8;
    
    private Dictionary<Guid, List<LegacyLocalizedString>> entries = new Dictionary<Guid, List<LegacyLocalizedString>>();
    private Guid currentEntry;
    
    public LogModel()
    {
        BeginNewEntry(new LegacyLocalizedString(LegacyLocalizedString.Table.AdventureLog, "RunStart", new LegacyLocalizedString(LegacyLocalizedString.Table.Characters, "Player")));
    }
    
    public Guid BeginNewEntry(LegacyLocalizedString content = null)
    {
        var id = Guid.NewGuid();
        
        if(entries.Count > MAX_ENTRIES - 1)
        {
            entries.Remove(entries.First().Key);
        }

        entries.Add(id, new List<LegacyLocalizedString>() {content});
        currentEntry = id;
        
        return id;
    }
    
    public void AddSubEntry(LegacyLocalizedString content)
    {
        entries[currentEntry].Add(content);
    }

    public List<LegacyLocalizedString> GetEntry(Guid id) => entries[id];   
}
