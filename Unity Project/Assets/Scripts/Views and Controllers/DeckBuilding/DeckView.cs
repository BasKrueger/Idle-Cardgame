using Cysharp.Threading.Tasks;
using System;
using System.Collections.Generic;
using System.Linq;
using Unity.VisualScripting;
using UnityEngine;

public class DeckView : MonoBehaviour, IGameView
{
    public event Action<CardSlot> SlotSelected;
    
    [SerializeField]
    private List<CardSlot> slots;
    [SerializeField]
    private CardSlot editSlot;

    private Dictionary<int, CardSlot> IdToSlot = new();
    private bool setuped = false;

    private void Awake()
    {
        foreach (CardSlot slot in slots)
        {
            slot.ContentChanged += OnSlotChanged;
            slot.Clicked += (slot) => SlotSelected?.Invoke(slot);
        }
    }

    public void OnGameStateUpdate(GameState gameState)
    {
        TrySpawnCards(gameState.player.generic);
        UpdateCards(gameState.player.generic);
    }

    private void TrySpawnCards(CharacterState playerState)
    {
        if (setuped) return;

        for(int i = 0;i < playerState.autoDeck.deckContent.Count && i < slots.Count; i++)
        {
            slots[i].TrySpawnCard(playerState.autoDeck.deckContent[i]);
        }

        setuped = true;
    }
    
    private void UpdateCards(CharacterState playerState)
    {
        playerState.autoDeck.deckContent.ForEach(state => { if (IdToSlot.ContainsKey(state.id)) IdToSlot[state.id].TryUpdateCard(state); });

        if(editSlot.card != null)
        {
            var previewState = playerState.autoDeck.deckContent.FirstOrDefault(state => editSlot.card.lastState.id == state.id);
            editSlot.TryUpdateCard(previewState, true);
        }
    }

    private void OnSlotChanged(CardSlot slot)
    {
        IdToSlot.TryRemoveByValue(slot);
        
        if (slot.card == null) return;
        IdToSlot.Add(slot.card.lastState.id, slot);
    }
}

public static partial class Extensions
{
    public static bool TryRemoveByValue<T, T2>(this Dictionary<T, T2> source, T2 toRemove)
    {
        foreach(var pair in source)
        {
            if(pair.Value.Equals(toRemove))
            {
                source.Remove(pair.Key);
                return true;
            }
        }

        return false;
    }
}
