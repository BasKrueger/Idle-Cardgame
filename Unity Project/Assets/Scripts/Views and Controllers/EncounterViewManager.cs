using System;
using Cysharp.Threading.Tasks;
using UnityEngine;

public class EncounterViewManager : MonoBehaviour, IGameViewAsync, IGameView
{
    private const float ENCOUNTER_TRANSITION_SPEED = 3;
    private const float ENCOUNTER_DISTANCE = 7.5f;

    [SerializeField]
    private GameModelManager modelManager;
    [SerializeField]
    private EncounterView encounterTemplate;
    [SerializeField]
    private CharacterSpriteView playerSprite;

    private (EncounterView encounter, int id) currentEncounter;
    private UniTaskCompletionSource ongoingLoopCompletionSource;
    private UniTaskCompletionSource cancelLoopCompletionSource;

    private void Awake()
    {
        foreach (Transform t in transform)
        {
            Destroy(t.gameObject);
        }
    }

    public async UniTask OnGameStateUpdate(GameState gameState)
    {
        var previousEncounter = currentEncounter;

        await TrySpawnEncounter(gameState.encounter);
        await TryMoveToEncounter(gameState.encounter, previousEncounter);
        await currentEncounter.encounter.OnEncounterStateUpdateAsync(gameState.encounter);

        TryLoopEncounter(gameState.encounter);
    }

    void IGameView.OnGameStateUpdate(GameState gameState)
    {
        currentEncounter.encounter?.OnEncounterStateUpdate(gameState.encounter);
    }

    private async UniTask TrySpawnEncounter(EncounterState encounter)
    {
        if (currentEncounter == default || encounter.id != currentEncounter.id)
        {
            if (ongoingLoopCompletionSource != null)
            {
                cancelLoopCompletionSource.TrySetResult();
                await ongoingLoopCompletionSource.Task;
            }

            var view = GetNewEncounterViewInstance(encounter, currentEncounter.encounter == null ? 0 : ENCOUNTER_DISTANCE);
            _ = view.OnEncounterStateUpdateAsync(encounter);

            currentEncounter = (view, encounter.id);
        }
    }

    private async UniTask TryMoveToEncounter(EncounterState encounter, (EncounterView encounter, int id) previousEncounter)
    {
        if (previousEncounter.id != default && encounter.id != previousEncounter.id)
        {
            playerSprite.SetRunning(true);

            await UniTask.WhenAll(
                MoveEncounterPositionTo(new Vector3(), currentEncounter.encounter),
                MoveEncounterPositionTo(new Vector3(-ENCOUNTER_DISTANCE, 0, 0), previousEncounter.encounter)
            );

            if (previousEncounter.encounter != null)
            {
                Destroy(previousEncounter.encounter.gameObject);
            }

            playerSprite.SetRunning(false);
        }
    }

    private async void TryLoopEncounter(EncounterState encounter)
    {
        if (encounter.isStaticEncounter || cancelLoopCompletionSource != null) return;

        ongoingLoopCompletionSource = new UniTaskCompletionSource();
        cancelLoopCompletionSource = new UniTaskCompletionSource();

        EncounterView nextLoopPart = null;

        while (cancelLoopCompletionSource.Task.Status == UniTaskStatus.Pending)
        {
            playerSprite.SetRunning(true);

            var currentLoopPart = currentEncounter.encounter;
            nextLoopPart = GetNewEncounterViewInstance(encounter, ENCOUNTER_DISTANCE);

            await UniTask.WhenAll(
                MoveEncounterPositionTo(new Vector3(), nextLoopPart),
                MoveEncounterPositionTo(new Vector3(-ENCOUNTER_DISTANCE, 0, 0), currentLoopPart)
            );

            Destroy(currentLoopPart.gameObject);
            currentEncounter.encounter = nextLoopPart;

            playerSprite.SetRunning(false);
        }

        ongoingLoopCompletionSource.TrySetResult();
        ongoingLoopCompletionSource = null;
        cancelLoopCompletionSource = null;

        await MoveEncounterPositionTo(new Vector3(-ENCOUNTER_DISTANCE, 0, 0), nextLoopPart);
        Destroy(nextLoopPart.gameObject);
    }

    private async UniTask MoveEncounterPositionTo(Vector3 targetPosition, EncounterView view)
    {
        while (view.transform.localPosition != targetPosition)
        {
            view.transform.localPosition = Vector3.MoveTowards(view.transform.localPosition, targetPosition, ENCOUNTER_TRANSITION_SPEED * Time.deltaTime);
            await UniTask.WaitForEndOfFrame();
        }
    }

    private EncounterView GetNewEncounterViewInstance(EncounterState encounterState, float distance)
    {
        var encounterView = Instantiate(encounterTemplate, transform);
        encounterView.transform.localPosition = new Vector3(distance, 0, 0);
        encounterView.gameObject.name = encounterState.encounterName;
        encounterView.OnEncounterStateUpdate(encounterState);

        return encounterView;
    }
}
