using Cysharp.Threading.Tasks;
using System;
using System.IO;
using System.Threading.Tasks;
using UnityEngine;
using UnityEngine.Localization.Settings;
using UnityEngine.SceneManagement;

public class LoadManager : MonoBehaviour
{
    private const string RunScene = "Run";

    public static bool loading { get; private set; } = false;
    public static bool simulated { get; private set; } = false;


    [SerializeField]
    private LoadingBar bar;

    private SaveMeta meta = null;

    private async void Start()
    {
        await Load();
    }

    private async UniTask Load()
    {
        var startTime = System.DateTime.Now;

        loading = true;
        simulated = false;

        await bar.DisplayPercent(0);
        await bar.DisplayText("GameLoad");
        await LoadScene();

        await bar.DisplayPercent(0.5f);
        await bar.DisplayText("SaveLoad");
        await LoadProgress();

        if (meta != null)
        {
            var saveTime = System.DateTime.Parse(meta.saveTime);
            var loadDelta = System.DateTime.Now.Subtract(startTime);
            var saveDelta = System.DateTime.Now.Subtract(saveTime);

            var totalDelta = loadDelta.Add(saveDelta);
             
            await bar.DisplayText("SimulateTime", Mathf.RoundToInt((float)totalDelta.TotalSeconds));
            await bar.DisplayPercent(0.9f);
            await SimulateAFKProgress(totalDelta);
            
            simulated = true;
        }

        await bar.DisplayText("AlmostDone");
        await bar.DisplayPercent(1f);
        SaveManager.Save();
        
        Destroy(this.gameObject);
        await SceneManager.UnloadSceneAsync("RunLoading");
        await bar.DisplayText("Done");

        loading = false;
    }

    private async UniTask LoadProgress()
    {
        await bar.DisplayText("Initializing");
        GameDLL.Initialize(LocalizationSettings.SelectedLocale.LocaleName);

        if (File.Exists(SaveManager.gameSavePath))
        {
            GameDLL.SetSaveState(File.ReadAllText(SaveManager.gameSavePath));
            Debug.LogWarning($"Loaded save from {SaveManager.gameSavePath}");
        }
        else 
        {
            await bar.DisplayText("MissingSaveFile");
            Debug.LogWarning($"No gameSave found at {SaveManager.gameSavePath}");
        }

        if (File.Exists(SaveManager.metaSavePath))
        {
            meta = JsonUtility.FromJson<SaveMeta>(File.ReadAllText(SaveManager.metaSavePath));
            Debug.LogWarning($"Loaded meta from {SaveManager.metaSavePath}");
        }
        else
        {
            await bar.DisplayText("MissingMetaFile");
            Debug.LogWarning($"Missing meta savefile at {SaveManager.metaSavePath}");
        }
    }

    private async Task SimulateAFKProgress(System.TimeSpan timeDelta)
    {
        while (timeDelta.TotalSeconds >= GameModelManager.TICK_COOLDOWN)
        {
            var startingTime = System.DateTime.Now;
            float skipTicks = Mathf.Floor((float)timeDelta.TotalSeconds / GameModelManager.TICK_COOLDOWN);

            GameDLL.Skip(skipTicks);

            var simulationTime = System.DateTime.Now.Subtract(startingTime);
            timeDelta = timeDelta.Subtract(System.TimeSpan.FromSeconds(skipTicks * GameModelManager.TICK_COOLDOWN));
            timeDelta = timeDelta.Add(simulationTime);

            await bar.DisplayText("SimulateTime", timeDelta.TotalSeconds);

            Debug.Log($"Simulated {skipTicks} seconds of gameplay in {System.DateTime.Now.Subtract(startingTime).TotalSeconds} seconds");
        }

        await bar.DisplayText("Saving");
        SaveManager.Save();
        await UniTask.WaitForSeconds((float)timeDelta.TotalSeconds);
    }

    private async UniTask LoadScene()
    {
        await SceneManager.LoadSceneAsync(RunScene, LoadSceneMode.Additive);
    }
}