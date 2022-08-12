/* This file is part of the ScriptDev2 Project. See AUTHORS file for Copyright information
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* ScriptData
SDName: Boss_Anubrekhan
SD%Complete: 95
SDComment: Intro text usage is not very clear. Requires additional research.
SDCategory: Naxxramas
EndScriptData */

#include "AI/ScriptDevAI/base/CombatAI.h"
#include "AI/ScriptDevAI/include/sc_common.h"
#include "naxxramas.h"
#include <chrono>

enum
{
    SAY_GREET                   = -1533000,
    SAY_AGGRO1                  = -1533001,
    SAY_AGGRO2                  = -1533002,
    SAY_AGGRO3                  = -1533003,
    SAY_TAUNT1                  = -1533004,
    SAY_TAUNT2                  = -1533005,
    SAY_TAUNT3                  = -1533006,
    SAY_TAUNT4                  = -1533007,
    SAY_SLAY                    = -1533008,

    EMOTE_CRYPT_GUARD           = -1533153,
    EMOTE_INSECT_SWARM          = -1533154,
    EMOTE_CORPSE_SCARABS        = -1533155,

    SPELL_DOUBLE_ATTACK         = 18943,
    SPELL_IMPALE                = 28783,                    // May be wrong spell id. Causes more dmg than I expect
    SPELL_IMPALE_H              = 56090,
    SPELL_LOCUSTSWARM           = 28785,                    // This is a self buff that triggers the dmg debuff
    SPELL_LOCUSTSWARM_H         = 54021,

    // spellId invalid
    // SPELL_SUMMONGUARD         = 29508,                   // Summons 1 crypt guard at targeted location - spell doesn't exist in 3.x.x

    SPELL_SELF_SPAWN_5          = 29105,                    // This spawns 5 corpse scarabs ontop of us (most likely the pPlayer casts this on death)
    SPELL_SELF_SPAWN_10         = 28864,                    // This is used by the crypt guards when they die

    NPC_CRYPT_GUARD             = 16573
};

static const DialogueEntry aIntroDialogue[] =
{
    {SAY_GREET,   NPC_ANUB_REKHAN,  7000},
    {SAY_TAUNT1,  NPC_ANUB_REKHAN,  13000},
    {SAY_TAUNT2,  NPC_ANUB_REKHAN,  11000},
    {SAY_TAUNT3,  NPC_ANUB_REKHAN,  10000},
    {SAY_TAUNT4,  NPC_ANUB_REKHAN,  0},
    {0, 0, 0}
};

static const float aCryptGuardLoc[4] = {3333.5f, -3475.9f, 287.1f, 3.17f};

enum AnubRekhanActions
{
    ANUBREKHAN_IMPALE,
    ANUBREKHAN_LOCUST_SWARM,
    ANUBREKHAN_ACTIONS_MAX,
    ANUBREKHAN_SUMMON,
};

struct boss_anubrekhanAI : public BossAI
{
    boss_anubrekhanAI(Creature* creature) : BossAI(creature, ANUBREKHAN_ACTIONS_MAX),
        m_introDialogue(aIntroDialogue),
        m_instance(static_cast<instance_naxxramas*>(creature->GetInstanceData())),
        m_isRegularMode(creature->GetMap()->IsRegularDifficulty())
    {
        m_introDialogue.InitializeDialogueHelper(m_instance);
        m_hasTaunted = false;
        SetDataType(TYPE_ANUB_REKHAN);
        AddOnAggroText(SAY_AGGRO1, SAY_AGGRO2, SAY_AGGRO3);
        AddCombatAction(ANUBREKHAN_IMPALE, 15s);
        AddCombatAction(ANUBREKHAN_LOCUST_SWARM, 90s);
        AddCustomAction(ANUBREKHAN_SUMMON, true, [&](){
            m_creature->SummonCreature(NPC_CRYPT_GUARD, aCryptGuardLoc[0], aCryptGuardLoc[1], aCryptGuardLoc[2], aCryptGuardLoc[3], TEMPSPAWN_TIMED_OOC_OR_DEAD_DESPAWN, 30000);
        });
        Reset();
    }

    instance_naxxramas* m_instance;
    DialogueHelper m_introDialogue;
    bool m_isRegularMode;
    bool m_hasTaunted;

    void Reset() override
    {
        DoCastSpellIfCan(m_creature, SPELL_DOUBLE_ATTACK, CAST_TRIGGERED | CAST_AURA_NOT_PRESENT);
        if (m_isRegularMode)
            ResetCombatAction(ANUBREKHAN_SUMMON, 20s);
    }

    void KilledUnit(Unit* pVictim) override
    {
        if (!pVictim)
            return;
        // Force the player to spawn corpse scarabs via spell
        if (pVictim->GetTypeId() == TYPEID_PLAYER)
            pVictim->CastSpell(pVictim, SPELL_SELF_SPAWN_5, TRIGGERED_OLD_TRIGGERED, nullptr, nullptr, m_creature->GetObjectGuid());

        if (urand(0, 4))
            return;
        DoScriptText(SAY_SLAY, m_creature);
    }

    void MoveInLineOfSight(Unit* pWho) override
    {
        if (!m_hasTaunted && pWho->GetTypeId() == TYPEID_PLAYER && m_creature->IsWithinDistInMap(pWho, 110.0f) && m_creature->IsWithinLOSInMap(pWho))
        {
            m_introDialogue.StartNextDialogueText(SAY_GREET);
            m_hasTaunted = true;
        }

        ScriptedAI::MoveInLineOfSight(pWho);
    }

    void JustSummoned(Creature* pSummoned) override
    {
        if (pSummoned->GetEntry() == NPC_CRYPT_GUARD)
            DoScriptText(EMOTE_CRYPT_GUARD, pSummoned);

        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            pSummoned->AI()->AttackStart(pTarget);
    }

    void SummonedCreatureDespawn(Creature* pSummoned) override
    {
        // If creature despawns on out of combat, skip this
        if (!m_creature->SelectHostileTarget() || !m_creature->GetVictim())
            return;

        if (pSummoned && pSummoned->GetEntry() == NPC_CRYPT_GUARD)
        {
            pSummoned->CastSpell(pSummoned, SPELL_SELF_SPAWN_10, TRIGGERED_OLD_TRIGGERED, nullptr, nullptr, m_creature->GetObjectGuid());
            DoScriptText(EMOTE_CORPSE_SCARABS, pSummoned);
        }
    }

    std::chrono::milliseconds GetSubsequentActionTimer(uint32 action)
    {
        switch (action)
        {
            case ANUBREKHAN_IMPALE: return 15s;
            case ANUBREKHAN_LOCUST_SWARM: return 100s;
        }
        return 0s;
    }

    void ExecuteAction(uint32 action) override
    {
        switch (action)
        {
            case ANUBREKHAN_IMPALE:
                if (!m_creature->HasAura(SPELL_LOCUSTSWARM) && !m_creature->HasAura(SPELL_LOCUSTSWARM_H))
                {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        DoCastSpellIfCan(pTarget, m_isRegularMode ? SPELL_IMPALE : SPELL_IMPALE_H);
                }
            break;
            case ANUBREKHAN_LOCUST_SWARM:
                if (DoCastSpellIfCan(m_creature, m_isRegularMode ? SPELL_LOCUSTSWARM : SPELL_LOCUSTSWARM_H) == CAST_OK)
                {
                    DoScriptText(EMOTE_INSECT_SWARM, m_creature);

                    // Summon a crypt guard
                    ResetTimer(ANUBREKHAN_SUMMON, 3s);
                    break;
                }
                return;
        }
        ResetCombatAction(action, GetSubsequentActionTimer(action));
    }
};

void AddSC_boss_anubrekhan()
{
    Script* pNewScript = new Script;
    pNewScript->Name = "boss_anubrekhan";
    pNewScript->GetAI = &GetNewAIInstance<boss_anubrekhanAI>;
    pNewScript->RegisterSelf();
}
