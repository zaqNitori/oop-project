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
	AUDIO_DING,				// 0
	AUDIO_LAKE,				// 1
	AUDIO_NTUT				// 2
};

namespace game_framework {

/////////////////////////////////////////////////////////////////////////////
// �o��class���ѭI���a��,�ä��|����(���Ocamera)
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
	void SetBlock(int, int, int, int);			//�s��a�ϥi���W����ê��
	CAnimation mapBmp;
	bool isMovingLeft;
	bool isMovingRight;
	int floor, roof;
	int mapX, mapY;				//�a�Ϯy��
	//int map[18][130];			//�a�Ͻs��
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
	int x, y;		//ø�s�y��


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
	void OnMove(int*, int*);		// ����
	void OnShow();					// �N�ϧζK��e��
	void OnShow_Shoot();
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
	bool isfinalBitmap(int);

protected:
	bool isMovingDown;				//�U��
	bool isMovingUp;				//�W��
	bool isMovingLeft;				//����
	bool isMovingRight;				//�k��
	bool isRising;					//���b�W�� 
	bool isFalling;
	bool isShooting;				//�g��
	int defaultHeight;

private:
	int x, y;
	CAnimation CmoveL;				//�ʵe
	CAnimation CmoveR;
	CShoot CMoveShoot;
	int floor;						//�a�O����
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
	bool isRising;					//���b�W��
	bool isFalling;					//���b�U��
	int x, y;						//�y��
	int floor;						//�̤U��a�O
	int velocity, ini_velocity;		//�t�סB��t��
	int mapX, mapY;					//�a�Ϯy��
	int direction, dir_horizontal;	//�����V�B�W�@�Ӥ�����V
	int _size;

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
	void OnShow_Move();
	void OnShow_Stand();
	void OnShow_Shoot();
	void SetDirection(int);
	bool isfinalBitmap(int);

private:
	CAnimation CMoveL;				//����
	CAnimation CMoveR;				//�k��
	CAnimation CStandL;				//�V����
	CAnimation CStandR;				//�V�k��
	CShoot CcrouchShoot;			//�B�z�g��
	bool isMovingLeft;
	bool isMovingRight;
	int x, y;						//�y��
	int floor;						//�̤U��a�O
	int step;
	int velocity, ini_velocity;		//�t�סB��t��
	int direction, dir_horizontal;	//�����V�B�W�@�Ӥ�����V

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
	void SetGameMap(CGameMap*);
	void SetMovingDown(bool flag);	// �]�w�O�_���b���U
	void SetMovingLeft(bool flag);	// �]�w�O�_���b��������
	void SetMovingRight(bool flag); // �]�w�O�_���b���k����
	void SetMovingUp(bool flag);	// �]�w�O�_���b���D
	void SetShooting(bool flag);	// �]�w�O�_����
	void SetRising(bool flag);
	void SetDirection(int);			// �]�w��V
	void ResumeDirection();			// �N��V���s�զ^���M�k
	//void SetFalling(bool flag);
	void getXY();
	void SetXY(int, int);	//��KDemo�ϥ�

private:
#pragma region Class����
	CMove heroMove;
	CStand heroStand;
	CJump heroJump;
	CCrouch heroCrouch;				//�U��
	CGameMap *gameMap;				
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
	int direction;					//���⭱�V
	int dir_horizontal;				//�e�@�����������V
	int floor;						//�a�O
	int mapX, mapY;					//�a�Ϫ��y��
	int x, y;						//����b�ù����y��
	int defaultW, defaultH;			//���O�Ϥ��e��
#pragma endregion
	
	void gameMap_OnMove();			//�B�z�a�ϲ���

};

/////////////////////////////////////////////////////////////////////////////
// �o��class������v������A�O�u����ܥX�ثe�a��
// 
/////////////////////////////////////////////////////////////////////////////

class CGameCamera
{
public:
	CGameCamera();
	~CGameCamera();
	void LoadBitmap();

private:
	int map[16][12];		//�H40px���@��,��v���|���16*12�������a��
	int nowX, nowY;			//��v�����W������l

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
protected:
	void OnShow();									// ��ܳo�Ӫ��A���C���e��
private:
	CMovingBitmap logo;								// csie��logo
	
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
	const int		NUMBALLS;	// �y���`��
	//CMovingBitmap	background;	// �I����
	CMovingBitmap	help;		// ������
	CBall			*ball;		// �y���}�C
	CMovingBitmap	corner;		// ������
	CEraser			eraser;		// ��l
	CInteger		hits_left;	// �ѤU��������
	CBouncingBall   bball;		// ���мu�����y
	
	CHero hero;
	CGameMap gameMap;
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