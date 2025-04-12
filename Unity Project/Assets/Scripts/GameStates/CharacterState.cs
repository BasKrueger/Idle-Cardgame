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
    public DeckState autoDeck;
}
