#include "XZHUD.h"

void AXZHUD::DrawHUD()
{
	Super::DrawHUD();

	FVector2D ViewportSize;
	if (IsValid(GEngine))
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
		const FVector2D ViewportCenter(ViewportSize.X / 2.0f, ViewportSize.Y / 2.0f);

		// Crosshair �׸���
		DrawCrosshair(CrosshairTexture2D, ViewportCenter, FLinearColor::White);
	}
}

void AXZHUD::DrawCrosshair(UTexture2D* InTexture, FVector2D ViewportCenter, FLinearColor CrosshairColor)
{
	const float TextureWidth = InTexture->GetSizeX();  // Texture �ʺ�
	const float TextureHeight = InTexture->GetSizeY(); // Texture ����

	// Texture �׸��� ��ġ ����
	const FVector2D TextureDrawPoint(
		ViewportCenter.X - (TextureWidth / 2.0f),
		ViewportCenter.Y - (TextureHeight / 2.0f)
	);

	// Texture �׸���
	DrawTexture(InTexture, TextureDrawPoint.X, TextureDrawPoint.Y, TextureWidth, TextureHeight, 0.0f, 0.0f, 1.0f, 1.0f, CrosshairColor);
}
