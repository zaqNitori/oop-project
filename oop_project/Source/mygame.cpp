/*
 * mygame.cpp: ���ɮ��x�C��������class��implementation
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
		y = 450;
		constDelay = 9;
		delayCount = constDelay;
		direction = dir_horizontal = 1;				//�w�]�V��
		isFalling = isRising = isMovingDown = isMovingLeft = isMovingRight = isMovingUp = isShooting = false;
	}

	void CHero::InitialBullet()
	{
		maxBullet = 5;
		for (unsigned i = 0; i < maxBullet; i++)
		{
			vCblt.push_back(new CBullet(0, 0, 0));
			vCblt[i]->SetLife(false);
			vCblt[i]->SetBulletClass(&heroBullet);
		}
	}

	void CHero::LoadBitmap()
	{
		char *fileBullet[] = { ".\\image\\bullet\\1.bmp" , ".\\image\\bullet\\2.bmp" , ".\\image\\bullet\\3.bmp" };
	#pragma region fileInput
		char *fileDefaultStand[] = { ".\\image\\stand\\L1.bmp" };
		char *fileDefaultCrouch[] = { ".\\image\\crouch\\L1.bmp" };

		char *filestandL[] = { ".\\image\\stand\\L1.bmp",".\\image\\stand\\L2.bmp",".\\image\\stand\\L3.bmp" , ".\\image\\stand\\L4.bmp" };
		char *filestandR[] = { ".\\image\\stand\\R1.bmp",".\\image\\stand\\R2.bmp",".\\image\\stand\\R3.bmp" , ".\\image\\stand\\R4.bmp" };
		char *filestandShootL[] = { ".\\image\\stand\\shoot\\L1-1.bmp" , ".\\image\\stand\\shoot\\L2-1.bmp" , ".\\image\\stand\\shoot\\L3-1.bmp" , ".\\image\\stand\\shoot\\L4-1.bmp" };
		char *filestandShootL2[] = { ".\\image\\stand\\shoot\\L1-2.bmp" , ".\\image\\stand\\shoot\\L2-2.bmp" , ".\\image\\stand\\shoot\\L3-2.bmp" , ".\\image\\stand\\shoot\\L4-2.bmp" };
		char *filestandShootR[] = { ".\\image\\stand\\shoot\\R1.bmp" , ".\\image\\stand\\shoot\\R2.bmp" , ".\\image\\stand\\shoot\\R3.bmp" , ".\\image\\stand\\shoot\\R4.bmp" };


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

	#pragma endregion

	#pragma region �ʵe���J
		CDefaultStand.LoadBitmap(fileDefaultStand[0], Blue);
		CDefaultCrouch.LoadBitmap(fileDefaultCrouch[0], Blue);
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
		for (int i = 0; i < 3; i++)
		{
			heroBullet.AddBitmap(fileBullet[i], Blue);
		}
	#pragma endregion

	}

#pragma region MapMove
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
		if (isMovingDown && !isRising)		//�ھڤ��P�ʧ@�������Pdefault�Ϥ��H��K�P�_
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

		if (dir_horizontal == 1)			//�V����
		{
			if (x < 200)			//��W�L�����ۥѲ��ʽd���
			{
				mapX -= x - 200;	//���o�W�L������ɪ��첾�q�A�åB�ϦV�[�ܦa�Ϯy��
				if (mapX >= 0)
					mapX = 0;		//�p�G�b�a�ϳ̥����h�i�H�L���������
				else x = 200;		//�Ϥ��h�|�Q�d��
			}
			if (x <= 0) x = 0;
		}
		else if (dir_horizontal == 2)	//�V�k��
		{
			if (x > 600 - defaultW)					//��W�L�k���ۥѲ��ʽd���
			{
				mapX -= x - (600 - defaultW);		//���o�W�L���첾�q�A�ϦV�[�ܦa�Ϯy��
				if (mapX <= (-4400 + defaultW))		//�p�G�a�Ϧb�̥k���A�h�i�H�L���������
					mapX = (-4400 + defaultW);
				else x = 600 - defaultW;			//�Ϥ��h�|�Q�d��
			}
			if (x >= 800 - defaultW) x = 800 - defaultW;
		}
		gameMap->setXY(mapX, mapY);					//�]�w�a�Ϯy��
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
				CHero::SetShooting(false);
			}
		}
	}

#pragma endregion

	void CHero::OnMove()
	{
		gameMap_OnMove();
		heroStand.OnMove(x, y);
		heroMove.OnMove(&x, &y);
		gravity();
		isFalling = heroJump.OnMove(&x, &y);
		if (isFalling) isRising = false;
		heroCrouch.OnMove(x, y);
		ResumeShooting();
		
	}

	void CHero::OnShow()
	{
		
		/*if (isRising || isFalling) heroJump.SetXY(x, y);
		if (isMovingLeft || isMovingRight) heroMove.SetXY(x, y);
		if (isMovingDown) heroCrouch.SetXY(x, y);
		heroStand.SetXY(x, y);*/

		#pragma region OnShow
		
		if (isRising||isFalling)
		{
			if (isShooting)
				heroJump.OnShow_Shoot();
			else
			{
				if (isRising) heroJump.OnShow_Rise();
				if (isFalling) heroJump.OnShow_Fall();
			}
			//isShooting = heroJump.isfinalBitmap(dir_horizontal);
		}
		else if (isMovingDown)
		{
			if (isShooting)
				heroCrouch.OnShow_Shoot();
			else
			{
				if (isMovingLeft || isMovingRight) heroCrouch.OnShow_Move();
				else heroCrouch.OnShow_Stand();
			}
			//isShooting = heroCrouch.isfinalBitmap(dir_horizontal);
		}
		else if (isMovingLeft||isMovingRight)
		{
			if (isShooting)
				heroMove.OnShow_Shoot();
			else heroMove.OnShow();
			//isShooting = heroMove.isfinalBitmap(dir_horizontal);
		}
		else if(!(isMovingDown || isMovingLeft || isMovingRight || isRising))
		{
			if (isShooting)
				heroStand.OnShow_Shoot();
			else heroStand.OnShow_Stand();
			//isShooting = heroStand.isfinalBitmap(dir_horizontal);
		}

		#pragma endregion

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
			if (dir < 3)		//�V���ΦV�k
				dir_horizontal = dir;
			heroStand.SetDirection(dir);
			heroJump.SetDirection(dir);
			heroCrouch.SetDirection(dir);
			heroMove.SetDirection(dir);
		}
		
		void CHero::SetRising(bool flag)
		{
			if (!(isFalling))
			{
				isRising = flag;
				heroJump.SetRising(flag);
			}
		}

		void CHero::SetMovingDown(bool flag)
		{
			isMovingDown = flag;
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
			heroCrouch.SetShooting(flag);	//���i�ٲ�
			heroJump.SetShooting(flag);		//���i�ٲ�
		}

		void CHero::SetXY(int nx, int ny)
		{
			x = nx;
			y = ny;
		}

	#pragma endregion

	#pragma region GetState
		
		int CHero::getX() { return x; }

		int CHero::getY() { return y; }

		int CHero::getDir() { return direction; }

	#pragma endregion

	#pragma region BulletState

		void CHero::addBullet()
		{
			for (unsigned i = 0; i < maxBullet; i++)
			{
				if (vCblt[i]->isShow() == false)
				{
					if (direction == 4) vCblt[i]->SetBullet(x, y + 60, dir_horizontal);
					else vCblt[i]->SetBullet(x, y+30, dir_horizontal);
					break;
				}
			}
		}

		void CHero::killBullet()
		{
			for (unsigned i = 0; i < vCblt.size(); i++)
				if (vCblt[i]->isDead())
					vCblt[i]->SetLife(false);
		}

		void CHero::OnMoveBullet()
		{
			for (unsigned i = 0; i < vCblt.size(); i++)
				if(vCblt[i]->isShow())
					vCblt[i]->OnMove();
		}

		void CHero::OnShowBullet()
		{
			for (unsigned i = 0; i < vCblt.size(); i++)
				if(vCblt[i]->isShow())
					vCblt[i]->OnShow();
		}

	#pragma endregion


	//CHero
#pragma endregion

#pragma region CEnemy

		CEnemy::CEnemy()
		{
			Initialize();
		}

		CEnemy::~CEnemy() {}

		void CEnemy::Initialize()
		{
			mapX = mapY = 0;
			x = y = 300;
			y = 525 - 165;
			direction = 1;
			step = 18;
			isOnBlock = false;
			isAlive = isDead = false;
			isAlive = true;	//test
		}

		void CEnemy::LoadBitmap()
		{
			defaultStand.LoadBitmap(".\\image\\enemy\\L1.bmp");
			defaultHeight = defaultStand.Height();
			defaultWidth = defaultStand.Width();
			enemyStand.LoadBitmap_StandL(".\\image\\enemy\\L1.bmp");
			enemyStand.LoadBitmap_StandR(".\\image\\enemy\\R1.bmp");
			/*char* fileDeadL[] = { ".\\image\\enemy\\die\\L1.bmp" , ".\\image\\enemy\\die\\L3.bmp" , ".\\image\\enemy\\die\\L5.bmp" , ".\\image\\enemy\\die\\L7.bmp" , ".\\image\\enemy\\die\\L8.bmp" };
			char* fileDeadR1[] = { ".\\image\\enemy\\die\\R1-1.bmp" , ".\\image\\enemy\\die\\R3-1.bmp" , ".\\image\\enemy\\die\\R5-1.bmp" , ".\\image\\enemy\\die\\R7-1.bmp" , ".\\image\\enemy\\die\\R8-1.bmp" };
			char* fileDeadR2[] = { ".\\image\\enemy\\die\\R1-2.bmp" , ".\\image\\enemy\\die\\R3-2.bmp" , ".\\image\\enemy\\die\\R5-2.bmp" , ".\\image\\enemy\\die\\R7-2.bmp" , ".\\image\\enemy\\die\\R8-2.bmp" };*/
		}

		void CEnemy::OnMove()
		{
			if (!isOnBlock) y += step;
			enemyStand.OnMove(x, y);
		}

		void CEnemy::OnShow()
		{
			if (isDead)
			{
				enemyDeadL.OnShow();	//�{�b�S�ɶ��g
				enemyDeadR.OnShow();	//����A��
			}
			else if(isAlive) enemyStand.OnShow_Stand();
		}

#pragma region SetState
		void CEnemy::SetAlive(bool flag) { isAlive = flag; }

		void CEnemy::SetDead(bool flag) { isDead = flag; }

		void CEnemy::SetDirection(int dir)
		{ 
			direction = dir; 
			enemyStand.SetDirection(dir);
		}

		void CEnemy::SetEnemy(int heroX)
		{
			//int r = rand() % (800 - defaultWidth);
		}

		void CEnemy::SetOnBlock(bool flag) { isOnBlock = flag; }

#pragma endregion

#pragma region GetState
		bool CEnemy::isShow() { return (isAlive || isDead); }

		bool CEnemy::getAlive() { return isAlive; }

		bool CEnemy::getDead() { return isDead; }

		int CEnemy::getX1() { return x; }
		int CEnemy::getX2() { return x + defaultWidth; }
		int CEnemy::getY1() { return y; }
		int CEnemy::getY2() { return y + defaultHeight; }

#pragma endregion



	//CEnemy
#pragma endregion




/////////////////////////////////////////////////////////////////////////////
// CBall: Ball class
/////////////////////////////////////////////////////////////////////////////

#pragma region CBall

	CBall::CBall()
	{
		is_alive = true;
		x = y = dx = dy = index = delay_counter = 0;
	}

	bool CBall::HitEraser(CEraser *eraser)
	{
		// �˴����l�Һc�����x�άO�_�I��y
		return HitRectangle(eraser->GetX1(), eraser->GetY1(),
			eraser->GetX2(), eraser->GetY2());
	}

	bool CBall::HitRectangle(int tx1, int ty1, int tx2, int ty2)
	{
		int x1 = x + dx;				// �y�����W��x�y��
		int y1 = y + dy;				// �y�����W��y�y��
		int x2 = x1 + bmp.Width();	// �y���k�U��x�y��
		int y2 = y1 + bmp.Height();	// �y���k�U��y�y��
		//
		// �˴��y���x�λP�ѼƯx�άO�_���涰
		//
		return (tx2 >= x1 && tx1 <= x2 && ty2 >= y1 && ty1 <= y2);
	}

	bool CBall::IsAlive()
	{
		return is_alive;
	}

	void CBall::LoadBitmap()
	{
		bmp.LoadBitmap(IDB_BALL, RGB(0, 0, 0));			// ���J�y���ϧ�
		bmp_center.LoadBitmap(IDB_CENTER, RGB(0, 0, 0));	// ���J�y��ߪ��ϧ�
	}

	void CBall::OnMove()
	{
		if (!is_alive)
			return;
		delay_counter--;
		if (delay_counter < 0) {
			delay_counter = delay;
			//
			// �p��y�V����ߪ��첾�qdx, dy
			//
			const int STEPS = 18;
			static const int DIFFX[] = { 35, 32, 26, 17, 6, -6, -17, -26, -32, -34, -32, -26, -17, -6, 6, 17, 26, 32, };
			static const int DIFFY[] = { 0, 11, 22, 30, 34, 34, 30, 22, 11, 0, -11, -22, -30, -34, -34, -30, -22, -11, };
			index++;
			if (index >= STEPS)
				index = 0;
			dx = DIFFX[index];
			dy = DIFFY[index];
		}
	}

	void CBall::SetDelay(int d)
	{
		delay = d;
	}

	void CBall::SetIsAlive(bool alive)
	{
		is_alive = alive;
	}

	void CBall::SetXY(int nx, int ny)
	{
		x = nx; y = ny;
	}

	void CBall::OnShow()
	{
		if (is_alive) {
			bmp.SetTopLeft(x + dx, y + dy);
			bmp.ShowBitmap();
			bmp_center.SetTopLeft(x, y);
			bmp_center.ShowBitmap();
		}
	}

	//CBall
#pragma endregion


#pragma region CBullet
	CBullet::CBullet(void) {}

	CBullet::~CBullet() {};

	CBullet::CBullet(int nx, int ny, int dir)
	{
		x = nx;
		y = ny;
		direction = dir;
		Initialize();
	}

	void CBullet::Initialize()
	{
		velocity = 20;
		isAlive = false;
		//bullet.SetDelayCount(2);
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
		case 1:
			x -= velocity;
			break;
		case 2:
			x += velocity;
			break;
		/*case 3:
			y -= velocity;
			break;
		case 4:
			y += velocity;
			break;*/
		default:
			break;
		}
		//bullet.OnMove();
		bullet->OnMove();
	}

	void CBullet::OnShow()
	{
		bullet->SetTopLeft(x, y);
		bullet->OnShow();
	}

	bool CBullet::isDead()
	{
		if (x  > 800 || x + bullet->Width() < 0 || y > 600 || y + bullet->Height() < 0) return true;
		return false;
	}

	bool CBullet::isShow() { return isAlive; }

	void CBullet::SetLife(bool life) { isAlive = life; }

	void CBullet::SetBullet(int nx, int ny, int dir)
	{
		x = nx;
		y = ny;
		direction = dir;
		isAlive = true;
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

	void CGameMap::Initialize()
	{
		mapX = 0;
		mapY = SIZE_Y - 721;
		size = 20;
		weight = 5130 / size;
		height = 721 / size;
		isMovingLeft = isMovingRight = false;
		for (int i = 0; i < height+1; i++)
			for (int j = 0; j < weight + 1; j++)
			{
				map[i][j] = 0;							//0���ť�
				if (i > (640 / 20 - 1)) map[i][j] = 1;	//1����ê��
			}
		#pragma region setBlock
			SetBlock(1, 8, 13, 14);
			SetBlock(9, 18, 22, 23);
			SetBlock(12, 28, 18, 19);
			SetBlock(30, 40, 24, 25);
			SetBlock(57, 67, 25, 26);
			SetBlock(75, 83, 24, 25);
			SetBlock(85, 94, 22, 23);
			SetBlock(95, 101, 17, 18);
			SetBlock(104, 118, 25, 26);
			SetBlock(162, 172, 23, 24);
			SetBlock(190, 198, 22, 23);
			SetBlock(42, 46, 17, 18);
			SetBlock(50, 68, 14, 15);
		#pragma endregion

	}

	void CGameMap::LoadBitmap()
	{
		//mapBmp.AddBitmap(IDB_GameMap);
		//mapBmp.AddBitmap(IDB_gameMapBlock);
		mapBmp.AddBitmap(".\\image\\Black_on_map.bmp");
		SetFloorRoof();
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
				map[j][i] = 1;				//1���i���W����ê��
		
	}

	void CGameMap::setXY(int mx, int my)
	{
		mapX = mx;
		mapY = my;
	}

	void CGameMap::SetFloorRoof()
	{
		floor = SIZE_Y - mapBmp.Height();
		roof = 0;
	}

#pragma endregion

#pragma region GetValue
	bool CGameMap::getMapBlock(int ny, int nx)
	{
		return map[ny][nx];
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
	

	//CGameMap
#pragma endregion



/////////////////////////////////////////////////////////////////////////////
// CBouncingBall: BouncingBall class
/////////////////////////////////////////////////////////////////////////////
	
#pragma region CBouncingBall

	CBouncingBall::CBouncingBall()
	{
		const int INITIAL_VELOCITY = 20;	// ��l�W�ɳt��
		const int FLOOR = 400;				// �a�O�y��
		floor = FLOOR;
		x = 95; y = FLOOR - 1;				// y�y�Ф�a�O��1�I(���b�a�O�W)
		rising = true;
		initial_velocity = INITIAL_VELOCITY;
		velocity = initial_velocity;
	}

	void CBouncingBall::LoadBitmap()
	{
		char *filename[4] = { ".\\bitmaps\\ball1.bmp",".\\bitmaps\\ball2.bmp",".\\bitmaps\\ball3.bmp",".\\bitmaps\\ball4.bmp" };
		for (int i = 0; i < 4; i++)	// ���J�ʵe(��4�i�ϧκc��)
			animation.AddBitmap(filename[i], RGB(0, 0, 0));
	}

	void CBouncingBall::OnMove()
	{
		if (rising) {			// �W�ɪ��A
			if (velocity > 0) {
				y -= velocity;	// ��t�� > 0�ɡAy�b�W��(����velocity���I�Avelocity����쬰 �I/��)
				velocity--;		// �����O�v�T�A�U�����W�ɳt�׭��C
			}
			else {
				rising = false; // ��t�� <= 0�A�W�ɲפ�A�U���אּ�U��
				velocity = 1;	// �U������t(velocity)��1
			}
		}
		else {				// �U�����A
			if (y < floor - 1) {  // ��y�y���٨S�I��a�O
				y += velocity;	// y�b�U��(����velocity���I�Avelocity����쬰 �I/��)
				velocity++;		// �����O�v�T�A�U�����U���t�׼W�[
			}
			else {
				y = floor - 1;  // ��y�y�ЧC��a�O�A�󥿬��a�O�W
				rising = true;	// �����ϼu�A�U���אּ�W��
				velocity = initial_velocity; // ���]�W�ɪ�l�t��
			}
		}
		animation.OnMove();		// ����@��animation.OnMove()�Aanimation�~�|����
	}

	void CBouncingBall::OnShow()
	{
		animation.SetTopLeft(x, y);
		animation.OnShow();
	}

	//CbouncingBall
#pragma endregion


/////////////////////////////////////////////////////////////////////////////
// CEraser: Eraser class
/////////////////////////////////////////////////////////////////////////////

#pragma region CEraser

	CEraser::CEraser()
	{
		Initialize();
	}

	int CEraser::GetX1()
	{
		return x;
	}

	int CEraser::GetY1()
	{
		return y;
	}

	int CEraser::GetX2()
	{
		return x + animation.Width();
	}

	int CEraser::GetY2()
	{
		return y + animation.Height();
	}

	void CEraser::Initialize()
	{
		const int X_POS = 280;
		const int Y_POS = 400;
		x = X_POS;
		y = Y_POS;
		isMovingLeft = isMovingRight = isMovingUp = isMovingDown = false;
	}

	void CEraser::LoadBitmap()
	{
		animation.AddBitmap(IDB_ERASER1, RGB(255, 255, 255));
		animation.AddBitmap(IDB_ERASER2, RGB(255, 255, 255));
		animation.AddBitmap(IDB_ERASER3, RGB(255, 255, 255));
		animation.AddBitmap(IDB_ERASER2, RGB(255, 255, 255));
	}

	void CEraser::OnMove()
	{
		const int STEP_SIZE = 15;
		animation.OnMove();
		if (isMovingLeft)
			x -= STEP_SIZE;
		if (isMovingRight)
			x += STEP_SIZE;
		if (isMovingUp)
			y -= STEP_SIZE;
		if (isMovingDown)
			y += STEP_SIZE;
	}

	void CEraser::SetMovingDown(bool flag)
	{
		isMovingDown = flag;
	}

	void CEraser::SetMovingLeft(bool flag)
	{
		isMovingLeft = flag;
	}

	void CEraser::SetMovingRight(bool flag)
	{
		isMovingRight = flag;
	}

	void CEraser::SetMovingUp(bool flag)
	{
		isMovingUp = flag;
	}

	void CEraser::SetXY(int nx, int ny)
	{
		x = nx; y = ny;
	}

	void CEraser::OnShow()
	{
		animation.SetTopLeft(x, y);
		animation.OnShow();
	}
	//CEraser
#pragma endregion

#pragma region CMove


	CMove::CMove()
	{
		Initialize();
	}

	void CMove::Initialize()
	{
		const int x_pos = 200;			//�w�]x��m
		const int y_pos = 450;			//�w�]y��m
		x = x_pos;
		y = y_pos;
		step = ini_step = 15;			//�w�]���ʳt��
		direction = dir_horizontal = 1;	//�w�]��V�V��
		isShooting = isRising = isMovingDown = isMovingLeft = isMovingRight = isMovingUp = false;
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

	void CMove::OnMove(int* nx, int* ny)
	{
		step = ini_step;
		x = *nx;
		y = *ny;
		if (isMovingLeft) x -= step;
		if (isMovingRight) x += step;
		if (isShooting)
		{
			if (direction == 1) CMoveShoot.OnMoveL();
			else if (direction == 2) CMoveShoot.OnMoveR();
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

	bool CMove::isfinalBitmap(int dir)
	{
		return CMoveShoot.isfinalBitmap(dir);
	}

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

	void CMove::OnShow_Shoot()
	{
		CMoveShoot.SetXY(x, y);
		if (direction == 1) CMoveShoot.OnShowL();
		else if (direction == 2) CMoveShoot.OnShowR();
	}

	//CMove
#pragma endregion

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
			if (dir_horizontal == 1) CStandShoot.OnMoveL();
			else if (dir_horizontal == 2) CStandShoot.OnMoveR();
		}
	}

	bool CStand::isfinalBitmap(int dir)
	{
		return CStandShoot.isfinalBitmap(dir);
	}

	void CStand::OnShow_Shoot()
	{
		CStandShoot.SetXY(x, y);
		if (direction == 1) CStandShoot.OnShowL();
		else if (direction == 2) CStandShoot.OnShowR();
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

#pragma region CJump

	CJump::CJump()
	{
		Initialize();
	}

	void CJump::Initialize()
	{
		const int FLOOR = 450;			//�a�O����
		const int INI_VELOCITY = 30;	//��t
		floor = FLOOR;
		direction = dir_horizontal = 1;	//�w�]��V�V��
		velocity = ini_velocity = INI_VELOCITY;
		isRising = isFalling = false;
	}

	#pragma region Loadpicture
		
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

	#pragma endregion


	bool CJump::OnMove(int *nx, int *ny)	//�^��isFalling�����A
	{
		x = *nx;
		y = *ny;
		if (isRising)
		{
			if (isShooting)
			{
				if (dir_horizontal == 1) CJumpShoot.OnMoveL();
				else if (dir_horizontal == 2) CJumpShoot.OnMoveR();
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
				isRising = false;		//�ɨ�̰��I�n�ର�U��
				isFalling = true;		//�ন�U��
				velocity = 1;			//�U����t��
			}
		}
		else if(isFalling)
		{
			mapX = gameMap->getX();
			mapY = gameMap->getY();
			_size = gameMap->getSize();
			if (isShooting)
			{
				if (dir_horizontal == 1) CJumpShoot.OnMoveL();
				else if (dir_horizontal == 2) CJumpShoot.OnMoveR();
			}
			else
			{
				if (dir_horizontal == 1) CFallL.OnMove();
				else if (dir_horizontal == 2) CFallR.OnMove();
			}
			if (isEmpty(x - mapX, y - mapY + defaultHeight))		//�]��y�b�@�}�l�N�b�̤U��,�ҥH�n�ϦV�[�~��o�쥿�T��y��
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

	bool CJump::isEmpty(int nx, int ny)
	{
		int gx = nx / gameMap->getSize();
		int gy = ny / gameMap->getSize();
		return !(gameMap->getMapBlock(gy,gx));
	}

	bool CJump::isfinalBitmap(int dir)
	{
		return CJumpShoot.isfinalBitmap(dir);
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

	void CJump::OnShow_Shoot()
	{
		CJumpShoot.SetXY(x, y);
		if (dir_horizontal == 1) CJumpShoot.OnShowL();
		else if (dir_horizontal == 2) CJumpShoot.OnShowR();
	}

	//CJump
#pragma endregion

#pragma region CCrouch

	CCrouch::CCrouch()
	{
		Initialize();
	}

	void CCrouch::Initialize()
	{
		const int x_pos = 200;	//��l��m
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
			if (dir_horizontal == 1)
			{
				CcrouchShoot.OnMoveL();
			}
			else if (dir_horizontal == 2)
			{
				CcrouchShoot.OnMoveR();
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

	bool CCrouch::isfinalBitmap(int dir)
	{
		return CcrouchShoot.isfinalBitmap(dir);
	}

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

	void CCrouch::OnShow_Shoot()
	{
		CcrouchShoot.SetXY(x, y);
		if (dir_horizontal == 1)
		{
			CcrouchShoot.OnShowL();
		}
		else if (dir_horizontal == 2)
		{
			CcrouchShoot.OnShowR();
		}
	}

#pragma endregion

	void CCrouch::SetDirection(int dir)
	{
		direction = dir;
		if (dir < 3)
			dir_horizontal = dir;
	}

	//CCrouch
#pragma endregion

#pragma region CShoot

	CShoot::CShoot()
	{
		Initialize();
	}

	CShoot::~CShoot() {}

	void CShoot::Initialize()
	{
		x = y = width = 0;
	}

	void CShoot::LoadShootLeft(char *file, char *file2)
	{
		CShootLHero.AddBitmap(file,Blue);
		CShootLGun.AddBitmap(file2,Blue);
	}

	void CShoot::LoadShootRight(char *file)
	{
		CShootR.AddBitmap(file,Blue);
	}

	void CShoot::OnMoveL()
	{
		CShootLHero.OnMove();
		CShootLGun.OnMove();
	}

	void CShoot::OnMoveR()
	{
		CShootR.OnMove();
	}

	void CShoot::OnShowL()
	{
		width = CShootLGun.Width();
		CShootLHero.SetTopLeft(x, y);
		CShootLHero.OnShow();
		CShootLGun.SetTopLeft(x - width, y);
		CShootLGun.OnShow();
	}

	void CShoot::OnShowR()
	{
		CShootR.SetTopLeft(x, y);
		CShootR.OnShow();
	}

	void CShoot::SetXY(int nx, int ny)
	{
		x = nx;
		y = ny;
	}

	bool CShoot::isfinalBitmap(int dir)
	{
		if (dir == 1)
		{
			if (CShootLGun.IsFinalBitmap())
			{
				CShootLGun.Reset();
				CShootLHero.Reset();
				return false;
			}
		}
		else if (dir == 2)
		{
			if (CShootR.IsFinalBitmap())
			{
				CShootR.Reset();
				return false;
			}
		}
		return false;
	}

	//CShoot
#pragma endregion




/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����C���}�Y�e������
/////////////////////////////////////////////////////////////////////////////

#pragma region CGameStateInit

	CGameStateInit::CGameStateInit(CGame *g)
		: CGameState(g)
	{
	}

	void CGameStateInit::OnInit()
	{
		//
		// ��ϫܦh�ɡAOnInit���J�Ҧ����ϭn��ܦh�ɶ��C���קK���C�����H
		//     �������@�СA�C���|�X�{�uLoading ...�v�A���Loading���i�סC
		//
		ShowInitProgress(0);	// �@�}�l��loading�i�׬�0%
		//
		// �}�l���J���
		//
		logo.LoadBitmap(IDB_BACKGROUND);
		Sleep(300);				// ��C�A�H�K�ݲM���i�סA��ڹC���ЧR����Sleep
		//
		// ��OnInit�ʧ@�|����CGameStaterRun::OnInit()�A�ҥH�i���٨S��100%
		//
		//srand(time(NULL));
	}

	void CGameStateInit::OnBeginState()
	{
	}

	void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		const char KEY_ESC = 27;
		const char KEY_SPACE = ' ';
		if (nChar == KEY_SPACE)
			GotoGameState(GAME_STATE_RUN);						// ������GAME_STATE_RUN
		else if (nChar == KEY_ESC)								// Demo �����C������k
			PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);	// �����C��
	}

	void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
	{
		GotoGameState(GAME_STATE_RUN);		// ������GAME_STATE_RUN
	}

	void CGameStateInit::OnShow()
	{
		//
		// �K�Wlogo
		//
		logo.SetTopLeft((SIZE_X - logo.Width()) / 2, SIZE_Y / 8);
		logo.ShowBitmap();
		//
		// Demo�ù��r�����ϥΡA���L�}�o�ɽкɶq�קK�����ϥΦr���A���CMovingBitmap����n
		//
		CDC *pDC = CDDraw::GetBackCDC();			// ���o Back Plain �� CDC 
		CFont f, *fp;
		f.CreatePointFont(160, "Times New Roman");	// ���� font f; 160���16 point���r
		fp = pDC->SelectObject(&f);					// ��� font f
		pDC->SetBkColor(RGB(0, 0, 0));
		pDC->SetTextColor(RGB(255, 255, 0));
		pDC->TextOut(120, 220, "Please click mouse or press SPACE to begin.");
		pDC->TextOut(5, 395, "Press Ctrl-F to switch in between window mode and full screen mode.");
		if (ENABLE_GAME_PAUSE)
			pDC->TextOut(5, 425, "Press Ctrl-Q to pause the Game.");
		pDC->TextOut(5, 455, "Press Alt-F4 or ESC to Quit.");
		pDC->SelectObject(fp);						// �� font f (�d�U���n�|�F��)
		CDDraw::ReleaseBackCDC();					// �� Back Plain �� CDC
	}
	//CGaneStateInit
#pragma endregion

								
/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����������A(Game Over)
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
		// ��ϫܦh�ɡAOnInit���J�Ҧ����ϭn��ܦh�ɶ��C���קK���C�����H
		//     �������@�СA�C���|�X�{�uLoading ...�v�A���Loading���i�סC
		//
		ShowInitProgress(66);	// ���ӫe�@�Ӫ��A���i�סA���B�i�׵���66%
		//
		// �}�l���J���
		//
		Sleep(300);				// ��C�A�H�K�ݲM���i�סA��ڹC���ЧR����Sleep
		//
		// �̲׶i�׬�100%
		//
		ShowInitProgress(100);
	}

	void CGameStateOver::OnShow()
	{
		CDC *pDC = CDDraw::GetBackCDC();			// ���o Back Plain �� CDC 
		CFont f, *fp;
		f.CreatePointFont(160, "Times New Roman");	// ���� font f; 160���16 point���r
		fp = pDC->SelectObject(&f);					// ��� font f
		pDC->SetBkColor(RGB(0, 0, 0));
		pDC->SetTextColor(RGB(255, 255, 0));
		char str[80];								// Demo �Ʀr��r�ꪺ�ഫ
		sprintf(str, "Game Over ! (%d)", counter / 30);
		pDC->TextOut(240, 210, str);
		pDC->SelectObject(fp);						// �� font f (�d�U���n�|�F��)
		CDDraw::ReleaseBackCDC();					// �� Back Plain �� CDC
	}
	//CGameStateOver
#pragma endregion


/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����C�����檫��A�D�n���C���{�����b�o��
/////////////////////////////////////////////////////////////////////////////

#pragma region CGameStateRun

	CGameStateRun::CGameStateRun(CGame *g)
		: CGameState(g), NUMBALLS(28)
	{
	}

	CGameStateRun::~CGameStateRun()
	{

	}

	void CGameStateRun::OnBeginState()
	{

		CAudio::Instance()->Play(AUDIO_LAKE, true);			// ���� WAVE
		CAudio::Instance()->Play(AUDIO_DING, false);		// ���� WAVE
		CAudio::Instance()->Play(AUDIO_NTUT, true);			// ���� MIDI

		gameMap.LoadBitmap();
		gameMap.Initialize();

		hero.Initialize();
		hero.SetGameMap(&gameMap);

		enemy.Initialize();
	}

	void CGameStateRun::OnMove()							// ���ʹC������
	{
		// �p�G�Ʊ�ק�cursor���˦��A�h�N�U���{����commment�����Y�i
		// SetCursor(AfxGetApp()->LoadCursor(IDC_GAMECURSOR));


		mapX = gameMap.getX();
		mapY = gameMap.getY();
		//���ʤl�u
		hero.OnMoveBullet();
		//�R���l�u
		hero.killBullet();

		hero.OnMove();

		

		if (!gameMap.getMapBlock(enemy.getY2(), enemy.getX1())) enemy.SetOnBlock(false);
		else enemy.SetOnBlock(true);
		enemy.OnMove();

		// �P�_���l�O�_�I��y
		//
		/*for (i = 0; i < NUMBALLS; i++)
			if (ball[i].IsAlive() && ball[i].HitEraser(&eraser)) {
				ball[i].SetIsAlive(false);
				CAudio::Instance()->Play(AUDIO_DING);
				hits_left.Add(-1);
				//
				// �Y�Ѿl�I�����Ƭ�0�A�h����Game Over���A
				//
				if (hits_left.GetInteger() <= 0) {
					CAudio::Instance()->Stop(AUDIO_LAKE);	// ���� WAVE
					CAudio::Instance()->Stop(AUDIO_NTUT);	// ���� MIDI
					GotoGameState(GAME_STATE_OVER);
				}
			}*/
		
	}

	void CGameStateRun::OnInit()  								// �C������Ȥιϧγ]�w
	{
		//
		// ��ϫܦh�ɡAOnInit���J�Ҧ����ϭn��ܦh�ɶ��C���קK���C�����H
		//     �������@�СA�C���|�X�{�uLoading ...�v�A���Loading���i�סC
		//
		ShowInitProgress(33);	// ���ӫe�@�Ӫ��A���i�סA���B�i�׵���33%
		//
		// �}�l���J���
		// ��������Loading�ʧ@�A�����i��
		ShowInitProgress(50);
		Sleep(300); // ��C�A�H�K�ݲM���i�סA��ڹC���ЧR����Sleep
		//
		// �~����J��L���
		//
		
		hero.LoadBitmap();
		enemy.LoadBitmap();

		hits_left.LoadBitmap();
		CAudio::Instance()->Load(AUDIO_DING, "sounds\\ding.wav");	// ���J�s��0���n��ding.wav
		CAudio::Instance()->Load(AUDIO_LAKE, "sounds\\lake.mp3");	// ���J�s��1���n��lake.mp3
		CAudio::Instance()->Load(AUDIO_NTUT, "sounds\\ntut.mid");	// ���J�s��2���n��ntut.mid
		//
		// ��OnInit�ʧ@�|����CGameStaterOver::OnInit()�A�ҥH�i���٨S��100%
		//
		hero.InitialBullet();
	}

	void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		const char KEY_LEFT = 0x25;		// keyboard���b�Y
		const char KEY_UP = 0x26;		// keyboard�W�b�Y
		const char KEY_RIGHT = 0x27;	// keyboard�k�b�Y
		const char KEY_DOWN = 0x28;		// keyboard�U�b�Y
		const char KEY_A = 0x41;		// keyboard a
		const char KEY_S = 0x53;		// keyboard s
		const char KEY_Q = 0x51;

		//��1 �k2 �W3 �U4
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
			hero.SetMovingDown(true);
			hero.SetDirection(4);
		}
		if (nChar == KEY_A)
		{
			hero.SetShooting(true);
			hero.addBullet();
		}
		if (nChar == KEY_S)
		{
			hero.SetRising(true);
		}
		if (nChar == KEY_Q)
		{
			//hero.SetXY(300, 400);
			enemy.SetAlive(true);
		}

	}

	void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		const char KEY_LEFT = 0x25;		// keyboard���b�Y
		const char KEY_UP = 0x26;		// keyboard�W�b�Y
		const char KEY_RIGHT = 0x27;	// keyboard�k�b�Y
		const char KEY_DOWN = 0x28;		// keyboard�U�b�Y
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
	}

#pragma region OnButton
	void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // �B�z�ƹ����ʧ@
	{
		//eraser.SetMovingLeft(true);
	}

	void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
	{
		//eraser.SetMovingLeft(false);
	}

	void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
	{
		// �S�ơC�p�G�ݭn�B�z�ƹ����ʪ��ܡA�gcode�b�o��
	}

	void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // �B�z�ƹ����ʧ@
	{
		//eraser.SetMovingRight(true);
	}

	void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
	{
		//eraser.SetMovingRight(false);
	}
#pragma endregion

	void CGameStateRun::OnShow()
	{
		//
		//  �`�N�GShow�̭��d�U���n���ʥ��󪫥󪺮y�СA���ʮy�Ъ��u�@����Move���~��A
		//        �_�h��������sø�Ϯ�(OnDraw)�A����N�|���ʡA�ݰ_�ӷ|�ܩǡC���ӳN�y
		//        ���AMove�t�dMVC����Model�AShow�t�dView�A��View�������Model�C
		//
		//
		//  �K�W�I���ϡB�����ơB�y�B���l�B�u�����y
		//
		gameMap.OnShow();
		//
		//  �K�W���W�Υk�U��������
		//
		if (enemy.isShow()) enemy.OnShow();
		hero.OnShowBullet();
		hero.OnShow();
	}
	//CGameStateRun
#pragma endregion



}	//namespace game_framwork