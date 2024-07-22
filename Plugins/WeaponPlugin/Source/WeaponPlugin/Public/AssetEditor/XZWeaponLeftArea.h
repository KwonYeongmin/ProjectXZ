#pragma once
#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Views/STableRow.h"

struct FWeaponRowData
{
	int Number;
	FString Name;
	class UXZDA_Weapon* DA_Weapon;

	FWeaponRowData() { }
	FWeaponRowData(int32 InNumber, FString InName, class UXZDA_Weapon* InAsset)
		: Number(InNumber), Name(InName), DA_Weapon(InAsset) {	}

	static TSharedPtr<FWeaponRowData> Make(int32 InNumber, FString InName, class UXZDA_Weapon* InAsset)
	{
		return MakeShareable(new FWeaponRowData(InNumber, InName, InAsset));
	}
};
typedef TSharedPtr<FWeaponRowData> FWeaponRowDataPtr; // ���� ����� �ڷ���


class WEAPONPLUGIN_API XZWeaponTableRow : public SMultiColumnTableRow<FWeaponRowDataPtr>
{
public:
	SLATE_BEGIN_ARGS(XZWeaponTableRow) {}
	SLATE_ARGUMENT(FWeaponRowDataPtr, RowData)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTable);

protected:
	TSharedRef<SWidget> GenerateWidgetForColumn(const FName& InColumnName) override;

private:
	FWeaponRowDataPtr Data;
};

// �̺�Ʈ ��������Ʈ. FWeaponRowDataPtr�� �������� ȣ��.
DECLARE_DELEGATE_OneParam(FWeaponListViewSelectedItemSignature, FWeaponRowDataPtr /* */) 

/** Slate UI â�� ���� �κ��� ����ϴ� Ŭ����
 * 
 */
class WEAPONPLUGIN_API XZWeaponLeftArea : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(XZWeaponLeftArea) {}
	SLATE_EVENT(FWeaponListViewSelectedItemSignature, WeaponSelectedItem)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	void SelectedDataAsset(UXZDA_Weapon* InDA_Weapon); // ������ XZDA_Weapon
	FWeaponRowDataPtr GetFirstDataPtr() { return RowDatas[0]; }

private:
	TSharedRef<ITableRow> OnGenerateRow(FWeaponRowDataPtr InRow, const TSharedRef<STableViewBase>& InTable);

	void ReadDataAssetList();
	void OnTextChanged(const FText& InText);
	void OnTextCommitted(const FText& InText, ETextCommit::Type InType);
	void OnSelectedChanged(FWeaponRowDataPtr InDataPtr, ESelectInfo::Type InType);
	FText OnGetAssetCount() const;

	FWeaponListViewSelectedItemSignature WeaponListViewSelectedItemDelegate;

	TArray<FWeaponRowDataPtr> RowDatas;
	TSharedPtr<SListView<FWeaponRowDataPtr>> ListView;

	TSharedPtr< SSearchBox > SearchBox;
	FText SearchText; // �˻��� ���ڿ��� ������ ����
};
