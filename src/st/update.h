#include <stage.h>

void Update(void) {
    s16 i;
    Entity* entity;
    s32* unk;

    for (i = 0; i < LEN(g_ItemIconSlots); i++) {
        if (g_ItemIconSlots[i]) {
            g_ItemIconSlots[i]--;
        }
    }

    unk = &g_BottomCornerTextTimer;
    if (*unk) {
        if (!--*unk) {
            g_api.FreePrimitives(g_BottomCornerTextPrims);
        }
    }

    for (entity = &g_Entities[STAGE_ENTITY_START];
         entity < &g_Entities[TOTAL_ENTITY_COUNT]; entity++) {
        if (!entity->pfnUpdate)
            continue;

        if (entity->step) {
            s32 flags = entity->flags;
            if (flags & FLAG_DESTROY_IF_OUT_OF_CAMERA) {
                s16 posX = i = entity->posX.i.hi;
                s16 posY = entity->posY.i.hi;
                if (flags & FLAG_DESTROY_IF_BARELY_OUT_OF_CAMERA) {
                    if (posX < -64 || posX > 320 || posY < -64 || posY > 288) {
                        DestroyEntity(entity);
                        continue;
                    }
                } else {
                    if (posX < -128 || posX > 384 || posY < -128 ||
                        posY > 352) {
                        DestroyEntity(entity);
                        continue;
                    }
                }
            }

            if ((flags & FLAG_UNK_02000000)) {
                s16 posY = entity->posY.i.hi + g_Tilemap.scrollY.i.hi;
                s16 test = (LOHU(g_Tilemap.vSize) * 256) + 128;
                if (posY > test) {
                    DestroyEntity(entity);
                    continue;
                }
            }

            if (flags & 0xF) {
                entity->palette =
                    UNK_Invincibility0[(entity->nFramesInvincibility << 1) |
                                       (flags & 1)];
                entity->flags--;
                if ((entity->flags & 0xF) == 0) {
                    entity->palette = entity->hitEffect;
                    entity->hitEffect = 0;
                }
            }

            if (!(flags & FLAG_UNK_20000000) || flags & FLAG_UNK_10000000 ||
                (entity->posX.i.hi >= -64 && entity->posX.i.hi <= 320 &&
                 entity->posY.i.hi >= -64 && entity->posY.i.hi <= 288)) {
                if (entity->stunFrames == 0 ||
                    (entity->stunFrames--, flags & FLAG_UNK_100000)) {
                    if (!D_800973FC || flags & 0x2100 ||
                        (flags & 0x200 && !(g_GameTimer & 3))) {
                        g_CurrentEntity = entity;
                        entity->pfnUpdate(entity);
                        entity->unk44 = 0;
                        entity->hitFlags = 0;
                    }
                }
            }
        } else {
            g_CurrentEntity = entity;
            entity->pfnUpdate(entity);
            entity->unk44 = 0;
            entity->hitFlags = 0;
        }
    }
}
