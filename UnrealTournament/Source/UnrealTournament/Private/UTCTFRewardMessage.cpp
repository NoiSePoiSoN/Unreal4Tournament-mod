// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "UnrealTournament.h"
#include "UTLocalMessage.h"
#include "UTCTFRewardMessage.h"
#include "GameFramework/LocalMessage.h"

UUTCTFRewardMessage::UUTCTFRewardMessage(const class FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	bIsUnique = true;
	Importance = 0.8f;
	bIsSpecial = true;
	Lifetime = 3.0f;
	MessageArea = FName(TEXT("GameMessages"));
	bIsConsoleMessage = false;
	AssistMessage = NSLOCTEXT("CTFRewardMessage", "Assist", "Assist!");
	DeniedMessage = NSLOCTEXT("CTFRewardMessage", "Denied", "Denied!");

	bIsStatusAnnouncement = false;
}

FLinearColor UUTCTFRewardMessage::GetMessageColor(int32 MessageIndex) const
{
	return FLinearColor::Yellow;
}


void UUTCTFRewardMessage::PrecacheAnnouncements_Implementation(UUTAnnouncer* Announcer) const
{
	for (int32 i = 0; i < 3; i++)
	{
		Announcer->PrecacheAnnouncement(GetAnnouncementName(i, NULL));
	}
}

float UUTCTFRewardMessage::GetScaleInSize(int32 MessageIndex) const
{
	return 3.f;
}

float UUTCTFRewardMessage::GetAnnouncementDelay(int32 Switch)
{
	return (Switch == 2) ? 1.5f : 0.f;
}

FName UUTCTFRewardMessage::GetAnnouncementName_Implementation(int32 Switch, const UObject* OptionalObject) const
{
	switch (Switch)
	{
	case 0: return TEXT("Denied"); break;
	case 1: return TEXT("LastSecondSave"); break;
	case 2: return TEXT("Assist"); break;
	}
	return NAME_None;
}

void UUTCTFRewardMessage::ClientReceive(const FClientReceiveData& ClientData) const
{
	Super::ClientReceive(ClientData);
	if ((ClientData.RelatedPlayerState_1 != NULL && ClientData.LocalPC == ClientData.RelatedPlayerState_1->GetOwner())
	 ||  (ClientData.RelatedPlayerState_2 != NULL && ClientData.LocalPC == ClientData.RelatedPlayerState_2->GetOwner()))
	{
		AUTPlayerController* PC = Cast<AUTPlayerController>(ClientData.LocalPC);
		if (PC != NULL && PC->RewardAnnouncer != NULL)
		{
			PC->RewardAnnouncer->PlayAnnouncement(GetClass(), ClientData.MessageIndex, ClientData.OptionalObject);
		}
	}
}

FText UUTCTFRewardMessage::GetText(int32 Switch, bool bTargetsPlayerState1, APlayerState* RelatedPlayerState_1, APlayerState* RelatedPlayerState_2, UObject* OptionalObject) const
{
	switch (Switch)
	{
	case 0: return DeniedMessage; break;
	case 2: return AssistMessage; break;
	}

	return FText::GetEmpty();
}

