#pragma once

#include "iStd.h"

/*
타일 상호작용
*/

void loadRoomTile();
void drawRoomTile(float dt);

void wallCheck(bool checkFall, iPoint& pos,iPoint mp, float halfOfTexW, float halfOfTexH);
bool fallCheck(Player* pc, float dt);

/*
void wallCheck2(bool checkFall, iPoint& pos, iPoint mp, float halfOfTexW, float halfOfTexH)
{
	// 이동시 벽 감지
	int i, j;

	if (mp.x < 0)
	{
		int LX = pos.x;							LX /= RGTILE_Width;
		int TLY = pos.y;						TLY /= RGTILE_Height;
		int BLY = pos.y + halfOfTexH * 2.0f;	BLY /= RGTILE_Height;
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

				if (checkFall)
				{
					if (RgTile[RGTILE_X * j + i] == FALLTILE)
					{
						stop = true;
						min = RGTILE_Width * (i + 1);
						break;
					}
				}
			}
			if (stop)
				break;
		}
		pos.x += mp.x;
		if (pos.x < min)
			pos.x = min;
	}
	else if (mp.x > 0)
	{
		int RX = pos.x + halfOfTexW * 2.0f;		RX /= RGTILE_Width;
		int TRY = pos.y;						TRY /= RGTILE_Height;
		int BRY = pos.y + halfOfTexH * 2.0f; 	BRY /= RGTILE_Height;
		int max = RGTILE_X * RGTILE_Width * 2;
		for (i = RX + 1; i < RGTILE_X*2; i++)
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

					if (checkFall)
					{
						if (RgTile[RGTILE_X * j + i] == FALLTILE)
						{
							stop = true;
							max = RGTILE_Width * i - 1;
							break;
						}
					}
			}
			if (stop)
				break;
		}

		pos.x += mp.x;
		if (pos.x > max - halfOfTexW * 2.0f)
			pos.x = max - halfOfTexW * 2.0f;
	}

	if (mp.y < 0)
	{
		int TY = pos.y;							TY /= RGTILE_Height;
		int TLX = pos.x;						TLX /= RGTILE_Width;
		int TRX = pos.x + halfOfTexW * 2.0f;	TRX /= RGTILE_Width;
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

				if (checkFall)
				{
					if (RgTile[RGTILE_X * j + i] == FALLTILE)
					{
						stop = true;
						min = RGTILE_Height * (j + 1);
						break;
					}
				}
			}
			if (stop)
				break;
		}
		pos.y += mp.y;
		if (pos.y < min)
			pos.y = min;
	}
	else if (mp.y > 0)
	{
		int BY =  pos.y + halfOfTexH * 2.0f;	BY /= RGTILE_Height;
		int BLX = pos.x;						BLX /= RGTILE_Width;
		int BRX = pos.x + halfOfTexW * 2.0f;	BRX /= RGTILE_Width;
		int max = RGTILE_Y * RGTILE_Height * 2;

		for (j = BY + 1; j < RGTILE_Y*2; j++)
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

				if (checkFall)
				{
					if (RgTile[RGTILE_X * j + i] == FALLTILE)
					{
						stop = true;
						max = RGTILE_Height * j - 1;
						break;
					}
				}
			}
			if (stop)
				break;
		}

		pos.y += mp.y;
		if (pos.y > max - halfOfTexH * 2.0f)
			pos.y = max - halfOfTexH * 2.0f;
	}
}
*/