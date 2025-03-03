using System.Collections.Generic;
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
    
        
    public List<CardState> autoDeck;
}
