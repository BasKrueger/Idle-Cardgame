using Cysharp.Threading.Tasks;
using System.Threading.Tasks;
using TMPro;
using UnityEngine;

public class DamageNumberParticle : UIParticle
{
    [SerializeField]
    private TextMeshProUGUI label;

    public async void SetUp(int number)
    {
        label.text = number.ToString();

        await UniTask.WaitForSeconds(1);
        Destroy(this.gameObject);
    }
}
