public class AttackCardModel : CardModel
{
    protected override int baseDamage => 5;
    protected override int tickCooldown => 10;
    protected override string localizationKey => "Attack";

    protected override void Play(CharacterModel source, CharacterModel target)
    {
        interactions.Add(new AttackInteraction(source, target, FullDamage));
    }
}
