//=========================================
//
//�����L���O�̃w�b�_�[
//Author YudaKaito
//
//=========================================
//------------------------------------
//�}�N����`
//------------------------------------
#define MAX_RANK		(5)	//�����N��
#define MAX_RANKSCORE	(8)	//����
#define FILE_NAME	"data/txt/ranking.txt"

//------------------------------------
//�v���g�^�C�v�錾
//------------------------------------
void InitRanking(void);
void UninitRanking(void);
void UpdateRanking(void);
void DrawRanking(void);
void ResetRanking(void);
void SaveRanking(void);
void SetRanking(int nScore);