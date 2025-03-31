using System.IO;
using System.Linq;
using Cysharp.Threading.Tasks;
using UnityEngine;

public class DllTest : MonoBehaviour
{
    public async void Start()
    {
        GameDLL.Initialize("German (Germany)(de-DE)");
        GameDLL.SetSaveState(File.ReadAllText(SaveManager.gameSavePath));

        GameDLL.Skip(80000);
        GameDLL.Tick();
        GameDLL.GetGameStates(true);

        Debug.LogWarning("done");
    }
}
