using System;
using System.Collections.Generic;
using UnityEngine;

public partial class PlayerModel : CharacterModel
{
    private Dictionary<Guid, CardModel> collection = new Dictionary<Guid, CardModel>();
    
    public override string characterNameKey => "Player";
    protected override int baseHP => 200;
    protected override int baseDMG => 0;

    public override void AfterRegistration()
    {
        for (int i = 0; i < 21; i++)
        {
            AddCard(interactions.CreateInstance<FastAttackCardModel>());
        }

        for (int i = 0; i < 3; i++)
        {
            AddToCollection(interactions.CreateInstance<AttackCardModel>());
        }
    }

    public void SwapCards(Guid collection, Guid deck)
    {
        var deckCard = base.autoDeck[deck];
        var collectionCard = this.collection[collection];

        base.autoDeck.RenameKeyAndValue(deck, collection, collectionCard);
        this.collection.RenameKeyAndValue(collection, deck, deckCard);
    }
    
    private void AddToCollection(CardModel card)
    {
        card._owner = this;
        collection.Add(card.id, card);
    }
}

public static partial class Extensions
{
    public static bool RenameKeyAndValue<TKey, TValue>(this IDictionary<TKey, TValue> dict, TKey toReplace, TKey newKey, TValue newValue)
    {
        if (!dict.ContainsKey(toReplace)) return false;

        var temp = new Dictionary<TKey, TValue>(dict);
        dict.Clear();
        
        foreach(var pair in temp)
        {
            if(pair.Key.Equals(toReplace))
            {
                dict.Add(newKey, newValue);
                continue;
            }
            
            dict.Add(pair);
        }
        
        return true;
    }
}
