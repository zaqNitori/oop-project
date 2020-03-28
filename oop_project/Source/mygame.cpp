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
		const int ini_x = 200;		//5121
		const int ini_y = 450;		//721
		x = ini_x;
		y = ini_y;
		floor = 450;
		isFalling = isRising = isMovingDown = isMovingLeft = isMovingRight = isMovingUp = isShooting = false;
	}

	void CHero::LoadBitmap()
	{
		char *filestand[] = { ".\\image\\108.bmp"};
		char *filemoveL[] = { ".\\image\\moveL\\left1.bmp",".\\image\\moveL\\left2.bmp",".\\image\\moveL\\left3.bmp" , ".\\image\\moveL\\left4.bmp" };
		char *filemoveR[] = { ".\\image\\moveR\\right1.bmp",".\\image\\moveR\\right2.bmp",".\\image\\moveR\\right3.bmp" , ".\\image\\moveR\\right4.bmp" };
		char *fileRise[] = { ".\\image\\jumpL\\left1.bmp",".\\image\\jumpL\\left2.bmp",".\\image\\jumpL\\left3.bmp",".\\image\\jumpL\\left4.bmp",".\\image\\jumpL\\left5.bmp",
			".\\image\\jumpL\\left6.bmp",".\\image\\jumpL\\left7.bmp",".\\image\\jumpL\\left8.bmp",".\\image\\jumpL\\left9.bmp",".\\image\\jumpL\\left10.bmp" };
		char *fileFall[] = { ".\\image\\jumpL\\left12-1.bmp" };

		#pragma region �ʵe���J

		heroStand.LoadBitmap(filestand[0]);
		//heroMoveUD.LoadBitmap(filestand[0]);
		for (int i = 0; i < 4; i++)
		{
			heroMoveL.LoadBitmap(filemoveL[i]);
			heroMoveR.LoadBitmap(filemoveR[i]);
		}
		for (int i = 0; i < 10; i+=2)
		{
			heroJump.LoadBitmap_Rise(fileRise[i]);
			heroMoveUD.LoadBitmap(fileRise[i]);
		}
		heroJump.LoadBitmap_Fall(fileFall[0]);

		#pragma endregion
		
	}

	void CHero::OnMove()
	{
		heroStand.OnMove(&x, &y);		//���ݭn
		heroMoveL.OnMove(&x, &y);
		heroMoveR.OnMove(&x, &y);
		isFalling = heroJump.OnMove(&x, &y);
		heroJump.OnMove(&x, &y);
		heroMoveUD.OnMove(&x, &y);
	}

	void CHero::OnShow()
	{
		
		#pragma region SetXY
		if (isRising || isFalling) heroJump.SetXY(x, y);
		if (isMovingLeft) heroMoveL.SetXY(x, y);
		if (isMovingRight) heroMoveR.SetXY(x, y);
		if (isMovingDown || isMovingUp) heroMoveUD.SetXY(x, y);
		#pragma endregion

		#pragma region OnShow
		if (isRising||isFalling)
		{
			if(isRising) heroJump.OnShow_Rise();
			else if(isFalling) heroJump.OnShow_Fall();
		}
		else if (isMovingLeft)
		{
			heroMoveL.OnShow();
		}
		else if (isMovingRight)
		{
			heroMoveR.OnShow();
		}
		else if (isMovingDown || isMovingUp)
		{
			heroMoveUD.OnShow();
		}
		if(!(isMovingDown || isMovingLeft || isMovingRight || isMovingUp))
		{
			heroStand.OnShow();
		}

		#pragma endregion

	}

	#pragma region SetState
		void CHero::SetRising(bool flag)
		{
			isRising = flag;
			heroJump.SetRising(flag);
		}

		void CHero::SetMovingDown(bool flag)
		{
			isMovingDown = flag;
			heroMoveUD.SetMovingDown(flag);
		}
	
		void CHero::SetMovingUp(bool flag)
		{
			isMovingUp = flag;
			heroMoveUD.SetMovingUp(flag);
		}

		void CHero::SetMovingLeft(bool flag)
		{
			isMovingLeft = flag;
			heroMoveL.SetMovingLeft(flag);
		}

		void CHero::SetMovingRight(bool flag)
		{
			isMovingRight = flag;
			heroMoveR.SetMovingRight(flag);
		}

		void CHero::SetShooting(bool flag)
		{
			isShooting = flag;
		}
	#pragma endregion

	

	//CHero
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
		isRising = isMovingDown = isMovingLeft = isMovingRight = isMovingUp = false;
	}

	int CMove::GetX1()
	{
		return x;
	}

	int CMove::GetX2()
	{
		return x + animation.Width();
	}

	int CMove::GetY1()
	{
		return y;
	}

	int CMove::GetY2()
	{
		return y + animation.Height();
	}

	void CMove::LoadBitmap(char *file)
	{
		animation.AddBitmap(file, RGB(0, 0, 255));
	}

	void CMove::OnMove(int* nx, int* ny)
	{
		const int step = 5;
		x = *nx;
		y = *ny;
		animation.OnMove();
		if (isMovingLeft)
			x -= step;
		if (isMovingRight)
			x += step;
		if (isMovingUp)			//�W��
		{
			y -= step;
		}
		if (isMovingDown)		//�U��
		{
			if(y+step<=450)
				y += step;
		}
		*nx = x;
		*ny = y;
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

	void CMove::OnShow()
	{
		animation.SetTopLeft(x, y);
		animation.OnShow();
	}
	//CMove
#pragma endregion

#pragma region CJump

	CJump::CJump()
	{
		Initialize();
	}

	void CJump::Initialize()
	{
		const int FLOOR = 450;			//�a�O����
		const int INI_VELOCITY = 20;	//��t
		floor = FLOOR;
		velocity = ini_velocity = INI_VELOCITY;
		isRising = isFalling = false;
	}

	void CJump::LoadBitmap_Fall(char* file)
	{
		CFall.AddBitmap(file, RGB(0, 0, 255));
	}

	void CJump::LoadBitmap_Rise(char* file)
	{
		CRise.AddBitmap(file, RGB(0, 0, 255));
	}

	void CJump::SetRising(bool flag)
	{
		isRising = flag;
	}

	bool CJump::OnMove(int *nx, int *ny)	//�^��isFalling�����A
	{
		x = *nx;
		y = *ny;
		//���B��Bug�b�٨S����floor���ɭԭ��s���D,�|�����b���_�ӡA�åB�t�׷|�ھڧA���U���t���~��W�[�C
		if (isRising)
		{
			CRise.OnMove();
			if (velocity > 0)
			{
				y -= velocity;
				velocity--;
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
			//CFall.OnMove();
			if (y + velocity <= floor)
			{
				y += velocity;
				velocity++;
			}
			else
			{
				y = floor - 1;
				isFalling = false;
				velocity = ini_velocity;
			}
		}
		*nx = x;
		*ny = y;
		return isFalling;
	}

	void CJump::OnShow_Rise()
	{
		CRise.SetTopLeft(x, y);
		CRise.OnShow();
	}

	void CJump::OnShow_Fall()
	{
		CFall.SetTopLeft(x, y);
		CFall.OnShow();
	}

	//CJump
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
		ball = new CBall[NUMBALLS];
	}

	CGameStateRun::~CGameStateRun()
	{
		delete[] ball;
	}

	void CGameStateRun::OnBeginState()
	{
		const int BALL_GAP = 90;
		const int BALL_XY_OFFSET = 45;
		const int BALL_PER_ROW = 7;
		const int HITS_LEFT = 10;
		const int HITS_LEFT_X = 590;
		const int HITS_LEFT_Y = 0;
		const int BACKGROUND_X = 0;
		const int ANIMATION_SPEED = 15;
		for (int i = 0; i < NUMBALLS; i++) {				// �]�w�y���_�l�y��
			int x_pos = i % BALL_PER_ROW;
			int y_pos = i / BALL_PER_ROW;
			ball[i].SetXY(x_pos * BALL_GAP + BALL_XY_OFFSET, y_pos * BALL_GAP + BALL_XY_OFFSET);
			ball[i].SetDelay(x_pos);
			ball[i].SetIsAlive(true);
		}
		eraser.Initialize();
		hero.Initialize();
		background.SetTopLeft(0, SIZE_Y-background.Height());		// �]�w�I�����_�l�y��
		help.SetTopLeft(0, SIZE_Y - help.Height());			// �]�w�����Ϫ��_�l�y��
		hits_left.SetInteger(HITS_LEFT);					// ���w�ѤU��������
		hits_left.SetTopLeft(HITS_LEFT_X, HITS_LEFT_Y);		// ���w�ѤU�����ƪ��y��
		CAudio::Instance()->Play(AUDIO_LAKE, true);			// ���� WAVE
		CAudio::Instance()->Play(AUDIO_DING, false);		// ���� WAVE
		CAudio::Instance()->Play(AUDIO_NTUT, true);			// ���� MIDI
	}

	void CGameStateRun::OnMove()							// ���ʹC������
	{
		//
		// �p�G�Ʊ�ק�cursor���˦��A�h�N�U���{����commment�����Y�i
		//
		// SetCursor(AfxGetApp()->LoadCursor(IDC_GAMECURSOR));
		//
		// ���ʭI���Ϫ��y��
		//

		/*if (background.Top() > SIZE_Y)
			background.SetTopLeft(60, -background.Height());
		background.SetTopLeft(background.Left(), background.Top() + 1);*/

		//
		// ���ʲy
		//
		int i;
		for (i = 0; i < NUMBALLS; i++)
			ball[i].OnMove();
		//
		// �������l
		//
		eraser.OnMove();
		hero.OnMove();
		//
		// �P�_���l�O�_�I��y
		//
		for (i = 0; i < NUMBALLS; i++)
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
			}
		//
		// ���ʼu�����y
		//
		bball.OnMove();
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
		//
		int i;
		for (i = 0; i < NUMBALLS; i++)
			ball[i].LoadBitmap();								// ���J��i�Ӳy���ϧ�
		eraser.LoadBitmap();
		background.LoadBitmap(IDB_GameMap);					// ���J�I�����ϧ�
		//
		// ��������Loading�ʧ@�A�����i��
		//
		ShowInitProgress(50);
		Sleep(300); // ��C�A�H�K�ݲM���i�סA��ڹC���ЧR����Sleep
		//
		// �~����J��L���
		//
		help.LoadBitmap(IDB_HELP, RGB(255, 255, 255));				// ���J�������ϧ�
		corner.LoadBitmap(IDB_CORNER);							// ���J�����ϧ�
		corner.ShowBitmap(background);							// �Ncorner�K��background
		bball.LoadBitmap();										// ���J�ϧ�
		hero.LoadBitmap();
		hits_left.LoadBitmap();
		CAudio::Instance()->Load(AUDIO_DING, "sounds\\ding.wav");	// ���J�s��0���n��ding.wav
		CAudio::Instance()->Load(AUDIO_LAKE, "sounds\\lake.mp3");	// ���J�s��1���n��lake.mp3
		CAudio::Instance()->Load(AUDIO_NTUT, "sounds\\ntut.mid");	// ���J�s��2���n��ntut.mid
		//
		// ��OnInit�ʧ@�|����CGameStaterOver::OnInit()�A�ҥH�i���٨S��100%
		//
	}

	void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		const char KEY_LEFT = 0x25;		// keyboard���b�Y
		const char KEY_UP = 0x26;		// keyboard�W�b�Y
		const char KEY_RIGHT = 0x27;	// keyboard�k�b�Y
		const char KEY_DOWN = 0x28;		// keyboard�U�b�Y
		const char KEY_A = 0x41;		// keyboard a
		const char KEY_S = 0x53;		// keyboard s
		if (nChar == KEY_LEFT)
		{
			eraser.SetMovingLeft(true);
			hero.SetMovingLeft(true);
		}
		if (nChar == KEY_RIGHT)
		{
			eraser.SetMovingRight(true);
			hero.SetMovingRight(true);
		}
		if (nChar == KEY_UP)
		{
			eraser.SetMovingUp(true);
			hero.SetMovingUp(true);
		}
		if (nChar == KEY_DOWN)
		{
			eraser.SetMovingDown(true);
			hero.SetMovingDown(true);
		}
		if (nChar == KEY_A)
		{
			hero.SetShooting(true);
		}
		if (nChar == KEY_S)
		{
			hero.SetRising(true);
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
			eraser.SetMovingLeft(false);
			hero.SetMovingLeft(false);
		}
		if (nChar == KEY_RIGHT)
		{
			eraser.SetMovingRight(false);
			hero.SetMovingRight(false);
		}
		if (nChar == KEY_UP)
		{
			eraser.SetMovingUp(false);
			hero.SetMovingUp(false);
		}
		if (nChar == KEY_DOWN)
		{
			eraser.SetMovingDown(false);
			hero.SetMovingDown(false);
		}
		if (nChar == KEY_A)
		{
			hero.SetShooting(false);
		}
	}

#pragma region OnButton
	void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // �B�z�ƹ����ʧ@
	{
		eraser.SetMovingLeft(true);
	}

	void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
	{
		eraser.SetMovingLeft(false);
	}

	void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
	{
		// �S�ơC�p�G�ݭn�B�z�ƹ����ʪ��ܡA�gcode�b�o��
	}

	void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // �B�z�ƹ����ʧ@
	{
		eraser.SetMovingRight(true);
	}

	void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
	{
		eraser.SetMovingRight(false);
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
		background.ShowBitmap();			// �K�W�I����
		help.ShowBitmap();					// �K�W������
		hits_left.ShowBitmap();
		for (int i = 0; i < NUMBALLS; i++)
			ball[i].OnShow();				// �K�W��i���y
		bball.OnShow();						// �K�W�u�����y
		eraser.OnShow();					// �K�W���l
		//
		//  �K�W���W�Υk�U��������
		//
		corner.SetTopLeft(0, 0);
		corner.ShowBitmap();
		corner.SetTopLeft(SIZE_X - corner.Width(), SIZE_Y - corner.Height());
		corner.ShowBitmap();
		hero.OnShow();
	}
	//CGameStateRun
#pragma endregion



}	//namespace game_framwork