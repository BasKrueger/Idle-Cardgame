using System.IO;
using UnityEngine;

public static class SaveManager 
{
    private const string gameSave = "Savestate.sv";
    private const string saveMeta = "SaveState.meta";

    public static string gameSavePath => $"{Application.persistentDataPath}/{gameSave}";
    public static string metaSavePath => $"{Application.persistentDataPath}/{saveMeta}";

    public static void Save()
    {
        var meta = new SaveMeta();
        meta.saveTime = System.DateTime.Now.ToString();

        File.WriteAllText(metaSavePath, JsonUtility.ToJson(meta, true));
        File.WriteAllText(gameSavePath, GameDLL.GetSaveState());

        Debug.LogWarning("saved progress");
    }
}