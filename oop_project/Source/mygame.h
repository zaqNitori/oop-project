/*
 * mygame.h: 本檔案儲遊戲本身的class的interface
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
 *	 2004-03-02 V4.0
 *      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
 *         demonstrate the use of states.
 *   2005-09-13
 *      Rewrite the codes for CBall and CEraser.
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *   2006-02-08 V4.2
 *      1. Rename OnInitialUpdate() -> OnInit().
 *      2. Replace AUDIO_CANYON as AUDIO_NTUT.
 *      3. Add help bitmap to CGameStateRun.
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *   2008-02-15 V4.4
 *      1. Add namespace game_framework.
 *      2. Replace the demonstration of animation as a new bouncing ball.
 *      3. Use ShowInitProgress(percent) to display loading progress. 
*/

/////////////////////////////////////////////////////////////////////////////
// Constants
/////////////////////////////////////////////////////////////////////////////

enum AUDIO_ID {				// 定義各種音效的編號
	AUDIO_DING,				// 0
	AUDIO_LAKE,				// 1
	AUDIO_NTUT				// 2
};

namespace game_framework {

/////////////////////////////////////////////////////////////////////////////
// 這個class提供背景地圖,並不會移動(不是camera)
// 
/////////////////////////////////////////////////////////////////////////////

class CGameMap
{
public:
	CGameMap();
	void Initialize();
	void LoadBitmap();
	void OnShow();
	void setXY(int, int);
	bool getMapBlock(int, int);
	int getX();
	int getY();
	int getSize();

private:
	void SetFloorRoof();
	void SetBlock(int, int, int, int);			//編輯地圖可跳上的障礙物
	CAnimation mapBmp;
	bool isMovingLeft;
	bool isMovingRight;
	int floor, roof;
	int mapX, mapY;				//地圖座標
	//int map[18][130];			//地圖編輯
	int size;
	int weight, height;
	int map[40][260];

};

class CShoot
{
public:
	CShoot();
	~CShoot();
	void Initialize();
	void LoadShootLeft(char*,char*);
	void LoadShootRight(char*);
	void OnMoveL();
	void OnMoveR();
	void OnShowL();
	void OnShowR();
	void SetXY(int, int);
	bool isfinalBitmap(int);

private:
	CAnimation CShootLHero;
	CAnimation CShootLGun;
	CAnimation CShootR;
	int width;
	int x, y;		//繪製座標


};

/////////////////////////////////////////////////////////////////////////////
// 這個class提供可以用鍵盤或滑鼠控制的擦子
// 看懂就可以改寫成自己的程式了
/////////////////////////////////////////////////////////////////////////////

#pragma region CEraser
class CEraser
{
public:
	CEraser();
	int  GetX1();					// 擦子左上角 x 座標
	int  GetY1();					// 擦子左上角 y 座標
	int  GetX2();					// 擦子右下角 x 座標
	int  GetY2();					// 擦子右下角 y 座標
	void Initialize();				// 設定擦子為初始值
	void LoadBitmap();				// 載入圖形
	void OnMove();					// 移動擦子
	void OnShow();					// 將擦子圖形貼到畫面
	void SetMovingDown(bool flag);	// 設定是否正在往下移動
	void SetMovingLeft(bool flag);	// 設定是否正在往左移動
	void SetMovingRight(bool flag); // 設定是否正在往右移動
	void SetMovingUp(bool flag);	// 設定是否正在往上移動
	void SetXY(int nx, int ny);		// 設定擦子左上角座標
protected:
	CAnimation animation;		// 擦子的動畫
	int x, y;					// 擦子左上角座標
	bool isMovingDown;			// 是否正在往下移動
	bool isMovingLeft;			// 是否正在往左移動
	bool isMovingRight;			// 是否正在往右移動
	bool isMovingUp;			// 是否正在往上移動
};
#pragma endregion

class CMove
{
public:
	CMove();
	void Initialize();				// 設定初始值
	void LoadBitmap_MoveL(char*);	// 載入圖形
	void LoadBitmap_MoveR(char*);
	void LoadBitmap_ShootL(char*, char*);
	void LoadBitmap_ShootR(char*);
	void OnMove(int*, int*);		// 移動
	void OnShow();					// 將圖形貼到畫面
	void OnShow_Shoot();
	void SetMovingDown(bool flag);	// 設定是否正在往下移動
	void SetMovingLeft(bool flag);	// 設定是否正在往左移動
	void SetMovingRight(bool flag); // 設定是否正在往右移動
	void SetMovingUp(bool flag);	// 設定是否正在往上移動
	void SetRising(bool flag);
	void SetFalling(bool flag);
	void SetDefaultHeight(int);
	void SetShooting(bool flag);
	void SetDirection(int);
	void SetXY(int nx, int ny);		// 設定左上角座標
	bool isfinalBitmap(int);

protected:
	bool isMovingDown;				//下動
	bool isMovingUp;				//上動
	bool isMovingLeft;				//左動
	bool isMovingRight;				//右動
	bool isRising;					//正在上升 
	bool isFalling;
	bool isShooting;				//射擊
	int defaultHeight;

private:
	int x, y;
	CAnimation CmoveL;				//動畫
	CAnimation CmoveR;
	CShoot CMoveShoot;
	int floor;						//地板高度
	int step, ini_step;				//移動速度
	int velocity, ini_velocity;		//速度(上升、下降)
	int direction, dir_horizontal;	//按鍵方向、前一個水平方向

};

class CStand : public CMove
{
public:
	CStand();
	void Initialize();
	void LoadBitmap_StandL(char*);
	void LoadBitmap_StandR(char*);
	void LoadBitmap_ShootL(char*, char*);
	void LoadBitmap_ShootR(char*);
	void SetDirection(int);
	void OnMove(int, int);
	void OnShow_Stand();
	void OnShow_Shoot();
	bool isfinalBitmap(int);

private:
	int x, y;
	CAnimation CStandL;
	CAnimation CStandR;
	CShoot CStandShoot;
	int direction, dir_horizontal;
};

class CJump : public CMove			//只處理上升下降
{
public:
	CJump();
	void Initialize();
	bool OnMove(int*, int*);		//回傳isFall的狀態
	void LoadBitmap_RiseL(char*);
	void LoadBitmap_RiseR(char*);
	void LoadBitmap_FallL(char*);
	void LoadBitmap_FallR(char*);
	void LoadBitmap_ShootL(char*, char*);
	void LoadBitmap_ShootR(char*);
	void OnShow_Rise();
	void OnShow_Fall();
	void OnShow_Shoot();
	void SetRising(bool flag);
	void SetDirection(int);
	void SetGameMap(CGameMap*);
	bool isfinalBitmap(int);

private:
	bool isEmpty(int,int);
	CAnimation CRiseL;
	CAnimation CRiseR;
	CAnimation CFallL;
	CAnimation CFallR;
	CGameMap* gameMap;
	CShoot CJumpShoot;
	bool isRising;					//正在上升
	bool isFalling;					//正在下降
	int x, y;						//座標
	int floor;						//最下方地板
	int velocity, ini_velocity;		//速度、初速度
	int mapX, mapY;					//地圖座標
	int direction, dir_horizontal;	//按鍵方向、上一個水平方向
	int _size;

};

class CCrouch : public CMove		//只處理下蹲相關動作
{
public:
	CCrouch();
	void Initialize();
	void OnMove(int,int);		//移動控制
	void LoadBitmap_MoveL(char*);
	void LoadBitmap_MoveR(char*);
	void LoadBitmap_StandL(char*);
	void LoadBitmap_StandR(char*);
	void LoadBitmap_ShootL(char*,char*);
	void LoadBitmap_ShootR(char*);
	void OnShow_Move();
	void OnShow_Stand();
	void OnShow_Shoot();
	void SetDirection(int);
	bool isfinalBitmap(int);

private:
	CAnimation CMoveL;				//左走
	CAnimation CMoveR;				//右走
	CAnimation CStandL;				//向左蹲
	CAnimation CStandR;				//向右蹲
	CShoot CcrouchShoot;			//處理射擊
	bool isMovingLeft;
	bool isMovingRight;
	int x, y;						//座標
	int floor;						//最下方地板
	int step;
	int velocity, ini_velocity;		//速度、初速度
	int direction, dir_horizontal;	//按鍵方向、上一個水平方向

};


/////////////////////////////////////////////////////////////////////////////
// 這個class提供Hero物件
// 
/////////////////////////////////////////////////////////////////////////////
class CHero
{
public:
	CHero();
	void LoadBitmap();
	void Initialize();
	void OnShow();
	void OnMove();
	void SetGameMap(CGameMap*);
	void SetMovingDown(bool flag);	// 設定是否正在落下
	void SetMovingLeft(bool flag);	// 設定是否正在往左移動
	void SetMovingRight(bool flag); // 設定是否正在往右移動
	void SetMovingUp(bool flag);	// 設定是否正在跳躍
	void SetShooting(bool flag);	// 設定是否攻擊
	void SetRising(bool flag);
	void SetDirection(int);			// 設定方向
	void ResumeDirection();			// 將方向重新調回左和右
	//void SetFalling(bool flag);
	void getXY();
	void SetXY(int, int);	//方便Demo使用

private:
#pragma region Class物件
	CMove heroMove;
	CStand heroStand;
	CJump heroJump;
	CCrouch heroCrouch;				//下蹲
	CGameMap *gameMap;				
	CMovingBitmap CDefaultStand;	//不顯示、不移動，只處理碰撞
	CMovingBitmap CDefaultCrouch;	//同上
#pragma endregion
	
#pragma region 變數宣告
	bool isRising;					//上升
	bool isFalling;					//墜落
	bool isMovingDown;				//下蹲、下看
	bool isMovingUp;				//上看
	bool isMovingLeft;				//左動
	bool isMovingRight;				//右動
	bool isShooting;				//攻擊
	int direction;					//角色面向
	int dir_horizontal;				//前一次的水平面向
	int floor;						//地板
	int mapX, mapY;					//地圖的座標
	int x, y;						//角色在螢幕的座標
	int defaultW, defaultH;			//站力圖片寬高
#pragma endregion
	
	void gameMap_OnMove();			//處理地圖移動

};

/////////////////////////////////////////////////////////////////////////////
// 這個class提供攝影機物件，是真正顯示出目前地圖
// 
/////////////////////////////////////////////////////////////////////////////

class CGameCamera
{
public:
	CGameCamera();
	~CGameCamera();
	void LoadBitmap();

private:
	int map[16][12];		//以40px為一格,攝影機會顯示16*12的部分地圖
	int nowX, nowY;			//攝影機左上角的格子

};


/////////////////////////////////////////////////////////////////////////////
// 這個class提供繞圈圈的球
// 看懂就可以改寫成自己的程式了
/////////////////////////////////////////////////////////////////////////////

class CBall
{
public:
	CBall();
	bool HitEraser(CEraser *eraser);						// 是否碰到擦子
	bool IsAlive();											// 是否活著
	void LoadBitmap();										// 載入圖形
	void OnMove();											// 移動
	void OnShow();											// 將圖形貼到畫面
	void SetXY(int nx, int ny);								// 設定圓心的座標
	void SetIsAlive(bool alive);							// 設定是否活著
	void SetDelay(int d);									// 設定旋轉的速度
protected:
	CMovingBitmap bmp;			// 球的圖
	CMovingBitmap bmp_center;	// 圓心的圖			
	int x, y;					// 圓心的座標
	int dx, dy;					// 球距離圓心的位移量
	int index;					// 球的「角度」，0-17表示0-360度
	int delay_counter;			// 調整旋轉速度的計數器
	int delay;					// 旋轉的速度
	bool is_alive;				// 是否活著
private:
	bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// 是否碰到參數範圍的矩形
};

/////////////////////////////////////////////////////////////////////////////
// 這個class提供會一直彈跳的球
// 看懂就可以改寫成自己的程式了
/////////////////////////////////////////////////////////////////////////////

class CBouncingBall
{
public:
	CBouncingBall();
	void LoadBitmap();		// 載入圖形
	void OnMove();			// 移動
	void OnShow();			// 將圖形貼到畫面
private:
	int x, y;				// 圖形座標
	int floor;				// 地板的Y座標
	bool rising;			// true表上升、false表下降
	int initial_velocity;	// 初始速度
	int velocity;			// 目前的速度(點/次)
	CAnimation animation;	// 利用動畫作圖形
};

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲開頭畫面物件
// 每個Member function的Implementation都要弄懂
/////////////////////////////////////////////////////////////////////////////

class CGameStateInit : public CGameState {
public:
	CGameStateInit(CGame *g);
	void OnInit();  								// 遊戲的初值及圖形設定
	void OnBeginState();							// 設定每次重玩所需的變數
	void OnKeyUp(UINT, UINT, UINT); 				// 處理鍵盤Up的動作
	void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
protected:
	void OnShow();									// 顯示這個狀態的遊戲畫面
private:
	CMovingBitmap logo;								// csie的logo
	
};

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
// 每個Member function的Implementation都要弄懂
/////////////////////////////////////////////////////////////////////////////

class CGameStateRun : public CGameState {
public:
	CGameStateRun(CGame *g);
	~CGameStateRun();
	void OnBeginState();							// 設定每次重玩所需的變數
	void OnInit();  								// 遊戲的初值及圖形設定
	void OnKeyDown(UINT, UINT, UINT);
	void OnKeyUp(UINT, UINT, UINT);
	void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
	void OnLButtonUp(UINT nFlags, CPoint point);	// 處理滑鼠的動作
	void OnMouseMove(UINT nFlags, CPoint point);	// 處理滑鼠的動作 
	void OnRButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
	void OnRButtonUp(UINT nFlags, CPoint point);	// 處理滑鼠的動作
protected:
	void OnMove();									// 移動遊戲元素
	void OnShow();									// 顯示這個狀態的遊戲畫面
private:
	const int		NUMBALLS;	// 球的總數
	//CMovingBitmap	background;	// 背景圖
	CMovingBitmap	help;		// 說明圖
	CBall			*ball;		// 球的陣列
	CMovingBitmap	corner;		// 角落圖
	CEraser			eraser;		// 拍子
	CInteger		hits_left;	// 剩下的撞擊數
	CBouncingBall   bball;		// 反覆彈跳的球
	
	CHero hero;
	CGameMap gameMap;
};

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的結束狀態(Game Over)
// 每個Member function的Implementation都要弄懂
/////////////////////////////////////////////////////////////////////////////

class CGameStateOver : public CGameState {
public:
	CGameStateOver(CGame *g);
	void OnBeginState();							// 設定每次重玩所需的變數
	void OnInit();
protected:
	void OnMove();									// 移動遊戲元素
	void OnShow();									// 顯示這個狀態的遊戲畫面
private:
	int counter;	// 倒數之計數器
};

}