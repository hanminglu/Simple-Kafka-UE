// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class SimpleKafka : ModuleRules
{

    private string ThirdPartyPath
    {
        get { return Path.GetFullPath(Path.Combine(ModuleDirectory, "../../ThirdParty/")); }
    }

    public bool LoadRdKafkaLib(ReadOnlyTargetRules Target)
    {
        PublicIncludePaths.Add(ThirdPartyPath + "RdKafka/include");
        PublicSystemIncludePaths.Add(ThirdPartyPath + "RdKafka/include");

        bool isLibararySupported = false;
        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            isLibararySupported = true;
            PublicSystemLibraryPaths.Add(ThirdPartyPath + "RdKafka/x64_v142/lib/");

            var kafkalibDir = Path.Combine(ThirdPartyPath, "RdKafka/x64_v142/lib/");
            var libPaths = Directory.GetFiles(kafkalibDir, "*.lib");
            PublicAdditionalLibraries.AddRange(libPaths);

            //RuntimeDependencies.Add(Path.Combine(ModuleDirectory, "../../Binaries/Win64/librdkafka.dll"));
            //RuntimeDependencies.Add(Path.Combine(ModuleDirectory, "../../Binaries/Win64/librdkafkacpp.dll"));
            //RuntimeDependencies.Add(Path.Combine(ModuleDirectory, "../../Binaries/Win64/zlib.dll"));

            RuntimeDependencies.Add("$(TargetOutputDir)/zlib.dll", Path.Combine(ModuleDirectory, "../../Binaries/Win64/zlib.dll"));
            RuntimeDependencies.Add("$(TargetOutputDir)/librdkafka.dll", Path.Combine(ModuleDirectory, "../../Binaries/Win64/librdkafka.dll"));
            RuntimeDependencies.Add("$(TargetOutputDir)/librdkafkacpp.dll", Path.Combine(ModuleDirectory, "../../Binaries/Win64/librdkafkacpp.dll"));

        }
        return isLibararySupported;

    }


    public SimpleKafka(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        CppStandard = CppStandardVersion.Cpp17;

        bLegacyPublicIncludePaths = false;
		PublicIncludePaths.AddRange(
			new string[] {
                
            }
            );
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
                "zlib",
                "OpenSSL",
                "Projects",
				// ... add other public dependencies that you statically link with here ...
	
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				// ... add private dependencies that you statically link with here ...	
				"Sockets",
                "Networking",
            }
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
		new string[]
		{
			// ... add any modules that your module loads dynamically here ...
		}
		);

        // Enable exceptions to allow error handling
        bEnableExceptions = true;
        LoadRdKafkaLib(Target);
    }
}
