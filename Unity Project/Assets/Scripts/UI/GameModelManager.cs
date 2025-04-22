using System;
using System.Collections.Generic;
using System.Linq;
using Cysharp.Threading.Tasks;
using UnityEngine;
using UnityEngine.Localization.Settings;

public class GameModelManager : MonoBehaviour
{
    public const float TICK_COOLDOWN = 1f;

    public List<IGameViewAsync> activeViewsAsync = new List<IGameViewAsync>();
    public List<IGameView> activeViews = new List<IGameView>();

    [SerializeField] private bool printStates;
    [SerializeField] private List<MonoBehaviour> views;
    private Queue<GameState> asyncStates = new Queue<GameState>();

    private void Awake()
    {
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

    private async void Start()
    {
        Application.targetFrameRate = 60;
        QualitySettings.vSyncCount = 0;
        LocalizationSettings.SelectedLocaleChanged += (locale) => GameDLL.SetLanguage(locale.LocaleName);

        await UniTask.WaitUntil(() => !LoadManager.loading);
        if (!LoadManager.simulated)
        {
            GameDLL.Initialize(LocalizationSettings.SelectedLocale.LocaleName);
        }

        ModelTickLoop();
        AsyncViewUpdateLoop();
    }

    private async void ModelTickLoop()
    {
        List<string> rawStates = new List<string>();
        GameDLL.GameStateReceived += (states) => rawStates.AddRange(states);

        while (true)
        {
            GameDLL.Tick();
            await UniTask.WaitForEndOfFrame();
            GameDLL.GetGameStates(printStates);

            while(rawStates.Count > 0)
            {
                var gameState = GameState.Parse(rawStates.First());
                
                asyncStates.Enqueue(gameState);
                activeViews.ForEach(view => view.OnGameStateUpdate(gameState));

                rawStates.RemoveAt(0);
            }

            await UniTask.WaitForSeconds(TICK_COOLDOWN);
        }
    }

    private async void AsyncViewUpdateLoop()
    {
        while (true)
        {
            while (asyncStates != null && asyncStates.Count > 0)
            {
                var state = asyncStates.Dequeue();
                foreach (var view in activeViewsAsync)
                {
                    await view.OnGameStateUpdate(state);
                }
            }
            
            await UniTask.WaitForEndOfFrame();
        }
    }
    private void Update()
    {
        if (Input.GetKeyDown(KeyCode.Space))
        {
            SaveManager.Save();
        }
    }
}
