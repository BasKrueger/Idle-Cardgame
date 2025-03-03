using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using Cysharp.Threading.Tasks;
using Newtonsoft.Json;
using Unity.Android.Gradle.Manifest;
using UnityEngine;

public static class GameDLL
{
    private const int STRING_MAX_LENGTH = 2048 * 8;

#if UNITY_EDITOR
    private const string dllName = "GameModelDLL";
#else
    private const string dllName = "__GameModelDLL";
#endif

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
    private static extern float Skip(float seconds);
    public static async UniTask Skip_(float seconds)
    {
        while (seconds >= 1)
        {
            var startTime = System.DateTime.Now;
            seconds = Skip(seconds);
            var endTime = System.DateTime.Now;
            seconds += (float)endTime.Subtract(startTime).TotalSeconds;

            Debug.LogError("rest seconds: " + seconds);
        }

        await UniTask.WaitForSeconds(seconds);
    }

    [DllImport(dllName)]
    public static extern int SwapCards(int collection, int deck);

    [DllImport(dllName)]
    public static extern void GenerateGameState();

    [DllImport(dllName)]
    private static extern bool PopCachedGameState(StringBuilder str, int maxLength);
    public static async UniTask<List<string>> GetGameStates(bool enableLog = false)
    {
        var result = new List<string>();
        var builder = new StringBuilder(STRING_MAX_LENGTH);

        while (PopCachedGameState(builder, STRING_MAX_LENGTH))
        {
            result.Add(builder.ToString());
            //Debug.Log($"Received state: {JsonUtility.ToJson(result.Last(), true)}");
        }

        if (enableLog && result.Count > 0)
        {
            Debug.Log(result.Last());
        }

        return result;
    }

    [DllImport(dllName)]
    private static extern void GetSaveState(StringBuilder str, int maxLength);
    public static string GetSaveState()
    {
        var builder = new StringBuilder(STRING_MAX_LENGTH);
        GetSaveState(builder, STRING_MAX_LENGTH);
        return builder.ToString();
    }

    [DllImport("GameModelDLL")]
    private static extern void SetSaveState(StringBuilder str, int maxLength);
    public static void SetSaveState(string save)
    {
        save = save.Length > STRING_MAX_LENGTH ? save.Substring(0, STRING_MAX_LENGTH) : save.PadRight(STRING_MAX_LENGTH);
        var builder = new StringBuilder(save, STRING_MAX_LENGTH);
        SetSaveState(builder, STRING_MAX_LENGTH);
    }

    [DllImport("GameModelDLL")]
    private static extern void SetLanguage(string language, string cardTablePath, string logLabelPath, string characterTablePath);
    public static void SetLanguage(string language)
    {
        TextAsset cards = Resources.Load<TextAsset>("Cards");
        TextAsset logs = Resources.Load<TextAsset>("AdventureLog");
        TextAsset characters = Resources.Load<TextAsset>("Characters");

        SetLanguage(language, cards.text, logs.text, characters.text);

        Debug.Log("Language set to " + language);
    }
}
