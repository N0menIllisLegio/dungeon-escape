#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <Windows.h>
#include <algorithm>
#include <vector>
#include <map>
#include <string>

#include "BlockType.h"
#include "Command.h"
#include "Bitmap.h"
#include "Player.h"
#include "Level.h"

using namespace std;

class Graphics
{
public:
	Graphics(HWND window, shared_ptr<Level> currentLevel);
	~Graphics();

	bool ShowMessage(string message);

	void BeginGraphics();
	void DrawBitmap(Bitmap bitmap, int x, int y);
	void DrawString(string text, COLORREF color, int x, int y);
	void DrawLevel(shared_ptr<Level> currentLevel);
	void DrawHud(shared_ptr<Level> currentLevel);
	void EndGraphics();

	int BLOCK_WIDTH;
	int BLOCK_HEIGHT;
	map<BlockType, Bitmap> bitmapDictionary;

private:
	bool LoadBitmapFromFile(const string filename, BlockType resource);
	void FreeBitmap(Bitmap bitmap);
	
	HWND window;
	int windowWidth;
	int windowHeight;
	HDC backbufferDC;
	HBITMAP backbufferBitmap;
	HDC bitmapDC;
	
	struct GDIBitmap
	{
		HBITMAP handle;
		int width;
		int height;

		GDIBitmap() : handle(0) { }
	};
	
	vector<GDIBitmap> gdiBitmaps;
	vector<Bitmap> bitmaps;
	HGDIOBJ oldObject;
};
#endif