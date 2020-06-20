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
	enemyScream,
	fire,
	Movie_moon,
	Movie_ufoCrash,
	Movie_ufoExplode,
	Movie_enemyShow,
	Movie_FBItalk,
	Movie_enemyGone,
	Movie_HeroBlink,
	Movie_KidWavehand,
	AUDIO_heroJump,					
	AUDIO_enemyDead,				
	AUDIO_normal_BGM,
	bossBGM,
	midBoss_Laser,
	midBoss_Stand
};

namespace game_framework {

	class CHero;
	class CEnemy;
	class CMidBoss;
	class CKid;
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
	void SetBullet(int, int, int);				//設定位置及方向(主角用)
	void SetBullet(int, int, int, int, int);	//設定起始座標及目標座標(敵人用)
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

/////////////////////////////////////////////////////////////////////////////
// 這個class處理攻擊動作
/////////////////////////////////////////////////////////////////////////////

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
// 這個class提供背景地圖
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
	void ClearMapBullet();						//主角復活，清場
	void InitialBullet();
	void addBullet(int,int,int,int);			//激活一個Bullet物件
	void addEnemyBullet(CEnemy*, int, int);
	void killBullet();							//反激活已經死亡的Bullet物件
	void killBullet(CMidBoss*);
	void OnMoveBullet();
	void OnShowBullet();
	bool isBulletHit(CEnemy*);		//子彈射到敵人
	bool isBulletHit(CHero*);		//主角被射到
	bool isBulletHit(CMidBoss*);	//子彈射到小Boss
	bool isBulletHit(CKid*);		//子彈射到小孩
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
// 這個class處理開場動畫
/////////////////////////////////////////////////////////////////////////////

class CMovie
{
public:
	CMovie();
	~CMovie();
	void Initialize();
	void LoadBitmap();
	void OnMove();
	void resetAnimation();
	bool OnShow();

private:

	CMovingBitmap bg1;					//background1
	CMovingBitmap bg1_1;				//combine with background1
	CMovingBitmap enemyShow;			//enemy show
	CMovingBitmap enemyGone;			//enemy gone
	CMovingBitmap bg2;					//background2
	CMovingBitmap bg3;					//background3
	CMovingBitmap spaceShip;			//spaceship img
	CAnimation spaceShipCrash;			//crash action
	CAnimation spaceShipExplode;		//boom action
	CAnimation heroWake;				//wake up action
	CAnimation heroWake2;				//wake up second
	CAnimation kidWave;					//wave hand action

	bool isShowMoon;
	bool isShowUFOCrash;
	bool isShowUFOExplode;
	bool isShowFire;
	bool isShowFBItalk;
	bool isShowEnemyShow;
	bool isShowEnemyGone;
	bool isShowKidWave;
	bool isShowHeroBlink;
	bool showMovie;
	bool changeBG;
	bool bg1CanGoNextStage;
	const int const_delay = 30;
	int delay;
	int x, y;
	int bg1Stage;
	int nowBG;							//toShow which background


};

/////////////////////////////////////////////////////////////////////////////
// 這個class處理左右移動
/////////////////////////////////////////////////////////////////////////////

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
	void SetStep(int);				//Set Speed
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
	void SetFallBack(bool);
	void SetLock(bool);
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
	bool isFallBack;
	bool isLock;
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

/////////////////////////////////////////////////////////////////////////////
// 這個class處理站立
/////////////////////////////////////////////////////////////////////////////

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

/////////////////////////////////////////////////////////////////////////////
// 這個class處理跳躍落下
/////////////////////////////////////////////////////////////////////////////

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

/////////////////////////////////////////////////////////////////////////////
// 這個class處理下蹲
/////////////////////////////////////////////////////////////////////////////

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

/////////////////////////////////////////////////////////////////////////////
// 這個class處理死亡
/////////////////////////////////////////////////////////////////////////////

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
	void SetLock(bool);
	void SetMapXY(int, int);
	void AddLife(int);
#pragma endregion

#pragma region Getstate
	int getLife();
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
	bool getDead();					//is hero Dead Now
	bool isNowRising();				//get isRising?
	bool isOverlapEnemy(CEnemy*);		//get is Hero and Enemy overlap
	bool getOverlap();
	bool getShooting();
	bool canDead;
#pragma endregion


private:
#pragma region Class物件
	CDead heroDead;
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
	bool isDead;
	bool isRising;					//上升
	bool isFalling;					//墜落
	bool isMovingDown;				//下蹲、下看
	bool isMovingUp;				//上看
	bool isMovingLeft;				//左動
	bool isMovingRight;				//右動
	bool isShooting;				//攻擊
	bool isOverlap;					//跟敵人重疊
	bool isMapLock;					//鎖住地圖卷軸，用於Boss關卡
	int direction;					//角色面向
	int dir_horizontal;				//前一次的水平面向
	int mapX, mapY;					//地圖的座標
	int x, y;						//角色在螢幕的座標
	int heartX, heartY;				//心臟起始座標
	int defaultW, defaultH;			//站立圖片寬高
	int delayCount,constDelay;		//machine gun持續射擊時間
	int deadDelay, constDeadDelay;
	int mapEdge;					//地圖卷軸邊界
	int heroLife;
#pragma endregion
	
	void gameMap_OnMove();			//處理地圖移動
	void gravity();					//重力
	void ResumeShooting();
	void ResumeDead();
	void SetHeart();
	void resetAnimation();

};

/////////////////////////////////////////////////////////////////////////////
// 這個class提供敵人物件
/////////////////////////////////////////////////////////////////////////////

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
	void SetDirection(int);			//設定面向方向
	void SetOnBlock(bool);			//設定是否站在block上，gravity使用
	void SetShootDelay(int);		//設定射擊延遲(彈藥裝填時間)
	void SetShootState(bool);		//設定目前射擊狀態
	void SetMapXY(int, int);		//設定地圖座標
	void SetXY(int, int);			//設定敵人座標
	void SetGunMode(int);				// 0->pistol 1->shotgun 2->machineGun 3->sniper
	void SetID(int);					//設定敵人編號
	void SetFallBack(bool, int);		//設定是否撤退
	void SetDestination(int, int, int);	//設定進入目的位置

	bool getShootState();				//取得是否可以射擊(彈藥裝填是否完成)
	bool isShow();						//是否顯示
	bool getAlive();					//取得存活狀態
	bool getDead();						//取得死亡狀態
	int getGunMode();					//取得槍枝種類
	int getX1();						//取得left
	int getY1();						//get top
	int getX2();						//get right
	int getY2();						//get bottom

private:
	
	CMove enemyMove;					//處理移動
	CStand enemyStand;					//處理站立
	CDead enemyDead;					//處理敵人死亡
	CMovingBitmap defaultStand;			//站立，不顯示用來處理碰撞
	int defaultHeight, defaultWidth;	//方便用來處理碰撞判斷
	bool isAlive;						//是否存活(各式移動顯示、除了死亡)
	bool isDead;						//是否死亡(死亡動畫顯示)
	bool isOnBlock;						//站在障礙物上
	bool canShoot;						//可否開槍
	bool isMovingLeft, isMovingRight;	//是否移動
	bool isFallBack;					//是否撤退
	bool goDestination;					//進場動畫
	int direction, step;				//方向、移動速度
	int desX, desY;						//進場目標位置
	int mapX, mapY;						//場景座標
	int x, y;							//敵人座標
	int gunMode;						//槍枝種類
	int constDelay, delayCount;			//子彈裝填時間
	int machineGunShootDelay, constMachineGunDelay;		//機槍持續射擊時間

	int enemyID;						//用來處理位置，讓每個敵人不要完全重疊
	int enemyPos;						//進場位置

};

/////////////////////////////////////////////////////////////////////////////
// 這個class提供Kid
/////////////////////////////////////////////////////////////////////////////

class CKid
{
public:
	CKid();
	~CKid();
	void Initialize();			//初始化
	void LoadBitmap();			//載入圖片
	void OnMove();				//移動
	void OnShow();				//顯示圖片
	void SetAlive(bool);		//設定存活
	void SetDead(bool);			//設定死亡

	int getX1();
	int getX2();
	int getY1();
	int getY2();
	bool getShow();				//取得是否顯示
	bool getDead();				//取得死亡
	bool getAlive();			//與德存活

private:
	CMovingBitmap kidDefault;	//取得長寬
	CAnimation kidWalk;			//移動動作
	CAnimation kidDead;			//死亡動作
	bool isDead;				//死亡狀態
	bool isAlive;				//存活狀態
	int step;					//移動速度
	int x, y;					//當前座標

};

/////////////////////////////////////////////////////////////////////////////
// 這個class提供小Boss
/////////////////////////////////////////////////////////////////////////////

class CMidBoss
{
public:
	CMidBoss();
	~CMidBoss();
	void Initialize();			//初始化
	void LoadBitmap();			//載入圖片
	void OnMove();				//boss move
	bool OnShow();				//boss show

	void AddLife(int);			//deal with boss Life
	void SetStart(bool);		//小Boss開始動作

	int getLife();				//get current boss life
	int getX1();				//get left
	int getX2();				//get right
	int getY1();				//get top
	int getY2();				//get bottom
	int getLaserVX1();			//get laser left
	int getLaserVX2();			//get laser right
	int getLaserVY1();			//get laser top
	int getLaserVY2();			//get laser bottom
	bool getShow();				//if boss can show? (isDead||isAlive)
	bool getDead();				//is boss dead
	bool getAlive();			//is boss alive
	bool isHitHero(CHero*);		//damaged hero?

private:

	CMove midBossMove;				//boss move
	CStand midBossStand;			//boss stand
	CAnimation midBossLaserOn;		//轉成攻擊模式
	CAnimation midBossLaserOff;		//轉回移動模式
	CAnimation midBossDead;			//boss dead
	CMovingBitmap laserLightH;		//laser attack horizontal
	CMovingBitmap laserLightV;		//laser attack vertical
	CMovingBitmap midBossLaserHead;	//laser attack part
	CMovingBitmap midBossDefault;	//更精確的處理圖片碰撞
	CMovingBitmap caution;			//攻擊警告

	int delay, const_delay;	//動作延遲
	int defaultWidth, defaultHeight;
	int x, y;				//Boss position
	int bossLife;			//Boss life
	int step;				//Boss movement speed
	int midBossAction;		//boss action change
	int laserX, laserY;
	int randomX;			//bosss attack from top random X position
	bool isGetRandomX;		//boss attack from top
	bool isAttack;			//boss attack
	bool isStand;			//bos stand
	bool isStart;			//isBoss Fight start?進場動畫
	bool isMovingLeft;		//Boss Move Left
	bool isMovingRight;		//Boss Move Right
	bool isDead;			//Boss dead
	bool isAlive;			//Boss Alive

};

/////////////////////////////////////////////////////////////////////////////
// 這個class提供Boss
/////////////////////////////////////////////////////////////////////////////

class CFinalBoss
{
public:
	CFinalBoss();
	~CFinalBoss();
	void Initialize();			//初始化Boss
	void LoadBitmap();			//load picture
	void OnMove();				//boss move
	void OnShow();				//boss show
	void SetStart(bool);		//start boss

	bool isHitHero(CHero*);		//damaged Hero?


private:

#pragma region bodyPart
	CAnimation bossFoot;			//boss foot
	CAnimation bossHand;			//boss hand
	CMovingBitmap bossGun;			//boss Gun
	CMovingBitmap bossBody;
	CMovingBitmap bossHead;			//boss head when life >= 1/2
	CMovingBitmap bossHead2;		//boss head when life < 1/2  
	CMovingBitmap bossHandV;
#pragma endregion

#pragma region bullet
	CAnimation gunBullet;			//bullet for gun
	CAnimation gunBulletExplode;	//when bullet hit hero
	CMovingBitmap handBullet;		//bullet for hand
	CMovingBitmap caution;
#pragma endregion

	bool isStart;						//show movie
	bool isHandHitGround;				//true->取消碰撞
	bool showGunExplode;				//show gunExplode animation
	int bossLife, gunLife, handLife;	//part life
	int bulletX, bulletY;				//bullet position
	int bulletStep;						//bullet move speed
	int handX, handY;					//hand position
	int handStepX, handStepY;			//hand move speed
	int handVX, handVY;					//handV position
	int handVStep;						//handV speed
	int x, y;							//boss位置偏移量



};

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲開頭畫面物件
// 每個Member function的Implementation都要弄懂
/////////////////////////////////////////////////////////////////////////////

class CGameStateInit : public CGameState {
public:
	CGameStateInit(CGame *g);
	~CGameStateInit();
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
	CMovingBitmap btnAbout;					//about按鈕
	CMovingBitmap btnAboutHover;			//about按鈕Hover
	CMovingBitmap gameAbout;				//about介面
	CMovingBitmap btnBack;
	CMovingBitmap btnBackHover;
	CMovingBitmap btnHowToPlay;
	CMovingBitmap btnHowToPlayHover;
	CMovingBitmap gameHowToPlay;			//遊玩方法介面
	CAnimation manScream;					//尖叫特效
	CAnimation fire1, fire2, fire3, fire4;	//火焰特效
	bool ishoverGo, ishoverExit, ishoverBack;
	bool isSoundShow;
	bool isShowAbout, ishoverAbout;
	bool isShowHow, ishoverHow;
	
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
	CMidBoss midBoss;			//midBoss
	CKid kid;					//kid
	CMovie movie;				//movie
	CFinalBoss finalBoss;		//finalBoss

	CMovingBitmap enemyImg;		//敵人ICON
	CInteger remainEnemy;		//剩餘敵人
	CMovingBitmap heroImg;		//主角ICON
	CInteger heroLife;			//主角剩餘血量
	CMovingBitmap goL;			//顯示往左
	CMovingBitmap goR;			//顯示往右

	bool isShowKid;					//開場小孩動畫
	bool showMovie;					//是否顯示開場動畫
	bool canAddEnemy;				//可否自動生成敵人
	bool isFallBack;				//關卡切換時，讓敵人撤退
	bool isNormalBGMShow;			//decide to show mp3 or not
	bool isBossBGMShow;				//show boss bgm
	unsigned maxEnemyNumber;		//最大敵人數
	unsigned nowAliveEnemy;			//場上存活敵人數
	unsigned nowShowEnemy;			//撤退時用
	unsigned loop;					//for使用
	unsigned seed;					//亂數種子
	unsigned come1EnemyDelay;		//1個敵人生成時間
	unsigned const_come1EnemyDelay;
	unsigned come2EnemyDelay;		//2個敵人生成時間
	unsigned const_come2EnemyDelay;
	int mapX, mapY;					//地圖座標
	int gunMode;					//敵人槍枝種類
	int stage;						//0-第一關小兵、1-小boss、2-第二關小兵、3-最終boss
	int delay, const_delay;

	void enemyProduce(int);			//敵人生成控制


	const bool test = true;			//測試用
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