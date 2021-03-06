// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameModeInfoCustomizer.h"


/**
 * Implements details panel customizations for AWorldSettings fields.
 */
class FWorldSettingsDetails
	: public IDetailCustomization
{
public:

	// IDetailCustomization interface

	virtual void CustomizeDetails( IDetailLayoutBuilder& DetailBuilder ) override;

public:

	/**
	 * Makes a new instance of this detail layout class.
	 *
	 * @return The created instance.
	 */
	static TSharedRef<IDetailCustomization> MakeInstance( )
	{
		return MakeShareable(new FWorldSettingsDetails);
	}

protected:

	/**
	 * Customizes an AGameInfo property with the given name.
	 *
	 * @param PropertyName The property to customize.
	 * @param DetailBuilder The detail builder.
	 * @param CategoryBuilder The category builder
	 */
	void CustomizeGameInfoProperty( const FName& PropertyName, IDetailLayoutBuilder& DetailBuilder, IDetailCategoryBuilder& CategoryBuilder );

	/**
	 * Adds the lightmap customization to the Lightmass section
	 *
	 * @param DetailBuilder The detail builder.
	 */
	void AddLightmapCustomization( IDetailLayoutBuilder& DetailBuilder );

private:

	// Handles checking whether a given asset is acceptable for drag-and-drop.
	bool HandleAssetDropTargetIsAssetAcceptableForDrop( const UObject* InObject ) const;

	// Handles dropping an asset.
	void HandleAssetDropped( UObject* Object, TSharedRef<IPropertyHandle> GameInfoProperty );


	/** Helper class to customizer GameMode property */
	TSharedPtr<FGameModeInfoCustomizer>	GameInfoModeCustomizer;
};


/** Custom struct for each group of arguments in the function editing details */
class FLightmapCustomNodeBuilder : public IDetailCustomNodeBuilder, public TSharedFromThis<FLightmapCustomNodeBuilder>
{
public:
	FLightmapCustomNodeBuilder(const TSharedPtr<FAssetThumbnailPool>& InThumbnailPool);
	~FLightmapCustomNodeBuilder();

protected:

	// IDetailCustomNodeBuilder interface
	virtual void SetOnRebuildChildren( FSimpleDelegate InOnRegenerateChildren ) override;
	virtual void GenerateHeaderRowContent( FDetailWidgetRow& NodeRow ) override;
	virtual void GenerateChildContent( IDetailChildrenBuilder& ChildrenBuilder ) override;
	virtual void Tick( float DeltaTime ) override {}
	virtual bool RequiresTick() const override { return false; }
	virtual FName GetName() const override { return FName(TEXT("Lightmaps")); }
	virtual bool InitiallyCollapsed() const override { return false; }

private:

	struct FLightmapItem
	{
		FString ObjectPath;
		TSharedPtr<class FAssetThumbnail> Thumbnail;

		FLightmapItem(const FString& InObjectPath, const TSharedPtr<class FAssetThumbnail>& InThumbnail)
			: ObjectPath(InObjectPath)
			, Thumbnail(InThumbnail)
		{}
	};

	/** Handler for the lightmap count text in the right hand column */
	FText GetLightmapCountText() const;

	/** Handler for when lighting has been rebuilt and kept */
	void HandleLightingBuildKept();

	/** Handler for when the map changed or was rebuilt */
	void HandleMapChanged(uint32 MapChangeFlags);

	/** Handler for when the current level changes */
	void HandleNewCurrentLevel();

	/** Handler for light map list view widget creation */
	TSharedRef<ITableRow> MakeLightMapListViewWidget(TSharedPtr<FLightmapItem> AssetItem, const TSharedRef<STableViewBase>& OwnerTable);

	/** Handler for context menus */
	TSharedPtr<SWidget> OnGetLightMapContextMenuContent();

	/** Handler for double clicking an item */
	void OnLightMapListMouseButtonDoubleClick(TSharedPtr<FLightmapItem> SelectedLightmap);

	/** Handler for when "View" is selected in the light map list */
	void ExecuteViewLightmap(FString SelectedLightmapPath);

	/** Refreshes the list of lightmaps to display */
	void RefreshLightmapItems();

private:

	/** Delegate to handle refreshing this group */
	FSimpleDelegate OnRegenerateChildren;

	/** The list view showing light maps in this world */
	TSharedPtr<SListView<TSharedPtr<FLightmapItem>>> LightmapListView;
	TArray<TSharedPtr<FLightmapItem>> LightmapItems;
	TSharedPtr<class FAssetThumbnailPool> ThumbnailPool;
};
