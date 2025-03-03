using System.Collections;
using System.IO;
using Cysharp.Threading.Tasks;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class RunLoadingScreen : MonoBehaviour
{
    private const float FILL_SPEED = 1.5f;
    
    private float sceneProgress;
    private float simulationProgress = 0;

    [SerializeField]
    private Image fill;
    
    public static GameModel game;
    
    private async void Awake()
    {
        fill.fillAmount = 0;
        if (!File.Exists(GameModel.savePath)) simulationProgress = 1;

        //statusText.text = new LocalizedString(LocalizedString.Table.Menus, "Simulate_Ticks");
        await LoadGameModel();
        //statusText.text = new LocalizedString(LocalizedString.Table.Menus, "Loading_Game");
        await LoadRunScene();
    }
    
    private async UniTask LoadGameModel()
    {
        game = new GameModel();
        game.TryLoadSave();
        await game.TryPerformAFKTicks(progress => simulationProgress = progress);

        simulationProgress = 1;
    }
    
    private async UniTask LoadRunScene()
    {
        await UniTask.WaitForSeconds(1);
        var operation = SceneManager.LoadSceneAsync("Run", LoadSceneMode.Additive);
        
        while(!operation.isDone)
        {
            sceneProgress = operation.progress;
            await UniTask.WaitForEndOfFrame();
        }

        await UniTask.WaitForSeconds(1.5f);
        sceneProgress = 1;
    }
    
    private async void Update()
    {
        fill.fillAmount = Mathf.MoveTowards(fill.fillAmount, sceneProgress * 0.5f + simulationProgress * 0.5f, Time.deltaTime * FILL_SPEED);
        
        if(fill.fillAmount == 1)
        {
            await SceneManager.UnloadSceneAsync("Run_LoadingScreen");
        }
    }
}
