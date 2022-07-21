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
SDName: Boss_Maexxna
SD%Complete: 100
SDComment:
SDCategory: Naxxramas
EndScriptData */

#include "AI/ScriptDevAI/base/CombatAI.h"
#include "AI/ScriptDevAI/base/TimerAI.h"
#include "AI/ScriptDevAI/include/sc_common.h"
#include "naxxramas.h"

enum
{
    // Maexxna Spells

    // Web Wrap spells
    SPELL_WEB_WRAP_200          = 28618,
    SPELL_WEB_WRAP_300          = 28619,
    SPELL_WEB_WRAP_400          = 28620,
    SPELL_WEB_WRAP_500          = 28621,
    SPELL_WEBWRAP_STUN          = 28622,                    // Triggered by spells 28618 - 28621
    SPELL_WEB_WRAP_SUMMON       = 28627,
    SPELL_CLEAR_WEB_WRAP_TARGET = 28628,
    SPELL_CLEAR_WEB_WRAP_SELF   = 28629,
    SPELL_SELF_STUN             = 29826,                    // Spell Id unsure
    SPELL_KILL_WEBWRAP          = 29108,

    NPC_INVISIBLE_MAN           = 17286,                    // Handle the summoning of the players and Web Wrap NPCs

    MAX_PLAYERS_WEB_WRAP_H      = 2,
    MAX_PLAYERS_WEB_WRAP        = 1,

    TIMER_40_SEC                = 40000,                    // Used by all main abilities

    EMOTE_SPIN_WEB              = -1533146,
    EMOTE_SPIDERLING            = -1533147,
    EMOTE_SPRAY                 = -1533148,
    EMOTE_BOSS_GENERIC_FRENZY   = -1000005,

    SPELL_DOUBLE_ATTACK         = 19818,

    SPELL_WEB_WRAP_INIT         = 28617,
    SPELL_WEBWRAP               = 28622,
    //SPELL_WEBWRAP_2             = 28673,                    // purpose unknown

    SPELL_WEBSPRAY              = 29484,
    SPELL_WEBSPRAY_H            = 54125,

    SPELL_POISONSHOCK           = 28741,
    SPELL_POISONSHOCK_H         = 54122,

    SPELL_NECROTICPOISON_H      = 28776,
    SPELL_NECROTICPOISON        = 54121,

    SPELL_FRENZY                = 54123,
    SPELL_FRENZY_H              = 54124,

    NPC_WEB_WRAP                = 16486,
    NPC_SPIDERLING              = 17055,

    MAX_SPIDERLINGS             = 8,
    MAX_WEB_WRAP_POSITIONS      = 3,
};

/*###################
#   npc_web_wrap
###################*/

// This NPC is summoned by the web wrapped player and act as a visual target for other raid members to free the player
/*struct npc_web_wrapAI : public ScriptedAI
{
    npc_web_wrapAI(Creature* creature) : ScriptedAI(creature) { Reset(); }

    Player* m_player;
    uint32 m_checkVictimAliveTimer;

    void Reset() override
    {
        SetCombatMovement(false);
        m_player = nullptr;
        m_checkVictimAliveTimer = 1 * IN_MILLISECONDS;

        DoCastSpellIfCan(m_creature, SPELL_SELF_STUN, CAST_TRIGGERED);
    }

    void JustDied(Unit* killer) override
    {
        // Remove polymporph and DoT auras from web wrapped player
        if (m_player)
        {
            if (m_player->IsAlive())
                DoCastSpellIfCan(m_player, SPELL_CLEAR_WEB_WRAP_TARGET, CAST_TRIGGERED);
        }
        m_creature->ForcedDespawn(5000);
    }

    void UpdateAI(const uint32 diff) override
    {
        // The nearest player is the one that summoned the NPC
        if (!m_player)
        {
            PlayerList closestPlayersList;
            GetPlayerListWithEntryInWorld(closestPlayersList, m_creature, 2.0f);
            if (!closestPlayersList.empty())
                m_player = closestPlayersList.front();
        }

        if (m_player)
        {
            // Check if the web wrapped player is still alive, if not, clear ourselves
            if (m_checkVictimAliveTimer <= diff)
            {
                if (!m_player->IsAlive())
                    DoCastSpellIfCan(m_creature, SPELL_CLEAR_WEB_WRAP_SELF, CAST_TRIGGERED);
                m_checkVictimAliveTimer = 1 * IN_MILLISECONDS;
            }
            else
                m_checkVictimAliveTimer -= diff;
        }
    }
};
*/
struct npc_web_wrapAI : public ScriptedAI
{
    npc_web_wrapAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    ObjectGuid m_victimGuid;
    uint32 m_uiWebWrapTimer;

    void Reset() override
    {
        m_uiWebWrapTimer = 0;
    }

    void MoveInLineOfSight(Unit* /*pWho*/) override {}
    void AttackStart(Unit* /*pWho*/) override {}

    void SetVictim(Unit* pVictim)
    {
        if (pVictim)
        {
            // Vanilla spell 28618, 28619, 28620, 28621 had effect SPELL_EFFECT_PULL_TOWARDS with EffectMiscValue = 200, 300, 400 and 500
            // All these spells trigger 28622 after 1 or 2 seconds
            // the EffectMiscValue may have been based on the distance between the victim and the target

            // NOTE: This implementation may not be 100% correct, but it gets very close to the expected result

            float fDist = m_creature->GetDistance(pVictim, false);
            // Switch the speed multiplier based on the distance from the web wrap
            uint32 uiEffectMiscValue = 500;
            if (fDist < 25.0f)
                uiEffectMiscValue = 200;
            else if (fDist < 50.0f)
                uiEffectMiscValue = 300;
            else if (fDist < 75.0f)
                uiEffectMiscValue = 400;

            // Note: normally we should use the Knockback effect to handle this, but because this doesn't behave as expected we'll just use Jump Movement
            // pVictim->KnockBackFrom(m_creature, -fDist, uiEffectMiscValue * 0.1f);

            float fSpeed = fDist * (uiEffectMiscValue * 0.01f);
            pVictim->GetMotionMaster()->MoveJump(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), fSpeed, 0.0f);

            m_victimGuid = pVictim->GetObjectGuid();
            m_uiWebWrapTimer = uiEffectMiscValue == 200 ? 1000 : 2000;
        }
    }

    void JustDied(Unit* /*pKiller*/) override
    {
        if (m_victimGuid)
        {
            if (Player* pVictim = m_creature->GetMap()->GetPlayer(m_victimGuid))
            {
                if (pVictim->IsAlive())
                    pVictim->RemoveAurasDueToSpell(SPELL_WEBWRAP);
            }
        }
    }

    void UpdateAI(const uint32 uiDiff) override
    {
        if (m_uiWebWrapTimer)
        {
            // Finally the player gets web wrapped and he should change the display id until the creature is killed
            if (m_uiWebWrapTimer <= uiDiff)
            {
                if (Player* pVictim = m_creature->GetMap()->GetPlayer(m_victimGuid))
                    pVictim->CastSpell(pVictim, SPELL_WEBWRAP, TRIGGERED_OLD_TRIGGERED, nullptr, nullptr, m_creature->GetObjectGuid());

                m_uiWebWrapTimer = 0;
            }
            else
                m_uiWebWrapTimer -= uiDiff;
        }
    }
};


/*###################
#   boss_maexxna
###################*/

enum MaexxnaActions
{
    MAEXXNA_WEBWRAP,
    MAEXXNA_WEBSPRAY,
    MAEXXNA_POISON_SHOCK,
    MAEXXNA_NECROTIC_POISON,
    MAEXXNA_SUMMON_SPIDERLING,
    MAEXXNA_ENRAGE_HP_CHECK,
    MAEXXNA_ACTION_MAX,
};

static const float aWebWrapLoc[MAX_WEB_WRAP_POSITIONS][3] =
{
    {3546.796f, -3869.082f, 296.450f},
    {3531.271f, -3847.424f, 299.450f},
    {3497.067f, -3843.384f, 302.384f}
};

struct boss_maexxnaAI : public CombatAI
{
    boss_maexxnaAI(Creature* creature) : CombatAI(creature, MAEXXNA_ACTION_MAX), m_instance(static_cast<ScriptedInstance*>(creature->GetInstanceData()))
    {
        m_isRegularMode = creature->GetMap()->IsRegularDifficulty();
        AddTimerlessCombatAction(MAEXXNA_ENRAGE_HP_CHECK, true);         // Soft enrage à 30%
        AddCombatAction(MAEXXNA_WEBWRAP, 20000u);
        AddCombatAction(MAEXXNA_WEBSPRAY, 40000u);
        AddCombatAction(MAEXXNA_POISON_SHOCK, 10000u, 20000u);
        AddCombatAction(MAEXXNA_NECROTIC_POISON, 20000u, 30000u);
        AddCombatAction(MAEXXNA_SUMMON_SPIDERLING, 30000u);
    }

    ScriptedInstance* m_instance;
    bool m_isRegularMode;

    CreatureList m_summoningTriggers;
    SelectAttackingTargetParams m_webWrapParams;

    void Reset() override
    {
        CombatAI::Reset();

        m_webWrapParams.range.minRange = 0;
        m_webWrapParams.range.maxRange = 100;

        DoCastSpellIfCan(m_creature, SPELL_DOUBLE_ATTACK, CAST_TRIGGERED | CAST_AURA_NOT_PRESENT);
    }

    uint32 GetSubsequentActionTimer(uint32 action)
    {
        switch (action)
        {
            case MAEXXNA_WEBWRAP: //fall through
            case MAEXXNA_WEBSPRAY: return 40u * IN_MILLISECONDS;
            case MAEXXNA_POISON_SHOCK: return urand(10u, 20u) * IN_MILLISECONDS;
            case MAEXXNA_NECROTIC_POISON: return urand(20u, 30u) * IN_MILLISECONDS;
            case MAEXXNA_SUMMON_SPIDERLING: return 30u * IN_MILLISECONDS;
            default: return 0;
        }
    }

    void Aggro(Unit* /*who*/) override
    {
        if (m_instance)
            m_instance->SetData(TYPE_MAEXXNA, IN_PROGRESS);
        else
            return;

        // Fill the list of summoning NPCs for the Web Wrap ability
        GetCreatureListWithEntryInGrid(m_summoningTriggers, m_creature, NPC_INVISIBLE_MAN, 100.0f);
    }

    void JustDied(Unit* /*killer*/) override
    {
        if (m_instance)
            m_instance->SetData(TYPE_MAEXXNA, DONE);
    }

    void JustReachedHome() override
    {
        if (m_instance)
            m_instance->SetData(TYPE_MAEXXNA, FAIL);

        DoCastSpellIfCan(m_creature, SPELL_DOUBLE_ATTACK, CAST_TRIGGERED | CAST_AURA_NOT_PRESENT);
    }

    void JustSummoned(Creature* summoned) override
    {
        if (summoned->GetEntry() == NPC_SPIDERLING)
            summoned->SetInCombatWithZone();
        if (summoned->GetEntry() == NPC_WEB_WRAP)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1, SPELL_WEBWRAP, SELECT_FLAG_PLAYER))
            {
                if (npc_web_wrapAI* pWebAI = dynamic_cast<npc_web_wrapAI*>(summoned->AI()))
                    pWebAI->SetVictim(pTarget);
            }
        }
    }

    void SummonedJustReachedHome(Creature* summoned) override
    {
        summoned->ForcedDespawn();
    }

    void SummonSpiderlings()
    {
        for (uint8 i = 0; i < MAX_SPIDERLINGS; ++i)
            m_creature->SummonCreature(NPC_SPIDERLING, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSPAWN_TIMED_OOC_OR_DEAD_DESPAWN, 60000);
    }

    bool DoCastWebWrap()
    {
        // If we can't select a player for web wrap then skip the summoning
        if (!m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1, uint32(0), SELECT_FLAG_PLAYER))
            return false;

        uint8 uiPos1 = urand(0, MAX_WEB_WRAP_POSITIONS - 1);
        m_creature->SummonCreature(NPC_WEB_WRAP, aWebWrapLoc[uiPos1][0], aWebWrapLoc[uiPos1][1], aWebWrapLoc[uiPos1][2], 0, TEMPSPAWN_TIMED_OOC_OR_DEAD_DESPAWN, 60000);

        // Summon a second web wrap on heroic
        if (!m_isRegularMode)
        {
            uint8 uiPos2 = (uiPos1 + urand(1, MAX_WEB_WRAP_POSITIONS - 1)) % MAX_WEB_WRAP_POSITIONS;
            m_creature->SummonCreature(NPC_WEB_WRAP, aWebWrapLoc[uiPos2][0], aWebWrapLoc[uiPos2][1], aWebWrapLoc[uiPos2][2], 0, TEMPSPAWN_TIMED_OOC_OR_DEAD_DESPAWN, 60000);
        }

        return true;
    }

    void ExecuteAction(uint32 action) override
    {
        switch (action)
        {
            case MAEXXNA_ENRAGE_HP_CHECK:
            {
                if (m_creature->GetHealthPercent() < 30.0f)
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_FRENZY) == CAST_OK)
                    {
                        DoScriptText(EMOTE_BOSS_GENERIC_FRENZY, m_creature);
                        DisableCombatAction(action);
                    }
                }
                break;
            }
            case MAEXXNA_WEBWRAP:
            {
                // Web Wrap
                // ToDo: the targets and triggers selection is probably done through spells 29280, 29281, 29282, 29283, 29285 & 29287
                // But because we are walking in Guessland until more reliable data are available, let's do the selection manually here
                // Randomly pick 2 targets on 25, 1 on 10, excluding current victim (main tank)
                /*std::vector<Unit*> targets;
                m_creature->SelectAttackingTargets(targets, ATTACKING_TARGET_ALL_SUITABLE, 0, nullptr, SELECT_FLAG_PLAYER | SELECT_FLAG_SKIP_TANK, m_webWrapParams);

                uint32 t_webWrap = m_isRegularMode ? MAX_PLAYERS_WEB_WRAP : MAX_PLAYERS_WEB_WRAP_H;
                if (targets.size() > t_webWrap)
                {
                    std::shuffle(targets.begin(), targets.end(), *GetRandomGenerator());
                    targets.resize(t_webWrap);
                }

                if (!targets.empty())
                {
                    // Check we have enough summoning NPCs spawned in regards of player targets
                    if (m_summoningTriggers.size() < targets.size())
                    {
                        script_error_log("Error in script Naxxramas::boss_maexxna: less summoning NPCs (entry %u) than expected targets (%u) for Web Wrap ability. Check your DB", NPC_INVISIBLE_MAN, t_webWrap);
                        break;
                    }

                    // Randomly pick up to the trigger NPCs
                    std::vector<Unit*> invisibleMen(m_summoningTriggers.begin(), m_summoningTriggers.end());
                    std::shuffle(invisibleMen.begin(), invisibleMen.end(), *GetRandomGenerator());
                    invisibleMen.resize(targets.size());

                    for (uint8 i = 0; i < targets.size(); i++)
                        targets[i]->CastSpell(invisibleMen[i], SPELL_WEB_WRAP_INIT, TRIGGERED_IGNORE_UNSELECTABLE_FLAG, nullptr, nullptr, m_creature->GetObjectGuid());

                    ResetCombatAction(action, GetSubsequentActionTimer(action));
                    break;
                }*/
                if (DoCastWebWrap())
                    ResetCombatAction(action, GetSubsequentActionTimer(action));
            }
            case MAEXXNA_WEBSPRAY:
            {
                if (DoCastSpellIfCan(m_creature, m_isRegularMode ? SPELL_WEBSPRAY : SPELL_WEBSPRAY_H) == CAST_OK)
                    ResetCombatAction(action, GetSubsequentActionTimer(action));
                break;
            }
            case MAEXXNA_POISON_SHOCK:
            {
                if (DoCastSpellIfCan(m_creature->GetVictim(), m_isRegularMode ? SPELL_POISONSHOCK : SPELL_POISONSHOCK_H) == CAST_OK)
                    ResetCombatAction(action, GetSubsequentActionTimer(action));
                break;
            }
            case MAEXXNA_NECROTIC_POISON:
            {
                if (DoCastSpellIfCan(m_creature->GetVictim(), m_isRegularMode ? SPELL_NECROTICPOISON : SPELL_NECROTICPOISON_H) == CAST_OK)
                    ResetCombatAction(action, GetSubsequentActionTimer(action));
                break;
            }
            case MAEXXNA_SUMMON_SPIDERLING:
            {
                SummonSpiderlings();
                ResetCombatAction(action, GetSubsequentActionTimer(action));
            }
            default:
                break;
        }
    }
};

/*###################
#   npc_invible_man
###################*/

// This NPC handles the spell sync between the player that is web wrapped (with a DoT) and the related Web Wrap NPC
/*struct npc_invisible_manAI : public ScriptedAI
{
    npc_invisible_manAI(Creature* creature) : ScriptedAI(creature) {
        AddCustomAction(1, true, [&]() { HandleVictimCheck(); });
        SetReactState(REACT_PASSIVE);
        Reset();
    }

    ObjectGuid m_victimGuid;

    void Reset() override
    {
        m_victimGuid.Clear();
    }

    void HandleVictimCheck() {
        if (Player* victim = m_creature->GetMap()->GetPlayer(m_victimGuid))
        {
            if (victim->IsAlive())
            {
                // Make the player cast the visual effects spells with a delay to ensure he/she has reach his/her destination
                victim->CastSpell(victim, SPELL_WEB_WRAP_SUMMON, TRIGGERED_OLD_TRIGGERED);
                DisableTimer(1);
            }
            else
                m_victimGuid.Clear();
        }
    };

    void MoveInLineOfSight(Unit* who) override {}
    void AttackStart(Unit* who) override {}

    // Store the GUID of the player that was pulled for later use
    void SpellHitTarget(Unit* target, const SpellEntry* spell) override
    {
        switch (spell->Id)
        {
            case SPELL_WEB_WRAP_200:
            case SPELL_WEB_WRAP_300:
            case SPELL_WEB_WRAP_400:
            case SPELL_WEB_WRAP_500:
            {
                m_victimGuid = target->GetObjectGuid();
                ResetTimer(1, (spell->Id == SPELL_WEB_WRAP_200 ? 5 : 6) * IN_MILLISECONDS);
                break;
            }
            default:
                break;
        }
    }
};
*/
// Web Wrap (Maexxna: pull spell initialiser)
/*struct WebWrap : public SpellScript
{
    void OnEffectExecute(Spell* spell, SpellEffectIndex effIdx) const override
    {
        if (effIdx == EFFECT_INDEX_0)
        {
            if (!spell->GetAffectiveCasterObject())
                return;

            Unit* unitTarget = spell->GetUnitTarget();
            if (!unitTarget || unitTarget->GetTypeId() != TYPEID_UNIT || spell->GetCaster()->GetTypeId() != TYPEID_PLAYER)
                return;

            float dist = spell->GetCaster()->GetDistance(unitTarget, false);
            // Switch the pull target spell based on the distance from the web wrap position
            uint32 pullSpellId = SPELL_WEB_WRAP_500;
            if (dist < 25.0f)
                pullSpellId = SPELL_WEB_WRAP_200;
            else if (dist < 50.0f)
                pullSpellId = SPELL_WEB_WRAP_300;
            else if (dist < 75.0f)
                pullSpellId = SPELL_WEB_WRAP_400;

            unitTarget->CastSpell(spell->GetCaster(), pullSpellId, TRIGGERED_INSTANT_CAST, nullptr, nullptr, spell->GetAffectiveCasterObject()->GetObjectGuid());
        }
    }
};

// Clear Web Wrap
struct ClearWebWrap : public SpellScript
{
    void OnEffectExecute(Spell* spell, SpellEffectIndex effIdx) const override
    {
        if (effIdx == EFFECT_INDEX_0)
        {
            Unit* unitTarget = spell->GetUnitTarget();
            switch(spell->m_spellInfo->Id)
            {
                case SPELL_CLEAR_WEB_WRAP_TARGET:   // Clear Web Wrap (Maexxna: clear effects on player)
                {
                    if (unitTarget && unitTarget->GetTypeId() == TYPEID_PLAYER)
                    {
                        unitTarget->RemoveAurasDueToSpell(SPELL_WEB_WRAP_SUMMON);   // Web Wrap polymorph
                        unitTarget->RemoveAurasDueToSpell(SPELL_WEBWRAP_STUN);      // Web Wrap stun and DoT
                    }
                    break;
                }
                case SPELL_CLEAR_WEB_WRAP_SELF:     // Clear Web Wrap (Maexxna: kill Web Wrap NPC)
                {
                    if (unitTarget && unitTarget->GetTypeId() == TYPEID_UNIT)
                        unitTarget->CastSpell(nullptr, SPELL_KILL_WEBWRAP, TRIGGERED_OLD_TRIGGERED);  // Kill Web Wrap
                    break;
                }
                default:
                    break;
            }
        }
    }
};*/

void AddSC_boss_maexxna()
{
    Script* newScript = new Script;
    newScript->Name = "boss_maexxna";
    newScript->GetAI = &GetNewAIInstance<boss_maexxnaAI>;
    newScript->RegisterSelf();

    newScript = new Script;
    newScript->Name = "npc_web_wrap";
    newScript->GetAI = &GetNewAIInstance<npc_web_wrapAI>;
    newScript->RegisterSelf();

/*    newScript = new Script;
    newScript->Name = "npc_invible_man";
    newScript->GetAI = &GetNewAIInstance<npc_invisible_manAI>;
    newScript->RegisterSelf();*/

//    RegisterSpellScript<WebWrap>("spell_web_wrap");
//    RegisterSpellScript<ClearWebWrap>("spell_clear_web_wrap");
}
