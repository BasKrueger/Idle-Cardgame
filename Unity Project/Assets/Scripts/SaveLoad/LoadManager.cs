using Cysharp.Threading.Tasks;
using System.IO;
using System.Threading.Tasks;
using UnityEngine;
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
        var startTime = System.DateTime.Now;

        loading = true;
        simulated = false;

        bar.DisplayPercent(0);
        bar.DisplayText("GameLoad");
        await LoadScene();

        bar.DisplayPercent(0.5f);
        bar.DisplayText("SaveLoad");
        LoadProgress();

        if (meta != null)
        {
            var saveTime = System.DateTime.Parse(meta.saveTime);
            var loadDelta = System.DateTime.Now.Subtract(startTime);
            var saveDelta = System.DateTime.Now.Subtract(saveTime);

            var totalDelta = loadDelta.Add(saveDelta);

            bar.DisplayPercent(0.1f);
            bar.DisplayText("SimulateTime", Mathf.RoundToInt((float)totalDelta.TotalSeconds));
            await SimulateAFKProgress(totalDelta);
            simulated = true;
        }

        bar.DisplayText("AlmostDone");
        bar.DisplayPercent(1f);

        loading = false;

        while (bar.updatingFill) await UniTask.WaitForEndOfFrame();
        Destroy(this.gameObject);
        await SceneManager.UnloadSceneAsync("RunLoading");
    }

    private void LoadProgress()
    {
        if (!File.Exists(SaveManager.gameSavePath))
        {
            Debug.LogWarning($"No gameSave found at {SaveManager.gameSavePath}");
            return;
        }
        if (!File.Exists(SaveManager.metaSavePath))
        {
            Debug.LogWarning($"Missing meta savefile at {SaveManager.metaSavePath}");
            return;
        }

        GameDLL.Initialize("English(en)");
        GameDLL.SetSaveState(File.ReadAllText(SaveManager.gameSavePath));

        meta = JsonUtility.FromJson<SaveMeta>(File.ReadAllText(SaveManager.metaSavePath));

        Debug.Log($"Loaded save from {SaveManager.gameSavePath}");
    }

    private async Task SimulateAFKProgress(System.TimeSpan timeDelta)
    {
        while(timeDelta.TotalSeconds >= GameModelManager.TICK_COOLDOWN)
        {
            var startingTime = System.DateTime.Now;
            float skipTicks = Mathf.Floor((float)timeDelta.TotalSeconds / GameModelManager.TICK_COOLDOWN);

            GameDLL.Skip(skipTicks);

            var simulationTime = System.DateTime.Now.Subtract(startingTime);
            timeDelta = timeDelta.Subtract(System.TimeSpan.FromSeconds(skipTicks * GameModelManager.TICK_COOLDOWN));
            timeDelta = timeDelta.Add(simulationTime);

            Debug.Log($"Simulated {skipTicks} seconds of gameplay in {System.DateTime.Now.Subtract(startingTime).TotalSeconds} seconds");
        }

        SaveManager.Save();
        await UniTask.WaitForSeconds((float)timeDelta.TotalSeconds);
    }

    private async UniTask LoadScene()
    {
        await SceneManager.LoadSceneAsync(RunScene, LoadSceneMode.Additive);
    }
}
