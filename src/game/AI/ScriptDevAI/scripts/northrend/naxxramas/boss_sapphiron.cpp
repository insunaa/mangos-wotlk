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
SDName: Boss_Sapphiron
SD%Complete: 80
SDComment: Some spells need core implementation
SDCategory: Naxxramas
EndScriptData */

/* Additional comments:
 * Bugged spells:   28560 (needs maxTarget = 1, Summon of 16474 implementation, TODO, 30s duration)
 *                  28526 (needs ScriptEffect to cast 28522 onto random target)
 *
 * Achievement-criteria check needs implementation
 *
 * Frost-Breath ability: the dummy spell 30101 is self cast, so it won't take the needed delay of ~7s until it reaches its target
 *                       As Sapphiron is displayed visually in hight (hovering), and the spell is cast with target=self-location
 *                       which is still on the ground, the client shows a nice slow falling of the visual animation
 *                       Insisting on using the Dummy-Effect to cast the breath-dmg spells, would require, to relocate Sapphi internally,
 *                       and to hack the targeting to be "on the ground" - Hence the prefered way as it is now!
 */
/*
#include "AI/ScriptDevAI/include/sc_common.h"
#include "naxxramas.h"

enum
{
    EMOTE_BREATH                = -1533082,
    EMOTE_GENERIC_ENRAGED       = -1000003,
    EMOTE_FLY                   = -1533022,
    EMOTE_GROUND                = -1533083,

    SPELL_CLEAVE                = 19983,
    SPELL_TAIL_SWEEP            = 55697,
    SPELL_TAIL_SWEEP_H          = 55696,
    SPELL_ICEBOLT               = 28526,
    SPELL_FROST_BREATH_DUMMY    = 30101,
    SPELL_FROST_BREATH_A        = 28524,
    SPELL_FROST_BREATH_B        = 29318,
    SPELL_FROST_AURA            = 28531,
    SPELL_FROST_AURA_H          = 55799,
    SPELL_LIFE_DRAIN            = 28542,
    SPELL_LIFE_DRAIN_H          = 55665,
    SPELL_CHILL                 = 28547,
    SPELL_CHILL_H               = 55699,
    SPELL_SUMMON_BLIZZARD       = 28560,
    SPELL_BESERK                = 26662,
    SPELL_ACHIEVEMENT_CHECK     = 60539,                    // unused

    NPC_YOU_KNOW_WHO            = 16474,
};

static const float aLiftOffPosition[3] = {3522.386f, -5236.784f, 137.709f};

enum Phases
{
    PHASE_GROUND        = 1,
    PHASE_LIFT_OFF      = 2,
    PHASE_AIR_BOLTS     = 3,
    PHASE_AIR_BREATH    = 4,
    PHASE_LANDING       = 5,
};

struct boss_sapphironAI : public ScriptedAI
{
    boss_sapphironAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_naxxramas*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_naxxramas* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiCleaveTimer;
    uint32 m_uiTailSweepTimer;
    uint32 m_uiIceboltTimer;
    uint32 m_uiFrostBreathTimer;
    uint32 m_uiLifeDrainTimer;
    uint32 m_uiBlizzardTimer;
    uint32 m_uiFlyTimer;
    uint32 m_uiBerserkTimer;
    uint32 m_uiLandTimer;

    uint32 m_uiIceboltCount;
    Phases m_Phase;

    void Reset() override
    {
        m_uiCleaveTimer = 5000;
        m_uiTailSweepTimer = 12000;
        m_uiFrostBreathTimer = 7000;
        m_uiLifeDrainTimer = 11000;
        m_uiBlizzardTimer = 15000;                          // "Once the encounter starts,based on your version of Naxx, this will be used x2 for normal and x6 on HC"
        m_uiFlyTimer = 46000;
        m_uiIceboltTimer = 5000;
        m_uiLandTimer = 0;
        m_uiBerserkTimer = 15 * MINUTE * IN_MILLISECONDS;
        m_Phase = PHASE_GROUND;
        m_uiIceboltCount = 0;

        SetCombatMovement(true);
        m_creature->SetLevitate(false);
    }
*/
//    void Aggro(Unit* /*pWho*/) override
/*    {
        DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_FROST_AURA : SPELL_FROST_AURA_H);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_SAPPHIRON, IN_PROGRESS);
    }
*/
//    void JustDied(Unit* /*pKiller*/) override
/*    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_SAPPHIRON, DONE);
    }

    void JustReachedHome() override
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_SAPPHIRON, FAIL);
    }

    void JustSummoned(Creature* pSummoned) override
    {
        if (pSummoned->GetEntry() == NPC_YOU_KNOW_WHO)
        {
            if (Unit* pEnemy = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                pSummoned->AI()->AttackStart(pEnemy);
        }
    }
*/
//    void MovementInform(uint32 uiType, uint32 /*uiPointId*/) override
/*    {
        if (uiType == POINT_MOTION_TYPE && m_Phase == PHASE_LIFT_OFF)
        {
            DoScriptText(EMOTE_FLY, m_creature);
            m_creature->HandleEmote(EMOTE_ONESHOT_LIFTOFF);
            m_creature->SetLevitate(true);
            m_Phase = PHASE_AIR_BOLTS;

            m_uiFrostBreathTimer = 5000;
            m_uiIceboltTimer = 5000;
            m_uiIceboltCount = 0;
        }
    }

    void UpdateAI(const uint32 uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->GetVictim())
            return;

        switch (m_Phase)
        {
            case PHASE_GROUND:
                if (m_uiCleaveTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature->GetVictim(), SPELL_CLEAVE) == CAST_OK)
                        m_uiCleaveTimer = urand(5000, 10000);
                }
                else
                    m_uiCleaveTimer -= uiDiff;

                if (m_uiTailSweepTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_TAIL_SWEEP : SPELL_TAIL_SWEEP_H) == CAST_OK)
                        m_uiTailSweepTimer = urand(7000, 10000);
                }
                else
                    m_uiTailSweepTimer -= uiDiff;

                if (m_uiLifeDrainTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_LIFE_DRAIN : SPELL_LIFE_DRAIN_H) == CAST_OK)
                        m_uiLifeDrainTimer = 23000;
                }
                else
                    m_uiLifeDrainTimer -= uiDiff;

                if (m_uiBlizzardTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_SUMMON_BLIZZARD) == CAST_OK)
                        m_uiBlizzardTimer = 20000;
                }
                else
                    m_uiBlizzardTimer -= uiDiff;

                if (m_creature->GetHealthPercent() > 10.0f)
                {
                    if (m_uiFlyTimer < uiDiff)
                    {
                        m_Phase = PHASE_LIFT_OFF;
                        SetDeathPrevention(true);
                        m_creature->InterruptNonMeleeSpells(false);
                        SetCombatMovement(false);
                        m_creature->GetMotionMaster()->Clear(false);
                        m_creature->GetMotionMaster()->MovePoint(1, aLiftOffPosition[0], aLiftOffPosition[1], aLiftOffPosition[2]);
                        // TODO This should clear the target, too

                        return;
                    }
                    m_uiFlyTimer -= uiDiff;
                }

                // Only Phase in which we have melee attack!
                DoMeleeAttackIfReady();
                break;
            case PHASE_LIFT_OFF:
                break;
            case PHASE_AIR_BOLTS:
                // WOTLK Changed, originally was 5
                if (m_uiIceboltCount == (uint32)(m_bIsRegularMode ? 2 : 3))
                {
                    if (m_uiFrostBreathTimer < uiDiff)
                    {
                        m_Phase = PHASE_AIR_BREATH;
                        DoScriptText(EMOTE_BREATH, m_creature);
                        DoCastSpellIfCan(m_creature, SPELL_FROST_BREATH_DUMMY);
                        m_uiFrostBreathTimer = 7000;
                    }
                    else
                        m_uiFrostBreathTimer -= uiDiff;
                }
                else
                {
                    if (m_uiIceboltTimer < uiDiff)
                    {
                        if (DoCastSpellIfCan(m_creature, SPELL_ICEBOLT))
                        {
                            ++m_uiIceboltCount;
                            m_uiIceboltTimer = 4000;
                        }
                    }
                    else
                        m_uiIceboltTimer -= uiDiff;
                }

                break;
            case PHASE_AIR_BREATH:
                if (m_uiFrostBreathTimer)
                {
                    if (m_uiFrostBreathTimer <= uiDiff)
                    {
                        DoCastSpellIfCan(m_creature, SPELL_FROST_BREATH_A, CAST_TRIGGERED);
                        DoCastSpellIfCan(m_creature, SPELL_FROST_BREATH_B, CAST_TRIGGERED);
                        m_uiFrostBreathTimer = 0;

                        m_uiLandTimer = 4000;
                    }
                    else
                        m_uiFrostBreathTimer -= uiDiff;
                }

                if (m_uiLandTimer)
                {
                    if (m_uiLandTimer <= uiDiff)
                    {
                        // Begin Landing
                        DoScriptText(EMOTE_GROUND, m_creature);
                        m_creature->HandleEmote(EMOTE_ONESHOT_LAND);
                        m_creature->SetLevitate(false);

                        m_Phase = PHASE_LANDING;
                        m_uiLandTimer = 2000;
                    }
                    else
                        m_uiLandTimer -= uiDiff;
                }

                break;
            case PHASE_LANDING:
                if (m_uiLandTimer < uiDiff)
                {
                    m_Phase = PHASE_GROUND;
                    SetDeathPrevention(false);
                    SetCombatMovement(true);
                    m_creature->GetMotionMaster()->Clear(false);
                    m_creature->GetMotionMaster()->MoveChase(m_creature->GetVictim());

                    m_uiFlyTimer = 67000;
                    m_uiLandTimer = 0;
                }
                else
                    m_uiLandTimer -= uiDiff;

                break;
        }

        // Enrage can happen in any phase
        if (m_uiBerserkTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_BESERK) == CAST_OK)
            {
                DoScriptText(EMOTE_GENERIC_ENRAGED, m_creature);
                m_uiBerserkTimer = 300000;
            }
        }
        else
            m_uiBerserkTimer -= uiDiff;
    }
};

UnitAI* GetAI_boss_sapphiron(Creature* pCreature)
{
    return new boss_sapphironAI(pCreature);
}
*/
//bool GOUse_go_sapphiron_birth(Player* /*pPlayer*/, GameObject* pGo)
/*{
    ScriptedInstance* pInstance = (ScriptedInstance*)pGo->GetInstanceData();

    if (!pInstance)
        return true;

    if (pInstance->GetData(TYPE_SAPPHIRON) != NOT_STARTED)
        return true;

    // If already summoned return (safety check)
    if (pInstance->GetSingleCreatureFromStorage(NPC_SAPPHIRON, true))
        return true;

    // Set data to special and allow the Go animation to proceed
    pInstance->SetData(TYPE_SAPPHIRON, SPECIAL);
    return false;
}

void AddSC_boss_sapphiron()
{
    Script* pNewScript = new Script;
    pNewScript->Name = "boss_sapphiron";
    pNewScript->GetAI = &GetAI_boss_sapphiron;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "go_sapphiron_birth";
    pNewScript->pGOUse = &GOUse_go_sapphiron_birth;
    pNewScript->RegisterSelf();
}
*/

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
SDName: Boss_Sapphiron
SD%Complete: 100
SDComment:
SDCategory: Naxxramas
EndScriptData */

#include "AI/ScriptDevAI/base/CombatAI.h"
#include "AI/ScriptDevAI/include/sc_common.h"
#include "Globals/SharedDefines.h"
#include "MotionGenerators/MotionMaster.h"
#include "Server/DBCEnums.h"
#include "naxxramas.h"

enum
{
    EMOTE_BREATH                = -1533082,
    EMOTE_GENERIC_ENRAGED       = -1000003,

    // All phases spells
    SPELL_FROST_AURA            = 28529,            // Periodically triggers 28531
    SPELL_FROST_AURA_H          = 55799,
    SPELL_BESERK                = 26662,

    // Ground phase spells
    SPELL_CLEAVE                = 19983,
    SPELL_TAIL_SWEEP            = 15847,
    SPELL_TAIL_SWEEP_H          = 55696,
    SPELL_LIFE_DRAIN            = 28542,
    SPELL_LIFE_DRAIN_H          = 55665,
    SPELL_SUMMON_BLIZZARD_INIT  = 28560,
    SPELL_SUMMON_BLIZZARD       = 28561,

    // Air phase spells
    SPELL_DRAGON_HOVER          = 18430,
    SPELL_ICEBOLT_INIT          = 28526,            // Triggers spell 28522 (Icebolt)
    SPELL_ICEBOLT               = 28522,
    SPELL_ICEBOLT_IMMUNITY      = 31800,
    SPELL_ICEBLOCK_SUMMON       = 28535,
    SPELL_FROST_BREATH_DUMMY    = 30101,
    SPELL_FROST_BREATH          = 28524,            // Triggers spells 29318 (Frost Breath) and 30132 (Despawn Ice Block)
    SPELL_DESPAWN_ICEBLOCK_GO   = 28523,
    SPELL_SUMMON_WING_BUFFET    = 29329,
    SPELL_DESPAWN_WING_BUFFET   = 29330,            // Triggers spell 29336 (Despawn Buffet)
    SPELL_WING_BUFFET           = 29328,
    SPELL_DESPAWN_BUFFET_EFFECT = 29336,
    SPELL_CHILL                 = 28547,
    SPELL_CHILL_H               = 55699,
    SPELL_ACHIEVEMENT_CHECK     = 60539,                    // unused

    NPC_BLIZZARD                = 16474,
};

static const float aLiftOffPosition[3] = {3522.386f, -5236.784f, 137.709f};

enum SapphironPhases
{
    PHASE_GROUND        = 1,
    PHASE_LIFT_OFF      = 2,
};

enum SapphironActions
{
    SAPPHIRON_CLEAVE,
    SAPPHIRON_TAIL_SWEEP,
    SAPPHIRON_ICEBOLT,
    SAPPHIRON_FROST_BREATH,
    SAPPHIRON_LIFE_DRAIN,
    SAPPHIRON_BLIZZARD,
    SAPPHIRON_AIR_PHASE,
    SAPPHIRON_LANDING_PHASE,
    SAPPHIRON_GROUND_PHASE,
    SAPPHIRON_BERSERK,
    SAPPHIRON_ACTION_MAX,
};

static const uint32 groundPhaseActions[] = {SAPPHIRON_CLEAVE, SAPPHIRON_TAIL_SWEEP, SAPPHIRON_BLIZZARD, SAPPHIRON_LIFE_DRAIN};

struct boss_sapphironAI : public CombatAI
{
    boss_sapphironAI(Creature* creature) : CombatAI(creature, SAPPHIRON_ACTION_MAX), m_instance(static_cast<ScriptedInstance*>(creature->GetInstanceData()))
    {
        AddCombatAction(SAPPHIRON_CLEAVE, 5u * IN_MILLISECONDS);
        AddCombatAction(SAPPHIRON_TAIL_SWEEP, 12u * IN_MILLISECONDS);
        AddCombatAction(SAPPHIRON_LIFE_DRAIN, 11u * IN_MILLISECONDS);
        AddCombatAction(SAPPHIRON_BLIZZARD, 15u * IN_MILLISECONDS);
        AddCombatAction(SAPPHIRON_FROST_BREATH, true);
        AddCombatAction(SAPPHIRON_ICEBOLT, true);
        AddCombatAction(SAPPHIRON_BERSERK, 15u * MINUTE * IN_MILLISECONDS);
        AddCustomAction(SAPPHIRON_AIR_PHASE, true, [&]() { HandleAirPhase(); });
        AddCustomAction(SAPPHIRON_LANDING_PHASE, true, [&]() { HandleLandingPhase(); });
        AddCustomAction(SAPPHIRON_GROUND_PHASE, true, [&]() { HandleGroundPhase(); });
        m_bIsRegularMode = creature->GetMap()->IsRegularDifficulty();
    }

    ScriptedInstance* m_instance;

    uint32 m_iceboltCount;
    SapphironPhases m_phase;
    bool m_bIsRegularMode;
    uint32 m_blizzardCount;

    void Reset() override
    {
        CombatAI:: Reset();

        m_iceboltCount = 0;
        m_blizzardCount = 0;
        m_phase = PHASE_GROUND;

        SetCombatMovement(true);
        SetDeathPrevention(false);
        SetMeleeEnabled(true);
        m_creature->SetHover(false);
    }

    uint32 GetSubsequentActionTimer(uint32 action)
    {
        switch (action)
        {
            case SAPPHIRON_CLEAVE: return urand(5, 10) * IN_MILLISECONDS;
            case SAPPHIRON_TAIL_SWEEP: return urand(7, 10) * IN_MILLISECONDS;
            case SAPPHIRON_LIFE_DRAIN: return 24u * IN_MILLISECONDS;
            case SAPPHIRON_BLIZZARD: return urand(10, 30) * IN_MILLISECONDS;
            case SAPPHIRON_ICEBOLT: return 3u * IN_MILLISECONDS;
            case SAPPHIRON_BERSERK: return 300u * IN_MILLISECONDS;
            case SAPPHIRON_AIR_PHASE: return 46u * IN_MILLISECONDS;
            default: return 0;
        }
    }

    void Aggro(Unit* /*who*/) override
    {
        DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_FROST_AURA : SPELL_FROST_AURA_H, CAST_TRIGGERED | CAST_AURA_NOT_PRESENT);

        if (m_instance)
            m_instance->SetData(TYPE_SAPPHIRON, IN_PROGRESS);

        ResetTimer(SAPPHIRON_AIR_PHASE, GetSubsequentActionTimer(SAPPHIRON_AIR_PHASE));
    }

    void JustDied(Unit* /*killer*/) override
    {
        if (m_instance)
            m_instance->SetData(TYPE_SAPPHIRON, DONE);
    }

    void JustReachedHome() override
    {
        if (m_instance)
            m_instance->SetData(TYPE_SAPPHIRON, FAIL);
    }

    void JustSummoned(Creature* summoned) override
    {
        if (summoned->GetEntry() == NPC_BLIZZARD)
        {
            if (Unit* pEnemy = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                summoned->AI()->AttackStart(pEnemy);
        }
    }

    void MovementInform(uint32 type, uint32 /*pointId*/) override
    {
        if (type == POINT_MOTION_TYPE && m_phase == PHASE_LIFT_OFF)
        {
            // Actual take off
            m_creature->HandleEmote(EMOTE_ONESHOT_LIFTOFF);
            m_creature->SetHover(true);
            m_creature->CastSpell(nullptr, SPELL_DRAGON_HOVER, TRIGGERED_OLD_TRIGGERED);
            ResetCombatAction(SAPPHIRON_ICEBOLT, 8u * IN_MILLISECONDS);
        }
    }

    void HandleAirPhase()
    {
        if (m_creature->GetHealthPercent() > 10.f)
        {
            m_phase = PHASE_LIFT_OFF;
            for (uint32 action : groundPhaseActions)
                DisableCombatAction(action);
            SetDeathPrevention(true);
            m_creature->InterruptNonMeleeSpells(false);
            SetCombatMovement(false);
            SetMeleeEnabled(false);
            m_creature->SetTarget(nullptr);
            m_creature->GetMotionMaster()->MovePoint(1, aLiftOffPosition[0], aLiftOffPosition[1], aLiftOffPosition[2], FORCED_MOVEMENT_RUN);
        }
        else
            DisableTimer(SAPPHIRON_AIR_PHASE);
    }

    void HandleLandingPhase()
    {
        m_creature->HandleEmote(EMOTE_ONESHOT_LAND);
        ResetTimer(SAPPHIRON_GROUND_PHASE, 2u * IN_MILLISECONDS);
        for (auto block : ((instance_naxxramas*)m_instance)->getIceBlockGOs())
        {
            m_creature->GetMap()->GetGameObject(block)->ForcedDespawn();
        }
    }

    void HandleGroundPhase()
    {
        m_phase = PHASE_GROUND;
        for (uint32 action : groundPhaseActions)
            ResetCombatAction(action, GetSubsequentActionTimer(action));
        SetDeathPrevention(false);
        SetCombatMovement(true);
        SetMeleeEnabled(true);
        m_creature->RemoveAurasDueToSpell(SPELL_DRAGON_HOVER);
        m_creature->SetHover(false);
        m_creature->GetMotionMaster()->Clear(false);
        m_creature->GetMotionMaster()->MoveChase(m_creature->GetVictim());

        ResetTimer(SAPPHIRON_AIR_PHASE, GetSubsequentActionTimer(SAPPHIRON_AIR_PHASE));
    }

    void ExecuteAction(uint32 action) override
    {
        switch(action)
        {
            case SAPPHIRON_CLEAVE:
            {
                if (DoCastSpellIfCan(m_creature->GetVictim(), m_bIsRegularMode ? SPELL_CLEAVE : SPELL_CLEAVE) == CAST_OK)
                    ResetCombatAction(action, GetSubsequentActionTimer(action));
                return;
            }
            case SAPPHIRON_TAIL_SWEEP:
            {
                if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_TAIL_SWEEP : SPELL_TAIL_SWEEP_H) == CAST_OK)
                    ResetCombatAction(action, GetSubsequentActionTimer(action));
                return;
            }
            case SAPPHIRON_LIFE_DRAIN:
            {
                if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_LIFE_DRAIN : SPELL_LIFE_DRAIN_H) == CAST_OK)
                    ResetCombatAction(action, GetSubsequentActionTimer(action));
                return;
            }
            case SAPPHIRON_BLIZZARD:
            {
                if (m_blizzardCount >= (m_bIsRegularMode ? 2 : 6))
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_SUMMON_BLIZZARD_INIT) == CAST_OK)
                        ResetCombatAction(action, GetSubsequentActionTimer(action));
                }
                else
                    DisableCombatAction(action);
                return;

            }
            case SAPPHIRON_ICEBOLT:
            {
                if (m_iceboltCount < (m_bIsRegularMode ? 2 : 3))
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_ICEBOLT_INIT) == CAST_OK)
                    {
                        ++m_iceboltCount;
                        ResetCombatAction(action, GetSubsequentActionTimer(action));
                    }
                }
                else
                {
                    m_iceboltCount = 0;
                    DisableCombatAction(action);
                    ResetCombatAction(SAPPHIRON_FROST_BREATH, 100u);    // Enough Icebolts were cast, switch to Frost Breath (Ice Bomb) after that
                }
                return;
            }
            case SAPPHIRON_FROST_BREATH:
            {
                if (DoCastSpellIfCan(m_creature, SPELL_FROST_BREATH) == CAST_OK)
                {
                    DoCastSpellIfCan(m_creature, SPELL_FROST_BREATH_DUMMY, CAST_TRIGGERED);
                    DoScriptText(EMOTE_BREATH, m_creature);
                    DisableCombatAction(action);
                    ResetTimer(SAPPHIRON_LANDING_PHASE, 10u * IN_MILLISECONDS);
                }
                return;
            }
            case SAPPHIRON_BERSERK:
            {
                if (DoCastSpellIfCan(m_creature, SPELL_BESERK) == CAST_OK)
                {
                    DoScriptText(EMOTE_GENERIC_ENRAGED, m_creature);
                    DisableCombatAction(action);
                }
                return;
            }
        }
    }
};

bool GOUse_go_sapphiron_birth(Player* /*player*/, GameObject* go)
{
    ScriptedInstance* instance = (ScriptedInstance*)go->GetInstanceData();

    if (!instance)
        return true;

    if (instance->GetData(TYPE_SAPPHIRON) != NOT_STARTED)
        return true;

    // If already summoned return (safety check)
    if (instance->GetSingleCreatureFromStorage(NPC_SAPPHIRON, true))
        return true;

    // Set data to special and allow the Go animation to proceed
    instance->SetData(TYPE_SAPPHIRON, SPECIAL);
    return false;
}

struct IceBolt : public SpellScript
{
    void OnEffectExecute(Spell* spell, SpellEffectIndex /* effIdx */) const override
    {
        if (Unit* caster = spell->GetCaster())
        {
            if (Unit* target = ((Creature*)caster)->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, SPELL_ICEBOLT, SELECT_FLAG_PLAYER | SELECT_FLAG_NOT_AURA))
                caster->CastSpell(target, SPELL_ICEBOLT, TRIGGERED_NONE); // Icebolt
        }
    }
};

struct PeriodicIceBolt : public AuraScript
{
    void OnPeriodicTrigger(Aura* aura, PeriodicTriggerData& data) const override
    {
        if (Unit* target =  aura->GetTarget())
        {
            if (target->IsAlive() && !target->IsImmuneToSchool(sSpellTemplate.LookupEntry<SpellEntry>(SPELL_FROST_BREATH), SPELL_SCHOOL_MASK_FROST))/*!target->HasAura(SPELL_ICEBOLT_IMMUNITY))*/
            {
                /*target->CastSpell(target, SPELL_ICEBOLT_IMMUNITY, TRIGGERED_OLD_TRIGGERED);     // Icebolt which causes immunity to frost dmg
                data.spellInfo = sSpellTemplate.LookupEntry<SpellEntry>(SPELL_ICEBLOCK_SUMMON); // Summon Ice Block
                */
                target->ApplySpellImmune(nullptr, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_FROST, false);
                target->AddGameObject(nullptr);
                
                GameObject* pGameObj = new GameObject;

                float x, y, z;
                Position targetPos = target->GetPosition();
                x = targetPos.GetPositionX();
                y = targetPos.GetPositionY();
                z = targetPos.GetPositionZ();

                Map* map = target->GetMap();

                if (!pGameObj->Create(map->GenerateLocalLowGuid(HIGHGUID_GAMEOBJECT), GO_ICEBLOCK, map, target->GetPhaseMask(), x, y, z, target->GetOrientation()))
                {
                    delete pGameObj;
                    return;
                }

                pGameObj->SetRespawnTime(0);
                pGameObj->SetSpawnerGuid(target->GetObjectGuid());

                // Wild object not have owner and check clickable by players
                map->Add(pGameObj);
                pGameObj->AIM_Initialize();
            }
        }
    }
};

struct SummonBlizzard : public SpellScript
{
    void OnEffectExecute(Spell* spell, SpellEffectIndex /* effIdx */) const override
    {
        if (Unit* unitTarget = spell->GetUnitTarget())
            unitTarget->CastSpell(unitTarget, SPELL_SUMMON_BLIZZARD, TRIGGERED_OLD_TRIGGERED, nullptr, nullptr, spell->GetCaster()->GetObjectGuid());
    }
};

struct DespawnIceBlock : public SpellScript
{
    void OnEffectExecute(Spell* spell, SpellEffectIndex /* effIdx */) const override
    {
        if (Unit* unitTarget = spell->GetUnitTarget())
        {
            if (unitTarget->GetTypeId() == TYPEID_PLAYER)
            {
                unitTarget->RemoveAurasDueToSpell(SPELL_ICEBOLT_IMMUNITY);                          // Icebolt immunity spell
                unitTarget->RemoveAurasDueToSpell(SPELL_ICEBOLT);                                   // Icebolt stun/damage spell
                unitTarget->CastSpell(nullptr, SPELL_DESPAWN_ICEBLOCK_GO, TRIGGERED_OLD_TRIGGERED); // Despawn Ice Block (targets Ice Block GOs)
            }
        }
    }
};

struct DespawnBuffet : public AuraScript
{
    void OnPeriodicTrigger(Aura* aura, PeriodicTriggerData& data) const override
    {
        if (Unit* target =  aura->GetTarget())
            data.spellInfo = sSpellTemplate.LookupEntry<SpellEntry>(SPELL_DESPAWN_BUFFET_EFFECT); // Despawn Ice Block
    }
};

void AddSC_boss_sapphiron()
{
    Script* newScript = new Script;
    newScript->Name = "boss_sapphiron";
    newScript->GetAI = &GetNewAIInstance<boss_sapphironAI>;
    newScript->RegisterSelf();

    newScript = new Script;
    newScript->Name = "go_sapphiron_birth";
    newScript->pGOUse = &GOUse_go_sapphiron_birth;
    newScript->RegisterSelf();

    RegisterSpellScript<PeriodicIceBolt>("spell_sapphiron_icebolt_aura");
    RegisterSpellScript<IceBolt>("spell_sapphiron_icebolt");
    RegisterSpellScript<SummonBlizzard>("spell_sapphiron_blizzard");
    RegisterSpellScript<DespawnIceBlock>("spell_sapphiron_iceblock");
    RegisterSpellScript<DespawnBuffet>("spell_sapphiron_despawn_buffet");
}