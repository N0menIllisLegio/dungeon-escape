#include "Graphics.h"

Graphics::Graphics(HWND window, shared_ptr<Level> currentLevel)
{
	bool spritesLoaded = true;
	spritesLoaded &= LoadBitmapFromFile("sprites/wall.bmp", BlockType::Unwalkable);
	spritesLoaded &= LoadBitmapFromFile("sprites/floor.bmp", BlockType::Walkable);
	spritesLoaded &= LoadBitmapFromFile("sprites/start.bmp", BlockType::Start);
	spritesLoaded &= LoadBitmapFromFile("sprites/end.bmp", BlockType::Finish);
	spritesLoaded &= LoadBitmapFromFile("sprites/player.bmp", BlockType::Trouper);
	spritesLoaded &= LoadBitmapFromFile("sprites/hud.bmp", BlockType::Hud);
	spritesLoaded &= LoadBitmapFromFile("sprites/fog.bmp", BlockType::Fog);

	if (!spritesLoaded)
	{
		string finishText = "Sprites are missing!\n";
		finishText += "In root directry of the game must be a folder 'sprites' with next sprites:\n";
		finishText += "wall.bmp, floor.bmp, start.bmp, end.bmp, player.bmp, hud.bmp, fog.bmp";
		if (MessageBox(window, finishText.c_str(), "Error!", MB_OK | MB_ICONERROR) == IDOK)
		{
			exit(EXIT_FAILURE);
		}
	}

	this->window = window;

	HDC hdc = GetDC(this->window);
	backbufferDC = CreateCompatibleDC(hdc);

	RECT windowSize = { 0,  0, BLOCK_WIDTH * currentLevel->COLUMNS, BLOCK_HEIGHT * (currentLevel->ROWS) };

	windowWidth = windowSize.right - windowSize.left;
	windowHeight = windowSize.bottom - windowSize.top;

	AdjustWindowRect(&windowSize, WS_OVERLAPPEDWINDOW, false);

	int MAPWIDTH = windowSize.right - windowSize.left;
	int MAPHEIGHT = windowSize.bottom - windowSize.top;
	int ScreenX = (GetSystemMetrics(SM_CXSCREEN) - MAPWIDTH) / 2;
	int ScreenY = (GetSystemMetrics(SM_CYSCREEN) - MAPHEIGHT) / 2;

	SetWindowPos(window, HWND_NOTOPMOST, ScreenX, ScreenY, MAPWIDTH,
		MAPHEIGHT, SWP_SHOWWINDOW);

	backbufferBitmap = CreateCompatibleBitmap(hdc, windowWidth, windowHeight);

	oldObject = SelectObject(backbufferDC, backbufferBitmap);

	SetBkMode(backbufferDC, TRANSPARENT);

	bitmapDC = CreateCompatibleDC(hdc);
}


Graphics::~Graphics()
{
	for (auto& bm : bitmaps)
	{
		FreeBitmap(bm);
	}

	DeleteDC(this->bitmapDC);
	DeleteObject(this->backbufferBitmap);
	SelectObject(this->backbufferDC, oldObject);
	DeleteDC(this->backbufferDC);
}

bool Graphics::LoadBitmapFromFile(const string filename, BlockType resource)
{
	Bitmap bitmap;
	GDIBitmap gdi;
	gdi.handle = (HBITMAP)LoadImage(0, filename.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (gdi.handle == 0)
	{
		return false;
	}

	BITMAP header;
	GetObject(gdi.handle, sizeof(header), &header);
	gdi.width = header.bmWidth;
	gdi.height = header.bmHeight;

	BLOCK_HEIGHT = header.bmHeight;
	BLOCK_WIDTH = header.bmWidth;

	gdiBitmaps.push_back(gdi);
	bitmap.index = gdiBitmaps.size() - 1;
	bitmaps.push_back(bitmap);
	bitmapDictionary[resource] = bitmap;

	return true;
}

void Graphics::EndGraphics()
{
	HDC windowDC = GetDC(window);
	BitBlt(windowDC, 0, 0, windowWidth, windowHeight, backbufferDC, 0, 0, SRCCOPY);
	ReleaseDC(window, windowDC);
}

void Graphics::FreeBitmap(Bitmap bitmap)
{
	auto findResult = find_if(begin(bitmapDictionary), end(bitmapDictionary), [&](const pair<BlockType, Bitmap>& pair)
	{
		return pair.second.index == bitmap.index;
	});
	BlockType foundKey;
	if (findResult != end(bitmapDictionary))
	{
		foundKey = findResult->first;

		bitmapDictionary.erase(foundKey);
	}

	if (gdiBitmaps[bitmap.index].handle != 0)
	{
		DeleteObject(gdiBitmaps[bitmap.index].handle);
		gdiBitmaps[bitmap.index].handle = 0;
	}
}

bool Graphics::ShowMessage(string message)
{
	if (MessageBox(this->window, message.c_str(), "Freedom at last...", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Graphics::DrawLevel(shared_ptr<Level> currentLevel)
{
	for (int y = 0; y < currentLevel->ROWS - 1; y++)
	{
		for (int x = 0; x < currentLevel->COLUMNS; x++)
		{
			auto resource = currentLevel->fogMap[y][x];
			DrawBitmap(bitmapDictionary.at(resource), x * BLOCK_WIDTH, y * BLOCK_HEIGHT);
		}
	}
}

void Graphics::DrawHud(shared_ptr<Level> currentLevel)
{
	int y = currentLevel->ROWS - 1;
	for (int x = 0; x < currentLevel->COLUMNS; x++)
	{
		DrawBitmap(bitmapDictionary.at(BlockType::Hud), x * BLOCK_WIDTH, y * BLOCK_HEIGHT);
	}
}

void Graphics::BeginGraphics()
{
	RECT rectangle = { 0, 0, windowWidth, windowHeight };
	FillRect(backbufferDC, &rectangle, (HBRUSH)COLOR_BACKGROUND);
}

void Graphics::DrawBitmap(Bitmap bitmap, int x, int y)
{
	const GDIBitmap& gdi = gdiBitmaps.at(bitmap.index);
	SelectObject(bitmapDC, gdi.handle);
	BitBlt(backbufferDC, x, y, gdi.width, gdi.height, bitmapDC, 0, 0, SRCCOPY);
}

void Graphics::DrawString(string text, COLORREF color, int x, int y)
{
	SetTextColor(backbufferDC, color);
	TextOut(backbufferDC, x, y, text.c_str(), text.size());
}