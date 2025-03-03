using System;
using System.Collections.Generic;
using System.Linq;

public partial class GameModel
{
    public event Action<State> StateUpdated;
    
    public struct State
    {
        public PlayerModel.State player;
        public EncounterModel.State encounter;
        public LogModel.State log;
    }
    
    public void PushState()
    {
        if (!generateStates) return;
        
        var state = new State()
        {
            player = player.GetState(),
            encounter = encounters.GetCurrentEncounterState(),
            log = log.GetState(),
        };

        StateUpdated?.Invoke(state);
    }
}

public partial class PlayerModel
{
    public new struct State
    {
        public CharacterModel.State generic;

        public Dictionary<Guid, CardModel.State> cardCollection;
    }

    public new State GetState()
    {
        return new State()
        {
            generic = base.GetState(),
            cardCollection = collection.ToDictionary(pair => pair.Key, pair => pair.Value.GetState()),
        };
    }
}

public partial class LogModel
{
    public struct State
    {
        public Dictionary<Guid, List<LegacyLocalizedString>> entries;
    }

    public State GetState()
    {
        return new State()
        {
            entries = entries.ToDictionary(pair => pair.Key, pair => new List<LegacyLocalizedString>(pair.Value))
        };
    }
}

public abstract partial class EncounterModel
{
    public struct State
    {
        public Guid id;
        public string encounterName;
        public bool isStaticEncounter;
        public Dictionary<Guid, CharacterModel.State> NPCs;
    }

    public State GetState() => new State()
    {
        id = id,
        encounterName = encounterName,
        isStaticEncounter = isStaticEncounter,
        NPCs = encounterNPCs.Select(npc => npc.GetState()).ToDictionary(npc => npc.id),
    };
}

public abstract partial class CharacterModel
{
    public struct State
    {
        public Guid id;

        public int baseHP;
        public int hp;
        public int dmg;
        public Dictionary<Guid, CardModel.State> autoDeckInPlayOrder;
        public Dictionary<Guid, CardModel.State> autoDeck;
        public CharacterAnimationTrigger animationTrigger;
        public CharacterPhase phase;
    }

    public State GetState()
    {
        var result = new State()
        {
            id = id,

            baseHP = baseHP,
            hp = hpInternal,
            dmg = dmgInternal,
            autoDeckInPlayOrder = new Dictionary<Guid, CardModel.State>(),
            autoDeck = autoDeck.ToDictionary(pair => pair.Key, pair => pair.Value.GetState()),
            animationTrigger = nextAnimationTrigger,
            phase = phase,
        };
        
        nextAnimationTrigger = CharacterAnimationTrigger.None;

        var found = false;
        var skipped = new List<KeyValuePair<Guid, CardModel>>();
        foreach(var pair in autoDeck)
        {
            if(pair.Key == nextCard.id)
            {
                result.autoDeckInPlayOrder.Add(pair.Key, pair.Value.GetState());
                found = true;
                continue;
            }
            
            if(!found)
            {
                skipped.Add(pair);
                continue;
            }
            
            result.autoDeckInPlayOrder.Add(pair.Key, pair.Value.GetState());
        }

        skipped.ForEach(pair => result.autoDeckInPlayOrder.Add(pair.Key, pair.Value.GetState()));

        return result;
    }
}

public abstract partial class CardModel
{
    public struct State
    {
        public Guid id;
        public int dmg;
        public float activeCooldown;
        public float cooldown;
        public LegacyLocalizedString name;
        public LegacyLocalizedString description;
    }
    
    public State GetState() => new State()
    {
        id = id,
        dmg = damage.totalValue,
        activeCooldown = activeTickCooldown,
        cooldown = tickCooldown,
        name = GetCardName(),
        description = GetCardDescription(),
    };
}