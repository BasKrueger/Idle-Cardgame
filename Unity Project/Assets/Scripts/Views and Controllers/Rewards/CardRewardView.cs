using Cysharp.Threading.Tasks;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using UnityEngine;

public class CardRewardView : MonoBehaviour
{
    public event Action<int> claimStarted;
    public event Action claimEnded;

    [SerializeField]
    private RectTransform content;
    [SerializeField]
    private CardSlot slotTemplate;

    private RewardState reward;
    private CollectionView collection;
    private List<CardSlot> activeSlots = new();

    public void SetUp(RewardState reward, CollectionView collection)
    {
        if(reward.cards.Count == 0)
        {
            this.gameObject.SetActive(false);
            claimEnded?.Invoke();
            return;
        }

        this.collection = collection;
        this.reward = reward;
        SpawnCards(reward);
        ScheduleReveal();
    }

    public void SpawnCards(RewardState reward)
    {
        foreach (var t in content.GetComponentsInChildren<CardSlot>())
        {
            Destroy(t.gameObject);
        }

        foreach (var state in reward.cards)
        {
            var instance = Instantiate(slotTemplate);

            instance.transform.SetParent(content, false);
            instance.transform.SetAsFirstSibling();
            instance.Clicked += OnSlotSelected;

            instance.TrySpawnCard(state);

            activeSlots.Add(instance);
        }

        activeSlots.Reverse();
    }

    private void OnSlotSelected(CardSlot clickedSlot)
    {
        activeSlots.ForEach(slot => slot.Clicked -= OnSlotSelected);
        activeSlots.Where(slot => slot != clickedSlot).ToList().ForEach(slot => slot.DetachCard());

        claimStarted?.Invoke(clickedSlot.card.lastState.id);

        collection.TryAddFrom(clickedSlot);

        claimEnded?.Invoke();
    }

    private void ScheduleReveal()
    {
        for (int i = 0; i < activeSlots.Count; i++)
        {
            activeSlots[i].card.RevealAfter(i * 0.4f + 0.1f);
        }
    }
}
