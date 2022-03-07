#include "glDraw.h"
// LOL you forgot to switch to matrix mode GL PROJECTION and as a result, we somehow stopped recieving text after the first injection upon starting the game
int viewport[4];

void GL::SetupOrtho()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glViewport(0, 0, viewport[2], viewport[3]);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, viewport[2], viewport[3], 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);
}
void GL::RestoreGL()
{
	glPopMatrix();
	glPopAttrib();


}

void GL::DrawFilledRect(float x, float y, float width, float height, const GLubyte color[3])
{
	glColor3ub(color[0], color[1], color[2]);
	glBegin(GL_QUADS);
	glVertex2f(x, y); //top left corner, you usually want to start drawing from the top left and draw clockwise
	glVertex2f(x + width, y); // top right corner
	glVertex2f(x + width, y + height); // bottom right corner
	glVertex2f(x, y + height); // bottom left corner
	glEnd();
}
void GL::DrawOutline(float x, float y, float width, float height, float lineWidth, const GLubyte color[3])
{
	glLineWidth(lineWidth);
	glBegin(GL_LINE_STRIP);
	glColor3ub(color[0], color[1], color[2]);
	glVertex2f(x - 0.5f, y - 0.5f); //top left corner, you usually want to start drawing from the top left and draw clockwise
	glVertex2f(x + width + 0.5f, y - 0.5f); // top right corner
	glVertex2f(x + width + 0.5f, y + height + 0.5f);
	glVertex2f(x - 0.5f, y + height + 0.5f); // bottom right corner
	glVertex2f(x - 0.5f, y - 0.5f); // bottom left corner
	glEnd();
	/*
		just imagine that we're drawing a rectangle with an outline drawn on it
		/////////
		/       /
		/       /
		/       /
		/////////
	*/
}

//fix stupid viewport displacement

void GL::DrawESPBox(Ent* p, Ent* localplayer, Vector3L vec)
{
	const GLubyte* color = nullptr;

	if (p->TeamIdentify == localplayer->TeamIdentify)
	{
		color = rgb::green;
	}
	else
	{
		color = rgb::red;
	}

	float dist = localplayer->BodyPos.Distance(p->BodyPos);

	float scale = (GAME_UNIT_MAGIC / dist) * (viewport[2] / VIRTUAL_SCREEN_WIDTH);
	float x = vec.x - scale;
	float y = vec.y - scale * PLAYER_ASPECT_RATIO;
	float width = scale * 2;
	float height = scale * PLAYER_ASPECT_RATIO * 2;


	GL::DrawOutline(x, y, width, height, 2.0f, color);

	


}