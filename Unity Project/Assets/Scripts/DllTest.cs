using System.Linq;
using Cysharp.Threading.Tasks;
using UnityEngine;

public class DllTest : MonoBehaviour
{
    async void Start()
    {
        GameDLL.Initialize("German (Germany)(de-DE)");
        GameDLL.Tick();

        GameDLL.GenerateGameState();
        await GameDLL.GetGameStates(true);

        GameDLL.SetLanguage("English(en)");
    }
}
