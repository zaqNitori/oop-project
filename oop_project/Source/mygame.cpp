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

		#pragma region 動畫載入

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
		heroStand.OnMove(&x, &y);		//不需要
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
		// 檢測擦子所構成的矩形是否碰到球
		return HitRectangle(eraser->GetX1(), eraser->GetY1(),
			eraser->GetX2(), eraser->GetY2());
	}

	bool CBall::HitRectangle(int tx1, int ty1, int tx2, int ty2)
	{
		int x1 = x + dx;				// 球的左上角x座標
		int y1 = y + dy;				// 球的左上角y座標
		int x2 = x1 + bmp.Width();	// 球的右下角x座標
		int y2 = y1 + bmp.Height();	// 球的右下角y座標
		//
		// 檢測球的矩形與參數矩形是否有交集
		//
		return (tx2 >= x1 && tx1 <= x2 && ty2 >= y1 && ty1 <= y2);
	}

	bool CBall::IsAlive()
	{
		return is_alive;
	}

	void CBall::LoadBitmap()
	{
		bmp.LoadBitmap(IDB_BALL, RGB(0, 0, 0));			// 載入球的圖形
		bmp_center.LoadBitmap(IDB_CENTER, RGB(0, 0, 0));	// 載入球圓心的圖形
	}

	void CBall::OnMove()
	{
		if (!is_alive)
			return;
		delay_counter--;
		if (delay_counter < 0) {
			delay_counter = delay;
			//
			// 計算球向對於圓心的位移量dx, dy
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
		const int INITIAL_VELOCITY = 20;	// 初始上升速度
		const int FLOOR = 400;				// 地板座標
		floor = FLOOR;
		x = 95; y = FLOOR - 1;				// y座標比地板高1點(站在地板上)
		rising = true;
		initial_velocity = INITIAL_VELOCITY;
		velocity = initial_velocity;
	}

	void CBouncingBall::LoadBitmap()
	{
		char *filename[4] = { ".\\bitmaps\\ball1.bmp",".\\bitmaps\\ball2.bmp",".\\bitmaps\\ball3.bmp",".\\bitmaps\\ball4.bmp" };
		for (int i = 0; i < 4; i++)	// 載入動畫(由4張圖形構成)
			animation.AddBitmap(filename[i], RGB(0, 0, 0));
	}

	void CBouncingBall::OnMove()
	{
		if (rising) {			// 上升狀態
			if (velocity > 0) {
				y -= velocity;	// 當速度 > 0時，y軸上升(移動velocity個點，velocity的單位為 點/次)
				velocity--;		// 受重力影響，下次的上升速度降低
			}
			else {
				rising = false; // 當速度 <= 0，上升終止，下次改為下降
				velocity = 1;	// 下降的初速(velocity)為1
			}
		}
		else {				// 下降狀態
			if (y < floor - 1) {  // 當y座標還沒碰到地板
				y += velocity;	// y軸下降(移動velocity個點，velocity的單位為 點/次)
				velocity++;		// 受重力影響，下次的下降速度增加
			}
			else {
				y = floor - 1;  // 當y座標低於地板，更正為地板上
				rising = true;	// 探底反彈，下次改為上升
				velocity = initial_velocity; // 重設上升初始速度
			}
		}
		animation.OnMove();		// 執行一次animation.OnMove()，animation才會換圖
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
		const int x_pos = 200;			//預設x位置
		const int y_pos = 450;			//預設y位置
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
		if (isMovingUp)			//上看
		{
			y -= step;
		}
		if (isMovingDown)		//下蹲
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
		const int FLOOR = 450;			//地板高度
		const int INI_VELOCITY = 20;	//初速
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

	bool CJump::OnMove(int *nx, int *ny)	//回傳isFalling的狀態
	{
		x = *nx;
		y = *ny;
		//此處有Bug在還沒掉到floor的時候重新跳躍,會直接在跳起來，並且速度會根據你落下的速度繼續增加。
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
				isRising = false;		//升到最高點要轉為下降
				isFalling = true;		//轉成下降
				velocity = 1;			//下降初速度
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
// 這個class為遊戲的遊戲開頭畫面物件
/////////////////////////////////////////////////////////////////////////////

#pragma region CGameStateInit

	CGameStateInit::CGameStateInit(CGame *g)
		: CGameState(g)
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
		logo.LoadBitmap(IDB_BACKGROUND);
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
		if (nChar == KEY_SPACE)
			GotoGameState(GAME_STATE_RUN);						// 切換至GAME_STATE_RUN
		else if (nChar == KEY_ESC)								// Demo 關閉遊戲的方法
			PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);	// 關閉遊戲
	}

	void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
	{
		GotoGameState(GAME_STATE_RUN);		// 切換至GAME_STATE_RUN
	}

	void CGameStateInit::OnShow()
	{
		//
		// 貼上logo
		//
		logo.SetTopLeft((SIZE_X - logo.Width()) / 2, SIZE_Y / 8);
		logo.ShowBitmap();
		//
		// Demo螢幕字型的使用，不過開發時請盡量避免直接使用字型，改用CMovingBitmap比較好
		//
		CDC *pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC 
		CFont f, *fp;
		f.CreatePointFont(160, "Times New Roman");	// 產生 font f; 160表示16 point的字
		fp = pDC->SelectObject(&f);					// 選用 font f
		pDC->SetBkColor(RGB(0, 0, 0));
		pDC->SetTextColor(RGB(255, 255, 0));
		pDC->TextOut(120, 220, "Please click mouse or press SPACE to begin.");
		pDC->TextOut(5, 395, "Press Ctrl-F to switch in between window mode and full screen mode.");
		if (ENABLE_GAME_PAUSE)
			pDC->TextOut(5, 425, "Press Ctrl-Q to pause the Game.");
		pDC->TextOut(5, 455, "Press Alt-F4 or ESC to Quit.");
		pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
		CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
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
		for (int i = 0; i < NUMBALLS; i++) {				// 設定球的起始座標
			int x_pos = i % BALL_PER_ROW;
			int y_pos = i / BALL_PER_ROW;
			ball[i].SetXY(x_pos * BALL_GAP + BALL_XY_OFFSET, y_pos * BALL_GAP + BALL_XY_OFFSET);
			ball[i].SetDelay(x_pos);
			ball[i].SetIsAlive(true);
		}
		eraser.Initialize();
		hero.Initialize();
		background.SetTopLeft(0, SIZE_Y-background.Height());		// 設定背景的起始座標
		help.SetTopLeft(0, SIZE_Y - help.Height());			// 設定說明圖的起始座標
		hits_left.SetInteger(HITS_LEFT);					// 指定剩下的撞擊數
		hits_left.SetTopLeft(HITS_LEFT_X, HITS_LEFT_Y);		// 指定剩下撞擊數的座標
		CAudio::Instance()->Play(AUDIO_LAKE, true);			// 撥放 WAVE
		CAudio::Instance()->Play(AUDIO_DING, false);		// 撥放 WAVE
		CAudio::Instance()->Play(AUDIO_NTUT, true);			// 撥放 MIDI
	}

	void CGameStateRun::OnMove()							// 移動遊戲元素
	{
		//
		// 如果希望修改cursor的樣式，則將下面程式的commment取消即可
		//
		// SetCursor(AfxGetApp()->LoadCursor(IDC_GAMECURSOR));
		//
		// 移動背景圖的座標
		//

		/*if (background.Top() > SIZE_Y)
			background.SetTopLeft(60, -background.Height());
		background.SetTopLeft(background.Left(), background.Top() + 1);*/

		//
		// 移動球
		//
		int i;
		for (i = 0; i < NUMBALLS; i++)
			ball[i].OnMove();
		//
		// 移動擦子
		//
		eraser.OnMove();
		hero.OnMove();
		//
		// 判斷擦子是否碰到球
		//
		for (i = 0; i < NUMBALLS; i++)
			if (ball[i].IsAlive() && ball[i].HitEraser(&eraser)) {
				ball[i].SetIsAlive(false);
				CAudio::Instance()->Play(AUDIO_DING);
				hits_left.Add(-1);
				//
				// 若剩餘碰撞次數為0，則跳到Game Over狀態
				//
				if (hits_left.GetInteger() <= 0) {
					CAudio::Instance()->Stop(AUDIO_LAKE);	// 停止 WAVE
					CAudio::Instance()->Stop(AUDIO_NTUT);	// 停止 MIDI
					GotoGameState(GAME_STATE_OVER);
				}
			}
		//
		// 移動彈跳的球
		//
		bball.OnMove();
	}

	void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
	{
		//
		// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
		//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
		//
		ShowInitProgress(33);	// 接個前一個狀態的進度，此處進度視為33%
		//
		// 開始載入資料
		//
		int i;
		for (i = 0; i < NUMBALLS; i++)
			ball[i].LoadBitmap();								// 載入第i個球的圖形
		eraser.LoadBitmap();
		background.LoadBitmap(IDB_GameMap);					// 載入背景的圖形
		//
		// 完成部分Loading動作，提高進度
		//
		ShowInitProgress(50);
		Sleep(300); // 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
		//
		// 繼續載入其他資料
		//
		help.LoadBitmap(IDB_HELP, RGB(255, 255, 255));				// 載入說明的圖形
		corner.LoadBitmap(IDB_CORNER);							// 載入角落圖形
		corner.ShowBitmap(background);							// 將corner貼到background
		bball.LoadBitmap();										// 載入圖形
		hero.LoadBitmap();
		hits_left.LoadBitmap();
		CAudio::Instance()->Load(AUDIO_DING, "sounds\\ding.wav");	// 載入編號0的聲音ding.wav
		CAudio::Instance()->Load(AUDIO_LAKE, "sounds\\lake.mp3");	// 載入編號1的聲音lake.mp3
		CAudio::Instance()->Load(AUDIO_NTUT, "sounds\\ntut.mid");	// 載入編號2的聲音ntut.mid
		//
		// 此OnInit動作會接到CGameStaterOver::OnInit()，所以進度還沒到100%
		//
	}

	void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		const char KEY_LEFT = 0x25;		// keyboard左箭頭
		const char KEY_UP = 0x26;		// keyboard上箭頭
		const char KEY_RIGHT = 0x27;	// keyboard右箭頭
		const char KEY_DOWN = 0x28;		// keyboard下箭頭
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
		const char KEY_LEFT = 0x25;		// keyboard左箭頭
		const char KEY_UP = 0x26;		// keyboard上箭頭
		const char KEY_RIGHT = 0x27;	// keyboard右箭頭
		const char KEY_DOWN = 0x28;		// keyboard下箭頭
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
	void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
	{
		eraser.SetMovingLeft(true);
	}

	void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
	{
		eraser.SetMovingLeft(false);
	}

	void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// 處理滑鼠的動作
	{
		// 沒事。如果需要處理滑鼠移動的話，寫code在這裡
	}

	void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
	{
		eraser.SetMovingRight(true);
	}

	void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
	{
		eraser.SetMovingRight(false);
	}
#pragma endregion

	void CGameStateRun::OnShow()
	{
		//
		//  注意：Show裡面千萬不要移動任何物件的座標，移動座標的工作應由Move做才對，
		//        否則當視窗重新繪圖時(OnDraw)，物件就會移動，看起來會很怪。換個術語
		//        說，Move負責MVC中的Model，Show負責View，而View不應更動Model。
		//
		//
		//  貼上背景圖、撞擊數、球、擦子、彈跳的球
		//
		background.ShowBitmap();			// 貼上背景圖
		help.ShowBitmap();					// 貼上說明圖
		hits_left.ShowBitmap();
		for (int i = 0; i < NUMBALLS; i++)
			ball[i].OnShow();				// 貼上第i號球
		bball.OnShow();						// 貼上彈跳的球
		eraser.OnShow();					// 貼上擦子
		//
		//  貼上左上及右下角落的圖
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