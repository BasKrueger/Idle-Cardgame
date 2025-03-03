using System;
using System.Collections.Generic;
using System.Linq;
using Unity.VisualScripting;

public abstract partial class CardModel : ActorModel
{
    public CharacterModel _owner;
    public override CharacterModel owner => _owner;
    
    protected abstract string localizationKey { get; }
    protected abstract int baseDamage { get; }
    protected abstract int tickCooldown { get; }
    
    protected Dictionary<string, float> variables = new();
    protected int FullDamage => damage + owner.dmg;

    private float activeTickCooldown;
    private IntStat damage;

    public CardModel() : base()
    {
        this.damage = new IntStat(baseDamage);
        this.activeTickCooldown = 0;
    }
    
    public bool CanPlay() => activeTickCooldown > tickCooldown;
    
    protected abstract void Play(CharacterModel source, CharacterModel target);

    public void Tick()
    {
        activeTickCooldown += 1;
    }
    
    public bool TryPlay(CharacterModel source, CharacterModel target)
    {
        if (CanPlay())
        {
            Play(source, target);
            Reset();
            return true;
        }

        return false;
    }

    protected virtual void Reset()
    {
        activeTickCooldown = 0;
        damage.Reset();
    }
    
    public LegacyLocalizedString GetCardName()
    {
        var args = new List<object> { FullDamage };
        args.AddRange(variables.Values);
        return new LegacyLocalizedString(LegacyLocalizedString.Table.Cards, $"{localizationKey}_Name", FullDamage, args.ToArray());
    }
    
    public LegacyLocalizedString GetCardDescription()
    {
        var args = new List<object> { FullDamage };
        args.AddRange(variables.Values);
        return new LegacyLocalizedString(LegacyLocalizedString.Table.Cards, $"{localizationKey}_Text", FullDamage, args.ToArray());
    }
}
