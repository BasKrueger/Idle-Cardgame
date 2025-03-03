using Cysharp.Threading.Tasks;
using TMPro;
using UnityEngine;
using UnityEngine.UI;

public class HealthView : MonoBehaviour
{
    private const float updateSpeed = 1;
    
    [SerializeField]
    private Image fill;
    [SerializeField]
    private TextMeshProUGUI label;
    
    private float targetPercent;
    private bool isUpdatingBar;

    private CharacterState lastState;
    
    public void OnCharacterStateUpdate(CharacterState characterState)
    {
        if(lastState != null && lastState.hp == characterState.hp && lastState.baseHP == characterState.baseHP)
        {
            return;
        }

        label.text = $"{characterState.hp} / {characterState.baseHP}";
        targetPercent = characterState.hp / (float)characterState.baseHP;
        UpdateHealthBar();

        lastState = characterState;
    }
    
    private async void UpdateHealthBar()
    {
        if(isUpdatingBar)
        {
            return;
        }
        
        isUpdatingBar = true;
        
        while(fill.fillAmount != targetPercent)
        {
            fill.fillAmount = Mathf.MoveTowards(fill.fillAmount, targetPercent, updateSpeed * Time.deltaTime);
            await UniTask.WaitForEndOfFrame();
        }
        
        isUpdatingBar = false;
    }
}
