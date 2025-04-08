using Cysharp.Threading.Tasks;
using System.Collections;
using UnityEngine;
using UnityEngine.Localization.Settings;
using UnityEngine.UI;

public class DllTest : MonoBehaviour
{
    [SerializeField]
    private Image img;

    public void Start()
    {
        StartCoroutine(Test());
    }

    IEnumerator Test()
    {
        img.color = Color.blue;

        yield return new WaitForSeconds(0.5f);

        img.color = Color.red;

        yield return new WaitForSeconds(1);

        for (int i = 0; i < 1000; i++)
        {
            img.color = Color.yellow;
            Debug.LogError("start" + i);
            GameDLL.Initialize(LocalizationSettings.SelectedLocale.LocaleName);
            //GameDLL.Tick();
            Debug.LogError("done " + i);
            yield return new WaitForEndOfFrame();
        }

        Debug.LogError("done");
        img.color = Color.green;
    }
}
