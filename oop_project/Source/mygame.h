/*
 * mygame.h: ���ɮ��x�C��������class��interface
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

enum AUDIO_ID {				// �w�q�U�ح��Ī��s��
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
// �o��class���Ѥl�u
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
	void SetBullet(int, int, int);				//�]�w��m�Τ�V(�D����)
	void SetBullet(int, int, int, int, int);	//�]�w�_�l�y�ФΥؼЮy��(�ĤH��)
	void SetBulletClass(CAnimation*);
	bool isHit(int, int, int, int);

private:
	CAnimation* bullet;
	int x, y;
	int direction;
	int vx, vy;
	bool isAlive;
	
	int mistake;			//�j�K�~�t
	int maxSpeed;			//�j�K�̤j�t��
	int mistakeRate;		//�~�t��v
	int getRandom(int);		//�j�K�~�t

};

/////////////////////////////////////////////////////////////////////////////
// �o��class�B�z�����ʧ@
/////////////////////////////////////////////////////////////////////////////

class CAttack				//�����ʧ@���檺class
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
	int x, y;		//ø�s�y��

};

/////////////////////////////////////////////////////////////////////////////
// �o��class���ѭI���a��
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
	void ClearMapBullet();						//�D���_���A�M��
	void InitialBullet();
	void addBullet(int,int,int,int);			//�E���@��Bullet����
	void addEnemyBullet(CEnemy*, int, int);
	void killBullet();							//�ϿE���w�g���`��Bullet����
	void killBullet(CMidBoss*);
	void OnMoveBullet();
	void OnShowBullet();
	bool isBulletHit(CEnemy*);		//�l�u�g��ĤH
	bool isBulletHit(CHero*);		//�D���Q�g��
	bool isBulletHit(CMidBoss*);	//�l�u�g��pBoss
	bool isBulletHit(CKid*);		//�l�u�g��p��
#pragma endregion

private:
	void SetBlock(int, int, int, int);			//�s��a�ϥi���W����ê��
	
	CAnimation heroBullet;
	CAnimation enemyBullet;
	vector<CBullet*> vCblt;
	vector<CBullet*> vCbltEnemy;
	CAnimation mapBmp;

	int mapX, mapY;						//�a�Ϯy��
	int size;
	int weight, height;
	int map[40][260];
	int bulletNumer;					//�j�K�l�u��
	unsigned loop;
	unsigned maxHeroBullet;				//���W�P�ɯ�s�b�ڤ誺�l�u�W��
	unsigned maxEnemyBullet;			//���W�P�ɯ�s�b�Ĥ誺�l�u�W��

};

/////////////////////////////////////////////////////////////////////////////
// �o��class�B�z�}���ʵe
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
// �o��class�B�z���k����
/////////////////////////////////////////////////////////////////////////////

class CMove
{
public:
	CMove();
	void Initialize();				// �]�w��l��
	void LoadBitmap_MoveL(char*);	// ���J�ϧ�
	void LoadBitmap_MoveR(char*);
	void LoadBitmap_ShootL(char*, char*);
	void LoadBitmap_ShootR(char*);
	void LoadBitmap_KnifeL(char*, char*);
	void LoadBitmap_KnifeR(char*);
	void OnMove(int*, int*);		// ����
	void OnShow();					// �N�ϧζK��e��
	void OnShow_Attack();
	void SetStep(int);				//Set Speed
	void SetMovingDown(bool flag);	// �]�w�O�_���b���U����
	void SetMovingLeft(bool flag);	// �]�w�O�_���b��������
	void SetMovingRight(bool flag); // �]�w�O�_���b���k����
	void SetMovingUp(bool flag);	// �]�w�O�_���b���W����
	void SetRising(bool flag);
	void SetFalling(bool flag);
	void SetDefaultHeight(int);
	void SetShooting(bool flag);
	void SetDirection(int);
	void SetXY(int nx, int ny);		// �]�w���W���y��
	void SetOverlap(bool);
	void SetFallBack(bool);
	void SetLock(bool);
	void resetShootAnimation();

protected:
	bool isMovingDown;				//�U��
	bool isMovingUp;				//�W��
	bool isMovingLeft;				//����
	bool isMovingRight;				//�k��
	bool isRising;					//���b�W�� 
	bool isFalling;
	bool isShooting;				//�g��
	bool isOverlap;					//��ĤH���|
	bool isFallBack;
	bool isLock;
	int defaultHeight;

private:
	int x, y;
	CAnimation CmoveL;				//�ʵe
	CAnimation CmoveR;
	CAttack CMoveShoot;
	int step, ini_step;				//���ʳt��
	int velocity, ini_velocity;		//�t��(�W�ɡB�U��)
	int direction, dir_horizontal;	//�����V�B�e�@�Ӥ�����V

};

/////////////////////////////////////////////////////////////////////////////
// �o��class�B�z����
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
// �o��class�B�z���D���U
/////////////////////////////////////////////////////////////////////////////

class CJump : public CMove			//�u�B�z�W�ɤU��
{
public:
	CJump();
	void Initialize();
	bool OnMove(int*, int*);		//�^��isFall�����A
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
	bool isRising;					//���b�W��
	bool isFalling;					//���b�U��
	bool canFall;
	int x, y;						//�y��
	int velocity, ini_velocity;		//�t�סB��t��
	int mapX, mapY;					//�a�Ϯy��
	int direction, dir_horizontal;	//�����V�B�W�@�Ӥ�����V
	int _size;
	int temp;

};

/////////////////////////////////////////////////////////////////////////////
// �o��class�B�z�U��
/////////////////////////////////////////////////////////////////////////////

class CCrouch : public CMove		//�u�B�z�U�۬����ʧ@
{
public:
	CCrouch();
	void Initialize();
	void OnMove(int,int);		//���ʱ���
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
	CAnimation CMoveL;				//����
	CAnimation CMoveR;				//�k��
	CAnimation CStandL;				//�V����
	CAnimation CStandR;				//�V�k��
	CAttack CcrouchShoot;			//�B�z�g��
	bool isMovingLeft;
	bool isMovingRight;
	int x, y;						//�y��
	int step;
	int velocity, ini_velocity;		//�t�סB��t��
	int direction, dir_horizontal;	//�����V�B�W�@�Ӥ�����V

};

/////////////////////////////////////////////////////////////////////////////
// �o��class�B�z���`
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
// �o��class����Hero����
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
	void SetMovingDown(bool flag);	// �]�w�O�_���b���U
	void SetMovingLeft(bool flag);	// �]�w�O�_���b��������
	void SetMovingRight(bool flag); // �]�w�O�_���b���k����
	void SetMovingUp(bool flag);	// �]�w�O�_���b���D
	void SetShooting(bool flag);	// �]�w�O�_����
	void SetRising(bool flag);
	void SetDirection(int);			// �]�w��V
	void SetFallDownFromBlock(bool);
	void SetOverlap(bool);
	void ResumeDirection();			// �N��V���s�զ^���M�k  
	void SetXY(int, int);			//��KDemo�ϥ�
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
#pragma region Class����
	CDead heroDead;
	CMove heroMove;
	CStand heroStand;
	CJump heroJump;
	CCrouch heroCrouch;				//�U��
	CGameMap *gameMap;				
	CMovingBitmap CHeart;			//��Ŧ�B�z�I
	CMovingBitmap CDefaultStand;	//����ܡB�����ʡA�u�B�z�I��
	CMovingBitmap CDefaultCrouch;	//�P�W
#pragma endregion
	
#pragma region �ܼƫŧi
	bool isDead;
	bool isRising;					//�W��
	bool isFalling;					//�Y��
	bool isMovingDown;				//�U�ۡB�U��
	bool isMovingUp;				//�W��
	bool isMovingLeft;				//����
	bool isMovingRight;				//�k��
	bool isShooting;				//����
	bool isOverlap;					//��ĤH���|
	bool isMapLock;					//���a�Ϩ��b�A�Ω�Boss���d
	int direction;					//���⭱�V
	int dir_horizontal;				//�e�@�����������V
	int mapX, mapY;					//�a�Ϫ��y��
	int x, y;						//����b�ù����y��
	int heartX, heartY;				//��Ŧ�_�l�y��
	int defaultW, defaultH;			//���߹Ϥ��e��
	int delayCount,constDelay;		//machine gun����g���ɶ�
	int deadDelay, constDeadDelay;
	int mapEdge;					//�a�Ϩ��b���
	int heroLife;
#pragma endregion
	
	void gameMap_OnMove();			//�B�z�a�ϲ���
	void gravity();					//���O
	void ResumeShooting();
	void ResumeDead();
	void SetHeart();
	void resetAnimation();

};

/////////////////////////////////////////////////////////////////////////////
// �o��class���ѼĤH����
/////////////////////////////////////////////////////////////////////////////

class CEnemy
{
public:
	CEnemy();
	~CEnemy();
	void Initialize();				//��l��
	void LoadBitmap();				//���J�Ϥ�
	void OnShow();					//��ܼĤH
	void OnMove();					//�ĤH����
	
	void SetAlive(bool);			//�]�w�ͩR(��ܳ]�w)
	void SetDead(bool);				//�]�w�����`���A
	void SetDead(bool,int);			//�]�w���`�Φ��`��V
	void SetDirection(int);			//�]�w���V��V
	void SetOnBlock(bool);			//�]�w�O�_���bblock�W�Agravity�ϥ�
	void SetShootDelay(int);		//�]�w�g������(�u�ĸ˶�ɶ�)
	void SetShootState(bool);		//�]�w�ثe�g�����A
	void SetMapXY(int, int);		//�]�w�a�Ϯy��
	void SetXY(int, int);			//�]�w�ĤH�y��
	void SetGunMode(int);				// 0->pistol 1->shotgun 2->machineGun 3->sniper
	void SetID(int);					//�]�w�ĤH�s��
	void SetFallBack(bool, int);		//�]�w�O�_�M�h
	void SetDestination(int, int, int);	//�]�w�i�J�ت���m

	bool getShootState();				//���o�O�_�i�H�g��(�u�ĸ˶�O�_����)
	bool isShow();						//�O�_���
	bool getAlive();					//���o�s�����A
	bool getDead();						//���o���`���A
	int getGunMode();					//���o�j�K����
	int getX1();						//���oleft
	int getY1();						//get top
	int getX2();						//get right
	int getY2();						//get bottom

private:
	
	CMove enemyMove;					//�B�z����
	CStand enemyStand;					//�B�z����
	CDead enemyDead;					//�B�z�ĤH���`
	CMovingBitmap defaultStand;			//���ߡA����ܥΨӳB�z�I��
	int defaultHeight, defaultWidth;	//��K�ΨӳB�z�I���P�_
	bool isAlive;						//�O�_�s��(�U��������ܡB���F���`)
	bool isDead;						//�O�_���`(���`�ʵe���)
	bool isOnBlock;						//���b��ê���W
	bool canShoot;						//�i�_�}�j
	bool isMovingLeft, isMovingRight;	//�O�_����
	bool isFallBack;					//�O�_�M�h
	bool goDestination;					//�i���ʵe
	int direction, step;				//��V�B���ʳt��
	int desX, desY;						//�i���ؼЦ�m
	int mapX, mapY;						//�����y��
	int x, y;							//�ĤH�y��
	int gunMode;						//�j�K����
	int constDelay, delayCount;			//�l�u�˶�ɶ�
	int machineGunShootDelay, constMachineGunDelay;		//���j����g���ɶ�

	int enemyID;						//�ΨӳB�z��m�A���C�ӼĤH���n�������|
	int enemyPos;						//�i����m

};

/////////////////////////////////////////////////////////////////////////////
// �o��class����Kid
/////////////////////////////////////////////////////////////////////////////

class CKid
{
public:
	CKid();
	~CKid();
	void Initialize();			//��l��
	void LoadBitmap();			//���J�Ϥ�
	void OnMove();				//����
	void OnShow();				//��ܹϤ�
	void SetAlive(bool);		//�]�w�s��
	void SetDead(bool);			//�]�w���`

	int getX1();
	int getX2();
	int getY1();
	int getY2();
	bool getShow();				//���o�O�_���
	bool getDead();				//���o���`
	bool getAlive();			//�P�w�s��

private:
	CMovingBitmap kidDefault;	//���o���e
	CAnimation kidWalk;			//���ʰʧ@
	CAnimation kidDead;			//���`�ʧ@
	bool isDead;				//���`���A
	bool isAlive;				//�s�����A
	int step;					//���ʳt��
	int x, y;					//��e�y��

};

/////////////////////////////////////////////////////////////////////////////
// �o��class���ѤpBoss
/////////////////////////////////////////////////////////////////////////////

class CMidBoss
{
public:
	CMidBoss();
	~CMidBoss();
	void Initialize();			//��l��
	void LoadBitmap();			//���J�Ϥ�
	void OnMove();				//boss move
	bool OnShow();				//boss show

	void AddLife(int);			//deal with boss Life
	void SetStart(bool);		//�pBoss�}�l�ʧ@

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
	CAnimation midBossLaserOn;		//�ন�����Ҧ�
	CAnimation midBossLaserOff;		//��^���ʼҦ�
	CAnimation midBossDead;			//boss dead
	CMovingBitmap laserLightH;		//laser attack horizontal
	CMovingBitmap laserLightV;		//laser attack vertical
	CMovingBitmap midBossLaserHead;	//laser attack part
	CMovingBitmap midBossDefault;	//���T���B�z�Ϥ��I��
	CMovingBitmap caution;			//����ĵ�i

	int delay, const_delay;	//�ʧ@����
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
	bool isStart;			//isBoss Fight start?�i���ʵe
	bool isMovingLeft;		//Boss Move Left
	bool isMovingRight;		//Boss Move Right
	bool isDead;			//Boss dead
	bool isAlive;			//Boss Alive

};

/////////////////////////////////////////////////////////////////////////////
// �o��class����Boss
/////////////////////////////////////////////////////////////////////////////

class CFinalBoss
{
public:
	CFinalBoss();
	~CFinalBoss();
	void Initialize();			//��l��Boss
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
	bool isHandHitGround;				//true->�����I��
	bool showGunExplode;				//show gunExplode animation
	int bossLife, gunLife, handLife;	//part life
	int bulletX, bulletY;				//bullet position
	int bulletStep;						//bullet move speed
	int handX, handY;					//hand position
	int handStepX, handStepY;			//hand move speed
	int handVX, handVY;					//handV position
	int handVStep;						//handV speed
	int x, y;							//boss��m�����q



};

/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����C���}�Y�e������
// �C��Member function��Implementation���n����
/////////////////////////////////////////////////////////////////////////////

class CGameStateInit : public CGameState {
public:
	CGameStateInit(CGame *g);
	~CGameStateInit();
	void OnInit();  								// �C������Ȥιϧγ]�w
	void OnBeginState();							// �]�w�C�������һݪ��ܼ�
	void OnKeyUp(UINT, UINT, UINT); 				// �B�z��LUp���ʧ@
	void OnLButtonDown(UINT nFlags, CPoint point);  // �B�z�ƹ����ʧ@
	void OnMouseMove(UINT nFlags, CPoint point);	// �ƹ��ư�

protected:
	void OnMove();
	void OnShow();									// ��ܳo�Ӫ��A���C���e��

private:

	CMovingBitmap gameUI;					//game����
	CMovingBitmap btnGo;					//�}�l���s
	CMovingBitmap btnGoHover;				//�}�l���sHover
	CMovingBitmap hoverEffect;				//Hover�S��
	CMovingBitmap btnExit;					//���}���s
	CMovingBitmap btnExitHover;				//���}���sHover
	CMovingBitmap btnAbout;					//about���s
	CMovingBitmap btnAboutHover;			//about���sHover
	CMovingBitmap gameAbout;				//about����
	CMovingBitmap btnBack;
	CMovingBitmap btnBackHover;
	CMovingBitmap btnHowToPlay;
	CMovingBitmap btnHowToPlayHover;
	CMovingBitmap gameHowToPlay;			//�C����k����
	CAnimation manScream;					//�y�s�S��
	CAnimation fire1, fire2, fire3, fire4;	//���K�S��
	bool ishoverGo, ishoverExit, ishoverBack;
	bool isSoundShow;
	bool isShowAbout, ishoverAbout;
	bool isShowHow, ishoverHow;
	
};

/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����C�����檫��A�D�n���C���{�����b�o��
// �C��Member function��Implementation���n����
/////////////////////////////////////////////////////////////////////////////

class CGameStateRun : public CGameState {
public:
	CGameStateRun(CGame *g);
	~CGameStateRun();
	void OnBeginState();							// �]�w�C�������һݪ��ܼ�
	void OnInit();  								// �C������Ȥιϧγ]�w
	void OnKeyDown(UINT, UINT, UINT);
	void OnKeyUp(UINT, UINT, UINT);
	void OnLButtonDown(UINT nFlags, CPoint point);  // �B�z�ƹ����ʧ@
	void OnLButtonUp(UINT nFlags, CPoint point);	// �B�z�ƹ����ʧ@
	void OnMouseMove(UINT nFlags, CPoint point);	// �B�z�ƹ����ʧ@ 
	void OnRButtonDown(UINT nFlags, CPoint point);  // �B�z�ƹ����ʧ@
	void OnRButtonUp(UINT nFlags, CPoint point);	// �B�z�ƹ����ʧ@
protected:
	void OnMove();									// ���ʹC������
	void OnShow();									// ��ܳo�Ӫ��A���C���e��
private:

	CHero hero;					//�D��
	CGameMap gameMap;			//�a��
	vector<CEnemy*> vecEnemy;	//�Ĥ�x��
	CMidBoss midBoss;			//midBoss
	CKid kid;					//kid
	CMovie movie;				//movie
	CFinalBoss finalBoss;		//finalBoss

	CMovingBitmap enemyImg;		//�ĤHICON
	CInteger remainEnemy;		//�Ѿl�ĤH
	CMovingBitmap heroImg;		//�D��ICON
	CInteger heroLife;			//�D���Ѿl��q
	CMovingBitmap goL;			//��ܩ���
	CMovingBitmap goR;			//��ܩ��k

	bool isShowKid;					//�}���p�İʵe
	bool showMovie;					//�O�_��ܶ}���ʵe
	bool canAddEnemy;				//�i�_�۰ʥͦ��ĤH
	bool isFallBack;				//���d�����ɡA���ĤH�M�h
	bool isNormalBGMShow;			//decide to show mp3 or not
	bool isBossBGMShow;				//show boss bgm
	unsigned maxEnemyNumber;		//�̤j�ĤH��
	unsigned nowAliveEnemy;			//���W�s���ĤH��
	unsigned nowShowEnemy;			//�M�h�ɥ�
	unsigned loop;					//for�ϥ�
	unsigned seed;					//�üƺؤl
	unsigned come1EnemyDelay;		//1�ӼĤH�ͦ��ɶ�
	unsigned const_come1EnemyDelay;
	unsigned come2EnemyDelay;		//2�ӼĤH�ͦ��ɶ�
	unsigned const_come2EnemyDelay;
	int mapX, mapY;					//�a�Ϯy��
	int gunMode;					//�ĤH�j�K����
	int stage;						//0-�Ĥ@���p�L�B1-�pboss�B2-�ĤG���p�L�B3-�̲�boss
	int delay, const_delay;

	void enemyProduce(int);			//�ĤH�ͦ�����


	const bool test = true;			//���ե�
};

/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����������A(Game Over)
// �C��Member function��Implementation���n����
/////////////////////////////////////////////////////////////////////////////

class CGameStateOver : public CGameState {
public:
	CGameStateOver(CGame *g);
	void OnBeginState();							// �]�w�C�������һݪ��ܼ�
	void OnInit();
protected:
	void OnMove();									// ���ʹC������
	void OnShow();									// ��ܳo�Ӫ��A���C���e��
private:
	int counter;	// �˼Ƥ��p�ƾ�
};

}