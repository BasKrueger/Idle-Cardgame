using System;
using System.Collections.Generic;
using System.Linq;
using Unity.VisualScripting;
using UnityEngine;

public class DeckView : MonoBehaviour, IGameView
{
    public event Action<CardView> CardSelected;
    
    [SerializeField]
    private Transform content;
    [SerializeField]
    private CardView viewModel;
    [SerializeField]
    private TargetFollower marker;

    private Dictionary<int, CardView> activeViews = new();
    
    private void Awake()
    {
        foreach(var t in content.GetComponentsInChildren<CardView>())
        {
            Destroy(t.gameObject);
        }
    }
        
    public void OnGameStateUpdate(GameState gameState)
    {
        TrySpawnCards(gameState.player.generic);
        UpdateMarker(gameState.player.generic);
    }
    
    private void TrySpawnCards(CharacterState playerState)
    {
        if (activeViews.Count > 0) return;

        foreach(var card in playerState.autoDeck)
        {
            var instance = Instantiate(viewModel, content, false);
            instance.Content.UpdateTexts(card);
            
            instance.Clicked += CardSelected.Invoke;
            instance.ContentChanged += OnCardContentChanged;
            
            instance.Content.ShowAsMedium();
            activeViews.Add(card.id, instance);
        }

        marker.transform.SetAsLastSibling();
    }
    
    private void OnCardContentChanged(CardView view)
    {
        activeViews.Clear();
        foreach(var card in content.GetComponentsInChildren<CardView>())
        {
            activeViews.Add(card.Content.displayID, card);
        }
    }
    
    private void UpdateMarker(CharacterState playerState)
    {
        marker.SetTarget(activeViews[playerState.autoDeck.First().id].transform);
    }
}
