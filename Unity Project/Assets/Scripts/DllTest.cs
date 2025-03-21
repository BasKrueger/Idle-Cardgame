using System.Linq;
using Cysharp.Threading.Tasks;
using UnityEngine;

public class DllTest : MonoBehaviour
{
    public async void Start()
    {
        GameDLL.Initialize("German (Germany)(de-DE)");
        await GameDLL.Skip_(1000);
        GameDLL.CreateGameState();
        
        GameDLL.GetGameStates(true);
        GameDLL.SetLanguage("English(en)");

        Debug.LogWarning("done");
    }
}
