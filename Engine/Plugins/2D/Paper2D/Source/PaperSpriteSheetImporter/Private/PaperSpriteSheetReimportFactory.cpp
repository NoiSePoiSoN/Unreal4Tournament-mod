// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "PaperSpriteSheetImporterPrivatePCH.h"
#include "PaperSpriteSheetReimportFactory.h"
#include "PaperSpriteSheet.h"

#define LOCTEXT_NAMESPACE "PaperJsonImporter"

//////////////////////////////////////////////////////////////////////////
// UPaperSpriteSheetReimportFactory

UPaperSpriteSheetReimportFactory::UPaperSpriteSheetReimportFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SupportedClass = UPaperSpriteSheet::StaticClass();

	bCreateNew = false;
}

bool UPaperSpriteSheetReimportFactory::CanReimport(UObject* Obj, TArray<FString>& OutFilenames)
{
	UPaperSpriteSheet* SpriteSheet = Cast<UPaperSpriteSheet>(Obj);
	if (SpriteSheet && SpriteSheet->AssetImportData)
	{
		OutFilenames.Add(FReimportManager::ResolveImportFilename(SpriteSheet->AssetImportData->SourceFilePath, SpriteSheet));
		return true;
	}
	return false;
}

void UPaperSpriteSheetReimportFactory::SetReimportPaths(UObject* Obj, const TArray<FString>& NewReimportPaths)
{
	UPaperSpriteSheet* SpriteSheet = Cast<UPaperSpriteSheet>(Obj);
	if (SpriteSheet && ensure(NewReimportPaths.Num() == 1))
	{
		SpriteSheet->AssetImportData->SourceFilePath = FReimportManager::ResolveImportFilename(NewReimportPaths[0], SpriteSheet);
	}
}

EReimportResult::Type UPaperSpriteSheetReimportFactory::Reimport(UObject* Obj)
{
	UPaperSpriteSheet* SpriteSheet = Cast<UPaperSpriteSheet>(Obj);
	if (!SpriteSheet)
	{
		return EReimportResult::Failed;
	}

	// Make sure file is valid and exists
	const FString Filename = FReimportManager::ResolveImportFilename(SpriteSheet->AssetImportData->SourceFilePath, SpriteSheet);
	if (!Filename.Len() || IFileManager::Get().FileSize(*Filename) == INDEX_NONE)
	{
		return EReimportResult::Failed;
	}

	// Configure the importer with the reimport settings
	Importer.SetReimportData(SpriteSheet->SpriteNames, SpriteSheet->Sprites);
	Importer.ExistingBaseTextureName = SpriteSheet->TextureName;
	Importer.ExistingBaseTexture = SpriteSheet->Texture;
	Importer.ExistingNormalMapTextureName = SpriteSheet->NormalMapTextureName;
	Importer.ExistingNormalMapTexture = SpriteSheet->NormalMapTexture;

	// Run the import again
	EReimportResult::Type Result = EReimportResult::Failed;
	if (UFactory::StaticImportObject(SpriteSheet->GetClass(), SpriteSheet->GetOuter(), *SpriteSheet->GetName(), RF_Public | RF_Standalone, *Filename, nullptr, this))
	{
		UE_LOG(LogPaperSpriteSheetImporter, Log, TEXT("Imported successfully"));
		// Try to find the outer package so we can dirty it up
		if (SpriteSheet->GetOuter())
		{
			SpriteSheet->GetOuter()->MarkPackageDirty();
		}
		else
		{
			SpriteSheet->MarkPackageDirty();
		}
		Result = EReimportResult::Succeeded;
	}
	else
	{
		UE_LOG(LogPaperSpriteSheetImporter, Warning, TEXT("-- import failed"));
		Result = EReimportResult::Failed;
	}

	return Result;
}

int32 UPaperSpriteSheetReimportFactory::GetPriority() const
{
	return ImportPriority;
}

//////////////////////////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE
