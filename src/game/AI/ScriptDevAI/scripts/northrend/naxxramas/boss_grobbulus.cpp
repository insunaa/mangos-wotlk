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
SDName: Boss_Grobbulus
SD%Complete: 80
SDComment: Timer need more care; Spells of Adds (Posion Cloud) need Mangos Fixes, and further handling
SDCategory: Naxxramas
EndScriptData */

/*Poison Cloud 26590
Slime Spray 28157
Fallout slime 28218
Mutating Injection 28169
Enrages 26527*/

#include "AI/ScriptDevAI/ScriptDevAIMgr.h"
#include "AI/ScriptDevAI/base/CombatAI.h"
#include "AI/ScriptDevAI/include/sc_common.h"
#include "Common.h"
#include "Spells/SpellDefines.h"
#include "naxxramas.h"

enum
{
    EMOTE_SPRAY_SLIME               = 32318,
    //EMOTE_INJECTION                 = -1533158, // unused

    SPELL_SLIME_STREAM              = 28137,
    SPELL_MUTATING_INJECTION        = 28169,
    SPELL_EMBALMING_CLOUD           = 28322,
    SPELL_MUTAGEN_EXPLOSION         = 28206,
    SPELL_POISON_CLOUD              = 28240,
    SPELL_SLIME_SPRAY               = 28157,
    SPELL_SLIME_SPRAY_H             = 54364,
    SPELL_SUMMON_FALLOUT_SLIME      = 28218,
    SPELL_BERSERK                   = 26662,

    SPELL_DESPAWN_SUMMONS           = 30134,

    NPC_FALLOUT_SLIME               = 16290
};

enum GrobbulusActions
{
    GROBBULUS_SLIME_STREAM,
    GROBBULUS_CHECK_MELEE,
    GROBBULUS_SLIME_SPRAY,
    GROBBULUS_INJECTION,
    GROBBULUS_POISON_CLOUD,
    GROBBULUS_BERSERK,
    GROBBULUS_ACTION_MAX,
};

struct boss_grobbulusAI : public CombatAI
{
    boss_grobbulusAI(Creature* creature) : CombatAI(creature, GROBBULUS_ACTION_MAX), m_instance(static_cast<ScriptedInstance*>(creature->GetInstanceData()))
    {
        SetDataType(TYPE_GROBBULUS);
        m_bIsRegularMode = creature->GetMap()->IsRegularDifficulty();
        m_uiBerserkTimeSecs = m_bIsRegularMode ? 12 * MINUTE : 9 * MINUTE;
        m_uiBerserkTimer = m_uiBerserkTimeSecs * IN_MILLISECONDS;
        AddCombatAction(GROBBULUS_SLIME_STREAM, true);
        AddCombatAction(GROBBULUS_SLIME_SPRAY, 20u * IN_MILLISECONDS, 30u * IN_MILLISECONDS); //Spell List
        AddCombatAction(GROBBULUS_INJECTION, 13u * IN_MILLISECONDS);
        AddCombatAction(GROBBULUS_POISON_CLOUD, 20u * IN_MILLISECONDS, 25u * IN_MILLISECONDS); //Spell List
        AddCombatAction(GROBBULUS_BERSERK, m_uiBerserkTimer);
        AddCombatAction(GROBBULUS_CHECK_MELEE, 5u * IN_MILLISECONDS);
    }

    ScriptedInstance* m_instance;
    bool m_bIsRegularMode;
    uint32 m_uiBerserkTimer, m_uiBerserkTimeSecs;

    void Aggro(Unit* /*who*/) override
    {
        if (m_instance)
            m_instance->SetData(TYPE_GROBBULUS, IN_PROGRESS);

        //ResetIfNotStarted(GROBBULUS_SLIME_STREAM, 5u * IN_MILLISECONDS);
    }

    void EnterEvadeMode() override
    {
        if (m_instance)
            m_instance->SetData(TYPE_GROBBULUS, FAIL);

        // Clean-up stage
        DoCastSpellIfCan(m_creature, SPELL_DESPAWN_SUMMONS, CAST_TRIGGERED);

        CombatAI::EnterEvadeMode();
    }

    void SpellHitTarget(Unit* target, const SpellEntry* spell) override
    {
        // Summon a Fallout Slime for every player hit by Slime Spray
        //if ((spell->Id == SPELL_SLIME_SPRAY) && target->GetTypeId() == TYPEID_PLAYER)
            //DoCastSpellIfCan(target, SPELL_SUMMON_FALLOUT_SLIME, TRIGGERED_OLD_TRIGGERED);

        if ((spell->Id == SPELL_SLIME_SPRAY || spell->Id == SPELL_SLIME_SPRAY_H) && target->GetTypeId() == TYPEID_PLAYER)
            m_creature->SummonCreature(NPC_FALLOUT_SLIME, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 0.0f, TEMPSPAWN_TIMED_OOC_DESPAWN, 10 * IN_MILLISECONDS);
    }

    uint32 GetSubsequentActionTimer(uint32 id)
    {
        switch (id)
        {
            // Mutagen Injection is used more often when below 30% HP
            case GROBBULUS_INJECTION:
            {
                if (m_bIsRegularMode)
                    return urand(10 * IN_MILLISECONDS, 13 * IN_MILLISECONDS) -  5 * (m_uiBerserkTimeSecs * IN_MILLISECONDS - m_uiBerserkTimer) / m_uiBerserkTimeSecs;
                else
                    return urand(10 * IN_MILLISECONDS, 13 * IN_MILLISECONDS) -  8 * (m_uiBerserkTimeSecs * IN_MILLISECONDS - m_uiBerserkTimer) / m_uiBerserkTimeSecs;
            }
            case GROBBULUS_POISON_CLOUD: return 15u * IN_MILLISECONDS;
            case GROBBULUS_SLIME_SPRAY: return urand(20, 30) * IN_MILLISECONDS;
            case GROBBULUS_CHECK_MELEE: return 1u * IN_MILLISECONDS;
            default: return 0;
        }
    }

    void ExecuteAction(uint32 action) override
    {
        switch (action)
        {
            case GROBBULUS_CHECK_MELEE:
            {
                if (!m_creature->CanReachWithMeleeAttack(m_creature->GetVictim()))
                {
                    ResetCombatAction(GROBBULUS_SLIME_STREAM, 3u * IN_MILLISECONDS);
                    DisableCombatAction(action);
                    break;
                }
                ResetCombatAction(action, GetSubsequentActionTimer(action));
            }
            case GROBBULUS_SLIME_STREAM:
            {
                if (!m_creature->CanReachWithMeleeAttack(m_creature->GetVictim()))
                    DoCastSpellIfCan(m_creature, SPELL_SLIME_STREAM, CAST_TRIGGERED);
                else
                    ResetCombatAction(GROBBULUS_CHECK_MELEE, GetSubsequentActionTimer(GROBBULUS_CHECK_MELEE));
                break;
            }
            case GROBBULUS_INJECTION:
            {
                if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, SPELL_MUTATING_INJECTION, SELECT_FLAG_PLAYER | SELECT_FLAG_NOT_AURA))
                {
                    if (DoCastSpellIfCan(target, SPELL_MUTATING_INJECTION, CAST_TRIGGERED) == CAST_OK)
                        ResetCombatAction(action, GetSubsequentActionTimer(action));
                }
                break;
            }
            case GROBBULUS_POISON_CLOUD:
            {
                if (DoCastSpellIfCan(m_creature, SPELL_POISON_CLOUD) == CAST_OK)
                    ResetCombatAction(action, GetSubsequentActionTimer(action));
                break;
            }
            case GROBBULUS_SLIME_SPRAY:
            {
                if (DoCastSpellIfCan(m_creature->GetVictim(), m_bIsRegularMode ? SPELL_SLIME_SPRAY : SPELL_SLIME_SPRAY_H) == CAST_OK)
                {
                    ResetCombatAction(action, GetSubsequentActionTimer(action));
                    DoBroadcastText(EMOTE_SPRAY_SLIME, m_creature);
                }
                break;
            }
            case GROBBULUS_BERSERK:
            {
                if (DoCastSpellIfCan(m_creature, SPELL_BERSERK) == CAST_OK)
                    DisableCombatAction(action);
                break;
            }
        }
    }
};

UnitAI* GetAI_boss_grobbulus(Creature* pCreature)
{
    return new boss_grobbulusAI(pCreature);
}

struct MutatingInjection : public AuraScript
{
    void OnApply(Aura* aura, bool apply) const override
    {
        if (!apply)
        {
            Unit* target = aura->GetTarget();
            if (!target)
                return;

            if (aura->GetRemoveMode() == AURA_REMOVE_BY_EXPIRE)
                // Embalming Cloud
                target->CastSpell(target, SPELL_EMBALMING_CLOUD, TRIGGERED_OLD_TRIGGERED, nullptr, aura);
            else // Removed by dispel
                // Mutagen Explosion
                target->CastSpell(target, SPELL_MUTAGEN_EXPLOSION, TRIGGERED_OLD_TRIGGERED, nullptr, aura);
            // Poison Cloud
            target->CastSpell(target, SPELL_POISON_CLOUD, TRIGGERED_OLD_TRIGGERED, nullptr, aura);
        }
    }
};

void AddSC_boss_grobbulus()
{
    Script* pNewScript = new Script;
    pNewScript->Name = "boss_grobbulus";
    pNewScript->GetAI = &GetAI_boss_grobbulus;
    pNewScript->RegisterSelf();

    RegisterSpellScript<MutatingInjection>("spell_grobbulus_mutating_injection");
}
