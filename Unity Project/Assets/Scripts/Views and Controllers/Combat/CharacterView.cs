using Cysharp.Threading.Tasks;
using UnityEngine;

public class CharacterView : MonoBehaviour
{
    [SerializeField]
    private CharacterUI UI;

    [SerializeField]
    private CharacterSpriteView sprite;

    public async UniTask OnCharacterStateUpdateAsync(CharacterState characterState)
    {
        await UI.OnCharacterStateUpdateAsync(characterState);
        await sprite.OnCharacterStateUpdateAsync(characterState);
    }

    public void OnCharacterStateUpdate(CharacterState characterState)
    {
        UI.OnCharacterStateUpdate(characterState);
    }
}
