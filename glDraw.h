#pragma once
#pragma comment(lib, "OpenGL32.lib")
#include "Winp.h"
#include <gl\GL.h>
#include "mMem.h"
#include "sig32.h"
#include "geom.h"

const int VIRTUAL_SCREEN_WIDTH = 800;
const int GAME_UNIT_MAGIC = 400;

const float PLAYER_HEIGHT = 5.25f;
const float PLAYER_WIDTH = 2.0f;
const float EYE_HEIGHT = 4.5f;

const float PLAYER_ASPECT_RATIO = PLAYER_HEIGHT / PLAYER_WIDTH;

const int ESP_FONT_HEIGHT = 15;
const int ESP_FONT_WIDTH = 9;
extern int viewport[4];

namespace rgb
{
	const GLubyte red[3] = { 255, 0, 0 };
	const GLubyte green[3] = { 0,255, 0 };
	const GLubyte gray[3] = { 55, 55, 55 };
	const GLubyte lightgray[3] = { 192, 192, 192 };
	const GLubyte black[3] = { 0, 0, 0 };
}

namespace GL
{
	void SetupOrtho();
	void RestoreGL();

	void DrawFilledRect(float x, float y, float width, float height, const GLubyte color[3]);
	void DrawOutline(float x, float y, float width, float height, float lineWidth, const GLubyte color[3]);

	void DrawESPBox(Ent* p, Ent* localplayer, Vector3L vec);

}