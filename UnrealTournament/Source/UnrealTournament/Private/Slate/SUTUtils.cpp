// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "../Public/UnrealTournament.h"
#include "../Public/UTLocalPlayer.h"
#include "SUTUtils.h"
#include "SUWindowsStyle.h"

TSharedRef<SToolTip> SUTUtils::CreateTooltip(const TAttribute<FText>& Text)
{
	return SNew(SToolTip)
		.Text(Text)
		.BorderImage(FCoreStyle::Get().GetBrush("ToolTip.BrightBackground"))
		.TextMargin(FMargin(11.0f));
}
