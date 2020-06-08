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
	AUDIO_heroJump,					// 0
	AUDIO_enemyDead,				// 1
	AUDIO_BGM_normal				// 2
};

namespace game_framework {

	class CHero;
	class CEnemy;
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
	
	int mistake;			//�j�K�~�t
	int maxSpeed;			//�j�K�̤j�t��
	int mistakeRate;		//�~�t��v
	int getRandom(int);		//�j�K�~�t

};

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
// �o��class���ѭI���a��,�ä��|����(���Ocamera)
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
	void addBullet(int,int,int,int);			//�E���@��Bullet����
	void addEnemyBullet(CEnemy*, int, int);
	void killBullet();							//�ϿE���w�g���`��Bullet����
	void OnMoveBullet();
	void OnShowBullet();
	bool isBulletHit(CEnemy*);		//�I���P�_
	bool isBulletHit(CHero*);
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
// �o��class���ѥi�H����L�ηƹ�������l
// �����N�i�H��g���ۤv���{���F
/////////////////////////////////////////////////////////////////////////////

#pragma region CEraser
class CEraser
{
public:
	CEraser();
	int  GetX1();					// ���l���W�� x �y��
	int  GetY1();					// ���l���W�� y �y��
	int  GetX2();					// ���l�k�U�� x �y��
	int  GetY2();					// ���l�k�U�� y �y��
	void Initialize();				// �]�w���l����l��
	void LoadBitmap();				// ���J�ϧ�
	void OnMove();					// �������l
	void OnShow();					// �N���l�ϧζK��e��
	void SetMovingDown(bool flag);	// �]�w�O�_���b���U����
	void SetMovingLeft(bool flag);	// �]�w�O�_���b��������
	void SetMovingRight(bool flag); // �]�w�O�_���b���k����
	void SetMovingUp(bool flag);	// �]�w�O�_���b���W����
	void SetXY(int nx, int ny);		// �]�w���l���W���y��
protected:
	CAnimation animation;		// ���l���ʵe
	int x, y;					// ���l���W���y��
	bool isMovingDown;			// �O�_���b���U����
	bool isMovingLeft;			// �O�_���b��������
	bool isMovingRight;			// �O�_���b���k����
	bool isMovingUp;			// �O�_���b���W����
};
#pragma endregion

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
	bool isLock;					//������-(�g��or�M��)
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
#pragma region Class����
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
	bool isRising;					//�W��
	bool isFalling;					//�Y��
	bool isMovingDown;				//�U�ۡB�U��
	bool isMovingUp;				//�W��
	bool isMovingLeft;				//����
	bool isMovingRight;				//�k��
	bool isShooting;				//����
	bool isOverlap;			//��ĤH���|
	int direction;					//���⭱�V
	int dir_horizontal;				//�e�@�����������V
	int mapX, mapY;					//�a�Ϫ��y��
	int x, y;						//����b�ù����y��
	int heartX, heartY;				//��Ŧ�_�l�y��
	int defaultW, defaultH;			//���߹Ϥ��e��
	int delayCount,constDelay;
#pragma endregion
	
	void gameMap_OnMove();			//�B�z�a�ϲ���
	void gravity();					//���O
	void ResumeShooting();
	void SetHeart();
	void resetAnimation();

};

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
	void SetDirection(int);
	void SetOnBlock(bool);			//�]�w�O�_���bblock�W�Agravity�ϥ�
	void SetShootDelay(int);
	void SetShootState(bool);
	void SetMapXY(int, int);
	void SetXY(int, int);
	void SetGunMode(int);			// 0->pistol 1->shotgun 2->machineGun 3->sniper
	void SetID(int);

	bool getShootState();
	bool isShow();					//�O�_���
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
	int gunMode;						//�j�K����
	int constDelay, delayCount;			//�l�u�˶�ɶ�
	int machineGunShootDelay, constMachineGunDelay;		//���j����g���ɶ�

	int enemyID;						//�ΨӳB�z��m�A���C�ӼĤH���n�������|

};


/////////////////////////////////////////////////////////////////////////////
// �o��class����¶��骺�y
// �����N�i�H��g���ۤv���{���F
/////////////////////////////////////////////////////////////////////////////

class CBall
{
public:
	CBall();
	bool HitEraser(CEraser *eraser);						// �O�_�I�����l
	bool IsAlive();											// �O�_����
	void LoadBitmap();										// ���J�ϧ�
	void OnMove();											// ����
	void OnShow();											// �N�ϧζK��e��
	void SetXY(int nx, int ny);								// �]�w��ߪ��y��
	void SetIsAlive(bool alive);							// �]�w�O�_����
	void SetDelay(int d);									// �]�w���઺�t��
protected:
	CMovingBitmap bmp;			// �y����
	CMovingBitmap bmp_center;	// ��ߪ���			
	int x, y;					// ��ߪ��y��
	int dx, dy;					// �y�Z����ߪ��첾�q
	int index;					// �y���u���סv�A0-17���0-360��
	int delay_counter;			// �վ����t�ת��p�ƾ�
	int delay;					// ���઺�t��
	bool is_alive;				// �O�_����
private:
	bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// �O�_�I��Ѽƽd�򪺯x��
};

/////////////////////////////////////////////////////////////////////////////
// �o��class���ѷ|�@���u�����y
// �����N�i�H��g���ۤv���{���F
/////////////////////////////////////////////////////////////////////////////

class CBouncingBall
{
public:
	CBouncingBall();
	void LoadBitmap();		// ���J�ϧ�
	void OnMove();			// ����
	void OnShow();			// �N�ϧζK��e��
private:
	int x, y;				// �ϧήy��
	int floor;				// �a�O��Y�y��
	bool rising;			// true��W�ɡBfalse��U��
	int initial_velocity;	// ��l�t��
	int velocity;			// �ثe���t��(�I/��)
	CAnimation animation;	// �Q�ΰʵe�@�ϧ�
};

/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����C���}�Y�e������
// �C��Member function��Implementation���n����
/////////////////////////////////////////////////////////////////////////////

class CGameStateInit : public CGameState {
public:
	CGameStateInit(CGame *g);
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
	CAnimation manScream;					//�y�s�S��
	CAnimation fire1, fire2, fire3, fire4;	//���K�S��
	bool ishoverGo, ishoverExit;
	
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
	CMovingBitmap enemyImg;
	CInteger remainEnemy;		//�Ѿl�ĤH
	CMovingBitmap heroImg;
	CInteger heroLife;

	bool canAddEnemy;
	unsigned maxEnemyNumber;		//�̤j�ĤH��
	unsigned nowAliveEnemy;			//���W�s���ĤH��
	unsigned loop;					//for�ϥ�
	unsigned seed;
	unsigned come1EnemyDelay;		//1�ӼĤH�ͦ��ɶ�
	unsigned const_come1EnemyDelay;
	unsigned come2EnemyDelay;		//2�ӼĤH�ͦ��ɶ�
	unsigned const_come2EnemyDelay;
	int mapX, mapY;
	int gunMode;					//�ĤH�j�K����

	void enemyProduce(int);			//�ĤH�ͦ�����
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