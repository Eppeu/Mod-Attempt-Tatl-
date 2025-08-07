#include "modding.h"
#include "global.h"
#include "overlays/actors/ovl_En_Elf/z_en_elf.h"
#include "libc64/qrand.h"
#include "rand.h"

RECOMP_IMPORT("*", u32 recomp_get_config_u32(const char *key));
RECOMP_IMPORT("*", double recomp_get_config_double(const char *key));

typedef enum
{
    FAIRY_TATL,
    FAIRY_TAEL,
    FAIRY_RANDOM,
    FAIRY_TRUE_RANDOM,
    FAIRY_NONE,
    FAIRY_CUSTOM,
} AlternativeFairy;

#define ALTERNATIVE_FAIRY_COLOR ((AlternativeFairy)recomp_get_config_u32("fairy_character"))

#define TATL_I_R (f32) recomp_get_config_double("tatl_inner_color_red")
#define TATL_I_G (f32) recomp_get_config_double("tatl_inner_color_green")
#define TATL_I_B (f32) recomp_get_config_double("tatl_inner_color_blue")
#define TATL_O_R (f32) recomp_get_config_double("tatl_outer_color_red")
#define TATL_O_G (f32) recomp_get_config_double("tatl_outer_color_green")
#define TATL_O_B (f32) recomp_get_config_double("tatl_outer_color_blue")

Attention *gAttention;

RECOMP_HOOK("Attention_SetTatlState")
void TatlColorVariable(Attention *attention, Actor *actor, ActorType actorCategory, PlayState *play)
{
    gAttention = attention;
}

RECOMP_HOOK_RETURN("Attention_SetTatlState")
void TatlCustomColor()
{
    Attention *attention = gAttention;
    if (ALTERNATIVE_FAIRY_COLOR == FAIRY_TATL)
    {
    }
    else if (ALTERNATIVE_FAIRY_COLOR == FAIRY_TAEL)
    {

        attention->tatlInnerColor.r = 60;
        attention->tatlInnerColor.g = 16;
        attention->tatlInnerColor.b = 88;
        attention->tatlInnerColor.a = 255;

        attention->tatlOuterColor.r = 255;
        attention->tatlOuterColor.g = 41;
        attention->tatlOuterColor.b = 15;
        attention->tatlOuterColor.a = 0;
    }
    else if (ALTERNATIVE_FAIRY_COLOR == FAIRY_TRUE_RANDOM)
    {
        int True_random[] = {(Rand_Next() % 255) + 0, (Rand_Next() % 255), (Rand_Next() % 255), (Rand_Next() % 255), (Rand_Next() % 255), (Rand_Next() % 255)};
        attention->tatlInnerColor.r = True_random[0];
        attention->tatlInnerColor.g = True_random[1];
        attention->tatlInnerColor.b = True_random[2];
        attention->tatlInnerColor.a = 255;

        attention->tatlOuterColor.r = True_random[3];
        attention->tatlOuterColor.g = True_random[4];
        attention->tatlOuterColor.b = True_random[5];
        attention->tatlOuterColor.a = 0;
    }
    else if (ALTERNATIVE_FAIRY_COLOR == FAIRY_RANDOM)
    {
        int Random[6];
        int i;
        for (i = 0; i == 6; i++)
        {
            Random[i] = (Rand_Next() % 255) + 0;
        }
        attention->tatlInnerColor.r = Random[0];
        attention->tatlInnerColor.g = Random[1];
        attention->tatlInnerColor.b = Random[2];
        attention->tatlInnerColor.a = 255;

        attention->tatlOuterColor.r = Random[3];
        attention->tatlOuterColor.g = Random[4];
        attention->tatlOuterColor.b = Random[5];
        attention->tatlOuterColor.a = 0;
    }
    else if (ALTERNATIVE_FAIRY_COLOR == FAIRY_CUSTOM)
    {

        attention->tatlInnerColor.r = TATL_I_R;
        attention->tatlInnerColor.g = TATL_I_G;
        attention->tatlInnerColor.b = TATL_I_B;
        attention->tatlInnerColor.a = 255;

        attention->tatlOuterColor.r = TATL_O_R;
        attention->tatlOuterColor.g = TATL_O_G;
        attention->tatlOuterColor.b = TATL_O_B;
        attention->tatlOuterColor.a = 0;
    }
}

RECOMP_PATCH void CutsceneCmd_GiveTatlToPlayer(PlayState *play, CutsceneContext *csCtx, CsCmdGiveTatl *cmd)
{
    if (ALTERNATIVE_FAIRY_COLOR == FAIRY_NONE)
    {
        Player *player = GET_PLAYER(play);

        if (csCtx->curFrame == cmd->startFrame)
        {
            if (cmd->giveTatl == true)
            {
                gSaveContext.save.hasTatl = false;
                if (player->tatlActor != NULL)
                {
                    return;
                }
                player->tatlActor =
                    Actor_Spawn(&play->actorCtx, play, ACTOR_EN_ELF, player->actor.world.pos.x, player->actor.world.pos.y,
                                player->actor.world.pos.z, 0, 0, 0, FAIRY_PARAMS(FAIRY_TYPE_0, false, 0));
            }
        }
    }
}
