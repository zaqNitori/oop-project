/*
 * mygame.cpp: 本檔案儲遊戲本身的class的implementation
 * Copyright (C) 2002-2008 Woei-Kae Chen <wkc@csie.ntut.edu.tw>
 *
 * This file is part of game, a free game development framework for windows.
 *
 * game is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * game is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * History:
 *   2002-03-04 V3.1
 *          Add codes to demostrate the use of CMovingBitmap::ShowBitmap(CMovingBitmap &).
 *	 2004-03-02 V4.0
 *      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
 *         demonstrate the use of states.
 *      2. Demo the use of CInteger in CGameStateRun.
 *   2005-09-13
 *      Rewrite the codes for CBall and CEraser.
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *      1. Add codes to display IDC_GAMECURSOR in GameStateRun.
 *   2006-02-08 V4.2
 *      1. Revise sample screens to display in English only.
 *      2. Add code in CGameStateInit to demo the use of PostQuitMessage().
 *      3. Rename OnInitialUpdate() -> OnInit().
 *      4. Fix the bug that OnBeginState() of GameStateInit is not called.
 *      5. Replace AUDIO_CANYON as AUDIO_NTUT.
 *      6. Add help bitmap to CGameStateRun.
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *   2006-12-30
 *      1. Bug fix: fix a memory leak problem by replacing PostQuitMessage(0) as
 *         PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE,0,0).
 *   2008-02-15 V4.4
 *      1. Add namespace game_framework.
 *      2. Replace the demonstration of animation as a new bouncing ball.
 *      3. Use ShowInitProgress(percent) to display loading progress. 
 *   2010-03-23 V4.6
 *      1. Demo MP3 support: use lake.mp3 to replace lake.wav.
*/

#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "mygame.h"
#include <cstdlib> 
#include <ctime>   

#define Blue RGB(0,0,255)
#define Black RGB(0,0,0)

namespace game_framework {

/////////////////////////////////////////////////////////////////////////////
// CHero: Hero class
/////////////////////////////////////////////////////////////////////////////
	
#pragma region CHero

	CHero::CHero()
	{
		Initialize();
	}

	void CHero::Initialize()
	{
		mapX = 0;
		mapY = SIZE_Y - 721;
		x = 200;
		y = 400;
		constDelay = 9;
		delayCount = constDelay;
		direction = dir_horizontal = 1;				//預設向左
		isFalling = isRising = isMovingDown = isMovingLeft = isMovingRight = isMovingUp = isShooting = isMapLock = false;
	}

	void CHero::LoadBitmap()
	{
	#pragma region fileInput
		char *fileDefaultStand[] = { ".\\image\\stand\\L1.bmp" };
		char *fileDefaultCrouch[] = { ".\\image\\crouch\\L1.bmp" };

		char *fileKnifeL1[] = { ".\\image\\knife\\L1-1.bmp" , ".\\image\\knife\\L2-1.bmp" , ".\\image\\knife\\L3-1.bmp" , ".\\image\\knife\\L4-1.bmp" };
		char *fileKnifeL2[] = { ".\\image\\knife\\L1-2.bmp" , ".\\image\\knife\\L2-2.bmp" , ".\\image\\knife\\L3-2.bmp" , ".\\image\\knife\\L4-2.bmp" };
		char *fileKnifeR[] = { ".\\image\\knife\\R1.bmp" , ".\\image\\knife\\R2.bmp" , ".\\image\\knife\\R3.bmp" , ".\\image\\knife\\R4.bmp" };


#pragma region fileStand
		char *filestandL[] = { ".\\image\\stand\\L1.bmp",".\\image\\stand\\L2.bmp",".\\image\\stand\\L3.bmp" , ".\\image\\stand\\L4.bmp" };
		char *filestandR[] = { ".\\image\\stand\\R1.bmp",".\\image\\stand\\R2.bmp",".\\image\\stand\\R3.bmp" , ".\\image\\stand\\R4.bmp" };
		char *filestandShootL[] = { ".\\image\\stand\\shoot\\L1-1.bmp" , ".\\image\\stand\\shoot\\L2-1.bmp" , ".\\image\\stand\\shoot\\L3-1.bmp" , ".\\image\\stand\\shoot\\L4-1.bmp" };
		char *filestandShootL2[] = { ".\\image\\stand\\shoot\\L1-2.bmp" , ".\\image\\stand\\shoot\\L2-2.bmp" , ".\\image\\stand\\shoot\\L3-2.bmp" , ".\\image\\stand\\shoot\\L4-2.bmp" };
		char *filestandShootR[] = { ".\\image\\stand\\shoot\\R1.bmp" , ".\\image\\stand\\shoot\\R2.bmp" , ".\\image\\stand\\shoot\\R3.bmp" , ".\\image\\stand\\shoot\\R4.bmp" };

#pragma endregion


	#pragma region fileMove
		
		char *filemoveL[] = { ".\\image\\move\\L1.bmp",".\\image\\move\\L2.bmp",".\\image\\move\\L3.bmp" , ".\\image\\move\\L4.bmp" };
		char *filemoveR[] = { ".\\image\\move\\R1.bmp",".\\image\\move\\R2.bmp",".\\image\\move\\R3.bmp" , ".\\image\\move\\R4.bmp" };
		char *fileMoveShootL[] = { ".\\image\\move\\shoot\\L1-1.bmp" , ".\\image\\move\\shoot\\L2-1.bmp" , ".\\image\\move\\shoot\\L3-1.bmp" , ".\\image\\move\\shoot\\L4-1.bmp" };
		char *fileMoveShootL2[] = { ".\\image\\move\\shoot\\L1-2.bmp" , ".\\image\\move\\shoot\\L2-2.bmp" , ".\\image\\move\\shoot\\L3-2.bmp" , ".\\image\\move\\shoot\\L4-2.bmp" };
		char *fileMoveShootR[] = { ".\\image\\move\\shoot\\R1.bmp" , ".\\image\\move\\shoot\\R2.bmp" , ".\\image\\move\\shoot\\R3.bmp" , ".\\image\\move\\shoot\\R4.bmp" };

	#pragma endregion
		
	#pragma region fileJump
		
		char *fileRiseL[] = { ".\\image\\jump\\L1.bmp" , ".\\image\\jump\\L2.bmp" , ".\\image\\jump\\L3.bmp" , ".\\image\\jump\\L4.bmp" , ".\\image\\jump\\L5.bmp" };
		char *fileFallL[] = { ".\\image\\jump\\L7-1.bmp" , ".\\image\\jump\\L7-2.bmp" };
		char *fileRiseR[] = { ".\\image\\jump\\R1.bmp" , ".\\image\\jump\\R2.bmp" , ".\\image\\jump\\R3.bmp" , ".\\image\\jump\\R4.bmp" , ".\\image\\jump\\R5.bmp" };
		char *fileFallR[] = { ".\\image\\jump\\R7-1.bmp" , ".\\image\\jump\\R7-2.bmp" };
		char *fileJumpShootL[] = { ".\\image\\jump\\shoot\\L1-1.bmp" , ".\\image\\jump\\shoot\\L2-1.bmp" , ".\\image\\jump\\shoot\\L3-1.bmp" , ".\\image\\jump\\shoot\\L4-1.bmp" };
		char *fileJumpShootL2[] = { ".\\image\\jump\\shoot\\L1-2.bmp" , ".\\image\\jump\\shoot\\L2-2.bmp" , ".\\image\\jump\\shoot\\L3-2.bmp" , ".\\image\\jump\\shoot\\L4-2.bmp" };
		char *fileJumpShootR[] = { ".\\image\\jump\\shoot\\R1.bmp" , ".\\image\\jump\\shoot\\R2.bmp" , ".\\image\\jump\\shoot\\R3.bmp" , ".\\image\\jump\\shoot\\R4.bmp" };

	#pragma endregion

	#pragma region fileCrouch

		char *fileCrouchL[] = { ".\\image\\crouch\\L1.bmp" };
		char *fileCrouchR[] = { ".\\image\\crouch\\R1.bmp" };
		char *fileCrouchMoveL[] = { ".\\image\\crouch\\move\\L1.bmp" , ".\\image\\crouch\\move\\L2.bmp" , ".\\image\\crouch\\move\\L3.bmp" , ".\\image\\crouch\\move\\L4.bmp" };
		char *fileCrouchMoveR[] = { ".\\image\\crouch\\move\\R1.bmp" , ".\\image\\crouch\\move\\R2.bmp" , ".\\image\\crouch\\move\\R3.bmp" , ".\\image\\crouch\\move\\R4.bmp" };
		char *fileCrouchShootL[] = { ".\\image\\crouch\\shoot\\L1-1.bmp" , ".\\image\\crouch\\shoot\\L2-1.bmp" , ".\\image\\crouch\\shoot\\L3-1.bmp" , ".\\image\\crouch\\shoot\\L4-1.bmp" };
		char *fileCrouchShootL2[] = { ".\\image\\crouch\\shoot\\L1-2.bmp" , ".\\image\\crouch\\shoot\\L2-2.bmp" , ".\\image\\crouch\\shoot\\L3-2.bmp" , ".\\image\\crouch\\shoot\\L4-2.bmp" };
		char *fileCrouchShootR[] = { ".\\image\\crouch\\shoot\\R1.bmp" , ".\\image\\crouch\\shoot\\R2.bmp" , ".\\image\\crouch\\shoot\\R3.bmp" , ".\\image\\crouch\\shoot\\R4.bmp" };

	#pragma endregion

		char *fileHeart[] = { ".\\image\\heart.bmp" };
	#pragma endregion

	#pragma region 動畫載入
		CDefaultStand.LoadBitmap(fileDefaultStand[0], Blue);
		CDefaultCrouch.LoadBitmap(fileDefaultCrouch[0], Blue);
		CHeart.LoadBitmap(fileHeart[0], Black);

		heroCrouch.LoadBitmap_StandL(fileCrouchL[0]);
		heroCrouch.LoadBitmap_StandR(fileCrouchR[0]);
		for (int i = 0; i < 4; i++)
		{
			heroStand.LoadBitmap_StandL(filestandL[i]);
			heroStand.LoadBitmap_StandR(filestandR[i]);
			heroMove.LoadBitmap_MoveL(filemoveL[i]);
			heroMove.LoadBitmap_MoveR(filemoveR[i]);
			heroCrouch.LoadBitmap_MoveL(fileCrouchMoveL[i]);
			heroCrouch.LoadBitmap_MoveR(fileCrouchMoveR[i]);
			heroCrouch.LoadBitmap_ShootL(fileCrouchShootL[i], fileCrouchShootL2[i]);
			heroCrouch.LoadBitmap_ShootR(fileCrouchShootR[i]);
			heroJump.LoadBitmap_ShootL(fileJumpShootL[i], fileJumpShootL2[i]);
			heroJump.LoadBitmap_ShootR(fileJumpShootR[i]);
			heroMove.LoadBitmap_ShootL(fileMoveShootL[i], fileMoveShootL2[i]);
			heroMove.LoadBitmap_ShootR(fileMoveShootR[i]);
			heroStand.LoadBitmap_ShootL(filestandShootL[i], filestandShootL2[i]);
			heroStand.LoadBitmap_ShootR(filestandShootR[i]);

			//knife
			heroMove.LoadBitmap_KnifeL(fileKnifeL1[i], fileKnifeL2[i]);
			heroMove.LoadBitmap_KnifeR(fileKnifeR[i]);
			heroStand.LoadBitmap_KnifeL(fileKnifeL1[i], fileKnifeL2[i]);
			heroStand.LoadBitmap_KnifeR(fileKnifeR[i]);
			heroJump.LoadBitmap_KnifeL(fileKnifeL1[i], fileKnifeL2[i]);
			heroJump.LoadBitmap_KnifeR(fileKnifeR[i]);
			heroCrouch.LoadBitmap_KnifeL(fileKnifeL1[i], fileKnifeL2[i]);
			heroCrouch.LoadBitmap_KnifeR(fileKnifeR[i]);
		}
		for (int i = 0; i < 5; i++)
		{
			heroJump.LoadBitmap_RiseL(fileRiseL[i]);
			heroJump.LoadBitmap_RiseR(fileRiseR[i]);
		}
		for (int i = 0; i < 2; i++)
		{
			heroJump.LoadBitmap_FallL(fileFallL[i]);
			heroJump.LoadBitmap_FallR(fileFallR[i]);
		}
	#pragma endregion

	}

#pragma region privateFunction
	void CHero::gravity()
	{
		int gx = (x - mapX) / gameMap->getSize();
		int gy = (y - mapY + CDefaultStand.Height()) / gameMap->getSize();
		if (!isRising && !isFalling)
		{
			if (!gameMap->getMapBlock(gy + 1, gx))
			{
				heroJump.SetFalling(true);
				isFalling = true;
			}
		}
	}

	void CHero::gameMap_OnMove()
	{
		if (isMovingDown && !isRising)		//根據不同動作給予不同default圖片以方便判斷
		{
			defaultW = CDefaultCrouch.Width();
			defaultH = CDefaultCrouch.Height();
		}
		else
		{
			defaultW = CDefaultStand.Width();
			defaultH = CDefaultStand.Height();
			heroJump.SetDefaultHeight(defaultH);
		}
		if (!isMapLock)					//Boss Stage MapLock
		{
			if (dir_horizontal == 1)			//向左走
			{
				if (x < 200)			//當超過左側自由移動範圍時
				{
					mapX -= x - 200;	//取得超過左側邊界的位移量，並且反向加至地圖座標
					if (mapX >= 0)
						mapX = 0;			//如果在地圖最左側則可以無視移動邊界
					else x = 200;		//反之則會被卡住
				}
				if (x <= 0) x = 0;
				//mapEdge = min(x, 200);
			}
			else if (dir_horizontal == 2)	//向右走
			{
				if (x > 600 - defaultW)					//當超過右側自由移動範圍時
				{
					mapX -= x - (600 - defaultW);		//取得超過的位移量，反向加至地圖座標
					if (mapX <= (-4400 + defaultW))		//如果地圖在最右側，則可以無視移動邊界
						mapX = (-4400 + defaultW);
					else x = 600 - defaultW;			//反之則會被卡住
				}
				//mapEdge = max(600 - defaultW, x);
				/*if (x > mapEdge)					//當超過右側自由移動範圍時
				{
					mapX -= x - (mapEdge);		//取得超過的位移量，反向加至地圖座標
					if (mapX <= (-4400 + defaultW))		//如果地圖在最右側，則可以無視移動邊界
						mapX = (-4400 + defaultW);
					else x = mapEdge;			//反之則會被卡住
				}*/
				if (x >= 800 - defaultW) x = 800 - defaultW;
			}
			gameMap->setXY(mapX, mapY);					//設定地圖座標
		}
	}

	void CHero::resetAnimation()
	{
		heroMove.resetShootAnimation();
		heroStand.resetShootAnimation();
		heroJump.resetShootAnimation();
		heroCrouch.resetShootAnimation();
	}

	void CHero::ResumeShooting()
	{
		if (isShooting)
		{
			delayCount--;
			if (delayCount < 0)
			{
				delayCount = constDelay;
				isShooting = false;
				SetShooting(false);
				resetAnimation();
			}
		}
	}

	void CHero::SetHeart()
	{
		heartX = x + CDefaultStand.Width() / 2 - CHeart.Width() / 2;
		heartY = y + 65;
		if (isMovingDown) heartY = y + 80;
	}

	//private Function
#pragma endregion

	void CHero::OnMove()
	{
		gameMap_OnMove();						//地圖卷軸移動，根據主角的位置
		heroStand.OnMove(x, y);					//站立
		heroMove.OnMove(&x, &y);				//移動
		gravity();								//重力
		isFalling = heroJump.OnMove(&x, &y);	//跳躍落下
		if (isFalling) isRising = false;		//狀態轉變
		heroCrouch.OnMove(x, y);				//下蹲
		ResumeShooting();						//射擊回復-(彈藥裝填)
		SetHeart();								//心臟位置
	}

	void CHero::OnShow()
	{

		#pragma region OnShowAction
		
		if (isRising||isFalling)
		{
			if (isShooting)
				heroJump.OnShow_Attack();
			else
			{
				if (isRising) heroJump.OnShow_Rise();
				if (isFalling) heroJump.OnShow_Fall();
			}
		}
		else if (isMovingDown)
		{
			if (isShooting)
				heroCrouch.OnShow_Attack();
			else
			{
				if (isMovingLeft || isMovingRight) heroCrouch.OnShow_Move();
				else heroCrouch.OnShow_Stand();
			}
		}
		else if (isMovingLeft||isMovingRight)
		{
			if (isShooting)
				heroMove.OnShow_Attack();
			else heroMove.OnShow();
		}
		else if(!(isMovingDown || isMovingLeft || isMovingRight || isRising))
		{
			if (isShooting)
				heroStand.OnShow_Attack();
			else heroStand.OnShow_Stand();
		}

		#pragma endregion

		CHeart.SetTopLeft(heartX, heartY);
		CHeart.ShowBitmap();
	}

	#pragma region SetState
		void CHero::SetGameMap(CGameMap* _gameMap)
		{
			gameMap = _gameMap;
			heroJump.SetGameMap(_gameMap);
		}

		void CHero::ResumeDirection()
		{
			if (isMovingLeft) direction = dir_horizontal = 1;
			else if (isMovingRight) direction = dir_horizontal = 2;
			else direction = dir_horizontal;
			heroStand.SetDirection(direction);
			heroJump.SetDirection(direction);
			heroCrouch.SetDirection(direction);
			heroMove.SetDirection(direction);
		}

		void CHero::SetDirection(int dir)
		{
			direction = dir;
			if (dir < 3)		//向左或向右
				dir_horizontal = dir;
			heroStand.SetDirection(dir);
			heroJump.SetDirection(dir);
			heroCrouch.SetDirection(dir);
			heroMove.SetDirection(dir);
		}
		
		void CHero::SetFallDownFromBlock(bool flag)
		{
			heroJump.SetCanFallFromBlock(flag);
		}

		void CHero::SetRising(bool flag)
		{
			if (!(isFalling || isRising))
			{
				isRising = flag;
				heroJump.SetRising(flag);
				CAudio::Instance()->Play(AUDIO_heroJump, false);
			}
		}

		void CHero::SetMovingDown(bool flag)
		{
			isMovingDown = flag;
			heroJump.SetMovingDown(flag);
		}
	
		void CHero::SetMovingUp(bool flag)
		{
			isMovingUp = flag;
		}

		void CHero::SetMovingLeft(bool flag)
		{
			isMovingLeft = flag;
			heroMove.SetMovingLeft(flag);
		}

		void CHero::SetMovingRight(bool flag)
		{
			isMovingRight = flag;
			heroMove.SetMovingRight(flag);
		}

		void CHero::SetShooting(bool flag)
		{
			isShooting = flag;
			heroStand.SetShooting(flag);
			heroMove.SetShooting(flag);
			heroCrouch.SetShooting(flag);	//不可省略
			heroJump.SetShooting(flag);		//不可省略
		}

		void CHero::SetXY(int nx, int ny)
		{
			x = nx;
			y = ny;
		}

		void CHero::SetMapXY(int mx, int my)
		{
			mapX = mx;
			mapY = my;
		}

		void CHero::SetOverlap(bool flag)
		{
			isOverlap = flag;
			heroMove.SetOverlap(flag);
			heroStand.SetOverlap(flag);
			heroJump.SetOverlap(flag);
			heroCrouch.SetOverlap(flag);
		}

		void CHero::SetLock(bool flag) 
		{ 
			isMapLock = flag; 
			heroMove.SetLock(flag);
		}

	#pragma endregion

	#pragma region GetState
		
		int CHero::getX1() { return x; }

		int CHero::getY1() { return y; }

		int CHero::getX2() 
		{ 
			if (isMovingDown)
				return x + CDefaultCrouch.Width();
			else
				return x + CDefaultStand.Width();
		}

		int CHero::getY2() 
		{ 
			if (isMovingDown)
				return y + CDefaultCrouch.Height();
			else
				return y + CDefaultStand.Height();
		}

		int CHero::getHeartX1() { return heartX; }

		int CHero::getHeartY1() { return heartY; }

		int CHero::getHeartX2() { return heartX + CHeart.Width(); }

		int CHero::getHeartY2() { return heartY + CHeart.Height(); }

		int CHero::getDir() { return direction; }

		int CHero::getDir_hor() { return dir_horizontal; }

		bool CHero::isNowRising() { return isRising; }

		bool CHero::isOverlapEnemy(CEnemy* enemy)
		{
			if (enemy->getX2() >= x && enemy->getX1() <= x + defaultW && enemy->getY2() >= y && enemy->getY1() <= y + defaultH)
			{
				isOverlap = true;
				SetOverlap(true);
				return true;
			}
			else
			{
				isOverlap = false;
				SetOverlap(false);
				return false;
			}
		}

		bool CHero::getOverlap() { return isOverlap; }

		bool CHero::getShooting() { return isShooting; }

	#pragma endregion

	//CHero
#pragma endregion

/////////////////////////////////////////////////////////////////////////////
// CEnemy: Enemy class
/////////////////////////////////////////////////////////////////////////////

#pragma region CEnemy

		CEnemy::CEnemy()
		{
			Initialize();
		}

		CEnemy::~CEnemy() {}

		void CEnemy::Initialize()
		{
			mapX = mapY = 0;
			x = y = 0;
			direction = 2;
			step = 18;
			isOnBlock = canShoot = false;
			isAlive = isDead = goDestination = false;
			isMovingLeft = isMovingRight = isFallBack = false;
			constDelay = delayCount = 20; 
			machineGunShootDelay = constMachineGunDelay = 12;
		}

		void CEnemy::LoadBitmap()
		{
			
			char* fileStandL[] = { ".\\image\\enemy\\L1.bmp" };
			char* fileStandR[] = { ".\\image\\enemy\\R1.bmp" };
			char* fileDeadL[] = { ".\\image\\enemy\\die\\L1.bmp" , ".\\image\\enemy\\die\\L3.bmp" , ".\\image\\enemy\\die\\L5.bmp" , ".\\image\\enemy\\die\\L7.bmp" , ".\\image\\enemy\\die\\L8.bmp" , ".\\image\\enemy\\die\\L8.bmp" };
			char* fileDeadR1[] = { ".\\image\\enemy\\die\\R1-1.bmp" , ".\\image\\enemy\\die\\R3-1.bmp" , ".\\image\\enemy\\die\\R5-1.bmp" , ".\\image\\enemy\\die\\R7-1.bmp" , ".\\image\\enemy\\die\\R8-1.bmp" , ".\\image\\enemy\\die\\R8-1.bmp" };
			char* fileDeadR2[] = { ".\\image\\enemy\\die\\R1-2.bmp" , ".\\image\\enemy\\die\\R3-2.bmp" , ".\\image\\enemy\\die\\R5-2.bmp" , ".\\image\\enemy\\die\\R7-2.bmp" , ".\\image\\enemy\\die\\R8-2.bmp" , ".\\image\\enemy\\die\\R8-2.bmp" };
			char* fileMoveL[] = { ".\\image\\enemy\\move\\L1.bmp" , ".\\image\\enemy\\move\\L2.bmp" , ".\\image\\enemy\\move\\L3.bmp" , ".\\image\\enemy\\move\\L4.bmp" };
			char* fileMoveR[] = { ".\\image\\enemy\\move\\R1.bmp" , ".\\image\\enemy\\move\\R2.bmp" , ".\\image\\enemy\\move\\R3.bmp" , ".\\image\\enemy\\move\\R4.bmp" };


			enemyStand.LoadBitmap_StandL(fileStandL[0]);
			enemyStand.LoadBitmap_StandR(fileStandR[0]);
			for (int i = 0; i < 6; i++)
			{
				enemyDead.LoadBitmap_L(fileDeadL[i]);
				enemyDead.LoadBitmap_R(fileDeadR1[i], fileDeadR2[i]);
			}

			for (int i = 0; i < 4; i++)
			{
				enemyMove.LoadBitmap_MoveL(fileMoveL[i]);
				enemyMove.LoadBitmap_MoveR(fileMoveR[i]);
			}

			defaultStand.LoadBitmap(fileStandL[0], Blue);
			defaultHeight = defaultStand.Height();
			defaultWidth = defaultStand.Width();
		}

		void CEnemy::OnMove()
		{
			//if (!isOnBlock) y += step;					//gravity
			if (isAlive) enemyStand.OnMove(x, y);		//站立動作
			else if (isDead)
			{
				enemyDead.OnMove(x, y);					//死亡動作
				if (enemyDead.isfinalBitmap())			//解除死亡狀態
					isDead = false;
			}

			
			enemyMove.OnMove(&x, &y);
			if (goDestination)
			{
				if ((enemyPos == 0 && x >= desX) || (enemyPos == 1 && x <= desX))
				{
					isMovingLeft = isMovingRight = false;
					enemyMove.SetMovingLeft(false);
					enemyMove.SetMovingRight(false);
					goDestination = false;
				}
			}
			else if (isFallBack)
			{
				if (x > 800 || x < 0 - defaultWidth)
				{
					isMovingLeft = isMovingRight = isFallBack = isAlive = false;
					enemyMove.SetMovingLeft(false);
					enemyMove.SetMovingRight(false);
					enemyMove.SetFallBack(false);
				}
			}

			if (!isFallBack)
			{
				if (isAlive)
				{
					if (!canShoot)			//射擊間隔處理
					{
						delayCount--;
						if (delayCount == 0)
						{
							canShoot = true;
							delayCount = constDelay;
						}
					}
					if (gunMode == 2)	//machineGun
					{
						if (canShoot)
						{
							machineGunShootDelay--;
							if (machineGunShootDelay == 0)
							{
								canShoot = false;
								machineGunShootDelay = constMachineGunDelay;
							}
						}
					}
				}
			}

		}

		void CEnemy::OnShow()
		{
			if (isDead)
			{
				enemyDead.OnShow();
			}
			else if (isAlive)
			{
				enemyMove.SetXY(x, y);
				if (goDestination) enemyMove.OnShow();
				else
				{
					if (isMovingLeft || isMovingRight)
						enemyMove.OnShow();
					else
					{
						enemyStand.SetXY(x, y);
						enemyStand.OnShow_Stand();
					}
				}
			}
		}

#pragma region SetState
		void CEnemy::SetAlive(bool flag) 
		{ 
			isAlive = flag; 
		}

		void CEnemy::SetDead(bool flag) { isDead = flag; }

		void CEnemy::SetDead(bool flag, int dir)
		{ 
			isDead = flag;
			enemyDead.SetDir(dir);
		}

		void CEnemy::SetDirection(int heroX)
		{ 
			if (isFallBack || goDestination) return;
			if (heroX > x) direction = 2;			//站著不動的時候方向朝著主角
			else direction = 1;
			enemyStand.SetDirection(direction);
		}

		void CEnemy::SetOnBlock(bool flag) { isOnBlock = flag; }

		void CEnemy::SetShootDelay(int delay) { delayCount = constDelay = delay; }

		void CEnemy::SetShootState(bool flag) { canShoot = flag; }

		void CEnemy::SetXY(int nx, int ny)
		{
			x = nx;
			y = ny;
		}

		void CEnemy::SetDestination(int dx, int dy,int pos)
		{
			desX = dx;
			desY = y = dy;
			enemyPos = pos;
			if (pos == 0)
			{
				x = 0 - defaultWidth;
				enemyMove.SetDirection(2);
				enemyMove.SetMovingRight(true);
				enemyMove.SetMovingLeft(false);
				desX += 5 * enemyID;
			}
			else
			{
				x = 800;
				enemyMove.SetDirection(1);
				enemyMove.SetMovingLeft(true);
				enemyMove.SetMovingRight(false);
				desX -= 5 * enemyID;
			}
			goDestination = true;
		}

		void CEnemy::SetMapXY(int mx, int my)
		{
			if (isFallBack) return;
			else if (goDestination)
			{
				mapX = mx;
				mapY = my;
				return;
			}
			isMovingLeft = isMovingRight = false;
			if (mx > mapX)			//主角向左走，新的地圖座標會>之前的地圖座標
			{
				x += (mx - mapX);	//敵人要追蹤，也向左走
				if (x > 614 - 8 * enemyID)
				{
					x = 614 - 8 * enemyID;
					isMovingLeft = true;
					enemyMove.SetDirection(1);
				}
			}
			else if (mx < mapX)		//主角向右走，新的地圖座標會<之前的地圖座標
			{
				x += (mx - mapX);	//敵人追蹤，也向右走
				if (x < 60 + 8 * enemyID)
				{
					x = 60 + 8 * enemyID;
					isMovingRight = true;
					enemyMove.SetDirection(2);
				}
			}
			mapX = mx;
			mapY = my;
		}

		// 0->pistol 1->shotgun 2->machineGun 3->sniper
		void CEnemy::SetGunMode(int mode) 
		{ 
			gunMode = mode; 
			switch (gunMode)
			{
			case 0:
				CEnemy::SetShootDelay(15);		//pistol
				break;
			case 1:
				CEnemy::SetShootDelay(30);		//shotgun
				break;
			case 2:
				CEnemy::SetShootDelay(50);		//machinegun
				break;
			case 3:
				CEnemy::SetShootDelay(40);		//sniper
				break;
			default:
				break;
			}
		}

		void CEnemy::SetID(int n) { enemyID = n; }

		void CEnemy::SetFallBack(bool flag, int heroX)
		{
			goDestination = false;
			isFallBack = flag;
			enemyMove.SetFallBack(flag);
			if (!isFallBack) return;
			if (x >= heroX)
			{
				isMovingRight = true;
				enemyMove.SetMovingRight(true);
				enemyMove.SetMovingLeft(false);
				enemyMove.SetDirection(2);
			}
			else
			{
				isMovingLeft = true;
				enemyMove.SetMovingLeft(true);
				enemyMove.SetMovingRight(false);
				enemyMove.SetDirection(1);
			}
		}

#pragma endregion

#pragma region GetState
		bool CEnemy::getShootState() { return canShoot; }

		bool CEnemy::isShow() { return (isAlive || isDead); }

		bool CEnemy::getAlive() { return isAlive; }

		bool CEnemy::getDead() { return isDead; }

		int CEnemy::getGunMode() { return gunMode; }

		int CEnemy::getX1() { return x; }
		int CEnemy::getX2() { return x + defaultWidth; }
		int CEnemy::getY1() { return y; }
		int CEnemy::getY2() { return y + defaultHeight; }

#pragma endregion



	//CEnemy
#pragma endregion

/////////////////////////////////////////////////////////////////////////////
// CMidBoss: MidBoss class
/////////////////////////////////////////////////////////////////////////////

#pragma region CMidBoss

		CMidBoss::CMidBoss()
		{
			Initialize();
		}

		CMidBoss::~CMidBoss() { ; }

		void CMidBoss::Initialize()
		{
			isStart = isDead = isAlive = false;
			x = 0;
			y = 525 - 237;
			bossLife = 10;
			step = 25;
			midBossMove.SetStep(step);
			midBossLazer.SetDelayCount(5);
			delay = const_delay = 30;
		}

		void CMidBoss::LoadBitmap()
		{
			char *fileLazer[] = { ".\\image\\midBoss\\attack\\L1.bmp" , ".\\image\\midBoss\\attack\\L2.bmp"
			, ".\\image\\midBoss\\attack\\L3.bmp" , ".\\image\\midBoss\\attack\\L4.bmp" };
			char *fileStandL[] = { ".\\image\\midBoss\\stand\\L1.bmp" };
			char *fileStandR[] = { ".\\image\\midBoss\\stand\\R1.bmp" };
			char *fileRunL[] = { ".\\image\\midBoss\\run\\L1.bmp" , ".\\image\\midBoss\\run\\L2.bmp" 
				, ".\\image\\midBoss\\run\\L3.bmp" , ".\\image\\midBoss\\run\\L4.bmp" };
			char *fileRunR[] = { ".\\image\\midBoss\\run\\R1.bmp" , ".\\image\\midBoss\\run\\R2.bmp"
				, ".\\image\\midBoss\\run\\R3.bmp" , ".\\image\\midBoss\\run\\R4.bmp" };

			midBossLazerHead.LoadBitmap(".\\image\\midBoss\\attack\\D1.bmp", Blue);
			midBossDefault.LoadBitmap(fileStandL[0]);
			midBossStand.LoadBitmap_StandL(fileStandL[0]);
			midBossStand.LoadBitmap_StandR(fileStandR[0]);
			for (int i = 0; i < 4; i++)
			{
				midBossMove.LoadBitmap_MoveL(fileRunL[i]);
				midBossMove.LoadBitmap_MoveR(fileRunR[i]);
				midBossLazer.AddBitmap(fileLazer[i], Blue);
			}
			defaultWidth = midBossDefault.Width();
			defaultHeight = midBossDefault.Height();
		}

		void CMidBoss::OnMove()
		{
			midBossMove.OnMove(&x, &y);
			if (isStart)
			{
				x -= 25;
				if (x <= 800 - midBossDefault.Width())
				{
					isStart = false;
					x = 800 - midBossDefault.Width();
				}
				return;
			}

			if (isStand)
			{
				if (delay > 0) delay--;
				else
				{
					delay = const_delay;
					if (x = 0)
					{
						midBossMove.SetMovingLeft(false);
						midBossMove.SetMovingRight(true);
						midBossMove.SetDirection(2);
					}
					else
					{
						midBossMove.SetMovingLeft(true);
						midBossMove.SetMovingRight(false);
						midBossMove.SetDirection(1);
					}
				}
				midBossStand.OnMove(x, y);
			}

			if (x < 0) 
			{
				x = 0;
				isStand = true;
				midBossStand.SetDirection(2);
			}
			else if (x > 800 - defaultWidth)
			{
				x = 800 - defaultWidth;
				isStand = true;
				midBossStand.SetDirection(1);
			}
			
		}

		void CMidBoss::OnShow()
		{
			if (isStand)
			{
				midBossStand.SetXY(x, y);
				midBossStand.OnShow();
			}
			else
			{
				midBossMove.SetXY(x, y);
				midBossMove.OnShow();
			}
		}

		void CMidBoss::AddLife(int n)
		{
			bossLife += n; 
			if (bossLife <= 0) 
			{
				isAlive = false;
				isDead = true;
			}
		}

		void CMidBoss::SetStart(bool flag) 
		{ 
			isStart = flag; 
			x = 800;
			y = 525 - midBossDefault.Height();
		}

		int CMidBoss::getLife() { return bossLife; }

		bool CMidBoss::getAlive() { return isAlive; }

		bool CMidBoss::getDead() { return isDead; }

		bool CMidBoss::getShow() { return (isDead || isAlive); }

	//CBoss
#pragma endregion

/////////////////////////////////////////////////////////////////////////////
// CBullet: bullet class
/////////////////////////////////////////////////////////////////////////////

#pragma region CBullet
	CBullet::CBullet(void) {}

	CBullet::~CBullet() {};

	CBullet::CBullet(int nx, int ny)
	{
		x = nx;
		y = ny;
		Initialize();
	}

	void CBullet::Initialize()
	{
		vx = vy = 23;			//預設速度
		isAlive = false;		//預設不顯示
		direction = 5;			//預設為朝著目標座標飛行
	}

	void CBullet::SetBulletClass(CAnimation* anime)
	{
		bullet = anime;
	}

	void CBullet::LoadBitmap(char* file)
	{
		/*char *fileBullet[] = { ".\\image\\bullet\\1.bmp" , ".\\image\\bullet\\2.bmp" , ".\\image\\bullet\\3.bmp" };
		bullet.AddBitmap(fileBullet[i], Blue);*/
	}

	void CBullet::OnMove()
	{
		switch (direction)
		{
		case 1:			//向左射
			x -= vx;
			break;
		case 2:			//向右射
			x += vx;
			break;
		case 5:			//往目標位置射
			x += vx;
			y += vy;
			break;
		default:
			break;
		}
		bullet->OnMove();
	}

	void CBullet::OnShow()
	{
		bullet->SetTopLeft(x, y);
		bullet->OnShow();
	}

	bool CBullet::isDead()		//判斷是否超出邊界導致死亡
	{
		if (x  > 800 || x + bullet->Width() < 0 || y > 600 || y + bullet->Height() < 0) return true;
		return false;
	}

	bool CBullet::isShow() { return isAlive; }				//取得生命狀態(顯示狀態)

	bool CBullet::isHit(int tx1, int ty1, int tx2, int ty2)	//判斷是否碰撞到物件導致死亡
	{
		return (tx2 >= x && tx1 <= x + bullet->Width() && ty2 >= y && ty1 <= y + bullet->Height());
	}

	void CBullet::SetLife(bool life) { isAlive = life; }	//生命設定(是否顯示)

	void CBullet::SetBullet(int nx, int ny, int dir)
	{
		x = nx;
		y = ny;
		direction = dir;
		isAlive = true;
	}

	void CBullet::SetBullet(int nowX, int nowY, int heroX, int heroY, int gunMode)
	{
		// 0->pistol 1->shotgun 2->machineGun 3->sniper
		x = nowX;
		y = nowY;
		switch (gunMode)
		{
		case 0:
			mistakeRate = 10;
			maxSpeed = 15;
			break;
		case 1:
			mistakeRate = 12;
			maxSpeed = 20;
			break;
		case 2:
			mistakeRate = 15;
			maxSpeed = 23;
			break;
		case 3:
			mistakeRate = 0;
			maxSpeed = 30;
			break;
		default:
			break;
		}
		heroX += getRandom(gunMode) * mistakeRate;		//x誤差調整
		heroY += getRandom(gunMode) * mistakeRate;		//y誤差調整
		vx = heroX - nowX;
		vy = heroY - nowY;
		if (abs(vx) > abs(vy) && abs(vx) > maxSpeed)
		{
			vy /= max(abs(vx),maxSpeed) / maxSpeed;		//預防/0的狀況
			if (vx < 0) vx = -(maxSpeed);
			else vx = maxSpeed;
		}
		else if (abs(vx) < abs(vy) && abs(vy) > 20)
		{
			vx /= max(abs(vy),maxSpeed) / maxSpeed;		//同上
			if (vy < 0) vy = -(maxSpeed);
			else vy = maxSpeed;
		}
		isAlive = true;
	}

	int CBullet::getRandom(int gunMode)
	{
		// 0->pistol 1->shotgun 2->machineGun 3->sniper
		if (gunMode == 0) mistake = 10;
		else if (gunMode == 1) mistake = 15;
		else if (gunMode == 2) mistake = 20;
		else return 0;
		int r = (rand() % mistake) - 5;
		return r;
	}

	//CBullet
#pragma endregion

/////////////////////////////////////////////////////////////////////////////
// CGameMap: GameMap class
/////////////////////////////////////////////////////////////////////////////

#pragma region CGameMap

	CGameMap::CGameMap()
	{
		Initialize();
	}

	CGameMap::~CGameMap()
	{
		for (loop = 0; loop < vCblt.size(); loop++)
			delete[] vCblt[loop];
		for (loop = 0; loop < vCbltEnemy.size(); loop++)
			delete[] vCbltEnemy[loop];
	}

	void CGameMap::Initialize()
	{
		mapX = 0;
		mapY = SIZE_Y - 721;
		size = 20;
		weight = 5130 / size;
		height = 721 / size;
		for (int i = 0; i < height+1; i++)
			for (int j = 0; j < weight + 1; j++)
			{
				map[i][j] = 0;							//0為空白
				if (i > (640 / 20 - 1)) map[i][j] = 1;	//1為地板
			}
		#pragma region setBlock
			SetBlock(1, 8, 13, 14);
			SetBlock(8, 18, 22, 23);
			SetBlock(19, 28, 18, 19);
			SetBlock(28, 40, 24, 25);
			SetBlock(40, 46, 17, 18);
			SetBlock(48, 68, 14, 15);
			SetBlock(55, 67, 25, 26);
			SetBlock(73, 83, 24, 25);
			SetBlock(83, 94, 22, 23);
			SetBlock(93, 101, 17, 18);
			SetBlock(102, 118, 25, 26);
			SetBlock(132, 153, 16, 17);
			SetBlock(160, 172, 23, 24);
			SetBlock(188, 198, 22, 23);
			SetBlock(40, 46, 17, 18);
			SetBlock(48, 68, 14, 15);
		#pragma endregion

	}

	void CGameMap::LoadBitmap()
	{
		mapBmp.AddBitmap(IDB_GameMap);
		char *fileBullet[] = { ".\\image\\bullet\\1.bmp" , ".\\image\\bullet\\2.bmp" , ".\\image\\bullet\\3.bmp" };
		char *fileBulletEnemy[] = { ".\\image\\bullet\\enemy\\1.bmp" , ".\\image\\bullet\\enemy\\2.bmp" , ".\\image\\bullet\\enemy\\3.bmp" };

		for (int i = 0; i < 3; i++)
			heroBullet.AddBitmap(fileBullet[i], Blue);
		for (int i = 0; i < 3; i++)
			enemyBullet.AddBitmap(fileBulletEnemy[i], Black);
	}

	void CGameMap::OnShow()
	{
		mapBmp.SetTopLeft(mapX, mapY);
		mapBmp.OnShow();
	}

#pragma region SetState
	void CGameMap::SetBlock(int x1, int x2, int y1, int y2)
	{
		for (int i = x1; i < x2; i++)
			for (int j = y1; j < y2; j++)
				map[j][i] = 2;				//2為可跳上的障礙物
		
	}

	void CGameMap::setXY(int mx, int my)
	{
		mapX = mx;
		mapY = my;
	}

#pragma endregion

#pragma region GetValue
	int CGameMap::getMapBlock(int ny, int nx)
	{
		int gx = nx / size;
		int gy = ny / size;
		return map[gy][gx];
	}

	int CGameMap::getX()
	{
		return mapX;
	}

	int CGameMap::getY()
	{
		return mapY;
	}

	int CGameMap::getSize()
	{
		return size;
	}

#pragma endregion
	
#pragma region BulletState
	void CGameMap::InitialBullet()
	{
		maxHeroBullet = 5;
		if (vCblt.size() == 0)
		{
			for (loop = 0; loop < maxHeroBullet; loop++)
			{
				vCblt.push_back(new CBullet(0, 0));
				vCblt[loop]->SetBulletClass(&heroBullet);
			}
		}
		else
		{
			for (loop = 0; loop < maxHeroBullet; loop++)
				vCblt[loop]->SetLife(false);
		}
		maxEnemyBullet = 150;
		if (vCbltEnemy.size() == 0)
		{
			for (loop = 0; loop < maxEnemyBullet; loop++)
			{
				vCbltEnemy.push_back(new CBullet(0, 0));
				vCbltEnemy[loop]->SetBulletClass(&enemyBullet);
			}
		}
		else
		{
			for (loop = 0; loop < maxEnemyBullet; loop++)
				vCbltEnemy[loop]->SetLife(false);
		}
	}

	void CGameMap::addBullet(int x, int y, int direction, int dir_horizontal)
	{
		int newY = 0, newX = 0;
		for (loop = 0; loop < maxHeroBullet; loop++)
		{
			if (vCblt[loop]->isShow() == false)
			{
				if (direction == 4) newY = y + 60;
				else newY = y + 30;
				if (dir_horizontal == 2) newX = x + 100;
				else newX = x - 40;
				vCblt[loop]->SetBullet(newX, newY, dir_horizontal);
				break;
			}
		}
	}

	void CGameMap::addEnemyBullet(CEnemy* enemy, int dx, int dy)
	{
		// 0->pistol 1->shotgun 2->machineGun 3->sniper
		int nx = enemy->getX1(), ny = enemy->getY1();
		int tx, ty;
		int gunMode = enemy->getGunMode();

		if (gunMode == 1) bulletNumer = 3;
		else bulletNumer = 1;

		for (loop = 0; loop < maxEnemyBullet && bulletNumer > 0; loop++)
		{
			if (!vCbltEnemy[loop]->isShow())		//子彈沒激活
			{
				if (dx >= nx) tx = nx + 80;
				else tx = nx - 40;
				ty = ny + 50;
				vCbltEnemy[loop]->SetBullet(tx, ty, dx, dy, gunMode);
				bulletNumer--;
			}
		}
		if (gunMode != 2) enemy->SetShootState(false);
	}

	void CGameMap::killBullet()
	{
		for (loop = 0; loop < maxHeroBullet; loop++)
		{
			if (vCblt[loop]->isShow())
			{
				if (vCblt[loop]->isDead())
					vCblt[loop]->SetLife(false);
			}
		}
		for (loop = 0; loop < maxEnemyBullet; loop++)
		{
			if (vCbltEnemy[loop]->isShow())
			{
				if (vCbltEnemy[loop]->isDead())
					vCbltEnemy[loop]->SetLife(false);
			}
		}
	}

	void CGameMap::OnMoveBullet()
	{
		for (loop = 0; loop < maxHeroBullet; loop++)
			if (vCblt[loop]->isShow())
				vCblt[loop]->OnMove();
		for (loop = 0; loop < maxEnemyBullet; loop++)
		{
			if (vCbltEnemy[loop]->isShow())
				vCbltEnemy[loop]->OnMove();
		}
	}

	void CGameMap::OnShowBullet()
	{
		for (loop = 0; loop < maxHeroBullet; loop++)
			if (vCblt[loop]->isShow())
				vCblt[loop]->OnShow();
		for (loop = 0; loop < maxEnemyBullet; loop++)
		{
			if (vCbltEnemy[loop]->isShow())
				vCbltEnemy[loop]->OnShow();
		}
	}

	bool CGameMap::isBulletHit(CEnemy *enemy)
	{
		for (loop = 0; loop < maxHeroBullet; loop++)
		{
			if (vCblt[loop]->isShow())					//子彈存活
			{
				if (vCblt[loop]->isHit(enemy->getX1(), enemy->getY1(), enemy->getX2(), enemy->getY2()))
				{
					vCblt[loop]->SetLife(false);		//子彈消失
					return true;
				}
			}
		}
		return false;
	}

	bool CGameMap::isBulletHit(CHero *hero)
	{
		for (loop = 0; loop < maxEnemyBullet; loop++)
		{
			if (vCbltEnemy[loop]->isShow())
			{
				if (vCbltEnemy[loop]->isHit(hero->getHeartX1(), hero->getHeartY1(), hero->getHeartX2(), hero->getHeartY2()))
				{
					vCbltEnemy[loop]->SetLife(false);
					return true;
				}
			}
		}
		return false;
	}

#pragma endregion


	//CGameMap
#pragma endregion

/////////////////////////////////////////////////////////////////////////////
// CMovie: movie class
/////////////////////////////////////////////////////////////////////////////

#pragma region CMovie

	CMovie::CMovie()
	{
		Initialize();
	}

	CMovie::~CMovie() {}

	void CMovie::Initialize()
	{
		nowBG = 0;
		bg1Stage = 1;
		bg1CanGoNextStage = changeBG = false;
		showMovie = true;
		kidWave.SetDelayCount(3);
		heroWake.SetDelayCount(5);
		heroWake2.SetDelayCount(5);
		spaceShipCrash.SetDelayCount(5);
		spaceShipExplode.SetDelayCount(5);
		delay = const_delay;
		x = 800;
		y = -50;

		isShowEnemyGone = isShowEnemyShow = isShowFBItalk = isShowFire = isShowHeroBlink
			= isShowKidWave = isShowMoon = isShowUFOCrash = isShowUFOExplode = false;
	}

	void CMovie::LoadBitmap()
	{
		bg1.LoadBitmap(".\\image\\movie\\bg1.bmp");
		bg1_1.LoadBitmap(".\\image\\movie\\bg1-1.bmp", Black);
		bg2.LoadBitmap(".\\image\\movie\\bg2.bmp");
		bg3.LoadBitmap(".\\image\\movie\\bg3.bmp");
		enemyShow.LoadBitmap(".\\image\\movie\\e1-1.bmp", Blue);
		enemyGone.LoadBitmap(".\\image\\movie\\e1-2.bmp", Black);
		spaceShip.LoadBitmap(".\\image\\movie\\spaceship.bmp", Black);
		char *fileCrash[] = { ".\\image\\movie\\crash1.bmp" , ".\\image\\movie\\crash2.bmp" , ".\\image\\movie\\crash3.bmp"
			,".\\image\\movie\\crash4.bmp" , ".\\image\\movie\\crash5.bmp" , ".\\image\\movie\\crash6.bmp" };
		char *fileExplode[] = { ".\\image\\movie\\explode1.bmp" , ".\\image\\movie\\explode2.bmp" , ".\\image\\movie\\explode3.bmp"
			,".\\image\\movie\\explode4.bmp" , ".\\image\\movie\\explode5.bmp" , ".\\image\\movie\\explode6.bmp" };
		char *fileWake[] = { ".\\image\\movie\\wakeup1.bmp" , ".\\image\\movie\\wakeup2.bmp" , ".\\image\\movie\\wakeup3.bmp"
			,".\\image\\movie\\wakeup4.bmp" , ".\\image\\movie\\wakeup5.bmp" };
		char *fileWave[] = { ".\\image\\movie\\wavehand1.bmp" , ".\\image\\movie\\wavehand2.bmp" , ".\\image\\movie\\wavehand3.bmp"
			,".\\image\\movie\\wavehand4.bmp" , ".\\image\\movie\\wavehand5.bmp" , ".\\image\\movie\\wavehand6.bmp" };
		for (int i = 0; i < 6; i++)
		{
			spaceShipCrash.AddBitmap(fileCrash[i], Black);
			spaceShipExplode.AddBitmap(fileExplode[i], Black);
			kidWave.AddBitmap(fileWave[i], Black);
			if (i < 5) heroWake.AddBitmap(fileWake[i], Black);
			if (i != 0 && i != 5) heroWake2.AddBitmap(fileWake[i], Black);
		}
	
	}

	void CMovie::OnMove()
	{
		if (nowBG == 0)
		{
			if (delay > 0) delay -= 2;
			else
			{
				delay = const_delay;
				nowBG++;
			}
		}
		else if (nowBG == 1)
		{
			if (!isShowMoon)
			{
				CAudio::Instance()->Play(Movie_moon, false);
				isShowMoon = true;
			}
			switch (bg1Stage)
			{
			case 1:
				x -= 70;
				y += 50;
				spaceShipCrash.OnMove();
				break;
			case 2:
				if (bg1CanGoNextStage)
				{
					if (delay > 0) delay--;
					else
					{
						delay = const_delay;
						bg1Stage++;
						bg1CanGoNextStage = false;
					}
				}
				else spaceShipExplode.OnMove();
				break;
			case 3:
				if (delay > 0) delay--;
				else
				{
					delay = const_delay;
					bg1Stage++;
				}
				break;
			case 4:
				x += 30;
				y += 25;
				break;
			default:
				break;
			}
			if ((x < -(spaceShip.Width()) || y>600) && bg1Stage == 1)	//切換條件
			{
				bg1Stage++;
				x = 800 - enemyShow.Width();
				y = 600 - enemyShow.Height();
			}
			else if ((x > 800 || y > 600) && bg1Stage == 4) changeBG = true;
		}
		else if (nowBG == 2 || nowBG == 4)
		{
			if (nowBG == 2)
			{
				if (!changeBG) heroWake.OnMove();
				if (heroWake.IsFinalBitmap()) changeBG = true;
			}
			else
			{
				if (showMovie) heroWake2.OnMove();
				if (heroWake2.IsFinalBitmap()) changeBG = true;
			}
			
		}
		else if(nowBG == 3)
		{
			kidWave.OnMove();
			if (delay > 0) delay--;
			else
			{
				changeBG = true;
				delay = const_delay;
			}
		}

	}

	void CMovie::resetAnimation()
	{
		spaceShipCrash.Reset();
		spaceShipExplode.Reset();
		kidWave.Reset();
		heroWake.Reset();
		heroWake2.Reset();
	}

	bool CMovie::OnShow()
	{
		if (nowBG == 1)
		{
			bg1.SetTopLeft(0, 0);
			bg1_1.SetTopLeft(800 - bg1_1.Width(), bg1.Height() - bg1_1.Height());
			bg1.ShowBitmap();
			bg1_1.ShowBitmap();
			switch (bg1Stage)
			{
			case 1:				//bg1 第一部分 UFO墜毀
				if (!isShowUFOCrash)
				{
					CAudio::Instance()->Play(Movie_ufoCrash, false);
					isShowUFOCrash = true;
				}
				spaceShip.SetTopLeft(x, y);
				spaceShip.ShowBitmap();
				spaceShipCrash.SetTopLeft(x + spaceShip.Width(), y - spaceShipCrash.Height());
				spaceShipCrash.OnShow();
				break;
			case 2:				//bg1 第二部分 ufo爆炸
				if (!isShowUFOExplode)
				{
					CAudio::Instance()->Play(Movie_ufoExplode, false);
					isShowUFOExplode = true;
				}
				spaceShipExplode.SetTopLeft(0, 600 - spaceShipExplode.Height());
				if (!bg1CanGoNextStage) spaceShipExplode.OnShow();
				if (spaceShipExplode.IsFinalBitmap()) bg1CanGoNextStage = true;
				break;
			case 3:				//bg1 第三部分 FBI出現
				if (!isShowEnemyShow)
				{
					CAudio::Instance()->Play(Movie_enemyShow, false);
					CAudio::Instance()->Play(Movie_FBItalk, false);
					isShowFBItalk = isShowEnemyShow = true;
				}
				enemyShow.SetTopLeft(x, y);
				enemyShow.ShowBitmap();
				break;
			case 4:				//bg1 第四部分 FBI離開
				if (!isShowEnemyGone)
				{
					CAudio::Instance()->Play(Movie_enemyGone, false);
					isShowEnemyGone = true;
				}
				enemyGone.SetTopLeft(800 - enemyShow.Width() + 50, 600 - enemyGone.Height());
				enemyGone.ShowBitmap();
				enemyShow.SetTopLeft(x, y);
				enemyShow.ShowBitmap();
				break;
			default:
				break;
			}
		}
		else if (nowBG == 2 || nowBG == 4)
		{
			if (!isShowHeroBlink)
			{
				CAudio::Instance()->Play(Movie_HeroBlink, false);
				isShowHeroBlink = true;
			}
			bg2.SetTopLeft(0, 0);
			bg2.ShowBitmap();
			if (nowBG == 2)
			{
				heroWake.SetTopLeft(45, 150);
				heroWake.OnShow();
			}
			else
			{
				heroWake2.SetTopLeft(45, 150);
				heroWake2.OnShow();
			}
		}
		else if (nowBG == 3)
		{
			if (!isShowKidWave)
			{
				CAudio::Instance()->Play(Movie_KidWavehand, false);
				isShowKidWave = true;
			}
			bg3.SetTopLeft(0, 0);
			bg3.ShowBitmap();
			kidWave.SetTopLeft(380, 190);
			kidWave.OnShow();
			isShowHeroBlink = false;
		}
		if (changeBG)
		{
			if (nowBG == 4) showMovie = false;			//結束動畫
			else
			{
				if (delay > 0) delay--;
				else
				{
					delay = const_delay;
					if (nowBG == 2) delay = const_delay * 2;
					changeBG = false;
					nowBG++;
				}
			}
		}
		if (!showMovie)
		{
			if (delay > 0) delay--;
			else
			{
				delay = const_delay;
				resetAnimation();
				return false;
			}
		}
		return true;
	}

	//CMovie
#pragma endregion


/////////////////////////////////////////////////////////////////////////////
// CMove: move class
/////////////////////////////////////////////////////////////////////////////

#pragma region CMove

	CMove::CMove()
	{
		Initialize();
	}

	void CMove::Initialize()
	{
		const int x_pos = 200;			//預設x位置
		const int y_pos = 450;			//預設y位置
		x = x_pos;
		y = y_pos;
		step = ini_step = 15;			//預設移動速度
		direction = dir_horizontal = 1;	//預設方向向左
		isLock = isFallBack = isShooting = isRising = isMovingDown = isMovingLeft = isMovingRight = isMovingUp = false;
	}

	void CMove::LoadBitmap_MoveL(char *file)
	{
		CmoveL.AddBitmap(file, Blue);
	}

	void CMove::LoadBitmap_MoveR(char* file)
	{
		CmoveR.AddBitmap(file, Blue);
	}

	void CMove::LoadBitmap_ShootL(char* file, char* file2)
	{
		CMoveShoot.LoadShootLeft(file, file2);
	}

	void CMove::LoadBitmap_ShootR(char* file)
	{
		CMoveShoot.LoadShootRight(file);
	}

	void CMove::LoadBitmap_KnifeL(char* file, char* file2)
	{
		CMoveShoot.LoadKnifeLeft(file, file2);
	}

	void CMove::LoadBitmap_KnifeR(char* file)
	{
		CMoveShoot.LoadKnifeRight(file);
	}

	void CMove::OnMove(int* nx, int* ny)
	{
		x = *nx;
		y = *ny;
		if (isMovingLeft)
		{
			x -= step;
			if (x < 0 && isLock) x = 0;
		}
		if (isMovingRight)
		{
			x += step;
			if (x + CmoveL.Width() > 800 && isLock) x = 800 - CmoveL.Width();
		}
		if (isShooting)
		{
			if (isOverlap)
			{
				if (direction == 1)
				{
					CMoveShoot.OnMoveKnifeL();
				}
				else if (direction == 2)
				{
					CMoveShoot.OnMoveKnifeR();
				}
			}
			else
			{
				if (direction == 1)
				{
					CMoveShoot.OnMoveShootL();
				}
				else if (direction == 2)
				{
					CMoveShoot.OnMoveShootR();
				}
			}
		}
		else
		{
			if (direction == 1) CmoveL.OnMove();
			else if (direction == 2) CmoveR.OnMove();
		}
		*nx = x;
		*ny = y;
	}

#pragma region SetState

	void CMove::SetStep(int speed) { step = speed; }

	void CMove::SetDefaultHeight(int _height)
	{
		defaultHeight = _height;
	}

	void CMove::SetDirection(int dir)
	{
		direction = dir;
		if (dir < 3) dir_horizontal = dir;
	}

	void CMove::SetXY(int nx, int ny)
	{
		x = nx;
		y = ny;
	}

	void CMove::SetOverlap(bool flag) 
	{ 
		if (!isShooting) isOverlap = flag;
	}

	void CMove::SetFallBack(bool flag) { isFallBack = flag; }

	void CMove::SetLock(bool flag) { isLock = flag; }

	void CMove::resetShootAnimation() { CMoveShoot.resetAnimation(); }

	void CMove::SetMovingDown(bool flag)
	{
		isMovingDown = flag;
	}

	void CMove::SetMovingUp(bool flag)
	{
		isMovingUp = flag;
	}

	void CMove::SetMovingLeft(bool flag)
	{
		isMovingLeft = flag;
	}

	void CMove::SetMovingRight(bool flag)
	{
		isMovingRight = flag;
	}

	void CMove::SetRising(bool flag)
	{
		isRising = flag;
	}

	void CMove::SetFalling(bool flag)
	{
		isFalling = flag;
	}

	void CMove::SetShooting(bool flag)
	{
		isShooting = flag;
	}

	//SetState
#pragma endregion


	void CMove::OnShow()
	{
		if (direction == 1)
		{
			CmoveL.SetTopLeft(x, y);
			CmoveL.OnShow();
		}
		else if (direction == 2)
		{
			CmoveR.SetTopLeft(x, y);
			CmoveR.OnShow();
		}
	}

	void CMove::OnShow_Attack()
	{
		CMoveShoot.SetXY(x, y);
		if (isOverlap)
		{
			if (direction == 1) CMoveShoot.OnShowKnifeL();
			else if(direction == 2) CMoveShoot.OnShowKnifeR();
		}
		else
		{
			if (direction == 1) CMoveShoot.OnShowShootL();
			else if (direction == 2) CMoveShoot.OnShowShootR();
		}
	}

	//CMove
#pragma endregion

/////////////////////////////////////////////////////////////////////////////
// CStand: stand class
/////////////////////////////////////////////////////////////////////////////

#pragma region CStand

	CStand::CStand()
	{
		Initialize();
	}

	void CStand::Initialize()
	{
		x = y = 0;
		direction = dir_horizontal = 1;
		isShooting = false;
	}

#pragma region load
	void CStand::LoadBitmap_StandL(char* file)
	{
		CStandL.AddBitmap(file, Blue);
	}

	void CStand::LoadBitmap_StandR(char* file)
	{
		CStandR.AddBitmap(file, Blue);
	}

	void CStand::LoadBitmap_ShootL(char* file,char* file2)
	{
		CStandShoot.LoadShootLeft(file, file2);
	}

	void CStand::LoadBitmap_ShootR(char *file)
	{
		CStandShoot.LoadShootRight(file);
	}

	void CStand::LoadBitmap_KnifeL(char* file, char* file2)
	{
		CStandShoot.LoadKnifeLeft(file, file2);
	}

	void CStand::LoadBitmap_KnifeR(char* file)
	{
		CStandShoot.LoadKnifeRight(file);
	}

#pragma endregion

	void CStand::SetDirection(int dir)
	{
		direction = dir;
		if (dir < 3) dir_horizontal = dir;
	}
	
	void CStand::OnMove(int nx, int ny)
	{
		x = nx;
		y = ny;
		
		if(!isShooting)
		{
			if (dir_horizontal == 1) CStandL.OnMove();
			else if (dir_horizontal == 2) CStandR.OnMove();
		}
		else
		{
			if (isOverlap)
			{
				if (dir_horizontal == 1) CStandShoot.OnMoveKnifeL();
				else if (dir_horizontal == 2) CStandShoot.OnMoveKnifeR();
			}
			else
			{
				if (dir_horizontal == 1) CStandShoot.OnMoveShootL();
				else if (dir_horizontal == 2) CStandShoot.OnMoveShootR();
			}
		}
	}

	void CStand::resetShootAnimation() { CStandShoot.resetAnimation(); }

	void CStand::OnShow_Attack()
	{
		CStandShoot.SetXY(x, y);
		if (isOverlap)
		{
			if (direction == 1) CStandShoot.OnShowKnifeL();
			else if (direction == 2) CStandShoot.OnShowKnifeR();
		}
		else
		{
			if (direction == 1) CStandShoot.OnShowShootL();
			else if (direction == 2) CStandShoot.OnShowShootR();
		}
	}

	void CStand::OnShow_Stand()
	{
		CStandL.SetTopLeft(x, y);
		CStandR.SetTopLeft(x, y);
		if (direction == 1) CStandL.OnShow();
		else if (direction == 2) CStandR.OnShow();
	}

	//CStand
#pragma endregion

/////////////////////////////////////////////////////////////////////////////
// CJump: jump class
/////////////////////////////////////////////////////////////////////////////

#pragma region CJump

	CJump::CJump()
	{
		Initialize();
	}

	void CJump::Initialize()
	{
		const int FLOOR = 450;			//地板高度
		const int INI_VELOCITY = 30;	//初速
		direction = dir_horizontal = 1;	//預設方向向左
		velocity = ini_velocity = INI_VELOCITY;
		isRising = isFalling = canFall = false;
	}

#pragma region Loadpicture
		
	void CJump::LoadBitmap_KnifeL(char* file, char* file2)
	{
		CJumpShoot.LoadKnifeLeft(file, file2);
	}

	void CJump::LoadBitmap_KnifeR(char* file)
	{
		CJumpShoot.LoadKnifeRight(file);
	}

	void CJump::LoadBitmap_ShootL(char* file1, char* file2)
	{
		CJumpShoot.LoadShootLeft(file1, file2);
	}

	void CJump::LoadBitmap_ShootR(char* file)
	{
		CJumpShoot.LoadShootRight(file);
	}

	void CJump::LoadBitmap_FallL(char* file)
	{
		CFallL.AddBitmap(file, Blue);
	}

	void CJump::LoadBitmap_RiseL(char* file)
	{
		CRiseL.AddBitmap(file, Blue);
	}

	void CJump::LoadBitmap_FallR(char* file)
	{
		CFallR.AddBitmap(file, Blue);
	}

	void CJump::LoadBitmap_RiseR(char* file)
	{
		CRiseR.AddBitmap(file, Blue);
	}

#pragma endregion

#pragma region SetState

	void CJump::SetGameMap(CGameMap* _map)
	{
		gameMap = _map;
	}

	void CJump::SetRising(bool flag)
	{
		isRising = flag;
	}

	void CJump::SetFalling(bool flag)
	{
		isFalling = flag;
	}

	void CJump::SetDirection(int dir)
	{
		direction = dir;
		if (dir < 3) dir_horizontal = dir;
	}

	void CJump::SetCanFallFromBlock(bool flag) { canFall = flag; }

	void CJump::resetShootAnimation() { CJumpShoot.resetAnimation(); }

#pragma endregion


	bool CJump::OnMove(int *nx, int *ny)	//回傳isFalling的狀態
	{
		x = *nx;
		y = *ny;
		mapX = gameMap->getX();
		mapY = gameMap->getY();
		if (gameMap->getMapBlock(y - mapY + defaultHeight, x - mapX) == 2)		//在障礙物上，是否可以往下跳
		{
			if (isMovingDown)			
			{
				if (canFall)
				{
					isFalling = true;
					isRising = false;
					y += 20;
				}
			}
		}
		if (isRising)
		{
			if (isShooting)
			{
				if (dir_horizontal == 1) CJumpShoot.OnMoveShootL();
				else if (dir_horizontal == 2) CJumpShoot.OnMoveShootR();
			}
			else
			{
				if (dir_horizontal == 1) CRiseL.OnMove();
				else if (dir_horizontal == 2) CRiseR.OnMove();
			}
			
			if (velocity > 0)
			{
				y -= velocity;
				velocity-=2;
			}
			else
			{
				isRising = false;		//升到最高點要轉為下降
				isFalling = true;		//轉成下降
				velocity = 1;			//下降初速度
			}
		}
		else if(isFalling)
		{
			if (isShooting)
			{
				if (dir_horizontal == 1) CJumpShoot.OnMoveShootL();
				else if (dir_horizontal == 2) CJumpShoot.OnMoveShootR();
			}
			else
			{
				if (dir_horizontal == 1) CFallL.OnMove();
				else if (dir_horizontal == 2) CFallR.OnMove();
			}
			//因為y軸一開始就在最下面,所以要反向加才能得到正確到座標
			if (gameMap->getMapBlock(y - mapY + defaultHeight, x - mapX) == 0)		//底下沒有障礙物
			{
				y += velocity;
				velocity += 2;
				velocity = min(velocity, 19);
			}
			else
			{
				isFalling = false;
				velocity = ini_velocity;
			}
		}
		*nx = x;
		*ny = y;
		return isFalling;
	}

	int CJump::isEmpty(int nx, int ny)
	{
		int gx = nx / gameMap->getSize();
		int gy = ny / gameMap->getSize();
		return gameMap->getMapBlock(gy, gx);
	}


	void CJump::OnShow_Rise()
	{
		if (dir_horizontal == 1)
		{
			CRiseL.SetTopLeft(x, y);
			CRiseL.OnShow();
		}
		else if (dir_horizontal == 2)
		{
			CRiseR.SetTopLeft(x, y);
			CRiseR.OnShow();
		}
	}

	void CJump::OnShow_Fall()
	{
		if (dir_horizontal == 1)
		{
			CFallL.SetTopLeft(x, y);
			CFallL.OnShow();
		}
		else if (dir_horizontal == 2)
		{
			CFallR.SetTopLeft(x, y);
			CFallR.OnShow();
		}
	}

	void CJump::OnShow_Attack()
	{
		CJumpShoot.SetXY(x, y);
		if (dir_horizontal == 1) CJumpShoot.OnShowShootL();
		else if (dir_horizontal == 2) CJumpShoot.OnShowShootR();
	}

	//CJump
#pragma endregion

/////////////////////////////////////////////////////////////////////////////
// CCrouch: crouch class
/////////////////////////////////////////////////////////////////////////////

#pragma region CCrouch

	CCrouch::CCrouch()
	{
		Initialize();
	}

	void CCrouch::Initialize()
	{
		const int x_pos = 200;	//初始位置
		const int y_pos = 450;
		x = x_pos;
		y = y_pos;
		step = 10;
		direction = dir_horizontal = 1;
	}

	void CCrouch::OnMove(int nx, int ny)
	{
		if (isShooting)
		{
			if (isOverlap)
			{
				if (dir_horizontal == 1) CcrouchShoot.OnMoveKnifeL();
				else if (dir_horizontal == 2) CcrouchShoot.OnMoveKnifeR();
			}
			else
			{
				if (dir_horizontal == 1) CcrouchShoot.OnMoveShootL();
				else if (dir_horizontal == 2) CcrouchShoot.OnMoveShootR();
			}
			
		}
		else
		{
			if (dir_horizontal == 1)
			{
				CMoveL.OnMove();
			}
			else if (dir_horizontal == 2)
			{
				CMoveR.OnMove();
			}
		}
		x = nx;
		y = ny + 20;
	}

#pragma region LoadPicture

	void CCrouch::LoadBitmap_KnifeL(char* file, char* file2)
	{
		CcrouchShoot.LoadKnifeLeft(file, file2);
	}

	void CCrouch::LoadBitmap_KnifeR(char* file)
	{
		CcrouchShoot.LoadKnifeRight(file);
	}

	void CCrouch::LoadBitmap_ShootL(char* file,char* file2)
	{
		CcrouchShoot.LoadShootLeft(file, file2);
	}

	void CCrouch::LoadBitmap_ShootR(char* file)
	{
		CcrouchShoot.LoadShootRight(file);
	}

	void CCrouch::LoadBitmap_MoveL(char* file)
	{
		CMoveL.AddBitmap(file, Blue);
	}

	void CCrouch::LoadBitmap_MoveR(char* file)
	{
		CMoveR.AddBitmap(file, Blue);
	}

	void CCrouch::LoadBitmap_StandL(char* file)
	{
		CStandL.AddBitmap(file, Blue);
	}

	void CCrouch::LoadBitmap_StandR(char* file)
	{
		CStandR.AddBitmap(file, Blue);
	}

#pragma endregion

#pragma region OnShow

	void CCrouch::OnShow_Move()
	{
		if (dir_horizontal == 1)
		{
			CMoveL.SetTopLeft(x, y);
			CMoveL.OnShow();
		}
		else if (dir_horizontal == 2)
		{
			CMoveR.SetTopLeft(x, y);
			CMoveR.OnShow();
		}
	}

	void CCrouch::OnShow_Stand()
	{
		if (dir_horizontal == 1)
		{
			CStandL.SetTopLeft(x, y);
			CStandL.OnShow();
		}
		else if (dir_horizontal == 2)
		{
			CStandR.SetTopLeft(x, y);
			CStandR.OnShow();
		}
	}

	void CCrouch::OnShow_Attack()
	{
		CcrouchShoot.SetXY(x, y);
		if (isOverlap)
		{
			if (dir_horizontal == 1)
				CcrouchShoot.OnShowKnifeL();
			else if (dir_horizontal == 2)
				CcrouchShoot.OnShowKnifeR();
		}
		else
		{
			if (dir_horizontal == 1)
				CcrouchShoot.OnShowShootL();
			else if (dir_horizontal == 2)
				CcrouchShoot.OnShowShootR();
		}
	}
	//OnShow
#pragma endregion

	void CCrouch::SetDirection(int dir)
	{
		direction = dir;
		if (dir < 3)
			dir_horizontal = dir;
	}

	void CCrouch::resetShootAnimation() { CcrouchShoot.resetAnimation(); }

	//CCrouch
#pragma endregion

/////////////////////////////////////////////////////////////////////////////
// CAttack: attack class
/////////////////////////////////////////////////////////////////////////////

#pragma region CAttack

	CAttack::CAttack()
	{
		Initialize();
	}

	CAttack::~CAttack() {}

	void CAttack::Initialize()
	{
		x = y = width = 0;
	}

	void CAttack::LoadShootLeft(char *file, char *file2)
	{
		CShootLHero.AddBitmap(file,Blue);
		CShootLGun.AddBitmap(file2,Blue);
	}

	void CAttack::LoadShootRight(char *file)
	{
		CShootR.AddBitmap(file,Blue);
	}

	void CAttack::LoadKnifeLeft(char* file, char* file2)
	{
		CKnifeLHero.AddBitmap(file, Blue);
		CKnifeLKnife.AddBitmap(file2, Blue);
	}

	void CAttack::LoadKnifeRight(char* file)
	{
		CKnifeR.AddBitmap(file, Blue);
	}

	void CAttack::OnMoveShootL()
	{
		CShootLHero.OnMove();
		CShootLGun.OnMove();
	}

	void CAttack::OnMoveShootR()
	{
		CShootR.OnMove();
	}

	void CAttack::OnMoveKnifeL()
	{
		CKnifeLHero.OnMove();
		CKnifeLKnife.OnMove();
	}

	void CAttack::OnMoveKnifeR()
	{
		CKnifeR.OnMove();
	}

	void CAttack::OnShowShootL()
	{
		width = CShootLGun.Width();
		CShootLHero.SetTopLeft(x, y);
		CShootLHero.OnShow();
		CShootLGun.SetTopLeft(x - width, y);
		CShootLGun.OnShow();
	}

	void CAttack::OnShowShootR()
	{
		CShootR.SetTopLeft(x, y);
		CShootR.OnShow();
	}

	void CAttack::OnShowKnifeL()
	{
		width = CKnifeLKnife.Width();
		CKnifeLHero.SetTopLeft(x, y);
		CKnifeLHero.OnShow();
		CKnifeLKnife.SetTopLeft(x - width, y);
		CKnifeLKnife.OnShow();
	}

	void CAttack::OnShowKnifeR()
	{
		CKnifeR.SetTopLeft(x, y);
		CKnifeR.OnShow();
	}

	void CAttack::SetXY(int nx, int ny)
	{
		x = nx;
		y = ny;
	}

	void CAttack::resetAnimation()
	{
		CShootLGun.Reset();
		CShootLHero.Reset();
		CShootR.Reset();
		/*CKnifeLHero.Reset();
		CKnifeLKnife.Reset();
		CKnifeR.Reset();*/
	}

	//CShoot
#pragma endregion

/////////////////////////////////////////////////////////////////////////////
// CDead: dead class
/////////////////////////////////////////////////////////////////////////////

#pragma region CDead

	CDead::CDead()
	{
		Initialize();
	}

	CDead::~CDead() {}

	void CDead::Initialize()
	{
		x = y = 0;
		direction = 1;
	}

	void CDead::LoadBitmap_L(char* file)
	{
		CDeadL.AddBitmap(file, Blue);
	}

	void CDead::LoadBitmap_R(char* file, char* file2)
	{
		CDeadRMan.AddBitmap(file, Blue);
		CDeadRGun.AddBitmap(file2, Blue);
	}

	void CDead::OnMove(int nx, int ny)
	{
		x = nx;
		y = ny;
		if (direction == 1) CDeadL.OnMove();
		else
		{
			CDeadRMan.OnMove();
			CDeadRGun.OnMove();
		}
	}

	void CDead::OnShow()
	{
		if (direction == 1)
		{
			CDeadL.SetTopLeft(x, y);
			CDeadL.OnShow();
		}
		else
		{
			CDeadRMan.SetTopLeft(x, y);
			CDeadRGun.SetTopLeft(x - CDeadRGun.Width(), y);
			CDeadRMan.OnShow();
			CDeadRGun.OnShow();
		}
	}

	void CDead::SetDir(int dir) { direction = dir; }

	bool CDead::isfinalBitmap()
	{
		if (direction == 1)
		{
			if (CDeadL.IsFinalBitmap())
			{
				CDeadL.Reset();
				return true;
			}
		}
		else
		{
			if (CDeadRMan.IsFinalBitmap())
			{
				CDeadRMan.Reset();
				CDeadRGun.Reset();
				return true;
			}
		}
		return false;
	}

	//CDead
#pragma endregion

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲開頭畫面物件
/////////////////////////////////////////////////////////////////////////////

#pragma region CGameStateInit

	CGameStateInit::CGameStateInit(CGame *g)
		: CGameState(g)
	{
	}

	CGameStateInit::~CGameStateInit()
	{
		
	}

	void CGameStateInit::OnInit()
	{
		//
		// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
		//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
		//
		ShowInitProgress(0);	// 一開始的loading進度為0%
		//
		// 開始載入資料
		//
		ishoverBack = isShowAbout = isSoundShow = ishoverGo = ishoverExit = ishoverAbout = false;
		char *fileFire[] = { ".\\image\\interface\\95.bmp" , ".\\image\\interface\\96.bmp" , ".\\image\\interface\\97.bmp" , ".\\image\\interface\\98.bmp" };
		char *fileScream[] = { ".\\image\\interface\\scream\\1.bmp" , ".\\image\\interface\\scream\\2.bmp" , ".\\image\\interface\\scream\\3.bmp" , ".\\image\\interface\\scream\\4.bmp"
			, ".\\image\\interface\\scream\\5.bmp" , ".\\image\\interface\\scream\\6.bmp" , ".\\image\\interface\\scream\\7.bmp" , ".\\image\\interface\\scream\\8.bmp" };
		for (int i = 0; i < 8; i++) manScream.AddBitmap(fileScream[i], Black);
		for (int i = 0; i < 4; i++)
		{
			fire1.AddBitmap(fileFire[i], Black);
			fire3.AddBitmap(fileFire[i], Black);
			fire2.AddBitmap(fileFire[3 - i], Black);
			fire4.AddBitmap(fileFire[3 - i], Black);
		}
		gameUI.LoadBitmap(".\\image\\interface\\interfaceBig.bmp");
		btnGo.LoadBitmap(".\\image\\interface\\btnGo.bmp", Black);
		btnGoHover.LoadBitmap(".\\image\\interface\\btnGoHover.bmp", Black);
		hoverEffect.LoadBitmap(".\\image\\interface\\hoverEffect.bmp", Black);
		btnExit.LoadBitmap(".\\image\\interface\\btnExit.bmp", Black);
		btnExitHover.LoadBitmap(".\\image\\interface\\btnExitHover.bmp", Black);
		btnAbout.LoadBitmap(".\\image\\interface\\btnAbout.bmp", Black);
		btnAboutHover.LoadBitmap(".\\image\\interface\\btnAboutHover.bmp", Black);
		gameAbout.LoadBitmap(".\\image\\interface\\about.bmp");
		btnBack.LoadBitmap(".\\image\\interface\\btnBack.bmp", Black);
		btnBackHover.LoadBitmap(".\\image\\interface\\btnBackHover.bmp", Black);

		CAudio::Instance()->Load(fire, "sounds\\fire.mp3");
		CAudio::Instance()->Load(enemyScream, "sounds\\enemyScream.mp3");

		Sleep(300);				// 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
		//
		// 此OnInit動作會接到CGameStaterRun::OnInit()，所以進度還沒到100%
		//
	}

	void CGameStateInit::OnBeginState()
	{
	}

	void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		const char KEY_ESC = 27;
		const char KEY_SPACE = ' ';
		if (nChar == KEY_ESC)								// Demo 關閉遊戲的方法
			PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);	// 關閉遊戲
	}

	void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
	{
		if (isShowAbout)
		{
			if (ishoverBack)
			{
				CAudio::Instance()->Play(fire, true);
				CAudio::Instance()->Play(enemyScream, true);
				isShowAbout = false;
				ishoverBack = false;
			}
			return;
		}
		if (ishoverGo)
		{
			CAudio::Instance()->Stop(fire);
			CAudio::Instance()->Stop(enemyScream);
			GotoGameState(GAME_STATE_RUN);		// 切換至GAME_STATE_RUN
		}
		else if (ishoverAbout)
		{
			isShowAbout = true;
			CAudio::Instance()->Stop(fire);
			CAudio::Instance()->Stop(enemyScream);
		}
		else if (ishoverExit)
			PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);
	}

	void CGameStateInit::OnMouseMove(UINT nFlags, CPoint point)
	{
		if (isShowAbout)
		{
			if (point.x >= 0 && point.x <= 140 && point.y <= 600 && point.y >= 600 - 59)
				ishoverBack = true;
			else
				ishoverBack = false;
			return;
		}
		if (point.x >= 310 && point.x <= 310 + 154 && point.y >= 100 && point.y <= 100 + 175)
			ishoverGo = true;
		else if (point.x >= 550 && point.x <= 550 + 191 && point.y >= 50 && point.y <= 50 + 83)
			ishoverExit = true;
		else if (point.x >= 40 && point.x <= 40 + 186 && point.y >= 40 && point.y <= 40 + 56)
			ishoverAbout = true;
		else
		{
			ishoverGo = ishoverExit = ishoverAbout = false;
		}
	}

	void CGameStateInit::OnMove()
	{
		if (!isSoundShow)
		{
			CAudio::Instance()->Play(fire, true);
			CAudio::Instance()->Play(enemyScream, true);
			isSoundShow = true;
		}
		fire1.OnMove();
		fire2.OnMove();
		fire3.OnMove();
		fire4.OnMove();
		manScream.OnMove();
	}

	void CGameStateInit::OnShow()
	{

		if (isShowAbout)
		{
			gameAbout.SetTopLeft(0, 0);
			gameAbout.ShowBitmap();

			if (ishoverBack)
			{
				btnBackHover.SetTopLeft(0, 600 - btnBackHover.Height());
				btnBackHover.ShowBitmap();
			}
			else
			{
				btnBack.SetTopLeft(0, 600 - btnBack.Height());
				btnBack.ShowBitmap();
			}
		}
		else
		{
			gameUI.SetTopLeft(0, 0);
			gameUI.ShowBitmap();


			manScream.SetTopLeft(60, 230);
			manScream.OnShow();

#pragma region fireAnimation
			fire1.SetTopLeft(230, 427);
			fire1.OnShow();

			fire2.SetTopLeft(488, 380);
			fire2.OnShow();

			fire3.SetTopLeft(323, 315);
			fire3.OnShow();

			fire4.SetTopLeft(400, 420);
			fire4.OnShow();
#pragma endregion

#pragma region button

			if (!ishoverAbout)
			{
				btnAbout.SetTopLeft(40, 40);
				btnAbout.ShowBitmap();
			}
			else
			{
				btnAboutHover.SetTopLeft(40, 40);
				btnAboutHover.ShowBitmap();
			}

			if (!ishoverGo)
			{
				btnGo.SetTopLeft(310, 100);
				btnGo.ShowBitmap();
			}
			else
			{
				int x = 310 - (hoverEffect.Width() - btnGoHover.Width()) / 2;
				int y = 100 - (hoverEffect.Height() - btnGoHover.Height());
				btnGoHover.SetTopLeft(310, 100);
				hoverEffect.SetTopLeft(x, y);
				btnGoHover.ShowBitmap();
				hoverEffect.ShowBitmap();
			}

			if (!ishoverExit)
			{
				btnExit.SetTopLeft(550, 50);
				btnExit.ShowBitmap();
			}
			else
			{
				btnExitHover.SetTopLeft(550, 50);
				btnExitHover.ShowBitmap();
			}

#pragma endregion
		}	//!isShowAbout

	}

	//CGaneStateInit
#pragma endregion

								
/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的結束狀態(Game Over)
/////////////////////////////////////////////////////////////////////////////

#pragma region CGameStateOver

	CGameStateOver::CGameStateOver(CGame *g)
		: CGameState(g)
	{
	}

	void CGameStateOver::OnMove()
	{
		counter--;
		if (counter < 0)
			GotoGameState(GAME_STATE_INIT);
	}

	void CGameStateOver::OnBeginState()
	{
		counter = 30 * 5; // 5 seconds
	}

	void CGameStateOver::OnInit()
	{
		//
		// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
		//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
		//
		ShowInitProgress(66);	// 接個前一個狀態的進度，此處進度視為66%
		//
		// 開始載入資料
		//
		Sleep(300);				// 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
		//
		// 最終進度為100%
		//
		ShowInitProgress(100);
	}

	void CGameStateOver::OnShow()
	{
		CDC *pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC 
		CFont f, *fp;
		f.CreatePointFont(160, "Times New Roman");	// 產生 font f; 160表示16 point的字
		fp = pDC->SelectObject(&f);					// 選用 font f
		pDC->SetBkColor(RGB(0, 0, 0));
		pDC->SetTextColor(RGB(255, 255, 0));
		char str[80];								// Demo 數字對字串的轉換
		sprintf(str, "Game Over ! (%d)", counter / 30);
		pDC->TextOut(240, 210, str);
		pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
		CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
	}
	//CGameStateOver
#pragma endregion


/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
/////////////////////////////////////////////////////////////////////////////

#pragma region CGameStateRun

	CGameStateRun::CGameStateRun(CGame *g)
		: CGameState(g)
	{
	}

	CGameStateRun::~CGameStateRun()
	{
		CAudio::Instance()->Stop(AUDIO_normal_BGM);
		for (loop = 0; loop < vecEnemy.size(); loop++)
			delete vecEnemy[loop];
	}

	void CGameStateRun::OnBeginState()
	{

		gameMap.Initialize();		//重製地圖
		gameMap.InitialBullet();	//重製地圖物件
		hero.SetGameMap(&gameMap);

		//重製主角狀態
#pragma region heroStateReset
		hero.SetMovingDown(false);
		hero.SetMovingLeft(false);
		hero.SetMovingRight(false);
		hero.SetMovingUp(false);
		hero.SetRising(false);
		hero.SetShooting(false);
		hero.SetOverlap(false);
		hero.SetLock(false);
		hero.SetMapXY(0, SIZE_Y - 721);
		heroLife.SetInteger(10);
#pragma endregion

		for (loop = 0; loop < maxEnemyNumber; loop++)
			vecEnemy[loop]->Initialize();		//重新初始化敵人

		nowAliveEnemy = 0;				//清空場上敵人數

		seed = (unsigned)time(NULL);	//亂數設置
		srand(seed);					//亂數種子設置
		mapX = mapY = 0;				//地圖座標

		const_come1EnemyDelay = come1EnemyDelay = 50;
		const_come2EnemyDelay = come2EnemyDelay = 75;
		remainEnemy.SetInteger(maxEnemyNumber);
		stage = 0;					//關卡設置為0
		isFallBack = false;			//目前沒有撤退
		gameMap.setXY(0, 0);		//地圖座標(0,0)

		showMovie = true;
		movie.Initialize();
		movie.resetAnimation();

		isNormalBGMShow = isBossBGMShow = false;
	}

	void CGameStateRun::OnMove()							// 移動遊戲元素
	{
		// 如果希望修改cursor的樣式，則將下面程式的commment取消即可
		// SetCursor(AfxGetApp()->LoadCursor(IDC_GAMECURSOR));
		
		if (!isNormalBGMShow && !showMovie)
		{
			CAudio::Instance()->Play(AUDIO_normal_BGM, true);
			isNormalBGMShow = true;
		}

		if (showMovie)
		{
			movie.OnMove();
			return;
		}

		mapX = gameMap.getX();			//-2040、小boss
		mapY = gameMap.getY();

		if (stage == 1)
		{
			if (!isBossBGMShow)
			{
				CAudio::Instance()->Stop(AUDIO_normal_BGM);
				CAudio::Instance()->Play(bossBGM, true);
				isBossBGMShow = true;
			}
			if (mapX <= -2035 && mapX >= -2050)		//鎖定小Bosss場地
			{
				hero.SetLock(true);
			}
			else				//沒到的話要顯示提示
			{
				if (mapX > -2035) goR.SetTopLeft(SIZE_X - goR.Width(), SIZE_Y / 2 - goR.Height() / 2);
				else if (mapX < -2050) goL.SetTopLeft(0, SIZE_Y / 2 - goL.Height() / 2);
			}
		}


		if (nowAliveEnemy <= 0) hero.SetOverlap(false);

		if (stage == 0 || stage == 2)		//0、2皆為小兵關卡
		{
			if (!isFallBack)				//撤退不生成敵人、子彈、仍會對場上物件做判定
			{

#pragma region addEnemyControl
				//敵人生成控制
				if (nowAliveEnemy == 0)
					enemyProduce(1);
				if (come1EnemyDelay > 0) come1EnemyDelay--;
				else
				{
					enemyProduce(1);
					come1EnemyDelay = const_come1EnemyDelay;
				}
				if (come2EnemyDelay > 0) come2EnemyDelay--;
				else
				{
					enemyProduce(2);
					come2EnemyDelay = const_come2EnemyDelay;
				}
#pragma endregion

#pragma region BulletControl
				//敵人子彈生成、敵人追蹤
				for (loop = 0; loop < maxEnemyNumber; loop++)
				{
					if (vecEnemy[loop]->isShow())					//狀態判定
					{
						vecEnemy[loop]->SetMapXY(mapX, mapY);		//追蹤設定
						if (vecEnemy[loop]->getShootState() && vecEnemy[loop]->getAlive())			//確認填彈狀態，是否可以射擊&&存活
							gameMap.addEnemyBullet(vecEnemy[loop], hero.getX1(), hero.getY1());
					}
				}

#pragma endregion

			}
			else nowShowEnemy = 0;		//isFallBack = true

#pragma region 敵人和主角攻擊碰撞判定

#pragma region knifeEnemy
			//刀砍到敵人
			for (loop = 0; loop < maxEnemyNumber; loop++)			//主角敵人是否重疊
			{
				if (vecEnemy[loop]->getAlive())
				{
					if (hero.isOverlapEnemy(vecEnemy[loop]))		//沒有重疊已經寫在裡面
					{
						if (hero.getShooting() && hero.getOverlap())	//刀砍，所有重疊的都會死
						{
							remainEnemy.Add(-1);
							nowAliveEnemy--;
							vecEnemy[loop]->SetDead(true, hero.getDir_hor());
							vecEnemy[loop]->SetAlive(false);
						}
						break;
					}

				}
			}
#pragma endregion

#pragma region shootEnemy
			//敵人被子彈射到判斷
			for (loop = 0; loop < maxEnemyNumber; loop++)
			{
				if (vecEnemy[loop]->getAlive())
				{
					if (gameMap.isBulletHit(vecEnemy[loop]))					//敵人是否被射到
					{
						remainEnemy.Add(-1);									//剩餘人數-1
						nowAliveEnemy--;										//現存人數-1
						CAudio::Instance()->Play(AUDIO_enemyDead, false);		//撥放敵人死亡聲音
						vecEnemy[loop]->SetDead(true, hero.getDir_hor());		//設定死亡&方向
						vecEnemy[loop]->SetAlive(false);						//設定存活
					}
				}
			}
#pragma endregion

#pragma region shootHero
			//主角被射到判斷
			if (gameMap.isBulletHit(&hero))
				heroLife.Add(-1);

#pragma endregion

#pragma endregion

#pragma region 敵人的移動

			nowShowEnemy = 0;
			for (loop = 0; loop < maxEnemyNumber; loop++)
			{
				if (vecEnemy[loop]->isShow())
				{
					nowShowEnemy++;
					if (gameMap.getMapBlock(vecEnemy[loop]->getY2() - mapY, vecEnemy[loop]->getX1() - mapX) == 0) vecEnemy[loop]->SetOnBlock(false);
					else vecEnemy[loop]->SetOnBlock(true);
				}
				vecEnemy[loop]->SetDirection(hero.getX1());
				vecEnemy[loop]->OnMove();
			}

#pragma endregion

#pragma region changeStage
		if (remainEnemy.GetInteger() <= 0)
		{	
			//敵人撤退
			if (!isFallBack)
			{
				isFallBack = true;
				CAudio::Instance()->Play(enemyScream, false);
				for (loop = 0; loop < maxEnemyNumber; loop++)
				{
					if (vecEnemy[loop]->getAlive())
						vecEnemy[loop]->SetFallBack(true, hero.getX1());
				}
			}
			else
			{
				if (nowShowEnemy == 0)
				{
					stage++;							//關卡數++，進入Boss戰
					remainEnemy.SetInteger(0);			//Boss戰，敵人人數調成0
				}
			}
		}

#pragma endregion

		}		//stage 0、2

		if (stage == 4)
		{
			CAudio::Instance()->Stop(AUDIO_normal_BGM);
			CAudio::Instance()->Stop(bossBGM);
			GotoGameState(GAME_STATE_OVER);
		}

		gameMap.OnMoveBullet();			//雙方子彈移動
		gameMap.killBullet();			//雙方子彈刪除(超出範圍)
		hero.OnMove();					//主角移動

#pragma region endGame
		if (heroLife.GetInteger() <= 0)
		{
			//CAudio::Instance()->Stop(AUDIO_BGM_normal);
			//GotoGameState(GAME_STATE_OVER);
		}

#pragma endregion

		midBoss.OnMove();

	}	//OnMove

	void CGameStateRun::enemyProduce(int n)
	{
#pragma region addEnemyControl
		//敵人生成控制

		for (loop = 0; loop < maxEnemyNumber && n > 0; loop++)
		{
			int pos = rand() % 2;						//用亂數去處理要從哪邊進入畫面
			if (!vecEnemy[loop]->isShow())
			{
				n--;									//生成人數--
				nowAliveEnemy++;						//存活人數++
				gunMode = rand() % 4;					//0手槍 1散彈槍 2機槍 3狙擊槍 
				vecEnemy[loop]->SetAlive(true);			//設定成存活
				vecEnemy[loop]->SetGunMode(gunMode);	//設定槍枝種類
				vecEnemy[loop]->SetID(loop);			//設定探員ID，用來處理位置
				//尚有BUG待修改
				if (pos == 0) vecEnemy[loop]->SetDestination(60 + 5 * loop, 360, pos);
				else vecEnemy[loop]->SetDestination(614 - 5 * loop, 360, pos);
				break;
			}
		}
#pragma endregion
	}		//enemyProduce

	void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
	{
		
		//
		// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
		//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
		//
		ShowInitProgress(33);	// 接個前一個狀態的進度，此處進度視為33%
		//
		// 開始載入資料
		// 完成部分Loading動作，提高進度
		ShowInitProgress(50);
		//
		// 繼續載入其他資料
		//
		gameMap.LoadBitmap();
		goL.LoadBitmap(".\\image\\number\\goL.bmp", Blue);
		goR.LoadBitmap(".\\image\\number\\goR.bmp", Blue);

#pragma region EnemyInitial

		maxEnemyNumber = 10;				//最大敵人數
		remainEnemy.SetInteger(maxEnemyNumber);
		if (vecEnemy.size() == 0)
		{
			for (loop = 0; loop < maxEnemyNumber; loop++)		
				vecEnemy.push_back(new CEnemy());				
		}
		else
		{
			for (loop = 0; loop < maxEnemyNumber; loop++)
			{
				vecEnemy[loop]->SetAlive(false);
				vecEnemy[loop]->SetDead(false);
			}
		}
		for (loop = 0; loop < maxEnemyNumber; loop++)
			vecEnemy[loop]->LoadBitmap();

#pragma endregion

		hero.LoadBitmap();
		heroLife.LoadBitmap();
		heroImg.LoadBitmap(".\\image\\number\\heroLife.bmp", Blue);
		heroImg.SetTopLeft(400, 0);
		heroLife.SetTopLeft(400 + heroImg.Width() + 10, 0);

		remainEnemy.LoadBitmap();
		enemyImg.LoadBitmap(".\\image\\number\\remainEnemy.bmp", Blue);
		enemyImg.SetTopLeft(0, 0);
		remainEnemy.SetTopLeft(enemyImg.Width() + 10, 0);
		
		midBoss.LoadBitmap();

		movie.LoadBitmap();
#pragma region LoadMP3

		CAudio::Instance()->Load(Movie_enemyGone, "sounds\\movie\\enemyGone.mp3");
		CAudio::Instance()->Load(Movie_enemyShow, "sounds\\movie\\enemyShow.mp3");
		CAudio::Instance()->Load(Movie_FBItalk, "sounds\\movie\\FBItalk.mp3");
		CAudio::Instance()->Load(Movie_HeroBlink, "sounds\\movie\\heroBlink.mp3");
		CAudio::Instance()->Load(Movie_KidWavehand, "sounds\\movie\\KidWave.mp3");
		CAudio::Instance()->Load(Movie_ufoCrash, "sounds\\movie\\ufoCrash.mp3");
		CAudio::Instance()->Load(Movie_ufoExplode, "sounds\\movie\\ufoExplode.mp3");
		CAudio::Instance()->Load(AUDIO_heroJump, "sounds\\heroJump.mp3");
		CAudio::Instance()->Load(AUDIO_enemyDead, "sounds\\enemyDead.mp3");
		CAudio::Instance()->Load(AUDIO_normal_BGM, "sounds\\BGM_normal.mp3");
		CAudio::Instance()->Load(bossBGM, "sounds\\bossBGM.mp3");
		CAudio::Instance()->Load(Movie_moon, "sounds\\movie\\moon.mp3");

#pragma endregion

		//0-第一關小兵、1-小boss、2-第二關小兵、3-最終boss
		stage = 0;
		
	}

	void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		const char KEY_LEFT = 0x25;		// keyboard左箭頭
		const char KEY_UP = 0x26;		// keyboard上箭頭
		const char KEY_RIGHT = 0x27;	// keyboard右箭頭
		const char KEY_DOWN = 0x28;		// keyboard下箭頭
		const char KEY_A = 0x41;		// keyboard a
		const char KEY_S = 0x53;		// keyboard s
		const char KEY_Q = 0x51;
		const char KEY_R = 0x52;

		//左1 右2 上3 下4
		if (nChar == KEY_LEFT)
		{
			hero.SetMovingLeft(true);
			hero.SetDirection(1);
		}
		if (nChar == KEY_RIGHT)
		{
			hero.SetMovingRight(true);
			hero.SetDirection(2);
		}
		if (nChar == KEY_UP)
		{
			//hero.SetMovingUp(true);
			//hero.SetDirection(3);
		}
		if (nChar == KEY_DOWN)
		{
			if (!hero.isNowRising())
			{
				hero.SetMovingDown(true);
				hero.SetDirection(4);
			}
		}
		if (nChar == KEY_A)
		{
			hero.SetShooting(true);
			if(!hero.getOverlap()) 
				gameMap.addBullet(hero.getX1(), hero.getY1(), hero.getDir(), hero.getDir_hor());
		}
		if (nChar == KEY_S)
		{
			hero.SetFallDownFromBlock(true);
			hero.SetRising(true);
		}
		if (nChar == KEY_Q)
		{
			if (showMovie) showMovie = false;
			if (stage == 1 || stage == 3)
			{
				stage++;
				isFallBack = false;
				hero.SetLock(false);
				remainEnemy.SetInteger(maxEnemyNumber);
				for (loop = 0; loop < maxEnemyNumber; loop++)
				{
					vecEnemy[loop]->Initialize();
				}
				CAudio::Instance()->Stop(bossBGM);
				isNormalBGMShow = false;
			}
		}
		if (nChar == KEY_R)
		{
			CAudio::Instance()->Stop(AUDIO_normal_BGM);
			CAudio::Instance()->Play(fire, true);
			CAudio::Instance()->Play(enemyScream, true);
			GotoGameState(GAME_STATE_INIT);
		}
	}

	void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		const char KEY_LEFT = 0x25;		// keyboard左箭頭
		const char KEY_UP = 0x26;		// keyboard上箭頭
		const char KEY_RIGHT = 0x27;	// keyboard右箭頭
		const char KEY_DOWN = 0x28;		// keyboard下箭頭
		const char KEY_A = 0x41;		//keyboard A
		const char KEY_S = 0x53;		//keyboard s

		if (nChar == KEY_LEFT)
		{
			hero.SetMovingLeft(false);
			hero.ResumeDirection();
		}
		if (nChar == KEY_RIGHT)
		{
			hero.SetMovingRight(false);
			hero.ResumeDirection();
		}
		if (nChar == KEY_UP)
		{
			//hero.SetMovingUp(false);
			//hero.ResumeDirection();
		}
		if (nChar == KEY_DOWN)
		{
			hero.SetMovingDown(false);
			hero.ResumeDirection();
		}
		if (nChar == KEY_A)
		{
			//hero.SetShooting(false);
		}
		if (nChar == KEY_S)
		{
			hero.SetFallDownFromBlock(false);
		}
	}

#pragma region OnButton
	void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
	{
		//eraser.SetMovingLeft(true);
	}

	void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
	{
		//eraser.SetMovingLeft(false);
	}

	void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// 處理滑鼠的動作
	{
		// 沒事。如果需要處理滑鼠移動的話，寫code在這裡
	}

	void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
	{
		//eraser.SetMovingRight(true);
	}

	void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
	{
		//eraser.SetMovingRight(false);
	}
#pragma endregion

	void CGameStateRun::OnShow()
	{

		if (showMovie)
		{
			showMovie = movie.OnShow();
			return;
		}

		gameMap.OnShow();

#pragma region mapInstruction
		enemyImg.ShowBitmap();
		remainEnemy.ShowBitmap();
		heroImg.ShowBitmap();
		heroLife.ShowBitmap();
		if (stage == 1)
		{
			if (mapX > -2035) goR.ShowBitmap();
			else if (mapX < -2050) goL.ShowBitmap();
		}
#pragma endregion

		for (loop = 0; loop < maxEnemyNumber; loop++)
			if (vecEnemy[loop]->isShow()) vecEnemy[loop]->OnShow();

		midBoss.OnShow();

		hero.OnShow();
		gameMap.OnShowBullet();
		
	}
	//CGameStateRun
#pragma endregion



}	//namespace game_framwork