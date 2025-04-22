using Cysharp.Threading.Tasks;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;

public class CardQueueView : MonoBehaviour
{
    [SerializeField]
    private bool flipCards;

    [SerializeField]
    private List<CardSlot> previewSlots;

    public void OnGameStateUpdate(CharacterState characterState)
    {
        var displayStates = characterState.autoDeck.InPlayOrder.Take(4).ToList();

        MoveCards(displayStates);
        SpawnCards(displayStates);
        UpdateCards(displayStates);
    }

    public async UniTask OnGameStateUpdateAsync(CharacterState characterState)
    {
        var displayStates = characterState.autoDeck.InPlayOrder.Take(4).ToList();
    }

    private async void MoveCards(List<CardState> states)
    {
        var moves = new List<(CardView card, CardSlot fromSlot, CardSlot toSlot)>();

        for (int i = 0; i < previewSlots.Count; i++)
        {
            if (previewSlots[i].card == null) continue;
            if (previewSlots[i].card.lastState.id == states[i].id) continue;

            var otherSlot = previewSlots.Where(slot => slot.card != null)
                .Where(slot => slot != previewSlots[0] && slot != previewSlots[i])
                .FirstOrDefault(slot => slot.card.lastState.id == states[i].id);

            if (otherSlot != default) moves.Add((otherSlot.card, otherSlot, previewSlots[i]));
        }

        moves.Reverse();

        foreach(var tuple in moves)
        {
            tuple.fromSlot.DetachCard();
            tuple.toSlot.DetachCard();
        }

        foreach(var tuple in moves)
        {
            var _ = tuple.toSlot.TryAddCard(tuple.card);
            tuple.card.transform.SetAsLastSibling();
        }
    }

    private void UpdateCards(List<CardState> states)
    {
        for(int i = 0;i < states.Count; i++)
        {
            previewSlots[i].TryUpdateCard(states[i]);
        }
    }

    private void SpawnCards(List<CardState> states)
    {
        for (int i = 0; i < states.Count; i++)
        {
            previewSlots[i].TrySpawnCard(states[i]);
        }
    }
}
