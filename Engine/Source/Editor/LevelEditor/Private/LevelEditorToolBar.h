// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.


#ifndef __LevelEditorToolBar_h__
#define __LevelEditorToolBar_h__

#pragma once


#include "LevelEditor.h"


/**
 * Unreal level editor main toolbar
 */
class FLevelEditorToolBar
{

public:

	/**
	 * Static: Creates a widget for the main tool bar
	 *
	 * @return	New widget
	 */
	static TSharedRef< SWidget > MakeLevelEditorToolBar( const TSharedRef<FUICommandList>& InCommandList, const TSharedRef<SLevelEditor> InLevelEditor );


protected:

	/**
	 * Generates menu content for the build combo button drop down menu
	 *
	 * @return	Menu content widget
	 */
	static TSharedRef< SWidget > GenerateBuildMenuContent( TSharedRef<FUICommandList> InCommandList );

	/**
	 * Generates menu content for the create actor combo button drop down menu
	 *
	 * @return	Menu content widget
	 */
	static TSharedRef< SWidget > GenerateCreateContent( TSharedRef<FUICommandList> InCommandList );

	/**
	 * Generates menu content for the quick settings combo button drop down menu
	 *
	 * @return	Menu content widget
	 */
	static TSharedRef< SWidget > GenerateQuickSettingsMenu( TSharedRef<FUICommandList> InCommandList );

	/**
	 * Generates menu content for the source control combo button drop down menu
	 *
	 * @return	Menu content widget
	 */
	static TSharedRef< SWidget > GenerateSourceControlMenu(TSharedRef<FUICommandList> InCommandList);

	/**
	 * Generates menu content for the compile combo button drop down menu
	 *
	 * @return	Menu content widget
	 */
	static TSharedRef< SWidget > GenerateOpenBlueprintMenuContent( TSharedRef<FUICommandList> InCommandList, TWeakPtr< SLevelEditor > InLevelEditor );

	/**
	 * Generates menu content for the matinee combo button drop down menu
	 *
	 * @return	Menu content widget
	 */
	static TSharedRef< SWidget > GenerateMatineeMenuContent( TSharedRef<FUICommandList> InCommandList, TWeakPtr<SLevelEditor> LevelEditorWeakPtr );

	/**
	 * Delegate for actor selection within the Matinee popup menu's SceneOutliner.
	 * Opens the matinee editor for the selected actor and dismisses all popup menus.
	 */
	static void OnMatineeActorPicked( AActor* Actor );

	/**
	 * Callback to open a sub-level script Blueprint
	 *
	 * @param InLevel	The level to open the Blueprint of (creates if needed)
	 */
	static void OnOpenSubLevelBlueprint( ULevel* InLevel );
};



#endif		// __LevelEditorToolBar_h__
