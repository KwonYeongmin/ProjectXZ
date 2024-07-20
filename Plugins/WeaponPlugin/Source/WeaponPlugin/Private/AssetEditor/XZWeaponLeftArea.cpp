#include "AssetEditor/XZWeaponLeftArea.h"
#include "EditorStyleSet.h"
#include "EngineUtils.h"
#include "ProjectXZ/Weapon/XZDA_Weapon.h"
#include "Widgets/Input/SSearchBox.h"

void XZWeaponTableRow::Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTable)
{
	Data = InArgs._RowData;//InArgs�� ���ؼ� ���´�.

	SMultiColumnTableRow<FWeaponRowDataPtr>::Construct
	(
		FSuperRowType::FArguments().Style(FAppStyle::Get(), "TableView.DarkRow"), InOwnerTable
	);
}

TSharedRef<SWidget> XZWeaponTableRow::GenerateWidgetForColumn(const FName& InColumnName)
{
	// InColumnName�� �Ʒ� Construct ������ SHeaderRow::Column("�̸�, ��ȣ ��")�� ����.
	FString Str;
	if (InColumnName == "Number") // ����
	{
		Str = FString::FromInt(Data->Number); //str�� ���ڸ� �־��ش�.
	}
	else if (InColumnName == "Name") // �̸�
	{
		Str = Data->Name; // Str�� �̸��� �־��ش�.
	}

	return SNew(STextBlock)
		.Text(FText::FromString(Str)); // ����� ����str�� �����Ͽ� ����.
}

void XZWeaponLeftArea::Construct(const FArguments& InArgs)
{
	WeaponListViewSelectedItemDelegate = InArgs._WeaponSelectedItem;

	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
			.AutoHeight().Padding(2, 0)
			[
				SAssignNew(SearchBox, SSearchBox) // SearchBox �����Ҵ�
					.SelectAllTextWhenFocused(true)
					.OnTextChanged(this, &XZWeaponLeftArea::OnTextChanged)
					.OnTextCommitted(this, &XZWeaponLeftArea::OnTextCommitted)
			]
		+ SVerticalBox::Slot()
			.FillHeight(1) // 1�� 100%�� �ǹ�(=������ �� ä��ڴٴ� �ǹ�)
			[
				SAssignNew(ListView, SListView<FWeaponRowDataPtr>) // ListView �����Ҵ�. �ڷ����� FWeaponRowDataPtr
					.HeaderRow
					(
						SNew(SHeaderRow)
						+ SHeaderRow::Column("Number")
							.DefaultLabel(FText::FromString(""))
							.ManualWidth(40) // ĭ�� �ʺ�
						+ SHeaderRow::Column("Name")
							.DefaultLabel(FText::FromString("Name"))
					)
					.ListItemsSource(&RowDatas)
					.OnGenerateRow(this, &XZWeaponLeftArea::OnGenerateRow) // �������� ��� ǥ������ ����� ���ش޶�� �ǹ�.
					.OnSelectionChanged(this, &XZWeaponLeftArea::OnSelectedChanged)
			]
		+ SVerticalBox::Slot()
			.AutoHeight()
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Right)
			.Padding(FMargin(8, 2))
			[
				SNew(STextBlock)
					.Text(this, &XZWeaponLeftArea::OnGetAssetCount) // const TAttribute<FText>. ������ text ������ �ٲ�� �ڵ����� �������ش�. Attribute�� Ư�� �����̴�. Attribute�� �Լ��� �� �� �ִ�. ���⼭�� &SWeaponLeftArea::OnGetAssetCount.
			]
	];

	ReadDataAssetList();
}

void XZWeaponLeftArea::SelectedDataAsset(UXZDA_Weapon* InDA_Weapon)
{
	if (RowDatas.Num() <= 0) return; // XZDA_Weapon�� �ϳ��� ���� ��� return

	for (FWeaponRowDataPtr RowData : RowDatas)
	{
		if (RowData->DA_Weapon == InDA_Weapon)
		{
			ListView->SetSelection(RowData);
			return;
		}
	}
}

TSharedRef<ITableRow> XZWeaponLeftArea::OnGenerateRow(FWeaponRowDataPtr InRow, const TSharedRef<STableViewBase>& InTable)
{
	/*��������� ����� ����*/
	return SNew(XZWeaponTableRow, InTable)
		.RowData(InRow);
}

void XZWeaponLeftArea::ReadDataAssetList()
{
	RowDatas.Empty();//�����͸� �ҷ����� ���� ����ְ� ����.

	// TODO: ��� ���� ������
	// "/Game"�� ������ ����. BP���� ������ ��� ���µ��� ã�� objects�� �����ϰڴ�.
	// ATL_Class�� Ŭ���� Ÿ��, ATL_Regular�� ������ Ÿ��(������ �ҷ��;� �ϹǷ� �̰� ���).
	TArray<UObject*> Objects;
	EngineUtils::FindOrLoadAssetsByPath("/Game/BP/", Objects, EngineUtils::ATL_Regular);

	int32 Idx = 0;
	for (UObject* obj : Objects)
	{
		UXZDA_Weapon* XZDA_Weapon = Cast<UXZDA_Weapon>(obj);
		if (false == IsValid(XZDA_Weapon)) continue;

		FString DataAssetName = XZDA_Weapon->GetName();
		if (false == SearchText.IsEmpty()) // �˻��ϴ� ���ڿ�(=SearchText) ��������� �ʴٸ�
		{
			// �ش� DataAssetName�� SearchText ���ڿ��� ���Ե��� �ʴ´ٸ� �����ְ� �ƴϹǷ� continue.
			if (false == DataAssetName.Contains(SearchText.ToString())) continue;
		}

		RowDatas.Add(FWeaponRowData::Make(++Idx, DataAssetName, XZDA_Weapon)); // �����͸� �ϳ��� �־��ش�.
	}

	RowDatas.Sort([](const FWeaponRowDataPtr& A, const FWeaponRowDataPtr& B)
		{
			return A->Number < B->Number; // �������� ����
		});

	ListView->RequestListRefresh(); // �ٸ������� ����ϱ� ������ �簻�Ÿ� ���ش�.
}

void XZWeaponLeftArea::OnTextChanged(const FText& InText)
{
	// ����ó��. ���� ���ڿ��� ���� �Էµ� ���ڿ��� ���ٸ� ����.
	if (SearchText.CompareToCaseIgnored(InText) == 0) return;

	SearchText = InText;
	ReadDataAssetList(); // ��˻�
}

void XZWeaponLeftArea::OnTextCommitted(const FText& InText, ETextCommit::Type InType)
{
	OnTextChanged(InText);
}

void XZWeaponLeftArea::OnSelectedChanged(FWeaponRowDataPtr InDataPtr, ESelectInfo::Type InType)
{
	// ����ó��. �� ���� Ŭ�� �� null�� ���� �����Ƿ� null�� �� ����.
	if (false == InDataPtr.IsValid()) return;

	WeaponListViewSelectedItemDelegate.ExecuteIfBound(InDataPtr);
}

FText XZWeaponLeftArea::OnGetAssetCount() const
{
	FString str = FString::Printf(TEXT("%d Asset"), RowDatas.Num()); // ����

	return FText::FromString(str);
}
