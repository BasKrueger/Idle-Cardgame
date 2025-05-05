using Cysharp.Threading.Tasks;
using System.Security.Cryptography;
using System.Threading.Tasks;
using TMPro;
using UnityEngine;
using UnityEngine.UI;

public class CardView : MonoBehaviour
{
    private const float fillSpeed = 2;

    [HideInInspector]
    public CardSlot slot;

    [SerializeField]
    private CardBuffViewManager buffs;

    public CardState lastState { get; private set; }
    public float scaleSpeed { set; private get; }
    public float moveSpeed { set; private get; }
    public bool hideText { set; private get; }
    public bool playAnimationEnabled { set; private get; }
    public ParticleUI preferedUI { set; private get; }

    public bool inPlayAnimation => anim.GetBool(playedKey);

    private static readonly int disappeardKey = Animator.StringToHash("disappeared");
    private static readonly int chargedKey = Animator.StringToHash("charged");
    private static readonly int playedKey = Animator.StringToHash("played");
    private static readonly int revealIdleKey = Animator.StringToHash("RevealIdle");
    private static readonly int revealedKey = Animator.StringToHash("reveal");

    [SerializeField]
    private Image icon;
    [SerializeField]
    private Image cooldown;
    [SerializeField]
    private TextMeshProUGUI text;
    [SerializeField]
    private Animator anim;

    private RectTransform rect;
    private RectTransform targetContent;
    private float cooldownTargetFill = -1;

    private void Awake()
    {
        text.transform.eulerAngles = new Vector3(0, 0);
    }

    public void Display(CardState state)
    {
        if (!enabled) return;

        icon.sprite = ResourceSpriteCache.Get($"CardArts/{state.cardIcon}");
        anim.SetBool(chargedKey, state.activeCooldown == 0);
        text.text = state.cardDescription;
        text.enabled = !hideText;

        UpdateCooldownFill(state.activeCooldown / state.cooldown);
        TryPlayPlayedAnimation(state);
        buffs.Display(state);

        lastState = state;
    }
    

    public async UniTask MoveTo(RectTransform otherContent)
    {
        if (rect == null) rect = GetComponent<RectTransform>();
        if (preferedUI == null) preferedUI = ParticleUI.mainInstance;

        if(targetContent != null)
        {
            preferedUI.TryReturn(targetContent.transform);
            preferedUI.Add(otherContent);
            targetContent = otherContent;

            transform.SetAsLastSibling();

            await UniTask.WaitUntil(() => targetContent == null);
            return;
        }

        preferedUI.Add(otherContent.transform);
        preferedUI.Add(transform);

        targetContent = otherContent;

        while (this.enabled)
        {
            rect.anchoredPosition = Vector2.MoveTowards(rect.anchoredPosition, targetContent.anchoredPosition, moveSpeed * Time.deltaTime * preferedUI.canvas.pixelRect.height);
            rect.sizeDelta = Vector2.MoveTowards(rect.sizeDelta, targetContent.sizeDelta, scaleSpeed * Time.deltaTime * preferedUI.canvas.pixelRect.height);
            buffs.UpdateIconSize();

            if (rect.anchoredPosition == targetContent.anchoredPosition && rect.sizeDelta == targetContent.sizeDelta) break;

            await UniTask.WaitForEndOfFrame();
        }

        transform.SetParent(targetContent.transform);
        preferedUI.TryReturn(targetContent.transform);
        preferedUI.TryClearData(transform);

        targetContent = null;
    }

    public void MoveToInstant(RectTransform other)
    {
        if (!this.enabled) return;

        if (rect == null) rect = GetComponent<RectTransform>();

        rect.anchoredPosition = other.anchoredPosition;
        rect.sizeDelta = other.sizeDelta;
    }

    public async void TryClearAtFrameEnd()
    {
        await UniTask.WaitForEndOfFrame();
        if (slot != null) return;

        this.enabled = false;
        anim.SetBool(disappeardKey, true);
    }

    public async void RevealAfter(float delay)
    {
        anim.Play(revealIdleKey);
        await UniTask.WaitForSeconds(delay);
        anim.SetTrigger(revealedKey);
    }

    private async void UpdateCooldownFill(float target)
    {
        if (cooldownTargetFill != -1)
        {
            cooldownTargetFill = target;
            return;
        }
        cooldownTargetFill = target;

        while (cooldown.fillAmount != cooldownTargetFill && cooldownTargetFill != 1)
        {
            cooldown.fillAmount = Mathf.MoveTowards(cooldown.fillAmount, cooldownTargetFill, fillSpeed * Time.deltaTime);
            await UniTask.WaitForEndOfFrame();
        }

        cooldown.fillAmount = cooldownTargetFill;
        cooldownTargetFill = -1;
    }

    private async Task TryPlayPlayedAnimation(CardState state)
    {
        if (preferedUI == null) preferedUI = ParticleUI.mainInstance;

        if (!playAnimationEnabled) return;
        if (state.activeCooldown != -1) return;
        if (lastState == null) return;
        if (lastState.activeCooldown == -1) return;
        
        anim.SetBool(playedKey, true);
        preferedUI.Add(transform, false);
        this.enabled = false;

        await UniTask.WaitForEndOfFrame();
        transform.SetAsLastSibling();
    }
}
