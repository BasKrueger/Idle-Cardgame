using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using Cysharp.Threading.Tasks;
using UnityEngine;

public partial class GameModel
{
    public readonly static string savePath = $"{Application.persistentDataPath}/save.json";
    private const int SIMULATION_INTERVALS = 30 * 60;

    public class Save
    {
        public string saveTime;
        public PlayerModel.Save player;
        public EncounterModelManager.Save encounter;
        public LogModel.Save log;
    }

    public void SaveProgress()
    {
        var save = new Save()
        {
            saveTime = System.DateTime.Now.ToString(),
            encounter = encounters.GetSave(),
            log = log.GetSave(),
            player = player.GetSave(),
        };

        var json = JsonUtility.ToJson(save, true);

        File.WriteAllText(savePath, json);

        Debug.Log("Saved to " + savePath);
    }

    public bool TryLoadSave()
    {
        return false;
        if (!File.Exists(savePath)) return false;

        var json = File.ReadAllText(savePath);
        var save = JsonUtility.FromJson<Save>(json);

        player.SetSave(save.player);
        encounters.SetSave(save.encounter, player, interactions);

        log.SetSave(save.log);

        Debug.Log($"Loaded from {savePath}");

        return true;
    }

    public async UniTask TryPerformAFKTicks(Action<float> progressUpdate)
    {
        if (!File.Exists(savePath)) return;
        progressUpdate?.Invoke(0);
        
        this.generateStates = false;

        var json = File.ReadAllText(savePath);
        var save = JsonUtility.FromJson<Save>(json);
        var afkDelta = System.DateTime.Now.Subtract(System.DateTime.Parse(save.saveTime)).Add(TimeSpan.FromHours(24));
        Debug.Log($"Simulating {afkDelta.Days} days and {afkDelta.Hours:00}:{afkDelta.Minutes:00}:{afkDelta.Seconds:00}:{afkDelta.Milliseconds} or {(int)afkDelta.TotalSeconds / (int)TICK_COOLDOWN} ticks...");

        var afkSeconds = afkDelta.TotalSeconds;
        do
        {
            var simulationStartTime = System.DateTime.Now;
            var simulationSeconds = (int)Mathf.Floor((float)afkSeconds);
            
            for (int i = 0; i < simulationSeconds; i++)
            {
                Tick();
                afkSeconds--;
                if (i % SIMULATION_INTERVALS == 0)
                {
                    progressUpdate?.Invoke(1 - ((float)afkSeconds / (float)afkDelta.TotalSeconds));
                    await UniTask.WaitForEndOfFrame();
                }
            }

            var simulationDelta = System.DateTime.Now.Subtract(simulationStartTime);
            afkSeconds += simulationDelta.TotalSeconds;
            
            Debug.Log($"Simulated {simulationSeconds} whole seconds within {simulationDelta.TotalSeconds} seconds.");
        }
        while (afkSeconds > TICK_COOLDOWN);

        await UniTask.WaitForSeconds((float)afkDelta.TotalSeconds % TICK_COOLDOWN);

        this.generateStates = true;
        SaveProgress();
    }

    private async UniTask SimulateSeconds(int seconds)
    {
        for (int i = 0; i < seconds; i++)
        {
            if (i % TICK_COOLDOWN == 0)
            {
                Tick();
            }

            //every 10 minutes
            if (i != 0 && i % (60 * 5) == 0)
            {
                await UniTask.WaitForEndOfFrame();
            }
        }
    }
}

public partial class PlayerModel
{
    [System.Serializable]
    public new class Save
    {
        public CharacterModel.Save baseSave;
        public List<string> collectionTypeNames;
        public List<CardModel.Save> collection;
    }

    public new Save GetSave()
    {
        return new Save()
        {
            baseSave = base.GetSave(),
            collectionTypeNames = collection.Values.Select(card => card.GetType().AssemblyQualifiedName).ToList(),
            collection = collection.Values.Select(card => card.GetSave()).ToList(),
        };
    }

    public void SetSave(Save save)
    {
        base.SetSave(save.baseSave);

        this.collection = new Dictionary<Guid, CardModel>();
        for (int i = 0; i < save.collectionTypeNames.Count; i++)
        {
            var cardType = Type.GetType(save.collectionTypeNames[i]);
            var cardInstance = interactions.CreateInstance(cardType);

            ((CardModel)cardInstance).SetSave(save.collection[i]);
            AddToCollection((CardModel)cardInstance);
        }
    }
}

public partial class LogModel
{
    [System.Serializable]
    public class Save
    {
        public string currentID;
        public List<string> entryIDs = new();
        public List<SubEntry> entries = new();

        [System.Serializable]
        public class SubEntry
        {
            //public List<LocalizedString.Save> subEntries = new();
        }
    }

    public Save GetSave()
    {
        var save = new Save();

        save.currentID = currentEntry.ToString();
        save.entryIDs = entries.Keys.Select(key => key.ToString()).ToList();
        
        foreach (var entry in entries.Values)
        {
            //var subEntries = entry.Select(sub => sub.GetSave()).ToList();
            //save.entries.Add(new Save.SubEntry() { subEntries = subEntries });
        }

        return save;
    }

    public void SetSave(Save save)
    {
        entries = new Dictionary<Guid, List<LegacyLocalizedString>>();
        currentEntry = Guid.Parse(save.currentID);

        for (int i = 0; i < save.entryIDs.Count; i++)
        {
            var id = Guid.Parse(save.entryIDs[i]);
            var content = new List<LegacyLocalizedString>();

/*
            foreach (var entry in save.entries[i].subEntries)
            {
                var s = new LocalizedString(LocalizedString.Table.AdventureLog, "");
                //s.SetSave(entry);
                content.Add(s);
            }
*/
            entries.Add(id, content);
        }
    }
}

public partial class EncounterModelManager
{
    [System.Serializable]
    public class Save
    {
        public string currentEncounterType;
        public EncounterModel.Save currentEncounter;
    }

    public Save GetSave()
    {
        return new Save()
        {
            currentEncounterType = currentEncounter.GetType().AssemblyQualifiedName,
            currentEncounter = currentEncounter.GetSave(),
        };
    }

    public void SetSave(Save save, PlayerModel player, InteractionResolver interactions)
    {
        var encounterType = Type.GetType(save.currentEncounterType);
        var encounterInstance = Activator.CreateInstance(encounterType);

        ((EncounterModel)encounterInstance).SetSave(save.currentEncounter);
        BeginNextEncounter(player, interactions, (EncounterModel)encounterInstance);
    }
}

public abstract partial class EncounterModel
{
    [System.Serializable]
    public class Save
    {
        public string id;
        public List<CharacterModel.Save> NPCs;
        public List<string> variableNames;
        public List<float> variableValues;
    }

    public Save GetSave()
    {
        return new Save()
        {
            id = id.ToString(),
            variableNames = variables.Keys.ToList(),
            variableValues = variables.Values.ToList(),
            NPCs = encounterNPCs.Select(npc => npc.GetSave()).ToList(),
        };
    }

    public void SetSave(Save save)
    {
        id = Guid.Parse(save.id);
        for (int i = 0; i < encounterNPCs.Count; i++)
        {
            encounterNPCs[i].SetSave(save.NPCs[i]);
        }

        variables.Clear();
        for (int i = 0; i < save.variableNames.Count; i++)
        {
            variables.Add(save.variableNames[i], save.variableValues[i]);
        }
    }
}

public abstract partial class CharacterModel
{
    [System.Serializable]
    public class Save
    {
        public string id;
        public IntStat.Save hpInternal;
        public IntStat.Save dmgInternal;

        public string currentCard;
        public List<string> autoDeckTypes;
        public List<CardModel.Save> autoDeckContent;

        public CharacterAnimationTrigger nextTrigger;
        public CharacterPhase phase;
    }

    public Save GetSave()
    {
        var save = new Save();
        save.id = id.ToString();
        save.hpInternal = hpInternal.GetSave();
        save.dmgInternal = dmgInternal.GetSave();
        save.currentCard = nextCard.ToString();
        save.autoDeckTypes = autoDeck.Values.Select(card => card.GetType().AssemblyQualifiedName).ToList();
        save.autoDeckContent = autoDeck.Values.Select(card => card.GetSave()).ToList();
        save.nextTrigger = nextAnimationTrigger;
        save.phase = phase;

        return save;
    }

    public void SetSave(Save save)
    {
        id = Guid.Parse(save.id);
        hpInternal = new IntStat(0);
        hpInternal.SetSave(save.hpInternal);
        dmgInternal = new IntStat(0);
        dmgInternal.SetSave(save.dmgInternal);
        nextAnimationTrigger = save.nextTrigger;
        phase = save.phase;

        autoDeck.Clear();
        for (int i = 0; i < save.autoDeckTypes.Count; i++)
        {
            var cardType = Type.GetType(save.autoDeckTypes[i]);
            var cardInstance = interactions.CreateInstance(cardType) as CardModel;

            cardInstance.SetSave(save.autoDeckContent[i]);
            AddCard(cardInstance);
        }
        nextCard = autoDeck.First(pair => pair.Key.ToString() == save.currentCard).Value;
    }
}

public abstract partial class CardModel
{
    [System.Serializable]
    public class Save
    {
        public float activeTickCooldown;
        public string ID;
        public IntStat.Save damage;
        public List<string> variableNames;
        public List<float> variableValues;
    }

    public Save GetSave()
    {
        return new Save()
        {
            activeTickCooldown = activeTickCooldown,
            ID = id.ToString(),
            damage = damage.GetSave(),
            variableNames = variables.Keys.ToList(),
            variableValues = variables.Values.ToList(),
        };
    }

    public void SetSave(Save save)
    {
        activeTickCooldown = save.activeTickCooldown;
        damage = new IntStat(0);
        damage.SetSave(save.damage);

        variables = new Dictionary<string, float>();
        for (int i = 0; i < save.variableNames.Count; i++)
        {
            variables.Add(save.variableNames[i], save.variableValues[i]);
        }

        id = Guid.Parse(save.ID);
    }
}

/*
public partial class LocalizedString
{
    [System.Serializable]
    public class Save
    {
        public string rootID;

        public List<LocStrings> allStrings;
        public List<Arg> allArgs;

        [System.Serializable]
        public class Arg
        {
            public string parentStringReferenceID;
            public string content;
            public string subStringReferenceID;
        }

        [System.Serializable]
        public class LocStrings
        {
            public string key;
            public Table table;
            public string id;
        }
    }

    public Save GetSave()
    {
        var save = new Save()
        {
            rootID = this.id.ToString(),
            allStrings = new List<Save.LocStrings>(),
            allArgs = new List<Save.Arg>(),
        };

        GetAllArgs(this, save.allArgs);
        GetAllStrings(this, save.allStrings);

        return save;
    }

    public void SetSave(Save save)
    {
        var stringData = save.allStrings.First(s => s.id == save.rootID);

        this.id = Guid.Parse(stringData.id);
        this.table = stringData.table;
        this.key = stringData.key;

        var args = save.allArgs.Where(arg => arg.parentStringReferenceID == stringData.id).ToList();

        List<object> tempArgs = new List<object>();
        foreach (var arg in args)
        {
            if (arg.subStringReferenceID != "")
            {
                var subString = save.allStrings.First(sub => sub.id == arg.subStringReferenceID);
                tempArgs.Add(BuildSubString(subString, save.allArgs, save.allStrings));
            }
            else
            {
                tempArgs.Add(arg.content);
            }
        }

        this.args = tempArgs.ToArray();
    }

    private void GetAllArgs(LocalizedString parent, in List<Save.Arg> args)
    {
        foreach (var arg in parent.args)
        {
            var save = new Save.Arg() { parentStringReferenceID = parent.id.ToString() };

            if (arg is LocalizedString)
            {
                var loc = arg as LocalizedString;
                save.subStringReferenceID = loc.id.ToString();
                GetAllArgs(loc, args);
            }
            else
            {
                save.content = arg.ToString();
            }

            args.Add(save);
        }
    }

    private void GetAllStrings(LocalizedString parent, in List<Save.LocStrings> strings)
    {
        var save = new Save.LocStrings()
        {
            key = parent.key,
            id = parent.id.ToString(),
            table = parent.table,
        };

        strings.Add(save);

        foreach (var arg in parent.args)
        {
            if (arg is LocalizedString)
            {
                GetAllStrings(arg as LocalizedString, strings);
            }
        }
    }

    public LocalizedString BuildSubString(Save.LocStrings stringData, List<Save.Arg> allArgs, List<Save.LocStrings> allStrings)
    {
        var result = new LocalizedString(Table.AdventureLog, "");
        result.id = Guid.Parse(stringData.id);
        result.table = stringData.table;
        result.key = stringData.key;

        var args = allArgs.Where(arg => arg.parentStringReferenceID == stringData.id).ToList();

        List<object> tempArgs = new List<object>();
        foreach (var arg in args)
        {
            if (arg.subStringReferenceID != "")
            {
                Debug.Log("looking for substring " + arg.subStringReferenceID);
                var subString = allStrings.First(sub => sub.id == arg.subStringReferenceID);
                tempArgs.Add(BuildSubString(subString, allArgs, allStrings));
            }
            else
            {
                tempArgs.Add(arg.content);
            }
        }

        result.args = tempArgs.ToArray();
        return result;
    }
}
*/

public partial class IntStat
{
    [System.Serializable]
    public class Save
    {
        public bool capped;
        public int value;
        public int maxValue;
        public int minValue;

        public float percentBonus;
        public int flatBonus;
    }

    public Save GetSave()
    {
        return new Save()
        {
            capped = capped,
            value = value,
            maxValue = maxValue,
            minValue = minValue,

            percentBonus = percentBonus,
            flatBonus = flatBonus,
        };
    }

    public void SetSave(Save save)
    {
        capped = save.capped;
        value = save.value;
        maxValue = save.maxValue;
        minValue = save.minValue;
        percentBonus = save.percentBonus;
        flatBonus = save.flatBonus;
    }
}