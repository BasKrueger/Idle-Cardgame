using System;
using System.Collections.Generic;
using UnityEngine;

public class CollectionView : MonoBehaviour, IGameView
{
    public event Action<CardView> CardSelected;
    
    [SerializeField]
    private Transform content;
    [SerializeField]
    private CardView viewModel;

    private Dictionary<int, CardView> activeViews = new();
    
    public void OnGameStateUpdate(GameState gameState)
    {
        TrySpawnCards(gameState.player);
    }
    
    private void TrySpawnCards(PlayerState playerState)
    {
        if (activeViews.Count > 0) return;
        
        foreach(var card in playerState.collection)
        {
            var instance = Instantiate(viewModel, content, false);
            instance.Content.UpdateTexts(card);
            instance.Clicked += CardSelected.Invoke;
            activeViews.Add(card.id, instance);
        }
    }
}
