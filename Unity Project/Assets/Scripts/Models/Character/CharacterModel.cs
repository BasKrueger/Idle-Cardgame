using System;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;

public abstract partial class CharacterModel : ActorModel
{
    public event Action StateUpdated;
    public override CharacterModel owner => this;
    public CharacterPhase phase { get; private set; }

    public abstract string characterNameKey { get; }
    protected Dictionary<Guid, CardModel> autoDeck = new Dictionary<Guid, CardModel>();
    protected abstract int baseHP { get; }
    protected abstract int baseDMG { get; }


    private IntStat hpInternal;
    public int hp => hpInternal;

    private IntStat dmgInternal;
    public int dmg => dmgInternal;
    
    private CharacterModel currentEnemy;
    private CardModel nextCard;

    private CharacterAnimationTrigger nextAnimationTrigger = CharacterAnimationTrigger.None;

    public CharacterModel() : base()
    {
        this.hpInternal = new IntStat(baseHP);
        this.dmgInternal = new IntStat(baseDMG);

        interactionEvents.play.Before += (interaction) =>
        {
            if (interaction.source.owner == this) SelectNextCard(interaction);
        };

        interactionEvents.attack.Before += (interaction) =>
        {
            if(interaction.source.owner == this)
            {
                PushState(CharacterAnimationTrigger.Attacked);
            }
        };

        PushState();
    }

    public void Tick()
    {
        if (currentEnemy != null)
        {
            nextCard.Tick();
            TryPlayNextCard();
            PushState();
        }
    }

    public bool TryPlayNextCard()
    {
        if (nextCard.CanPlay())
        {
            interactions.Add(new PlayInteraction(this, currentEnemy, nextCard));
            return true;
        }

        return false;
    }
    
    private void SelectNextCard(PlayInteraction play)
    {
        var valueList = autoDeck.Values.ToList();
        var cardIndex = valueList.IndexOf(play.card);
        nextCard = valueList[cardIndex + 1 < valueList.Count ? cardIndex + 1 : 0];
    }
    
    protected void AddCard(CardModel card)
    {
        if (nextCard == default) nextCard = card;
        card._owner = this;
        autoDeck.Add(card.id, card);
    }

    public void EngageInCombat(CharacterModel enemy)
    {
        phase = CharacterPhase.Combat;
        currentEnemy = enemy;
        PushState();
    }

    public void DisEngageInCombat(CharacterModel enemy)
    {
        if (phase == CharacterPhase.Dead) return;

        phase = CharacterPhase.Idle;
        currentEnemy = null;
        PushState();
    }

    public void TakeDamage(ActorModel source, int amount)
    {
        hpInternal -= amount;
        
        PushState(CharacterAnimationTrigger.DamageTaken);

        if (hpInternal <= 0)
        {
            interactions.Add(new DeathInteraction(source, this));
        }
    }

    public void Die(CharacterModel source)
    {
        DisEngageInCombat(source);
        phase = CharacterPhase.Dead;
        PushState();
    }

    private void PushState(CharacterAnimationTrigger display = CharacterAnimationTrigger.None)
    {
        nextAnimationTrigger = display;
        StateUpdated?.Invoke();
        nextAnimationTrigger = CharacterAnimationTrigger.None;
    }
}