// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "UnrealTournament.h"
#include "UTReplicatedGameRuleset.h"
#include "Net/UnrealNetwork.h"

#if !UE_SERVER
#include "Slate/SUWindowsStyle.h"
#endif

AUTReplicatedGameRuleset::AUTReplicatedGameRuleset(const class FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	SetRemoteRoleForBackwardsCompat(ROLE_SimulatedProxy);
	bReplicates = true;
	bAlwaysRelevant = true;
	bReplicateMovement = false;
	bNetLoadOnClient = false;
}

void AUTReplicatedGameRuleset::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AUTReplicatedGameRuleset, UniqueTag);
	DOREPLIFETIME(AUTReplicatedGameRuleset, Categories);
	DOREPLIFETIME(AUTReplicatedGameRuleset, Title);
	DOREPLIFETIME(AUTReplicatedGameRuleset, Tooltip);
	DOREPLIFETIME(AUTReplicatedGameRuleset, Description);
	DOREPLIFETIME(AUTReplicatedGameRuleset, MapPlaylist);
	DOREPLIFETIME(AUTReplicatedGameRuleset, MaxPlayers);
	DOREPLIFETIME(AUTReplicatedGameRuleset, DisplayTexture);
	DOREPLIFETIME(AUTReplicatedGameRuleset, MapPlaylistSize);
}

void AUTReplicatedGameRuleset::SetRules(UUTGameRuleset* NewRules)
{
	UniqueTag = NewRules->UniqueTag;
	Categories = NewRules->Categories;
	Title = NewRules->Title;
	Tooltip = NewRules->Tooltip;
	Description = Fixup(NewRules->Description);
	MapPlaylistSize = FMath::Clamp<int32>(NewRules->MapPlaylistSize, 1, 6);
	MapPlaylist = NewRules->MapPlaylist;
	MaxPlayers = NewRules->MaxPlayers;

	DisplayTexture = NewRules->DisplayTexture;
	GameMode = NewRules->GameMode;
	GameOptions = NewRules->GameOptions;

	BuildSlateBadge();
}

FString AUTReplicatedGameRuleset::Fixup(FString OldText)
{
	FString Final = OldText.Replace(TEXT("\\n"), TEXT("\n"), ESearchCase::IgnoreCase);
	Final = Final.Replace(TEXT("\\n"), TEXT("\n"), ESearchCase::IgnoreCase);

	return Final;
}


void AUTReplicatedGameRuleset::BuildSlateBadge()
{
#if !UE_SERVER
	UTexture2D* BadgeTexture = LoadObject<UTexture2D>(nullptr, *DisplayTexture, nullptr, LOAD_None, nullptr);
	SlateBadge = MakeShareable( new FSlateDynamicImageBrush(BadgeTexture, FVector2D(256.0f, 256.0f), FName(TEXT("SlateBadge"))) );
#endif
}

#if !UE_SERVER
const FSlateBrush* AUTReplicatedGameRuleset::GetSlateBadge() const
{
	if (SlateBadge.IsValid()) 
	{
		return SlateBadge.Get();
	}
	else
	{
		return SUWindowsStyle::Get().GetBrush("UWindows.Lobby.MatchBadge");	
	}
}
#endif

void AUTReplicatedGameRuleset::GotTag()
{
	UE_LOG(UT,Log,TEXT("GotTag: %s"), *UniqueTag);
}
