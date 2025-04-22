using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using Cysharp.Threading.Tasks;
using UnityEngine;

public static class GameDLL
{
    private const int STRING_MAX_LENGTH = 2048 * 500;

#if UNITY_EDITOR_WIN
    private const string dllName = "GameModelDLL";
#elif UNITY_STANDALONE_WIN
    private const string dllName = "__GameModelDLL";
#elif UNITY_ANDROID
    private const string dllName = "__GameModelSo";
#endif

    public static event System.Action<List<string>> GameStateReceived;

    [DllImport(dllName)]
    private static extern void Initialize();
    public static void Initialize(string language)
    {
        SetLanguage(language);
        Initialize();
    }

    [DllImport(dllName)]
    public static extern void Tick();

    [DllImport(dllName)]
    public static extern void Skip(float seconds);

    [DllImport(dllName)]
    public static extern int SwapCards(int A, int B);

    [DllImport(dllName)]
    private static extern bool PopCachedGameState(StringBuilder str, int maxLength);
    public static void GetGameStates(bool enableLog = false)
    {
        var result = new List<string>();
        var builder = new StringBuilder(STRING_MAX_LENGTH);

        while (PopCachedGameState(builder, STRING_MAX_LENGTH))
        {
            result.Add(builder.ToString());
        }

        if (enableLog && result.Count > 0)
        {
            Debug.Log(result.Last());
        }

        GameStateReceived?.Invoke(result);
    }

    [DllImport(dllName)]
    private static extern void GetSaveState(StringBuilder str, int maxLength);
    public static string GetSaveState()
    {
        var builder = new StringBuilder(STRING_MAX_LENGTH);
        GetSaveState(builder, STRING_MAX_LENGTH);
        return builder.ToString();
    }

    [DllImport(dllName)]
    private static extern void SetSaveState(StringBuilder str, int maxLength);
    public static void SetSaveState(string save)
    {
        save = save.Length > STRING_MAX_LENGTH ? save.Substring(0, STRING_MAX_LENGTH) : save.PadRight(STRING_MAX_LENGTH);
        var builder = new StringBuilder(save, STRING_MAX_LENGTH);
        SetSaveState(builder, STRING_MAX_LENGTH);
    }

    [DllImport(dllName)]
    private static extern void SetLanguage(string language, string cardTablePath, string logLabelPath, string characterTablePath);
    public static void SetLanguage(string language)
    {
        TextAsset cards = Resources.Load<TextAsset>("Cards");
        TextAsset logs = Resources.Load<TextAsset>("AdventureLog");
        TextAsset characters = Resources.Load<TextAsset>("Characters");

        SetLanguage(language, cards.text, logs.text, characters.text);

        Debug.Log("Language set to " + language);
    }

    [DllImport(dllName)]
    public static extern void ClaimReward(int rewardID, int cardID);

    [DllImport(dllName)]
    private static extern void GenerateGameState();

    public static void CreateGameState()
    {
        GenerateGameState();
        GetGameStates();
    }
}
