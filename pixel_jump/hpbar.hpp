// ---------------------------------------------------------------------------
// includes
extern int maxhealth;
extern int currenthealth;

extern float maxhp;
extern float currenthp;
extern float timedelta;

AEGfxVertexList* mesh_create_square();

AEGfxVertexList* mesh_create_hpbar(float x1, float y1, float x2, float y2, float x3, float y3);

void mesh_square_transform(float posX, float posY, float sizeX, float sizeY, int rotation, AEMtx33& scale, AEMtx33& rotate, AEMtx33& translate, AEMtx33& transform, float r, float g, float b, float e, float a);

int calculateDistance(float src_posX, float src_posY, float dst_posX, float dst_posY, float distance);

void playerhplock();

void hpcheck();

s32 GameOver(); 

s32 WinState();

