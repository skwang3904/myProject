#include "Room.h"
#include "RgTile.h"
#include "Player.h"

#include <math.h>



void loadRoomTile()
{
	
}

void drawRoomTile(float dt)
{
	int i, num = RGTILE_X * RGTILE_Y;
	for (i = 0; i < num; i++)
	{
		if (RgTile[i] == MOVETILE) setRGBA(MOVETILE_RGBA);
		else if (RgTile[i] == WALLTILE)setRGBA(WALLTILE_RGBA);
		else if (RgTile[i] == FALLTILE)setRGBA(FALLTILE_RGBA);

		fillRect(RGTILE_Width * (i % RGTILE_X), RGTILE_Height * (i / RGTILE_X), 
			RGTILE_Width, RGTILE_Height);
	}
		setRGBA(1, 1, 1, 1);
}

float findMoveTile(Player* pc, int x, int y)
{
	float min = 0xffff;
	if (RgTile[RGTILE_X * y + x] != MOVETILE)
		return min; 

	float distance = fabs(iPointLength(iPointMake(RGTILE_Width* x + RGTILE_Width/2, 
		RGTILE_Height * y+ RGTILE_Height/2) - pc->playerPosition));

	if(min > distance)
		return distance;
	return min;
}

void findMoveTile(Player* pc)
{
	int x = (int)(pc->playerPosition.x + HALF_OF_TEX_WIDTH) / RGTILE_Width;
	int y = (int)(pc->playerPosition.y + HALF_OF_TEX_HEIGHT) / RGTILE_Height;
	
	float minD= 0xffff;
	float dis;

	int pcX = x;
	int pcY = y;

	for (int i = 0; ; i++)
	{
		// x-1, y-1,	x, y-1,		x+1, y-1
		// x-1, y					x+1, y
		// x-1, y+1		x, y+1		x+1, y+1

		dis = findMoveTile(pc, x - i, y - i);
		if (dis < minD)
		{
			minD = dis;
			pcX = x - i;
			pcY = y - i;
		}
		dis = findMoveTile(pc, x - i, y);
		if (dis < minD)
		{
			minD = dis;
			pcX = x - i;
			pcY = y;
		}
		dis = findMoveTile(pc, x - i, y + i);
		if (dis < minD)
		{
			minD = dis;
			pcX = x - i;
			pcY = y + i;
		}

		dis = findMoveTile(pc, x, y - i);
		if (dis < minD)
		{
			minD = dis;
			pcX = x;
			pcY = y - i;
		}
		dis = findMoveTile(pc, x, y + i);
		if (dis < minD)
		{
			minD = dis;
			pcX = x;
			pcY = y + i;
		}

		dis = findMoveTile(pc, x + i, y - i);
		if (dis < minD)
		{
			minD = dis;
			pcX = x + i;
			pcY = y - i;
		}
		dis = findMoveTile(pc, x + i, y);
		if (dis < minD)
		{
			minD = dis;
			pcX = x + i;
			pcY = y;
		}

		dis = findMoveTile(pc, x + i, y + i);
		if (dis < minD)
		{
			minD = dis;
			pcX = x + i;
			pcY = y + i;
		}

		if (minD != 0xffff)
			break;
	}

	pc->playerPosition = iPointMake(RGTILE_Width * pcX + HALF_OF_TEX_WIDTH,
		RGTILE_Height * pcY + HALF_OF_TEX_HEIGHT);
}


void wallCheck2(Player* pc, iPoint mp)
{
	// 이동시 벽 감지
	int i, j;

	if (mp.x < 0)
	{
		int LX = pc->playerPosition.x;								LX /= RGTILE_Width;
		int TLY = pc->playerPosition.y;								TLY /= RGTILE_Height;
		int BLY = pc->playerPosition.y + HALF_OF_TEX_HEIGHT * 2.0f;	BLY /= RGTILE_Height;
		int min = 0;
		for (i = LX - 1; i > -1; i--)
		{
			bool stop = false;
			for (j = TLY; j < BLY + 1; j++)
			{
				if (RgTile[RGTILE_X * j + i] == WALLTILE)
				{
					stop = true;
					min = RGTILE_Width * (i + 1);
					break;
				}
			}
			if (stop)
				break;
		}
		pc->playerPosition.x += mp.x;
		if (pc->playerPosition.x < min)
			pc->playerPosition.x = min;
	}
	else if (mp.x > 0)
	{
		int RX = pc->playerPosition.x + HALF_OF_TEX_WIDTH * 2.0f;		RX /= RGTILE_Width;
		int TRY = pc->playerPosition.y;									TRY /= RGTILE_Height;
		int BRY = pc->playerPosition.y + HALF_OF_TEX_HEIGHT * 2.0f; 	BRY /= RGTILE_Height;
		int max = RGTILE_X * RGTILE_Width;
		for (i = RX + 1; i < RGTILE_X; i++)
		{
			bool stop = false;
			for (j = TRY; j < BRY + 1; j++)
			{
				if (RgTile[RGTILE_X * j + i] == WALLTILE)
				{
					stop = true;
					max = RGTILE_Width * i - 1;
					break;
				}
			}
			if (stop)
				break;
		}

		pc->playerPosition.x += mp.x;
		if (pc->playerPosition.x > max - HALF_OF_TEX_WIDTH * 2.0f)
			pc->playerPosition.x = max - HALF_OF_TEX_WIDTH * 2.0f;
	}

	if (mp.y < 0)
	{
		int TY = pc->playerPosition.y;								TY /= RGTILE_Height;
		int TLX = pc->playerPosition.x;								TLX /= RGTILE_Width;
		int TRX = pc->playerPosition.x + HALF_OF_TEX_WIDTH * 2.0f;	TRX /= RGTILE_Width;
		int min = 0;
		for (j = TY - 1; j > -1; j--)
		{
			bool stop = false;
			for (i = TLX; i < TRX + 1; i++)
			{
				if (RgTile[RGTILE_X * j + i] == WALLTILE)
				{
					stop = true;
					min = RGTILE_Height * (j + 1);
					break;
				}
			}
			if (stop)
				break;
		}
		pc->playerPosition.y += mp.y;
		if (pc->playerPosition.y < min)
			pc->playerPosition.y = min;
	}
	else if (mp.y > 0)
	{
		int BY = pc->playerPosition.y + HALF_OF_TEX_HEIGHT * 2.0f;	   	BY /= RGTILE_Height;
		int BLX = pc->playerPosition.x;									BLX /= RGTILE_Width;
		int BRX = pc->playerPosition.x + HALF_OF_TEX_WIDTH * 2.0f;		BRX /= RGTILE_Width;
		int max = RGTILE_Y * RGTILE_Height;

		for (j = BY + 1; j < RGTILE_Y; j++)
		{
			bool stop = false;
			for (i = BLX; i < BRX + 1; i++)
			{
				if (RgTile[RGTILE_X * j + i] == WALLTILE)
				{
					stop = true;
					max = RGTILE_Height * j - 1;
					break;
				}
			}
			if (stop)
				break;
		}

		pc->playerPosition.y += mp.y;
		if (pc->playerPosition.y > max - HALF_OF_TEX_HEIGHT * 2.0f)
			pc->playerPosition.y = max - HALF_OF_TEX_HEIGHT * 2.0f;
	}

}

void wallCheck(Player* pc, iPoint mp)
{
	wallCheck2(pc, mp);
}

bool fallCheck(Player* pc, float dt)
{
	// 임시 - 낭떨어지에 진입시 가장 가까이있는 타일로 이동  - 어색함
	// 라이프 감소
	// 잠시 무적


	int x = (pc->playerPosition.x + HALF_OF_TEX_WIDTH) / RGTILE_Width;
	int y = (pc->playerPosition.y + HALF_OF_TEX_HEIGHT) / RGTILE_Height;
	
	static bool falling = false;

	if (RgTile[RGTILE_X * y + x] == FALLTILE)
	{
		if (falling == false)
			pc->img[8]->startAnimation();


		if (pc->img[8]->animation == false)
		{
			falling = false;
			//이동
			findMoveTile(pc);
			return false;
		}

		falling = true;
		iPoint p = iPointMake(	pc->playerPosition.x - HALF_OF_TEX_WIDTH,
								pc->playerPosition.y - HALF_OF_TEX_HEIGHT);

		pc->img[8]->paint(dt, p, REVERSE_NONE);

		return true;
	}
	else
	{
		falling = false;
		return false;
	}


	
}

