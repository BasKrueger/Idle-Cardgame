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
    private RewardCardView cardTemplate;

    private RewardState reward;
    private CollectionView collection;
    private List<RewardCardView> activeCards = new();

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
    }

    public void SpawnCards(RewardState reward)
    {
        foreach (CardView t in content.GetComponentsInChildren<CardView>())
        {
            Destroy(t.gameObject);
        }

        foreach (var state in reward.cards)
        {
            var instance = Instantiate(cardTemplate);
            instance.Content.Show(state);
            instance.Content.ShowAsDefault();
            instance.Content.SetFill(1);
            instance.Clicked += OnCardSelected;

            instance.transform.SetParent(content, false);
            instance.transform.SetAsFirstSibling();

            activeCards.Add(instance);
        }

        activeCards.Reverse();
    }

    private async void OnCardSelected(CardView clickedCard)
    {
        activeCards.ForEach(card => card.Clicked -= OnCardSelected);
        activeCards.Where(card => card != clickedCard).ToList().ForEach(card => card.HideCard());

        claimStarted?.Invoke(clickedCard.Content.displayID);

        await AnimateClaim(clickedCard);

        claimEnded?.Invoke();
    }

    private async UniTask AnimateClaim(CardView card)
    {
        card.Content.transform.SetParent(transform.parent.parent.parent.parent, true);

        var emptyCard = collection.SpawnEmptyCard();
        await MoveCardContent(card, emptyCard);
        emptyCard.Content = card.Content;
        emptyCard.Content.ShowAsMedium();
    }

    private async UniTask MoveCardContent(CardView cardA, CardView cardB)
    {
        cardA.Content.OverrideGlobalPosition(true, cardA.Content.transform.position);
        await cardA.Content.SetTarget(cardB.transform);
        cardA.Content.transform.SetParent(cardB.transform, true);
        cardA.Content.OverrideGlobalPosition(false);
    }

    public async void Show()
    {
        for(int i = 0;i < activeCards.Count; i++)
        {
            activeCards[i].DiscoverCard(i * 0.4f);
        }

        while(activeCards.Any(card => card.animationDoneSource.Task.Status == UniTaskStatus.Pending))
        {
            await UniTask.WaitForEndOfFrame();
        }
        activeCards.ForEach(card => card.ReturnToPreAnimationParent());
    }
}
