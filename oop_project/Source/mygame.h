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
	AUDIO_heroJump,					// 0
	AUDIO_enemyDead,				// 1
	AUDIO_BGM_normal				// 2
};

namespace game_framework {

	class CHero;
	class CEnemy;
/////////////////////////////////////////////////////////////////////////////
// 這個class提供子彈
/////////////////////////////////////////////////////////////////////////////
class CBullet
{
public:
	CBullet();
	CBullet(int, int);			
	~CBullet();
	void Initialize();
	void LoadBitmap(char*);
	void OnShow();
	void OnMove();
	bool isDead();
	bool isShow();
	void SetLife(bool);
	void SetBullet(int, int, int);
	void SetBullet(int, int, int, int, int);
	void SetBulletClass(CAnimation*);
	bool isHit(int, int, int, int);

private:
	CAnimation* bullet;
	int x, y;
	int direction;
	int vx, vy;
	bool isAlive;
	
	int mistake;			//槍枝誤差
	int maxSpeed;			//槍枝最大速度
	int mistakeRate;		//誤差比率
	int getRandom(int);		//槍枝誤差

};

class CAttack				//此為動作執行的class
{
public:
	CAttack();
	~CAttack();
	void Initialize();
	void LoadShootLeft(char*,char*);
	void LoadShootRight(char*);
	void LoadKnifeLeft(char*, char*);
	void LoadKnifeRight(char*);
	void OnMoveShootL();
	void OnMoveShootR();
	void OnMoveKnifeL();
	void OnMoveKnifeR();
	void OnShowShootL();
	void OnShowShootR();
	void OnShowKnifeL();
	void OnShowKnifeR();
	void SetXY(int, int);
	void resetAnimation();

private:
	CAnimation CShootLHero;
	CAnimation CShootLGun;
	CAnimation CShootR;
	CAnimation CKnifeLHero;
	CAnimation CKnifeLKnife;
	CAnimation CKnifeR;
	int width;
	int x, y;		//繪製座標

};

/////////////////////////////////////////////////////////////////////////////
// 這個class提供背景地圖,並不會移動(不是camera)
/////////////////////////////////////////////////////////////////////////////

class CGameMap
{
public:
	CGameMap();
	~CGameMap();
	void Initialize();
	void LoadBitmap();
	void OnShow();
	void setXY(int, int);
	int getMapBlock(int, int);
	int getX();
	int getY();
	int getSize();

#pragma region Bullet
	void InitialBullet();
	void addBullet(int,int,int,int);			//激活一個Bullet物件
	void addEnemyBullet(CEnemy*, int, int);
	void killBullet();							//反激活已經死亡的Bullet物件
	void OnMoveBullet();
	void OnShowBullet();
	bool isBulletHit(CEnemy*);		//碰撞判斷
	bool isBulletHit(CHero*);
#pragma endregion

private:
	void SetBlock(int, int, int, int);			//編輯地圖可跳上的障礙物
	
	CAnimation heroBullet;
	CAnimation enemyBullet;
	vector<CBullet*> vCblt;
	vector<CBullet*> vCbltEnemy;
	CAnimation mapBmp;

	int mapX, mapY;						//地圖座標
	int size;
	int weight, height;
	int map[40][260];
	int bulletNumer;					//槍枝子彈數
	unsigned loop;
	unsigned maxHeroBullet;				//場上同時能存在我方的子彈上限
	unsigned maxEnemyBullet;			//場上同時能存在敵方的子彈上限

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
	void LoadBitmap_KnifeL(char*, char*);
	void LoadBitmap_KnifeR(char*);
	void OnMove(int*, int*);		// 移動
	void OnShow();					// 將圖形貼到畫面
	void OnShow_Attack();
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
	void SetOverlap(bool);
	void resetShootAnimation();

protected:
	bool isMovingDown;				//下動
	bool isMovingUp;				//上動
	bool isMovingLeft;				//左動
	bool isMovingRight;				//右動
	bool isRising;					//正在上升 
	bool isFalling;
	bool isShooting;				//射擊
	bool isOverlap;					//跟敵人重疊
	bool isLock;					//控制鎖-(射擊or刀砍)
	int defaultHeight;

private:
	int x, y;
	CAnimation CmoveL;				//動畫
	CAnimation CmoveR;
	CAttack CMoveShoot;
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
	void LoadBitmap_KnifeL(char*, char*);
	void LoadBitmap_KnifeR(char*);
	void SetDirection(int);
	void OnMove(int, int);
	void OnShow_Stand();
	void OnShow_Attack();
	void resetShootAnimation();

private:
	int x, y;
	CAnimation CStandL;
	CAnimation CStandR;
	CAttack CStandShoot;
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
	void LoadBitmap_KnifeL(char*, char*);
	void LoadBitmap_KnifeR(char*);
	void OnShow_Rise();
	void OnShow_Fall();
	void OnShow_Attack();
	void SetRising(bool flag);
	void SetFalling(bool flag);
	void SetDirection(int);
	void SetGameMap(CGameMap*);
	void SetCanFallFromBlock(bool);
	void resetShootAnimation();

private:
	int isEmpty(int,int);
	CAnimation CRiseL;
	CAnimation CRiseR;
	CAnimation CFallL;
	CAnimation CFallR;
	CGameMap* gameMap;
	CAttack CJumpShoot;
	bool isRising;					//正在上升
	bool isFalling;					//正在下降
	bool canFall;
	int x, y;						//座標
	int velocity, ini_velocity;		//速度、初速度
	int mapX, mapY;					//地圖座標
	int direction, dir_horizontal;	//按鍵方向、上一個水平方向
	int _size;
	int temp;

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
	void LoadBitmap_KnifeL(char*, char*);
	void LoadBitmap_KnifeR(char*);
	void OnShow_Move();
	void OnShow_Stand();
	void OnShow_Attack();
	void SetDirection(int);
	void resetShootAnimation();

private:
	CAnimation CMoveL;				//左走
	CAnimation CMoveR;				//右走
	CAnimation CStandL;				//向左蹲
	CAnimation CStandR;				//向右蹲
	CAttack CcrouchShoot;			//處理射擊
	bool isMovingLeft;
	bool isMovingRight;
	int x, y;						//座標
	int step;
	int velocity, ini_velocity;		//速度、初速度
	int direction, dir_horizontal;	//按鍵方向、上一個水平方向

};

class CDead
{
public:
	CDead();
	~CDead();
	void Initialize();
	void LoadBitmap_L(char*);
	void LoadBitmap_R(char*,char*);
	void OnMove(int, int);
	void OnShow();
	void SetDir(int);
	bool isfinalBitmap();

private:
	CAnimation CDeadL;
	CAnimation CDeadRGun;
	CAnimation CDeadRMan;
	int x, y;
	int direction;

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

#pragma region Setstate
	void SetGameMap(CGameMap*);
	void SetMovingDown(bool flag);	// 設定是否正在落下
	void SetMovingLeft(bool flag);	// 設定是否正在往左移動
	void SetMovingRight(bool flag); // 設定是否正在往右移動
	void SetMovingUp(bool flag);	// 設定是否正在跳躍
	void SetShooting(bool flag);	// 設定是否攻擊
	void SetRising(bool flag);
	void SetDirection(int);			// 設定方向
	void SetFallDownFromBlock(bool);
	void SetOverlap(bool);
	void ResumeDirection();			// 將方向重新調回左和右  
	void SetXY(int, int);			//方便Demo使用
#pragma endregion

#pragma region Getstate
	int getX1();					//get heroLeft
	int getY1();					//get heroTop
	int getX2();					//get heroRight
	int getY2();					//get heroBottom
	int getHeartX1();				//get heart left
	int getHeartY1();				//get heart top
	int getHeartX2();				//get heart right
	int getHeartY2();				//get heart bottom
	int getDir();					//get now dir
	int getDir_hor();				//get last horizontal dir
	bool isNowRising();				//get isRising?
	bool isOverlapEnemy(CEnemy*);		//get is Hero and Enemy overlap
	bool getOverlap();
	bool getShooting();
#pragma endregion


private:
#pragma region Class物件
	CMove heroMove;
	CStand heroStand;
	CJump heroJump;
	CCrouch heroCrouch;				//下蹲
	CGameMap *gameMap;				
	CMovingBitmap CHeart;			//心臟、弱點
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
	bool isOverlap;			//跟敵人重疊
	int direction;					//角色面向
	int dir_horizontal;				//前一次的水平面向
	int mapX, mapY;					//地圖的座標
	int x, y;						//角色在螢幕的座標
	int heartX, heartY;				//心臟起始座標
	int defaultW, defaultH;			//站立圖片寬高
	int delayCount,constDelay;
#pragma endregion
	
	void gameMap_OnMove();			//處理地圖移動
	void gravity();					//重力
	void ResumeShooting();
	void SetHeart();
	void resetAnimation();

};

class CEnemy
{
public:
	CEnemy();
	~CEnemy();
	void Initialize();				//初始化
	void LoadBitmap();				//載入圖片
	void OnShow();					//顯示敵人
	void OnMove();					//敵人移動
	
	void SetAlive(bool);			//設定生命(顯示設定)
	void SetDead(bool);				//設定為死亡狀態
	void SetDead(bool,int);			//設定死亡及死亡方向
	void SetDirection(int);
	void SetOnBlock(bool);			//設定是否站在block上，gravity使用
	void SetShootDelay(int);
	void SetShootState(bool);
	void SetMapXY(int, int);
	void SetXY(int, int);
	void SetGunMode(int);			// 0->pistol 1->shotgun 2->machineGun 3->sniper
	void SetID(int);

	bool getShootState();
	bool isShow();					//是否顯示
	bool getAlive();					
	bool getDead();
	int getGunMode();
	int getX1();
	int getY1();
	int getX2();
	int getY2();

private:
	
	CMove enemyMove;
	CStand enemyStand;
	CDead enemyDead;
	CMovingBitmap defaultStand;
	int defaultHeight, defaultWidth;
	bool isAlive;
	bool isDead;
	bool isOnBlock;
	bool canShoot;
	bool isMovingLeft, isMovingRight;
	int direction, step;
	int mapX, mapY;
	int x, y;
	int gunMode;						//槍枝種類
	int constDelay, delayCount;			//子彈裝填時間
	int machineGunShootDelay, constMachineGunDelay;		//機槍持續射擊時間

	int enemyID;						//用來處理位置，讓每個敵人不要完全重疊

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
	void OnMouseMove(UINT nFlags, CPoint point);	// 滑鼠滑動

protected:
	void OnMove();
	void OnShow();									// 顯示這個狀態的遊戲畫面

private:

	CMovingBitmap gameUI;					//game介面
	CMovingBitmap btnGo;					//開始按鈕
	CMovingBitmap btnGoHover;				//開始按鈕Hover
	CMovingBitmap hoverEffect;				//Hover特效
	CMovingBitmap btnExit;					//離開按鈕
	CMovingBitmap btnExitHover;				//離開按鈕Hover
	CAnimation manScream;					//尖叫特效
	CAnimation fire1, fire2, fire3, fire4;	//火焰特效
	bool ishoverGo, ishoverExit;
	
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

	CHero hero;					//主角
	CGameMap gameMap;			//地圖
	vector<CEnemy*> vecEnemy;	//敵方軍隊
	CMovingBitmap enemyImg;
	CInteger remainEnemy;		//剩餘敵人
	CMovingBitmap heroImg;
	CInteger heroLife;

	bool canAddEnemy;
	unsigned maxEnemyNumber;		//最大敵人數
	unsigned nowAliveEnemy;			//場上存活敵人數
	unsigned loop;					//for使用
	unsigned seed;
	unsigned come1EnemyDelay;		//1個敵人生成時間
	unsigned const_come1EnemyDelay;
	unsigned come2EnemyDelay;		//2個敵人生成時間
	unsigned const_come2EnemyDelay;
	int mapX, mapY;
	int gunMode;					//敵人槍枝種類

	void enemyProduce(int);			//敵人生成控制
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