#include <game.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cJSON/cJSON.h>
#include "stage_loader.h"
#include "sfx.h"

void Update(void);
static void TestCollisions(void);
void func_8018A7AC(void);
void UpdateStageEntities(void);
static void MyInitRoomEntities(s32 objLayoutId);

static RoomHeader g_Rooms[2] = {0};

// TODO populate from assets/st/wrp/D_80186D88.animset.json
static SpriteParts* D_80186D88[] = {NULL, NULL, NULL, NULL};
static s16** g_SpriteBanks[] = {
    /* 0x040 */ 0x00000000,
    /* 0x044 */ D_80186D88,
    /* 0x048 */ 0x00000000,
    /* 0x04C */ 0x00000000,
    /* 0x050 */ 0x00000000,
    /* 0x054 */ 0x00000000,
    /* 0x058 */ 0x00000000,
    /* 0x05C */ 0x00000000,
    /* 0x060 */ 0x00000000,
    /* 0x064 */ 0x00000000,
    /* 0x068 */ 0x00000000,
    /* 0x06C */ 0x00000000,
    /* 0x070 */ 0x00000000,
    /* 0x074 */ 0x00000000,
    /* 0x078 */ 0x00000000,
    /* 0x07C */ 0x00000000,
    /* 0x080 */ 0x00000000,
    /* 0x084 */ 0x00000000,
    /* 0x088 */ 0x00000000,
    /* 0x08C */ 0x00000000,
    /* 0x090 */ 0x00000000,
    /* 0x094 */ 0x00000000,
    /* 0x098 */ 0x00000000,
    /* 0x09C */ 0x00000000,
};

static u8 D_80181D08[256];
static void* D_801800A0[] = {
    /* 0x0A0 */ (void*)0x00000005,
    /* 0x0A4 */ (void*)0x00002000,
    /* 0x0A8 */ (void*)0x00000010,
    /* 0x0AC */ (void*)D_80181D08,
    /* 0x0B0 */ (void*)0xFFFFFFFF,
};
static void* g_Cluts[] = {
    /* 0x0B4 */ D_801800A0,
};

extern void* WRP_g_EntityGfxs[];

// DATA SECTION START
u32 D_80186EC0[] = {0x10101504, 0x0000FF10};
u32 D_80186EC8[] = {0x10101204, 0x0000FF10};
u32 D_80186ED0[] = {0x10101104, 0x0000FF10};
u32 D_80186ED8[] = {0x10101704, 0x000000FF};
u32 D_80186EE0[] = {0x10101404, 0x000000FF};
u32 D_80186EE8[] = {0x10151204, 0x000000FF};
u32 D_80186EF0[] = {0x10101104, 0x000000FF};
u32 D_80186EF8[] = {0xFF101504, 0x00000000};
u32 D_80186F00[] = {0xFF151204, 0x00000000};
u32 D_80186F08[] = {0x00FF1104, 0x00000000};
SVECTOR g_UnkPrimHelperRot = {0, 0, 0};

// DATA SECTION END

// BSS SECTION START
s32 D_80193AA0 = 0;
s32 D_80193AA4 = 0;
s32 D_80193AA8 = 0;
s32 D_80193AAC = 0;
LayoutEntity* D_80193AB0 = NULL;
u16* D_80193AB4 = NULL;
u8 D_80193AB8 = 0;
u8 D_80193ABC = 0;
u16 g_ItemIconSlots[32] = {0};
// BSS SECTION END

static Overlay g_StageDesc = {
    Update,
    TestCollisions,
    func_8018A7AC,
    MyInitRoomEntities,
    g_Rooms,
    g_SpriteBanks,
    g_Cluts,
    NULL,
    NULL,
    WRP_g_EntityGfxs,
    UpdateStageEntities,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
};

LayoutEntity* g_Stub_80181228;
LayoutEntity* g_Stub_80181250;
LayoutEntity* g_Stub_801812A0;
LayoutEntity* g_Stub_801812C8;
LayoutEntity* g_Stub_80181278;
LayoutEntity* g_Stub_801812F0;
LayoutEntity* g_Stub_80181304;

LayoutEntity* g_Stub_80181324;
LayoutEntity* g_Stub_8018134C;
LayoutEntity* g_Stub_80181374;
LayoutEntity* g_Stub_8018139C;
LayoutEntity* g_Stub_801813C4;
LayoutEntity* g_Stub_801813EC;
LayoutEntity* g_Stub_80181400;

extern Overlay g_WRP_Stage;
extern LayoutEntity* g_pStObjLayout[];
extern LayoutEntity* D_80180310[];
void InitStageWrp(Overlay* o) {
    LoadReset();

    FILE* f = fopen("assets/st/wrp/D_80181D08.dec", "rb");
    if (f) {
        fseek(f, 0, SEEK_END);
        size_t len = ftell(f);
        fseek(f, 0, SEEK_SET);
        fread(D_80181D08, len, 1, f);
        fclose(f);
    }

    g_Rooms[0].left = 0;
    g_Rooms[0].top = 0;
    g_Rooms[0].right = 0x3F;
    g_Rooms[0].bottom = 0x3F;
    g_Rooms[0].load.tileLayoutId = 0;
    g_Rooms[0].load.tilesetId = 0;
    g_Rooms[0].load.objGfxId = 0;
    g_Rooms[0].load.objLayoutId = 1;
    g_Rooms[1].left = 0x40;

    g_Stub_80181228 = LoadObjLayout("assets/st/wrp/D_80181228.layoutobj.json");
    g_Stub_80181250 = LoadObjLayout("assets/st/wrp/D_80181250.layoutobj.json");
    g_Stub_801812A0 = LoadObjLayout("assets/st/wrp/D_801812A0.layoutobj.json");
    g_Stub_801812C8 = LoadObjLayout("assets/st/wrp/D_801812C8.layoutobj.json");
    g_Stub_80181278 = LoadObjLayout("assets/st/wrp/D_80181278.layoutobj.json");
    g_Stub_801812F0 = LoadObjLayout("assets/st/wrp/D_801812F0.layoutobj.json");
    g_Stub_80181304 = LoadObjLayout("assets/st/wrp/D_80181304.layoutobj.json");

    g_Stub_80181324 = LoadObjLayout("assets/st/wrp/D_80181324.layoutobj.json");
    g_Stub_8018134C = LoadObjLayout("assets/st/wrp/D_8018134C.layoutobj.json");
    g_Stub_80181374 = LoadObjLayout("assets/st/wrp/D_80181374.layoutobj.json");
    g_Stub_8018139C = LoadObjLayout("assets/st/wrp/D_8018139C.layoutobj.json");
    g_Stub_801813C4 = LoadObjLayout("assets/st/wrp/D_801813C4.layoutobj.json");
    g_Stub_801813EC = LoadObjLayout("assets/st/wrp/D_801813EC.layoutobj.json");
    g_Stub_80181400 = LoadObjLayout("assets/st/wrp/D_80181400.layoutobj.json");

    int layoutId = 0;
    g_pStObjLayout[layoutId++] = g_Stub_801812F0;
    g_pStObjLayout[layoutId++] = g_Stub_80181228;
    g_pStObjLayout[layoutId++] = g_Stub_80181250;
    g_pStObjLayout[layoutId++] = g_Stub_801812A0;
    g_pStObjLayout[layoutId++] = g_Stub_801812C8;
    g_pStObjLayout[layoutId++] = g_Stub_80181278;
    g_pStObjLayout[layoutId++] = g_Stub_801812F0;
    g_pStObjLayout[layoutId++] = g_Stub_801812F0;
    g_pStObjLayout[layoutId++] = g_Stub_801812F0;
    g_pStObjLayout[layoutId++] = g_Stub_801812F0;
    g_pStObjLayout[layoutId++] = g_Stub_801812F0;
    g_pStObjLayout[layoutId++] = g_Stub_801812F0;
    g_pStObjLayout[layoutId++] = g_Stub_801812F0;
    g_pStObjLayout[layoutId++] = g_Stub_801812F0;
    g_pStObjLayout[layoutId++] = g_Stub_801812F0;
    g_pStObjLayout[layoutId++] = g_Stub_801812F0;
    g_pStObjLayout[layoutId++] = g_Stub_801812F0;
    g_pStObjLayout[layoutId++] = g_Stub_801812F0;
    g_pStObjLayout[layoutId++] = g_Stub_801812F0;
    g_pStObjLayout[layoutId++] = g_Stub_801812F0;
    g_pStObjLayout[layoutId++] = g_Stub_801812F0;
    g_pStObjLayout[layoutId++] = g_Stub_801812F0;
    g_pStObjLayout[layoutId++] = g_Stub_801812F0;
    g_pStObjLayout[layoutId++] = g_Stub_801812F0;
    g_pStObjLayout[layoutId++] = g_Stub_801812F0;
    g_pStObjLayout[layoutId++] = g_Stub_801812F0;
    g_pStObjLayout[layoutId++] = g_Stub_801812F0;
    g_pStObjLayout[layoutId++] = g_Stub_801812F0;
    g_pStObjLayout[layoutId++] = g_Stub_801812F0;
    g_pStObjLayout[layoutId++] = g_Stub_801812F0;
    g_pStObjLayout[layoutId++] = g_Stub_801812F0;
    g_pStObjLayout[layoutId++] = g_Stub_801812F0;
    g_pStObjLayout[layoutId++] = g_Stub_801812F0;
    g_pStObjLayout[layoutId++] = g_Stub_801812F0;
    g_pStObjLayout[layoutId++] = g_Stub_801812F0;
    g_pStObjLayout[layoutId++] = g_Stub_801812F0;
    g_pStObjLayout[layoutId++] = g_Stub_801812F0;
    g_pStObjLayout[layoutId++] = g_Stub_801812F0;
    g_pStObjLayout[layoutId++] = g_Stub_801812F0;
    g_pStObjLayout[layoutId++] = g_Stub_801812F0;
    g_pStObjLayout[layoutId++] = g_Stub_801812F0;
    g_pStObjLayout[layoutId++] = g_Stub_801812F0;
    g_pStObjLayout[layoutId++] = g_Stub_801812F0;
    g_pStObjLayout[layoutId++] = g_Stub_801812F0;
    g_pStObjLayout[layoutId++] = g_Stub_801812F0;
    g_pStObjLayout[layoutId++] = g_Stub_801812F0;
    g_pStObjLayout[layoutId++] = g_Stub_801812F0;
    g_pStObjLayout[layoutId++] = g_Stub_801812F0;
    g_pStObjLayout[layoutId++] = g_Stub_80181304;
    g_pStObjLayout[layoutId++] = g_Stub_80181304;
    g_pStObjLayout[layoutId++] = g_Stub_80181304;
    g_pStObjLayout[layoutId++] = g_Stub_80181304;
    g_pStObjLayout[layoutId++] = g_Stub_80181304;

    layoutId = 0;
    D_80180310[layoutId++] = g_Stub_801813EC;
    D_80180310[layoutId++] = g_Stub_80181324;
    D_80180310[layoutId++] = g_Stub_8018134C;
    D_80180310[layoutId++] = g_Stub_8018139C;
    D_80180310[layoutId++] = g_Stub_801813C4;
    D_80180310[layoutId++] = g_Stub_80181374;
    D_80180310[layoutId++] = g_Stub_801813EC;
    D_80180310[layoutId++] = g_Stub_801813EC;
    D_80180310[layoutId++] = g_Stub_801813EC;
    D_80180310[layoutId++] = g_Stub_801813EC;
    D_80180310[layoutId++] = g_Stub_801813EC;
    D_80180310[layoutId++] = g_Stub_801813EC;
    D_80180310[layoutId++] = g_Stub_801813EC;
    D_80180310[layoutId++] = g_Stub_801813EC;
    D_80180310[layoutId++] = g_Stub_801813EC;
    D_80180310[layoutId++] = g_Stub_801813EC;
    D_80180310[layoutId++] = g_Stub_801813EC;
    D_80180310[layoutId++] = g_Stub_801813EC;
    D_80180310[layoutId++] = g_Stub_801813EC;
    D_80180310[layoutId++] = g_Stub_801813EC;
    D_80180310[layoutId++] = g_Stub_801813EC;
    D_80180310[layoutId++] = g_Stub_801813EC;
    D_80180310[layoutId++] = g_Stub_801813EC;
    D_80180310[layoutId++] = g_Stub_801813EC;
    D_80180310[layoutId++] = g_Stub_801813EC;
    D_80180310[layoutId++] = g_Stub_801813EC;
    D_80180310[layoutId++] = g_Stub_801813EC;
    D_80180310[layoutId++] = g_Stub_801813EC;
    D_80180310[layoutId++] = g_Stub_801813EC;
    D_80180310[layoutId++] = g_Stub_801813EC;
    D_80180310[layoutId++] = g_Stub_801813EC;
    D_80180310[layoutId++] = g_Stub_801813EC;
    D_80180310[layoutId++] = g_Stub_801813EC;
    D_80180310[layoutId++] = g_Stub_801813EC;
    D_80180310[layoutId++] = g_Stub_801813EC;
    D_80180310[layoutId++] = g_Stub_801813EC;
    D_80180310[layoutId++] = g_Stub_801813EC;
    D_80180310[layoutId++] = g_Stub_801813EC;
    D_80180310[layoutId++] = g_Stub_801813EC;
    D_80180310[layoutId++] = g_Stub_801813EC;
    D_80180310[layoutId++] = g_Stub_801813EC;
    D_80180310[layoutId++] = g_Stub_801813EC;
    D_80180310[layoutId++] = g_Stub_801813EC;
    D_80180310[layoutId++] = g_Stub_801813EC;
    D_80180310[layoutId++] = g_Stub_801813EC;
    D_80180310[layoutId++] = g_Stub_801813EC;
    D_80180310[layoutId++] = g_Stub_801813EC;
    D_80180310[layoutId++] = g_Stub_801813EC;
    D_80180310[layoutId++] = g_Stub_80181400;
    D_80180310[layoutId++] = g_Stub_80181400;
    D_80180310[layoutId++] = g_Stub_80181400;
    D_80180310[layoutId++] = g_Stub_80181400;
    D_80180310[layoutId++] = g_Stub_80181400;

    g_SpriteBanks[1] = LoadSpriteParts("assets/st/wrp/D_80186D88.animset.json");

    g_StageDesc.tileLayers = LoadRooms("assets/st/wrp/rooms.layers.json");
    memcpy(o, &g_StageDesc, sizeof(Overlay));
}

static void TestCollisions(void) { NOT_IMPLEMENTED; }

void EntityNumericDamage(Entity* self) { NOT_IMPLEMENTED; }
void EntityRedDoor(Entity* self) { NOT_IMPLEMENTED; }

void SetGameState(GameState gameState);
void PlaySfx(s32 sfxId);
void InitRoomEntities(s32 objLayoutId);

static void MyInitRoomEntities(s32 objLayoutId) {
    if (g_StageId == STAGE_SEL) {
        SetGameState(Game_NowLoading);
        g_GameStep = NowLoading_2;
        g_StageId = STAGE_WRP;
        return;
    }

    INFOF("Stage: %02X, objLayoutId: %i", g_StageId, objLayoutId);
    InitRoomEntities(objLayoutId);
}
