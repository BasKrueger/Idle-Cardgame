using System.IO;
using System.Text;
using UnityEditor;
using UnityEditor.Localization.Plugins.CSV;
using UnityEditor.Localization;
using UnityEngine;
using UnityEditor.Build;
using UnityEditor.Build.Reporting;

public class LocalizationExport : Editor
{
    [MenuItem("Localization/Export All CSV Files")]
    public static void ExportAllCsv()
    {
        var stringTableCollections = LocalizationEditorSettings.GetStringTableCollections();

        var path = $"{Application.dataPath}/Localization/Resources";

        if (string.IsNullOrEmpty(path))
            return;

        foreach (var collection in stringTableCollections)
        {
            var file = Path.Combine(path, collection.TableCollectionName + ".csv");
            using (var stream = new StreamWriter(file, false, Encoding.UTF8))
            {
                Csv.Export(stream, collection);
            }
        }

        AssetDatabase.Refresh();
    }
}

class MyCustomBuildProcessor : IPreprocessBuildWithReport
{
    public int callbackOrder { get { return 0; } }
    public void OnPreprocessBuild(BuildReport report)
    {
        LocalizationExport.ExportAllCsv();
    }
}
