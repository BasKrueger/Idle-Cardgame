using System;
using System.Collections.Generic;
using System.Linq;
using Cysharp.Threading.Tasks;
using UnityEngine;
using UnityEngine.Localization.Settings;

public class GameModelManager : MonoBehaviour
{
    public List<IGameViewAsync> activeViewsAsync = new List<IGameViewAsync>();
    public List<IGameView> activeViews = new List<IGameView>();

    [SerializeField] private List<MonoBehaviour> views;
    private Queue<GameState> states = new Queue<GameState>();

    private void Awake()
    {
        LocalizationSettings.SelectedLocaleChanged += (lang) => GameDLL.SetLanguage(lang.LocaleName);

        foreach (var view in views)
        {
            var v = view.GetComponent<IGameView>();
            if (v != null)
            {
                activeViews.Add(v);
            }

            var vAsync = view.GetComponent<IGameViewAsync>();
            if (vAsync != null)
            {
                activeViewsAsync.Add(vAsync);
            }
        }
    }

    private void Start()
    {
        Application.targetFrameRate = 60;

        GameDLL.Initialize("German (Germany)(de-DE)");

        ModelTickLoop();
        AsyncViewUpdateLoop();
    }

    private async void ModelTickLoop()
    {
        while (true)
        {
            GameDLL.Tick();

            var rawStates = await GameDLL.GetGameStates();
            foreach (var state in rawStates)
            {
                var gameState = GameState.Parse(state);
                activeViews.ForEach(view => view.OnGameStateUpdate(gameState));
                states.Enqueue(gameState);
            }

            await UniTask.WaitForSeconds(GameModel.TICK_COOLDOWN);
        }
    }

    private async void AsyncViewUpdateLoop()
    {
        while (true)
        {
            while (states != null && states.Count > 0)
            {
                var state = states.Dequeue();
                var copy = new List<IGameViewAsync>(activeViewsAsync);

                foreach (var view in copy)
                {
                    await view.OnGameStateUpdate(state);
                }
            }
            
            await UniTask.WaitForEndOfFrame();
        }
    }

    private void Update()
    {
        if (Input.GetKeyDown(KeyCode.S))
        {
            GameDLL.SetLanguage("English(en)");
        }
    }
}
