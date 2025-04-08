using System.Collections.Generic;
using System.Linq;
using UnityEngine;

[System.Serializable]
public class CharacterState
{
    public int id;
    public int baseHP;
    public int hp;
    public int dmg;
    public CharacterPhase phase;
    public CharacterAnimationTrigger animationTrigger;

    public int currentCard;
    public List<CardState> autoDeck;

    private List<CardState> deckInPlayOrder_;
    public List<CardState> autoDeckInPlayOrder
    {
        get
        {
            if(autoDeck == null) 
            {
                return new List<CardState>();
            }

            if (deckInPlayOrder_ == null)
            {
                deckInPlayOrder_ = new List<CardState>();
                for (int i = currentCard; deckInPlayOrder_.Count != autoDeck.Count; i++)
                {
                    if(i >= autoDeck.Count)
                    {
                        i = 0;
                    }

                    deckInPlayOrder_.Add(autoDeck[i]);
                }
            }

            return deckInPlayOrder_;
        }
    } 
}
