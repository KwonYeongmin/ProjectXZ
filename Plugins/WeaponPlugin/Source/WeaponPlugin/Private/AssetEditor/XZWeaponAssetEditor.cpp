#include "AssetEditor/XZWeaponAssetEditor.h"
#include "AssetEditor/XZWeaponLeftArea.h"
#include "ProjectXZ/Weapon/XZDA_Weapon.h"

const FName FXZWeaponAssetEditor::EditorName = "WeaponAssetEditor";
const FName FXZWeaponAssetEditor::LeftAreaTabId = "LeftArea";
const FName FXZWeaponAssetEditor::DetailTabId = "Details";

TSharedPtr< FXZWeaponAssetEditor > FXZWeaponAssetEditor::Instance = nullptr;

void FXZWeaponAssetEditor::OpenWindow(FString InAssetName)
{
	if (Instance.IsValid())
	{
		Instance->OnClose();

		Instance.Reset();
		Instance = nullptr;
	}

	Instance = MakeShareable(new FXZWeaponAssetEditor());
	Instance->Open(InAssetName);
}

void FXZWeaponAssetEditor::Shutdown()
{
	if (Instance.IsValid())
	{
		Instance->OnClose();

		Instance.Reset();
		Instance = nullptr;
	}
}

// â�� ���� �� ���� �ǰ� ����.
bool FXZWeaponAssetEditor::OnRequestClose()
{
	if (LeftArea.IsValid())
	{
		LeftArea.Reset();
	}

	if (DetailsView.IsValid())
	{
		// AssetEditorSubsystem�ȿ�(=DetailView�ȿ�) GetEditingObject()�� ��ϵǾ� �־��ٸ� �����ϰ� Editor�� �˸���.
		if (IsValid(GEditor) && IsValid(GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()))
		{
			GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->NotifyAssetClosed(GetEditingObject(), this);
		}

		DetailsView.Reset();
	}

	return true; // true ����(=â ����), false ����(= â�� ���� �� ����)
}

void FXZWeaponAssetEditor::Open(FString InAssetName)
{
	//**************************************************************************************
	//** LeftArea
	//	 XZWeaponLeftArea���� ���� �ڷ����� �����Ͽ� �־��ش�.
	LeftArea = SNew(XZWeaponLeftArea) 
		.WeaponSelectedItem(this, &FXZWeaponAssetEditor::OnLeftAreaSelectedItem); //LeftArea���� ������ ������
	//**************************************************************************************

	//**************************************************************************************
	//** DetailsView
	FPropertyEditorModule& Prop = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	FDetailsViewArgs Args(false, false, true, FDetailsViewArgs::HideNameArea);
	Args.ViewIdentifier = "WeaponAssetEditorDetailsView"; //�ĺ��� ����. ���� Editor�ʿ��� DetailView ���ٽ� �� �ĺ��ڷ� ã�� �� �ִ�.

	DetailsView = Prop.CreateDetailView(Args);
	//**************************************************************************************

	//**************************************************************************************
	//**  Layout ����
	TSharedRef<FTabManager::FLayout> Layout = FTabManager::NewLayout("WeaponAssetEditor_Layout")
		->AddArea
		(
			FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
			->Split
			(
				FTabManager::NewSplitter()->SetOrientation(Orient_Horizontal)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.175f) // ���� 17.5% ���
					->AddTab(LeftAreaTabId, ETabState::OpenedTab) // LeftAreaTabId
					->SetHideTabWell(true)
				)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.725f) // ������ 72.5% ���
					->AddTab(DetailTabId, ETabState::OpenedTab) // DetailTabId
					->SetHideTabWell(true)
				)
			)
		);
	//**************************************************************************************

	UXZDA_Weapon* DA_Weapon = LeftArea->GetFirstDataPtr()->DA_Weapon; // LeftArea�� �� �� ù��° ������ ����

	FAssetEditorToolkit::InitAssetEditor(EToolkitMode::Standalone, TSharedPtr<IToolkitHost>(), EditorName, Layout, true, true, DA_Weapon);

	LeftArea->SelectedDataAsset(DA_Weapon);
}

void FXZWeaponAssetEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	FOnSpawnTab Tab_LeftArea;
	Tab_LeftArea.BindSP(this, &FXZWeaponAssetEditor::Spawn_LeftAreaTab);
	TabManager->RegisterTabSpawner(LeftAreaTabId, Tab_LeftArea);

	FOnSpawnTab Tab_DetailsView;
	Tab_DetailsView.BindSP(this, &FXZWeaponAssetEditor::Spawn_DetailsViewTab);
	TabManager->RegisterTabSpawner(DetailTabId, Tab_DetailsView);
}

TSharedRef<SDockTab> FXZWeaponAssetEditor::Spawn_LeftAreaTab(const FSpawnTabArgs& InArgs)
{
	return SNew(SDockTab)
		[
			LeftArea.ToSharedRef()
		];
}

TSharedRef<SDockTab> FXZWeaponAssetEditor::Spawn_DetailsViewTab(const FSpawnTabArgs& InArgs)
{
	return SNew(SDockTab)
		[
			DetailsView.ToSharedRef()
		];
}

FName FXZWeaponAssetEditor::GetToolkitFName() const
{
	return EditorName;
}

FText FXZWeaponAssetEditor::GetBaseToolkitName() const
{
	return FText::FromName(EditorName);
}

FString FXZWeaponAssetEditor::GetWorldCentricTabPrefix() const
{
	return EditorName.ToString();
}

FLinearColor FXZWeaponAssetEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor(0, 0, 1);
}

void FXZWeaponAssetEditor::OnLeftAreaSelectedItem(FWeaponRowDataPtr InRowData)
{
	if (nullptr == InRowData) return; // LeftArea���� �����Ѱ� ���ٸ�(�Ǵ� �� ������ �����ߴٸ�)

	if (IsValid(GetEditingObject())) // �����ϴ� ��ü�� �ִٸ�
	{
		RemoveEditingObject(GetEditingObject()); // ���� â���� �������� �ֵ� ����
	}

	AddEditingObject(InRowData->DA_Weapon);		 // ���� â�� �������� ��ü�� ���
	DetailsView->SetObject(InRowData->DA_Weapon);// â ���� DetailsView�� ����
}
