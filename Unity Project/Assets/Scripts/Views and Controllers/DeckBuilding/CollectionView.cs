using System;
using System.Collections.Generic;
using UnityEngine;

public class CollectionView : MonoBehaviour, IGameView
{
    public event Action<CardView> CardSelected;
    
    [SerializeField]
    private Transform content;
    [SerializeField]
    private CardView cardView;
    [SerializeField]
    private CardView emptyCardView;

    private Dictionary<int, CardView> activeViews = new();

    private void Awake()
    {
        foreach(Transform t in content)
        {
            Destroy(t.gameObject);
        }
    }

    public void OnGameStateUpdate(GameState gameState)
    {
        TrySpawnCards(gameState.player);
    }
    
    private void TrySpawnCards(PlayerState playerState)
    {
        if (activeViews.Count > 0) return;
        
        foreach(var card in playerState.collection)
        {
            var instance = Instantiate(cardView, content, false);
            instance.Content.UpdateTexts(card);
            instance.Content.ShowAsMedium();

            instance.Clicked += CardSelected.Invoke;
            instance.ContentChanged += OnCardContentChanged;

            activeViews.Add(card.id, instance);
        }
    }

    public CardView SpawnEmptyCard()
    {
        var instance = Instantiate(emptyCardView, content, false);
        instance.Clicked += CardSelected.Invoke;
        instance.ContentChanged += OnCardContentChanged;

        return instance;
    }

    private void OnCardContentChanged(CardViewContent oldContent, CardView card)
    {
        if (oldContent != null && activeViews.ContainsKey(oldContent.displayID))
        {
            activeViews.Remove(oldContent.displayID);
        }

        activeViews.TryAdd(card.Content.displayID, card);
    }
}
