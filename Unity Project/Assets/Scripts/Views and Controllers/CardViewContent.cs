using System;
using Cysharp.Threading.Tasks;
using TMPro;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.UI;

public class CardViewContent : TargetFollower
{
    public event Action<float> ChargeUpdated;
    public event Action Clicked;
    
    private const float fillSpeed = 1.5f;
    private readonly static int miniKey = Animator.StringToHash("Mini");
    private readonly static int mikroKey = Animator.StringToHash("Mikro");
    private readonly static int megaKey = Animator.StringToHash("Mega");
    private readonly static int mediumKey = Animator.StringToHash("Medium");

    
    [SerializeField]
    private Image fill;
    [SerializeField]
    private TextMeshProUGUI descriptionLabel;
    [SerializeField]
    private TextMeshProUGUI nameLabel;
    [SerializeField]
    private Animator anim;
    
    public int displayID { get; private set; }

    private float targetFill;
    private bool isUpdatingFill;

    public async void UpdateFill(CardState cardState, bool allowShaking = true)
    {
        targetFill = cardState.activeCooldown / (float)cardState.cooldown;
        if (allowShaking) ChargeUpdated?.Invoke(targetFill);
        
        if(targetFill == 1)
        {
            fill.fillAmount = 1;
            return;
        }

        if (isUpdatingFill)
        {
            return;
        }

        isUpdatingFill = true;
        while (fill.fillAmount != targetFill)
        {
            fill.fillAmount = Mathf.MoveTowards(fill.fillAmount, targetFill, fillSpeed * Time.fixedDeltaTime);
            await UniTask.WaitForFixedUpdate();
        }
        isUpdatingFill = false;
    }

    public void UpdateTexts(CardState cardState)
    {
        displayID = cardState.id;

        descriptionLabel.text = cardState.cardDescription;
        nameLabel.text = cardState.cardName;
    }

    public void ShowAsMedium() => SetSizeActive(mediumKey);
    public void ShowAsMini() => SetSizeActive(miniKey);
    public void ShowAsMikro() => SetSizeActive(mikroKey);
    public void ShowAsMega() => SetSizeActive(megaKey);
    public void ShowAsDefault() => SetSizeActive(mediumKey, false);
    
    private void SetSizeActive(int size, bool active = true)
    {
        anim.SetBool(mikroKey, false);
        anim.SetBool(miniKey, false); 
        anim.SetBool(megaKey, false);

        anim.SetBool(size, active);
    }

    public void OnClick() => Clicked?.Invoke();
}
