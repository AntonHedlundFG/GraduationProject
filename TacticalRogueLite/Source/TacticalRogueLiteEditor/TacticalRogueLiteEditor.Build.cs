using UnrealBuildTool;

public class TacticalRogueLiteEditor : ModuleRules
{
    public TacticalRogueLiteEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        // TactialRogueLite needed for the objects we’re visualizing
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "Engine", "CoreUObject", "TacticalRogueLite" });
        // Needed for our editor logic
        PrivateDependencyModuleNames.AddRange(new string[] { "UnrealEd" });
    }
}