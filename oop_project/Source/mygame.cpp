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
		y = 525 - 115;
		deadDelay = constDeadDelay = 20;			//死後無敵時間
		delayCount = constDelay = 9;
		direction = dir_horizontal = 1;				//預設向左
		isDead = isFalling = false;
#pragma region heroStateReset
		SetMovingDown(false);
		SetMovingLeft(false);
		SetMovingRight(false);
		SetMovingUp(false);
		SetRising(false);
		SetShooting(false);
		SetOverlap(false);
		SetLock(false);
		SetMapXY(0, SIZE_Y - 721);
#pragma endregion
		canDead = true;
		heroLife = 10;
	}

	void CHero::LoadBitmap()
	{
	#pragma region fileInput
		char *fileDefaultStand[] = { ".\\image\\stand\\L1.bmp" };
		char *fileDefaultCrouch[] = { ".\\image\\crouch\\L1.bmp" };

		char *fileKnifeL1[] = { ".\\image\\knife\\L1-1.bmp" , ".\\image\\knife\\L2-1.bmp" , ".\\image\\knife\\L3-1.bmp" , ".\\image\\knife\\L4-1.bmp" };
		char *fileKnifeL2[] = { ".\\image\\knife\\L1-2.bmp" , ".\\image\\knife\\L2-2.bmp" , ".\\image\\knife\\L3-2.bmp" , ".\\image\\knife\\L4-2.bmp" };
		char *fileKnifeR[] = { ".\\image\\knife\\R1.bmp" , ".\\image\\knife\\R2.bmp" , ".\\image\\knife\\R3.bmp" , ".\\image\\knife\\R4.bmp" };

#pragma region fileDead

		char *fileDeadL[] = { ".\\image\\dead\\L1.bmp" , ".\\image\\dead\\L2.bmp" , ".\\image\\dead\\L3.bmp" , ".\\image\\dead\\L4.bmp"
		, ".\\image\\dead\\L5.bmp" , ".\\image\\dead\\L6.bmp" , ".\\image\\dead\\L7.bmp" , ".\\image\\dead\\L8.bmp" };
		char *fileDeadR1[] = { ".\\image\\dead\\R1-1.bmp" , ".\\image\\dead\\R2-1.bmp" , ".\\image\\dead\\R3-1.bmp" , ".\\image\\dead\\R4-1.bmp"
		, ".\\image\\dead\\R5-1.bmp" , ".\\image\\dead\\R6-1.bmp" , ".\\image\\dead\\R7-1.bmp" , ".\\image\\dead\\R8-1.bmp" };
		char *fileDeadR2[] = { ".\\image\\dead\\R1-2.bmp" , ".\\image\\dead\\R2-2.bmp" , ".\\image\\dead\\R3-2.bmp" , ".\\image\\dead\\R4-2.bmp"
		, ".\\image\\dead\\R5-2.bmp" , ".\\image\\dead\\R6-2.bmp" , ".\\image\\dead\\R7-2.bmp" , ".\\image\\dead\\R8-2.bmp" };

#pragma endregion


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

		for (int i = 0; i < 8; i++)
		{
			heroDead.LoadBitmap_L(fileDeadL[i]);
			heroDead.LoadBitmap_R(fileDeadR1[i], fileDeadR2[i]);
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

	void CHero::ResumeDead()
	{
		if (!canDead)
		{
			if (deadDelay > 0) deadDelay--;
			else
			{
				deadDelay = constDeadDelay;
				canDead = true;
			}
		}
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
		if (isDead)
		{
			heroDead.OnMove(x, y);
			canDead = false;
			return;
		}
		ResumeDead();							//死亡無敵時間
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
		if (isDead)
		{
			heroDead.OnShow();
			if (heroDead.isfinalBitmap())
			{
				isDead  = isRising = isMovingDown = isOverlap
					= isMovingLeft = isMovingRight = isMovingUp = isShooting = false;
				SetShooting(false);
				SetOverlap(false);
				SetRising(false);
				isFalling = true;
				gameMap->ClearMapBullet();
			}
			return;
		}

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
			heroDead.SetDir(dir);
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

		void CHero::AddLife(int n)
		{
			if (!isDead && canDead)
			{
				heroLife += n;
				isDead = true;
			}
		}

	#pragma endregion

	#pragma region GetState
		
		int CHero::getLife() { return heroLife; }

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

		bool CHero::getDead() { return isDead; }

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
			isGetRandomX = isAttack = isStand = isStart = isDead = isAlive = false;
			laserX = laserY = 800;
			x = midBossAction = 0;
			y = 525 - 237;
			bossLife = 50;
			step = 25;
			midBossMove.SetStep(step);
			midBossLaserOn.SetDelayCount(5);
			midBossLaserOff.SetDelayCount(5);
			delay = const_delay = 30;
		}

		void CMidBoss::LoadBitmap()
		{
			char *fileLaser[] = { ".\\image\\midBoss\\attack\\L1.bmp" , ".\\image\\midBoss\\attack\\L2.bmp"
			, ".\\image\\midBoss\\attack\\L3.bmp" , ".\\image\\midBoss\\attack\\L4.bmp" };
			char *fileStandL[] = { ".\\image\\midBoss\\stand\\L1.bmp" };
			char *fileStandR[] = { ".\\image\\midBoss\\stand\\R1.bmp" };
			char *fileRunL[] = { ".\\image\\midBoss\\run\\L1.bmp" , ".\\image\\midBoss\\run\\L2.bmp" 
				, ".\\image\\midBoss\\run\\L3.bmp" , ".\\image\\midBoss\\run\\L4.bmp" };
			char *fileRunR[] = { ".\\image\\midBoss\\run\\R1.bmp" , ".\\image\\midBoss\\run\\R2.bmp"
				, ".\\image\\midBoss\\run\\R3.bmp" , ".\\image\\midBoss\\run\\R4.bmp" };
			char *fileDead[] = { ".\\image\\movie\\explode1.bmp" , ".\\image\\movie\\explode2.bmp" , ".\\image\\movie\\explode3.bmp"
			,".\\image\\movie\\explode4.bmp" , ".\\image\\movie\\explode5.bmp" , ".\\image\\movie\\explode6.bmp" };
			caution.LoadBitmap(".\\image\\midBoss\\attack\\caution.bmp", Blue);
			laserLightH.LoadBitmap(".\\image\\midBoss\\attack\\laserH.bmp", Black);
			laserLightV.LoadBitmap(".\\image\\midBoss\\attack\\laserV.bmp", Black);
			midBossLaserHead.LoadBitmap(".\\image\\midBoss\\attack\\D1.bmp", Blue);
			midBossStand.LoadBitmap_StandL(fileStandL[0]);
			midBossStand.LoadBitmap_StandR(fileStandR[0]);
			midBossDefault.LoadBitmap(fileStandL[0]);
			for (int i = 0; i < 4; i++)
			{
				midBossMove.LoadBitmap_MoveL(fileRunL[i]);
				midBossMove.LoadBitmap_MoveR(fileRunR[i]);
				midBossLaserOn.AddBitmap(fileLaser[i], Blue);
				midBossLaserOff.AddBitmap(fileLaser[3 - i], Blue);
			}
			for (int i = 0; i < 6; i++) midBossDead.AddBitmap(fileDead[i], Black);
			defaultWidth = midBossDefault.Width();
			defaultHeight = midBossDefault.Height();
		}

		void CMidBoss::OnMove()
		{
			if (isDead)
			{
				midBossDead.OnMove();
				return;
			}
			if (isStart)
			{
				if (x < 800 - defaultWidth)
				{
					x = 800 - defaultWidth;
					midBossMove.SetMovingLeft(false);
					isStart = false;
				}
				else if (x > 800 - defaultWidth)
				{
					x -= 25;
					midBossMove.SetMovingLeft(true);
					midBossMove.SetDirection(1);
				}
			}
			else		//!isStart
			{
				if (isStand)
				{
					if (delay > 0) delay--;
					else
					{
						if (x == 0)						//在最左邊，準備向右跑
						{
							isStand = false;			//取消站立，等等可移動
							midBossMove.SetMovingRight(true);
							midBossMove.SetDirection(2);
							delay = const_delay;
						}
						else										//在最右邊，準備做攻擊，之後再向左跑
						{
							if (midBossAction == 0)
								midBossAction++;
							else if (midBossAction == 1 && !isAttack)	//變成攻擊模式
								midBossLaserOn.OnMove();
							else if (midBossAction == 2)				//轉回移動模式
								midBossLaserOff.OnMove();
							else if (midBossAction == 3)				//往左跑
							{
								isStand = false;
								midBossMove.SetMovingLeft(true);
								midBossMove.SetDirection(1);
								delay = const_delay;
							}
							if (isAttack)							//攻擊中
							{
								laserX -= 50;
								laserY += 50;
								laserLightH.SetTopLeft(laserX,
									y + defaultHeight - midBossLaserOn.Height() + 10);
								laserLightV.SetTopLeft(randomX + 10, laserY);
							}
						}
						
					}
				}
				else	//!isStand
				{
					delay = const_delay;
					if (x <= 0)								//最左邊，接著往回跑
					{
						x = 0;
						isStand = true;
						midBossStand.SetDirection(2);
						midBossMove.SetMovingLeft(false);
						CAudio::Instance()->Play(midBoss_Stand, false);
					}
					else if (x >= 800 - defaultWidth)		//最右邊，接著做攻擊
					{
						x = 800 - defaultWidth;
						isStand = true;
						midBossStand.SetDirection(1);
						midBossMove.SetMovingRight(false);
						midBossAction = 0;
					}
				}
			}
			midBossStand.OnMove(x, y);
			midBossMove.OnMove(&x, &y);
		}

		bool CMidBoss::OnShow()
		{
			if (isDead)
			{
				midBossStand.SetXY(x, y);
				midBossStand.OnShow_Stand();
				midBossDead.SetTopLeft(x + defaultWidth - midBossDead.Width()
					, y + defaultHeight - midBossDead.Height());
				midBossDead.OnShow();
				if (midBossDead.IsFinalBitmap())
				{
					CAudio::Instance()->Stop(bossBGM);
					midBossDead.Reset();
					isDead = false;
					return true;		//關卡切換
				}
				return false;
			}
			if (!isAlive) return false;
			if (isStart)
			{
				if (x == 800 - defaultWidth)
				{
					midBossStand.SetXY(x, y);
					midBossStand.OnShow_Stand();
				}
				else
				{
					midBossMove.SetXY(x, y);
					midBossMove.OnShow();
				}
				return false;
			}
			if (isStand)
			{
				if (x == 0)
				{
					midBossStand.SetXY(x, y);
					midBossStand.OnShow_Stand();
				}
				else
				{
					if (isAttack)					//攻擊中，攻擊模式仍然要顯示
					{
						midBossLaserHead.SetTopLeft(randomX, -80);
						laserLightH.ShowBitmap();
						laserLightV.ShowBitmap();
						midBossLaserHead.ShowBitmap();						
						if (laserX + laserLightH.Width() <= 0)		//攻擊結束
						{
							midBossAction++;		//轉回移動模式
							isAttack = false;
							midBossLaserOn.Reset();
							isGetRandomX = false;	//重新取得random
						}
					}
					if (midBossAction == 0 || midBossAction == 3)
					{
						midBossStand.SetXY(x, y);
						midBossStand.OnShow_Stand();
					}
					else if (midBossAction == 1)			//轉變成攻擊模式
					{
						if (!isGetRandomX)
						{
							randomX = rand() % 400;
							isGetRandomX = true;
						}
						midBossLaserOn.SetTopLeft(x + defaultWidth - midBossLaserOn.Width()
							, y + defaultHeight - midBossLaserOn.Height());
						midBossLaserOn.OnShow();
						if (!isAttack)
						{
							caution.SetTopLeft(randomX, 0);
							caution.ShowBitmap();
						}
						if (midBossLaserOn.IsFinalBitmap() && !isAttack)
						{
							laserX = x;				//雷射光座標
							laserY = - laserLightV.Height();
							isAttack = true;
							CAudio::Instance()->Play(midBoss_Laser, false);
						}
					}
					else if (midBossAction == 2)		//轉回移動模式
					{
						midBossLaserOff.SetTopLeft(x + defaultWidth - midBossLaserOff.Width()
							, y + defaultHeight - midBossLaserOff.Height());
						midBossLaserOff.OnShow();
						if (midBossLaserOff.IsFinalBitmap())	//準備往左跑
						{
							midBossLaserOff.Reset();
							midBossAction++;		//先轉回站著不動
							delay = const_delay;	//站著等
							CAudio::Instance()->Play(midBoss_Stand, false);
						}
					}
				}	//x!=0
			}
			else
			{
				midBossMove.SetXY(x, y);
				midBossMove.OnShow();
			}
			return false;		//不用換關卡
		}

		void CMidBoss::AddLife(int n)
		{
			bossLife += n; 
			if (bossLife <= 0) 
			{
				isAlive = false;
				isDead = true;
				CAudio::Instance()->Play(Movie_ufoExplode, false);
			}
		}

		void CMidBoss::SetStart(bool flag) 
		{ 
			isAlive = isStart = flag; 
			x = 800;
			y = 525 - defaultHeight;
		}

		int CMidBoss::getLife() { return bossLife; }

		int CMidBoss::getX1() 
		{ 
			if(isStand) return x; 
			else return x + 126 - defaultWidth / 2;
		}

		int CMidBoss::getX2() { return x + defaultWidth; }

		int CMidBoss::getY1() { return y; }

		int CMidBoss::getY2() { return y + defaultHeight; }

		int CMidBoss::getLaserVX1() { return randomX + 10; }

		int CMidBoss::getLaserVX2() { return randomX + 10 + laserLightV.Width(); }

		int CMidBoss::getLaserVY1() { return laserY; }

		int CMidBoss::getLaserVY2() { return laserY + laserLightV.Height(); }

		bool CMidBoss::getAlive() { return isAlive; }

		bool CMidBoss::getDead() { return isDead; }

		bool CMidBoss::getShow() { return (isDead || isAlive); }

		bool CMidBoss::isHitHero(CHero *hero)
		{
			if(isStart) return false;
			if (hero->getX1() <= getX2() && hero->getX2() >= getX1()
				&& hero->getY1() <= getY2() && hero->getY2() >= getY1())
				return true;
			if (hero->getX1() <= laserX + laserLightH.Width() && hero->getX2() >= laserX
				&& hero->getY1() <= y + defaultHeight - midBossLaserOn.Height() + 10 + laserLightH.Height() && hero->getY2() >= y + defaultHeight - midBossLaserOn.Height() + 10)
				return true;
			if (hero->getX1() <= randomX + 10 + laserLightV.Width() && hero->getX2() >= randomX + 10
				&& hero->getY1() <= laserY + laserLightV.Height() && hero->getY2() >= laserY)
				return true;
			return false;
		}

	//CBoss
#pragma endregion

/////////////////////////////////////////////////////////////////////////////
// CKid: kid class
/////////////////////////////////////////////////////////////////////////////

#pragma region CKid
		CKid::CKid()
		{
			Initialize();
		}

		CKid::~CKid() {}

		void CKid::Initialize()
		{
			x = 600;
			y = 525 - 155; 
			isAlive = true;
			isDead =  false;
			step = 5;
			kidWalk.SetDelayCount(5);
			kidDead.SetDelayCount(5);
		}

		void CKid::LoadBitmap()
		{
			char *filewalk[] = { ".\\image\\kid\\walk\\1.bmp" , ".\\image\\kid\\walk\\2.bmp"
			, ".\\image\\kid\\walk\\3.bmp" , ".\\image\\kid\\walk\\4.bmp" };
			char *filedead[] = { ".\\image\\kid\\dead\\1.bmp" , ".\\image\\kid\\dead\\2.bmp" , ".\\image\\kid\\dead\\3.bmp"
			, ".\\image\\kid\\dead\\4.bmp" , ".\\image\\kid\\dead\\5.bmp" , ".\\image\\kid\\dead\\6.bmp" };

			for (int i = 0; i < 4; i++) kidWalk.AddBitmap(filewalk[i], Blue);
			for (int i = 0; i < 6; i++) kidDead.AddBitmap(filedead[i], Blue);
			kidDefault.LoadBitmap(".\\image\\kid\\walk\\1.bmp");
		}

		void CKid::OnMove()
		{
			if (isAlive)
			{
				x -= step;
				kidWalk.OnMove();
				if (x < -kidWalk.Width()) isAlive = false;
			}
			else if (isDead)
			{
				kidDead.OnMove();
			}
		}

		void CKid::OnShow()
		{
			if (isAlive)
			{
				kidWalk.SetTopLeft(x, y);
				kidWalk.OnShow();
			}
			else if (isDead)
			{
				kidDead.SetTopLeft(x, y);
				kidDead.OnShow();
				if (kidDead.IsFinalBitmap())
				{
					kidDead.Reset();
					isDead = false;
				}
			}
		}

		void CKid::SetAlive(bool flag) { isAlive = flag; }

		void CKid::SetDead(bool flag) { isDead = flag; }

		int CKid::getX1() { return x;}
		
		int CKid::getX2() { return x + kidDefault.Width(); }

		int CKid::getY1() { return y; }
		
		int CKid::getY2() { return y + kidDefault.Height(); }

		bool CKid::getShow() { return (isAlive || isDead); }

		bool CKid::getAlive() { return isAlive; }

		bool CKid::getDead() { return isDead; }

		//CKid
#pragma endregion

/////////////////////////////////////////////////////////////////////////////
// CFinalBoss: finalBoss class
/////////////////////////////////////////////////////////////////////////////

#pragma region CFinalBoss

		CFinalBoss::CFinalBoss()
		{
			Initialize();
		}

		CFinalBoss::~CFinalBoss() {}

		void CFinalBoss::Initialize()
		{
			y = 0;
			x = 236;
			handX = handY = 800;
			bulletX = bulletY = -100;
			bulletStep = 20;
			bossLife = 10;
			gunLife = handVLife = handLife = 5;
			handDelay = const_handDelay = 150;
			handVDelay = const_handVDelay = 30;
			gunBullet.SetDelayCount(4);
			gunBulletExplode.SetDelayCount(4);
			explode.SetDelayCount(5);
			canHandAttack = canHandVAttack = handOnground = handVOnground = canCaution =
				canCaution2 = showGunExplode = isHandHitGround = isHandVHitGround = false;
			isStart = true;
		}

		void CFinalBoss::LoadBitmap()
		{
			char *fileFoot[] = { ".\\image\\finalBoss\\foot1.bmp" , ".\\image\\finalBoss\\foot2.bmp" };
			char *fileHand[] = { ".\\image\\finalBoss\\hand1.bmp" , ".\\image\\finalBoss\\hand2.bmp" };
			char *fileBullet[] = { ".\\image\\finalBoss\\bullet\\b1.bmp" , ".\\image\\finalBoss\\bullet\\b2.bmp" , ".\\image\\finalBoss\\bullet\\b3.bmp" };
			char *fileBulletExplode[] = { ".\\image\\finalBoss\\bullet\\h1.bmp" , ".\\image\\finalBoss\\bullet\\h2.bmp" 
				, ".\\image\\finalBoss\\bullet\\h3.bmp" , ".\\image\\finalBoss\\bullet\\h4.bmp" };
			char *fileExplode[] = { ".\\image\\finalBoss\\explode\\explode1.bmp" , ".\\image\\finalBoss\\explode\\explode2.bmp" , ".\\image\\finalBoss\\explode\\explode3.bmp"
			,".\\image\\finalBoss\\explode\\explode4.bmp" , ".\\image\\finalBoss\\explode\\explode5.bmp" , ".\\image\\finalBoss\\explode\\explode6.bmp" };
			
			for (int i = 0; i < 4; i++)
			{
				if (i < 2)
				{
					bossFoot.AddBitmap(fileFoot[i], Blue);
					bossHand.AddBitmap(fileHand[i], Blue);
				}
				if (i < 3)
				{
					gunBullet.AddBitmap(fileBullet[i], Black);
				}
				gunBulletExplode.AddBitmap(fileBulletExplode[i], Black);
			}
			for (int i = 0; i < 6; i++)
				explode.AddBitmap(fileExplode[i], Black);
			bossGun.LoadBitmap(".\\image\\finalBoss\\gun.bmp", Blue);
			bossBody.LoadBitmap(".\\image\\finalBoss\\body.bmp", Blue);
			bossHead.LoadBitmap(".\\image\\finalBoss\\head1.bmp", Blue);
			bossHead2.LoadBitmap(".\\image\\finalBoss\\head2.bmp", Blue);
			bossHandV.LoadBitmap(".\\image\\finalBoss\\handV.bmp", Blue);
			handBullet.LoadBitmap(".\\image\\finalBoss\\bullet\\hand.bmp", Blue);
			caution.LoadBitmap(".\\image\\finalBoss\\caution.bmp", Blue);
			caution2.LoadBitmap(".\\image\\finalBoss\\caution2.bmp", Blue);
			handFist.LoadBitmap(".\\image\\finalBoss\\handFist.bmp");
			handWrist.LoadBitmap(".\\image\\finalBoss\\handWrist.bmp");
		}

		void CFinalBoss::OnMove()
		{
			if (isStart)				//開場動畫
			{
				handX = x + 800 - bossHand.Width() / 2;
				handY = 50;
				handVX = 800;
				handVY = 600;
				x -= 3;
				if (x <= 50)			//控制偏移量
				{
					isStart = false;
					x = 50;
				}
				bossHand.OnMove();
				bossFoot.OnMove();		//foot齒輪轉動
				return;
			}

#pragma region gunAttack
			if (gunLife > 0)
			{
				if (showGunExplode) gunBulletExplode.OnMove();
				else
				{
					gunBullet.OnMove();
					if (bulletX <= -(2 * gunBullet.Width()))
					{
						bulletX = x + 800 - bossBody.Width() - gunBullet.Width();
						bulletY = 600 - bossFoot.Height() - bossBody.Height() / 2 + 15 + gunBullet.Height();
					}
					else bulletX -= bulletStep;
				}
			}
			//gunAttack
#pragma endregion

#pragma region handAttack
			if (handLife > 0)
			{
				if (!canHandAttack)
				{
					if (handDelay > 0)
					{
						handDelay--;
						if (handDelay <= 40)		//攻擊警告
							canCaution = true;
					}
					else
					{
						canCaution = false;
						canHandAttack = true;
						handDelay = 30;
						handStepX = abs(heroX - handX) / 20;			//飛行速度
						handStepY = max(abs(heroY - handY) / 25, 5);	//飛行速度
					}
				}
				else		//canHandAttack
				{
					if (!isHandHitGround)		//是否打到地板
					{
						handX -= handStepX;
						handY += handStepY;
						if (handY >= 600 - bossHand.Height())
						{
							isHandHitGround = true;
							handOnground = true;
						}
					}
					else						//是，做回收拳頭的動作
					{
						if (handDelay > 0) handDelay--;
						else
						{
							handOnground = false;
							handX += handStepX;
							handY -= handStepY;
							if (handY <= 50)
							{
								handY = 50;
								handDelay = const_handDelay;
								isHandHitGround = false;
								canHandAttack = false;
							}
						}
					}		//ishandhitground
				}		//canhandAttack
			}
		//handAttack
#pragma endregion

#pragma region handVAttack
			if (handVLife > 0 && gunLife <= 0)
			{
				if (!canHandVAttack)
				{
					if (!handVgetX)				//position setting
					{
						handVX = rand() % 400;
						handVY = -handBullet.Height();
						handVgetX = true;
					}
					if (handVDelay > 0)			//attackDelay
					{
						handVDelay--;
						if (handVDelay < 10)			//攻擊警告
							canCaution2 = true;
					}
					else
					{
						handVDelay = const_handVDelay;
						handVStep = 25;
						canCaution2 = false;
						canHandVAttack = true;
					}
				}		//!canHandVAttack
				else		//canHandVAttack
				{
					if (!isHandVHitGround)		//擊中地板
					{
						handVY += handVStep;
						if (handVY >= 600 - handBullet.Height())
						{
							isHandVHitGround = true;
							handVOnground = true;
						}
					}
					else						//是，做回收拳頭的動作
					{
						if (handVDelay > 0) handVDelay--;
						else
						{
							handVOnground = false;
							handVY -= handVStep;
							if (handVY <= -handBullet.Height())
							{
								handVDelay = const_handVDelay;
								isHandVHitGround = false;
								canHandVAttack = false;
								handVgetX = false;
							}
						}
					}		//isHandVhitground
				}		//canHandVAttack
			}		//handVLife

		//handVAttack
#pragma endregion

			if (gunLife == 0 || handLife == 0 || handVLife == 0 || bossLife == 0) explode.OnMove();

			bossHand.OnMove();
			bossFoot.OnMove();
		}
		
		bool CFinalBoss::OnShow()
		{
			bossHead.SetTopLeft(x + 800 + bossHead.Width() - bossBody.Width(), 0);
			bossBody.SetTopLeft(x + 800 - bossBody.Width() + 100, 600 - bossFoot.Height() - bossBody.Height() + 50);
			bossFoot.SetTopLeft(x + 800 - bossFoot.Width() + 100, 600 - bossFoot.Height());
			bossGun.SetTopLeft(x + 800 - bossBody.Width()
				, 600 - bossFoot.Height() - bossBody.Height() / 2 + 30);
			bossHand.SetTopLeft(handX, handY);

			if (showGunExplode)					//顯示子彈爆炸動畫
			{
				gunBulletExplode.SetTopLeft(bulletX, bulletY);
				gunBulletExplode.OnShow();
				if (gunBulletExplode.IsFinalBitmap())	//動畫結束
				{
					gunBulletExplode.Reset();			//重製
					showGunExplode = false;				//關閉鎖定
					bulletX = -50;						//將子彈位置設置在地圖外繼續判斷
				}
			}
			else								//顯示子彈
			{
				if (gunLife > 0)
				{
					gunBullet.SetTopLeft(bulletX, bulletY);
					gunBullet.OnShow();
				}
			}

			if (canHandVAttack)
			{
				handBullet.SetTopLeft(handVX, handVY);
				if (handVLife > 0) handBullet.ShowBitmap();
			}

			if(gunLife > 0) bossGun.ShowBitmap();
			bossFoot.OnShow();
			bossBody.ShowBitmap();
			bossHead.ShowBitmap();
			if(handLife > 0) bossHand.OnShow();
			
			if (canCaution)
			{
				caution.SetTopLeft(handX + 30, handY + 50);
				caution.ShowBitmap();
			}
			if (canCaution2)
			{
				caution2.SetTopLeft(handVX, 0);
				caution2.ShowBitmap();
			}

			if (gunLife == 0)
			{
				explode.SetTopLeft(x + 800 - bossBody.Width() + bossGun.Width() - explode.Width()
					, 600 - bossFoot.Height() - bossBody.Height() / 2 + 30 + bossGun.Height() - explode.Height());
				explode.OnShow();
				if (explode.IsFinalBitmap())
				{
					explode.Reset();
					gunLife = -10;
				}
			}
			else if (handLife == 0)
			{
				explode.SetTopLeft(handX + bossHand.Width() - explode.Width()
					, handY + bossHand.Height() - explode.Height());
				explode.OnShow();
				if (explode.IsFinalBitmap())
				{
					explode.Reset();
					handLife = -10;
				}
			}
			else if (handVLife == 0)
			{
				explode.SetTopLeft(handVX
					, handVY + handBullet.Height() - explode.Height());
				explode.OnShow();
				if (explode.IsFinalBitmap())
				{
					explode.Reset();
					handVLife = -10;
				}
			}
			else if (bossLife == 0)
			{
				explode.SetTopLeft(x + 800 - explode.Width()
					, 600 - bossFoot.Height() + 50 - explode.Height());
				explode.OnShow();
				if (explode.IsFinalBitmap())
				{
					explode.Reset();
					bossLife = -10;
					return true;
				}
			}
			return false;
		}

		void CFinalBoss::SetStart(bool flag) { isStart = flag; }

		void CFinalBoss::SetDestination(int dx, int dy)
		{
			heroX = dx - 50;		//更精確的瞄準
			heroY = dy - 50;		
		}

		void CFinalBoss::AddLife(int part,int n)
		{
			switch (part)
			{
			case 1:
				gunLife += n;
				break;
			case 2:
				handLife += n;
				break;
			case 3:
				handVLife += n;
				break;
			case 4:
				bossLife += n;
				break;
			default:
				break;
			}
		}

		bool CFinalBoss::getGunState() { return gunLife > 0; }

		bool CFinalBoss::getHandState() { return handLife > 0; }

		bool CFinalBoss::getHandVState() { return handVLife > 0; }

		bool CFinalBoss::getHandOnground() { return handOnground; }

		bool CFinalBoss::getHandVOnground() { return handVOnground; }

		bool CFinalBoss::getDead() { return bossLife <= 0; }

		bool CFinalBoss::isHitHero(CHero *hero)
		{
			if (hero->getDead()) return false;
			if (gunLife > 0)				//槍壞掉了就不用判定
			{
				if (hero->getX1() <= bulletX + gunBullet.Width() && hero->getX2() >= bulletX
					&& hero->getY1() <= bulletY + gunBullet.Height() && hero->getY2() >= bulletY)
				{
					showGunExplode = true;
					return true;
				}
			}
			if (!isHandHitGround)			//拳頭還在空中飛行
			{
				//兩段式碰撞判定，由於圖片過大，所以需要分成Fist跟Wrist
				if (hero->getX1() <= handX + handFist.Width() && hero->getX2() >= handX &&
					hero->getY1() <= handY + bossHand.Height() && hero->getY2() >= handY + bossHand.Height() - handFist.Height())
				{
					return true;
				}
				else if (hero->getX1() <= handX + bossHand.Width() && hero->getX2() >= handX + bossHand.Width() - handWrist.Width() &&
					hero->getY1() <= handY + handWrist.Height() && hero->getY2() >= handY)
				{
					return true;
				}
			}
			if (!isHandVHitGround)			//拳頭在空中飛行
			{
				if (hero->getX1() <= handVX + handBullet.Width() && hero->getX2() >= handVX
					&& hero->getY1() <= handVY + handBullet.Height() && hero->getY2() >= handVY)
				{
					return true;
				}
			}
			//撞到boss身體
			if (hero->getX1() <= 800 && hero->getX2() >= x + 800 - bossFoot.Width() + 100
				&& hero->getY1() <= 600 && hero->getY2() >= 0)
			{
				return true;
			}
			return false;
		}

#pragma region getXY

		int CFinalBoss::getGunX1() { return x + 800 - bossBody.Width(); }

		int CFinalBoss::getGunX2() { return x + 800 - bossBody.Width() + bossGun.Width(); }

		int CFinalBoss::getGunY1() { return 600 - bossFoot.Height() - bossBody.Height() / 2 + 30; }

		int CFinalBoss::getGunY2() { return 600 - bossFoot.Height() - bossBody.Height() / 2 + 30 + bossGun.Height(); }

		int CFinalBoss::getHandX1() { return handX; }

		int CFinalBoss::getHandX2() { return handX + bossHand.Width(); }

		int CFinalBoss::getHandY1() { return handY; }

		int CFinalBoss::getHandY2() { return handY + bossHand.Height(); }

		int CFinalBoss::getHandVX1() { return handVX; }

		int CFinalBoss::getHandVX2() { return handVX + handBullet.Width(); }

		int CFinalBoss::getHandVY1() { return handVY; }

		int CFinalBoss::getHandVY2() { return handVY + handBullet.Height(); }

		int CFinalBoss::getBodyX1() { return x + 800 - bossBody.Width() + 100; }

		int CFinalBoss::getBodyX2() { return x + 800 + 100; }

		int CFinalBoss::getBodyY1() { return 600 - bossFoot.Height() - bossBody.Height() + 50; }

		int CFinalBoss::getBodyY2() { return 600 - bossFoot.Height() - bossBody.Height() / 2 + 50; }
#pragma endregion


		//CFinalBoss
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
		mapBmp.AddBitmap(".\\image\\263.bmp");
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
	void CGameMap::ClearMapBullet()
	{
		for (loop = 0; loop < maxHeroBullet; loop++)
			vCblt[loop]->SetLife(false);
		for (loop = 0; loop < maxEnemyBullet; loop++)
			vCbltEnemy[loop]->SetLife(false);
	}

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

	void CGameMap::killBullet(CMidBoss *midBoss)
	{
		for (loop = 0; loop < maxHeroBullet; loop++)
		{
			if (vCblt[loop]->isShow())
			{
				if (vCblt[loop]->isHit(midBoss->getLaserVX1(),midBoss->getLaserVY1()
					,midBoss->getLaserVX2(),midBoss->getLaserVY2()))
					vCblt[loop]->SetLife(false);
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

	bool CGameMap::isBulletHit(CMidBoss *midBoss)
	{
		for (loop = 0; loop < maxHeroBullet; loop++)
		{
			if (vCblt[loop]->isShow())					//子彈存活
			{
				if (vCblt[loop]->isHit(midBoss->getX1(), midBoss->getY1(), midBoss->getX2(), midBoss->getY2()))
				{
					vCblt[loop]->SetLife(false);		//子彈消失
						return true;
				}
			}
		}
		return false;
	}

	bool CGameMap::isBulletHit(CKid *kid)
	{
		for (loop = 0; loop < maxHeroBullet; loop++)
		{
			if (vCblt[loop]->isShow())
			{
				if (vCblt[loop]->isHit(kid->getX1(), kid->getY1(), kid->getX2(), kid->getY2()))
				{
					vCblt[loop]->SetLife(false);
					return true;
				}
			}
		}
		return false;
	}

	int CGameMap::isBulletHit(CFinalBoss *boss)
	{
		for (loop = 0; loop < maxHeroBullet; loop++)
		{
			if (vCblt[loop]->isShow())
			{
				if (boss->getGunState())
				{	//射中槍
					if (vCblt[loop]->isHit(boss->getGunX1(), boss->getGunY1(), boss->getGunX2(), boss->getGunY2()))
					{
						vCblt[loop]->SetLife(false);
						return 1;
					}
				}
				else if (boss->getHandOnground() && boss->getHandState())
				{	//射中手
					if (vCblt[loop]->isHit(boss->getHandX1(), boss->getHandY1(), boss->getHandX2(), boss->getHandY2()))
					{
						vCblt[loop]->SetLife(false);
						return 2;
					}
				}
				else if (boss->getHandVOnground() && boss->getHandVState())
				{	//射中手V
					if (vCblt[loop]->isHit(boss->getHandVX1(), boss->getHandVY1(), boss->getHandVX2(), boss->getHandVY2()))
					{
						vCblt[loop]->SetLife(false);
						return 3;
					}
				}
				else if(!boss->getDead())
		    	{	//射中身體
					if (vCblt[loop]->isHit(boss->getBodyX1(), boss->getBodyY1(), boss->getBodyX2(), boss->getBodyY2()))
					{
						vCblt[loop]->SetLife(false);
						return 4;
					}
				}
			}	//isShow
		}
		return 0;
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
		if (dir_horizontal == 1) CStandL.OnShow();
		else if (dir_horizontal == 2) CStandR.OnShow();
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

	void CDead::SetDir(int dir) 
	{ 
		if (dir < 3)
			direction = dir;
	}

	bool CDead::isfinalBitmap()
	{
		if (direction == 1)
		{
			if (CDeadL.IsFinalBitmap())
			{
				CDeadL.Reset();
				CDeadRGun.Reset();
				CDeadRMan.Reset();
				return true;
			}
		}
		else
		{
			if (CDeadRMan.IsFinalBitmap())
			{
				CDeadL.Reset();
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
		isShowHow = ishoverHow = ishoverBack = isShowAbout = isSoundShow = ishoverGo = ishoverExit = ishoverAbout = false;
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
		btnHowToPlay.LoadBitmap(".\\image\\interface\\btnHowToPlay.bmp", Black);
		btnHowToPlayHover.LoadBitmap(".\\image\\interface\\btnHowToPlayHover.bmp", Black);
		gameHowToPlay.LoadBitmap(".\\image\\interface\\howtoplay.bmp");

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
		if (isShowAbout || isShowHow)
		{
			if (ishoverBack)
			{
				CAudio::Instance()->Play(enemyScream, true);
				isShowAbout = false;
				isShowHow = false;
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
			CAudio::Instance()->Stop(enemyScream);
		}
		else if (ishoverHow)
		{
			isShowHow = true;
			CAudio::Instance()->Stop(enemyScream);
		}
		else if (ishoverExit)
			PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);
	}

	void CGameStateInit::OnMouseMove(UINT nFlags, CPoint point)
	{
		if (isShowAbout || isShowHow)
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
		else if (point.x >= 40 && point.x <= 40 + 200 && point.y >= 200 && point.y <= 200 + 59)
			ishoverHow = true;
		else
		{
			ishoverHow = ishoverGo = ishoverExit = ishoverAbout = false;
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
		if (isShowAbout || isShowHow)
		{
			if (isShowAbout)
			{
				gameAbout.SetTopLeft(0, 0);
				gameAbout.ShowBitmap();
			}
			else if (isShowHow)
			{
				gameHowToPlay.SetTopLeft(0, 0);
				gameHowToPlay.ShowBitmap();
			}

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


			manScream.SetTopLeft(60, 300);
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

			if (!ishoverHow)
			{
				btnHowToPlay.SetTopLeft(60, 200);
				btnHowToPlay.ShowBitmap();
			}
			else
			{
				btnHowToPlayHover.SetTopLeft(60, 200);
				btnHowToPlayHover.ShowBitmap();
			}

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
		hero.Initialize();

		for (loop = 0; loop < maxEnemyNumber; loop++)
			vecEnemy[loop]->Initialize();		//重新初始化敵人

		nowAliveEnemy = 0;				//清空場上敵人數

		seed = (unsigned)time(NULL);	//亂數設置
		srand(seed);					//亂數種子設置
		mapX = mapY = 0;

		const_come1EnemyDelay = come1EnemyDelay = 50;		//敵人生成延遲
		const_come2EnemyDelay = come2EnemyDelay = 75;		//敵人生成延遲
		remainEnemy.SetInteger(maxEnemyNumber);				//設定通關所需擊殺數
		stage = 0;					//關卡設置為0
		isFallBack = false;			//目前沒有撤退

		showMovie = true;			//開場動畫顯示
		movie.Initialize();			//初始化動畫
		movie.resetAnimation();		//reset動畫內容

		isNormalBGMShow = isBossBGMShow = false;	//判斷音樂是否播放
		midBoss.Initialize();			//midBoss初始化

		delay = const_delay = 30;		//轉場延遲

		kid.Initialize();				//kid 初始化
		isShowKid = true;				//開場顯示小孩
		finalBoss.Initialize();

	}

	void CGameStateRun::OnMove()							// 移動遊戲元素
	{
		// 如果希望修改cursor的樣式，則將下面程式的commment取消即可
		// SetCursor(AfxGetApp()->LoadCursor(IDC_GAMECURSOR));
		
		if (test)
		{
			return;
		}

		if (!isNormalBGMShow && !showMovie)
		{
			CAudio::Instance()->Play(AUDIO_normal_BGM, true);
			isNormalBGMShow = true;
		}

		mapX = gameMap.getX();					//地圖偏移座標
		mapY = gameMap.getY();

		if (showMovie)
		{
			movie.OnMove();
			return;
		}

		if (isShowKid)
		{
			hero.SetLock(true);
			kid.OnMove();					//小孩移動
			if (gameMap.isBulletHit(&kid))
			{
				kid.SetAlive(false);
				kid.SetDead(true);
			}
			isShowKid = kid.getShow();		//取得小孩顯示狀態
			if (!isShowKid) hero.SetLock(false);
		}
		else
		{
			heroLife.SetInteger(hero.getLife());	//取得主角生命

			if (nowAliveEnemy <= 0) hero.SetOverlap(false);

			if (stage == 1 || stage == 3)				//midBoss
			{
				if (!isBossBGMShow)
				{
					CAudio::Instance()->Play(bossBGM, true);
					isBossBGMShow = true;
				}
				if (stage == 1)
				{
					if (gameMap.isBulletHit(&midBoss))		//midBoss被主角攻擊
						midBoss.AddLife(-1);

					if (midBoss.isHitHero(&hero))			//主角被midBoss攻擊
						hero.AddLife(-1);

					gameMap.killBullet(&midBoss);
					midBoss.OnMove();
				}
				else if (stage == 3)
				{
					finalBoss.OnMove();
					finalBoss.SetDestination(hero.getX1(), hero.getY1());
					if (finalBoss.isHitHero(&hero))
						hero.AddLife(-1);
					bossPart = gameMap.isBulletHit(&finalBoss);
					finalBoss.AddLife(bossPart, -1);
				}
			}
			else if (stage == 0 || stage == 2)		//0、2皆為小兵關卡
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
								CAudio::Instance()->Play(AUDIO_enemyDead, false);
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

#pragma region damageHero

				//主角被小兵射到判斷
				if (gameMap.isBulletHit(&hero))
					hero.AddLife(-1);

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
						CAudio::Instance()->Stop(AUDIO_normal_BGM);
					}
					else
					{
						if (stage == 0)
						{
							if (mapX <= -2035 && mapX >= -2050)		//鎖定小Bosss場地
							{
								hero.SetLock(true);
								if (nowShowEnemy == 0)
								{
									if (delay > 0) delay--;
									else
									{
										delay = const_delay;
										stage++;
										remainEnemy.SetInteger(0);
										midBoss.SetStart(true);
									}
								}
							}
							else									//沒到的話要顯示提示
							{
								if (mapX > -2035) goR.SetTopLeft(SIZE_X - goR.Width(), SIZE_Y / 2 - goR.Height() / 2);
								else if (mapX < -2050) goL.SetTopLeft(0, SIZE_Y / 2 - goL.Height() / 2);
							}
						}
						else if (stage == 2)							//stage == 2
						{
							if (mapX <= -4300 && mapX >= -4315)		//鎖定FinalBosss場地
							{
								hero.SetLock(true);
								if (nowShowEnemy == 0)
								{
									if (delay > 0) delay--;
									else
									{
										delay = const_delay;
										stage++;
										remainEnemy.SetInteger(0);
										midBoss.Initialize();
										midBoss.SetStart(true);
									}
								}
							}
							else				//沒到的話要顯示提示
							{
								if (mapX > -4300) goR.SetTopLeft(SIZE_X - goR.Width(), SIZE_Y / 2 - goR.Height() / 2);
							}
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
		}		//!isShowKid
		gameMap.OnMoveBullet();			//雙方子彈移動
		gameMap.killBullet();			//雙方子彈刪除(超出範圍)
		hero.OnMove();					//主角移動

#pragma region endGame
		if (hero.getLife() <= 0)
		{
			CAudio::Instance()->Stop(AUDIO_normal_BGM);
			GotoGameState(GAME_STATE_OVER);
		}

#pragma endregion

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

		maxEnemyNumber = 5;				//最大敵人數
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
		kid.LoadBitmap();
		finalBoss.LoadBitmap();

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
		CAudio::Instance()->Load(midBoss_Laser, "sounds\\midBossLaser.mp3");
		CAudio::Instance()->Load(midBoss_Stand, "sounds\\midBossStand.mp3");


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

		if (hero.getDead()) return;
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
			if(!hero.getOverlap() && !hero.getDead()) 
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

		if (test)
		{
			return;
		}

		if (showMovie)						
		{
			showMovie = movie.OnShow();		//開場動畫顯示
			return;
		}

		gameMap.OnShow();					//背景顯示

		if (isShowKid)
		{
			kid.OnShow();
		}
		else
		{
			if (stage == 1)		//midBoss
			{
				if (midBoss.OnShow())			//顯示midBoss
				{
					stage++;					//midBoss死亡，關卡切換
					isFallBack = false;			//關閉撤退
					hero.SetLock(false);		//取消地圖鎖定
					remainEnemy.SetInteger(maxEnemyNumber);		//重新設定所需擊殺數
					for (loop = 0; loop < maxEnemyNumber; loop++)
						vecEnemy[loop]->Initialize();			//敵人重新初始化
					CAudio::Instance()->Stop(bossBGM);			//關閉boss戰音效
					isBossBGMShow = isNormalBGMShow = false;	//重製音效顯示狀態
				}
			}
			else if (stage == 3)
			{
				if (finalBoss.OnShow())
				{
					stage++;
					CAudio::Instance()->Stop(bossBGM);
				}
			}
			for (loop = 0; loop < maxEnemyNumber; loop++)		//敵人顯示
				if (vecEnemy[loop]->isShow()) vecEnemy[loop]->OnShow();

#pragma region mapInstruction
			enemyImg.ShowBitmap();			//敵人小圖顯示
			remainEnemy.ShowBitmap();		//剩餘敵人數顯示
			heroImg.ShowBitmap();			//主角小圖顯示
			heroLife.ShowBitmap();			//主角血量顯示

#pragma endregion

			if (remainEnemy.GetInteger() <= 0)			//指示顯示
			{
				if (stage == 0)
				{
					if (mapX > -2035) goR.ShowBitmap();
					else if (mapX < -2050) goL.ShowBitmap();
				}
				else if (stage == 2)
				{
					if (mapX > -4300) goR.ShowBitmap();
				}
			}
		}	//!isShowkid

		hero.OnShow();				//主角顯示
		gameMap.OnShowBullet();		//子彈顯示
		
	}	//OnShow

	//CGameStateRun
#pragma endregion



}	//namespace game_framwork