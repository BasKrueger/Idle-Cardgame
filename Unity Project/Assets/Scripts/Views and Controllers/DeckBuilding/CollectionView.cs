using Cysharp.Threading.Tasks;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using UnityEngine;
using UnityEngine.UIElements;

public class CollectionView : MonoBehaviour, IGameView
{
    public event Action<CardSlot> SlotSelected;
    
    [SerializeField]
    private Transform content;
    [SerializeField]
    private CardSlot slotTemplate;
    [SerializeField]
    private ParticleUI slotMoveUI;
    [SerializeField]
    private Animator screenAnim;

    private HashSet<int> spawnedCards = new();
    private bool spawnedInitial = false;

    private void Awake()
    {
        content.GetComponentsInChildren<CardSlot>().ToList()
            .ForEach(s => { s.ContentChanged += OnSlotChanged; s.Clicked += OnSlotClicked; });
    }

    public void OnGameStateUpdate(GameState gameState)
    {
        if (!spawnedInitial)
        {
            spawnedInitial = true;
            InitialSpawn(gameState.player);
        }
    }

    public async Task TryAddFrom(CardSlot slot)
    {
        var newSlot = GetOrCreateEmptySlot().TrySwapCards(slot);
    }

    private void InitialSpawn(PlayerState playerState)
    {
        var slots = content.GetComponentsInChildren<CardSlot>().ToList().OrderAsSnake(6, 1);
        var states = new List<CardState>(playerState.collection.deckContent);
        states.Reverse();
        for (int i = 0;i < states.Count; i++)
        {
            var state = states[i];
            if (spawnedCards.Contains(state.id)) continue;

            if(slots.Count <= i)
            {
                SpawnNewSlots();
                slots = content.GetComponentsInChildren<CardSlot>().ToList().OrderAsSnake(6, 1);
            }

            slots[i].TrySpawnCard(state);
        }
    }

    private void OnSlotChanged(CardSlot slot)
    {
        if (slot.card != null && !spawnedCards.Contains(slot.card.lastState.id))
        {
            spawnedCards.Add(slot.card.lastState.id);
        }
    }

    private CardSlot GetOrCreateEmptySlot()
    {
        var mainSlot = content.GetComponentsInChildren<CardSlot>().First();
        if (mainSlot.card != null)
        {
            if (!TryMoveExistingSlots())
            {
                SpawnNewSlots();
                TryMoveExistingSlots();
            }
        }

        return mainSlot;
    }

    private void SpawnNewSlots()
    {
        for(int i = 0;i < 6; i++)
        {
            var newSlot = Instantiate(slotTemplate, content, false);
            newSlot.preferedUI = slotMoveUI;
            newSlot.ContentChanged += OnSlotChanged;
            newSlot.Clicked += OnSlotClicked;
        }
    }

    private bool TryMoveExistingSlots()
    {
        var slots = content.GetComponentsInChildren<CardSlot>().ToList().OrderAsSnake(6, 1);
        if (slots.Last().card != null) return false;

        var cards = slots.Where(slot => slot.card != null).Select(slot => (slot.card, slots.IndexOf(slot))).ToList();

        slots.ToList().ForEach(slot => slot.DetachCard());

        foreach(var card in cards)
        {
            slots[card.Item2 + 1].preferedUI = slotMoveUI;
            slots[card.Item2 + 1].TryAddCard(card.card, true);
        }

        return true;
    }

    private void OnSlotClicked(CardSlot slot)
    {
        if (slot.card == null) return;
        if (screenAnim.GetBool(RewardView.rewardModeBool)) return;

        content.GetComponentsInChildren<CardSlot>().ToList().ForEach(slot => slot.preferedUI = null);
        SlotSelected?.Invoke(slot);
    }
}

public partial class Extensions 
{ 
    public static List<T> OrderAsSnake<T>(this List<T> unSorted, int rowLength, int offset = 0)
    {
        var sorted = new List<T>();

        for (int i = 0; i < rowLength - offset && i < unSorted.Count + 1; i++)
        {
            sorted.Add(unSorted.First());
            unSorted.RemoveAt(0);
        }

        var subLists = Enumerable.Range(0, (unSorted.Count + rowLength - 1) / rowLength)
        .Select(n => unSorted.Skip(n * rowLength).Take(rowLength).ToList())
        .ToList();

        for (int i = 0; i < subLists.Count; i++)
        {
            if (i % 2 == 0) subLists[i].Reverse();
            sorted.AddRange(subLists[i]);
        }

        unSorted.Clear();
        sorted.ForEach(entry => unSorted.Add(entry));

        return unSorted;
    }
}
