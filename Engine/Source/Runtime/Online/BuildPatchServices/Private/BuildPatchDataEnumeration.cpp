// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "BuildPatchServicesPrivatePCH.h"

#if WITH_BUILDPATCHGENERATION

bool FBuildDataEnumeration::EnumerateManifestData(FString ManifestFilePath, FString OutputFile)
{
	bool bSuccess = false;
	TArray<FGuid> DataList;
	FBuildPatchAppManifestRef AppManifest = MakeShareable(new FBuildPatchAppManifest());
	GLog->Logf(ELogVerbosity::Log, TEXT("Load manifest %s"), *ManifestFilePath);
	const double StartLoadManifest = FPlatformTime::Seconds();
	if (AppManifest->LoadFromFile(ManifestFilePath))
	{
		const double LoadManifestTime = FPlatformTime::Seconds() - StartLoadManifest;
		GLog->Logf(ELogVerbosity::Log, TEXT("Loaded manifest in %.1f seconds"), LoadManifestTime);

		// Enumerate
		AppManifest->GetDataList(DataList);

		// Output text file
		GLog->Log(ELogVerbosity::Log, TEXT("Data file list:-"));
		FString FullList;
		for (FGuid& DataGuid : DataList)
		{
			FString Path = FBuildPatchUtils::GetDataFilename(AppManifest, FString(), DataGuid);
			FullList += Path;
			FullList += TEXT("\r\n");
			GLog->Log(ELogVerbosity::Log, Path);
		}
		if (FFileHelper::SaveStringToFile(FullList, *OutputFile))
		{
			GLog->Logf(ELogVerbosity::Log, TEXT("Saved out to %s"), *OutputFile);
			bSuccess = true;
		}
		else
		{
			GLog->Logf(ELogVerbosity::Error, TEXT("Failed to save output %s"), *OutputFile);
		}
	}
	else
	{
		GLog->Logf(ELogVerbosity::Error, TEXT("Failed to load manifest %s"), *ManifestFilePath);
	}
	return bSuccess;
}

#endif //WITH_BUILDPATCHGENERATION