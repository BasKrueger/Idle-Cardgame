public class FastAttackCardModel : CardModel
{
    protected override string localizationKey => "FastAttack";
    protected override int baseDamage => 1;
    protected override int tickCooldown => 5;

    protected override void Play(CharacterModel source, CharacterModel target)
    {
        interactions.Add(new AttackInteraction(source, target, FullDamage));
    }
}
