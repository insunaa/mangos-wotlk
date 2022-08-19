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
SDName: Boss_Gluth
SD%Complete: 95
SDComment: Gluth should turn around to face the victim when he devours a Zombie
SDCategory: Naxxramas
EndScriptData */

#include "AI/ScriptDevAI/base/CombatAI.h"
#include "AI/ScriptDevAI/include/sc_common.h"
#include "MotionGenerators/MotionMaster.h"
#include "naxxramas.h"

enum
{
    EMOTE_ZOMBIE                    = -1533119,
    EMOTE_BOSS_GENERIC_ENRAGED      = -1000006,
    EMOTE_DECIMATE                  = -1533152,

    SPELL_DOUBLE_ATTACK             = 19818,
    SPELL_MORTALWOUND               = 54378,                // old vanilla spell was 25646,
    SPELL_DECIMATE                  = 28374,
    SPELL_DECIMATE_H                = 54426,
    SPELL_ENRAGE                    = 28371,
    SPELL_ENRAGE_H                  = 54427,
    SPELL_BERSERK                   = 26662,
    // SPELL_TERRIFYING_ROAR         = 29685,               // no longer used in 3.x.x
    // SPELL_SUMMON_ZOMBIE_CHOW      = 28216,               // removed from dbc: triggers 28217 every 6 secs
    // SPELL_CALL_ALL_ZOMBIE_CHOW    = 29681,               // removed from dbc: triggers 29682
    // SPELL_ZOMBIE_CHOW_SEARCH      = 28235,               // removed from dbc: triggers 28236 every 3 secs
    SPELL_ZOMBIE_CHOW_SEARCH_INSTAKILL_TARGETED = 28239,    // Add usage
    SPELL_ZOMBIE_CHOW_SEARCH_INSTAKILL_AOE = 28404,

    NPC_ZOMBIE_CHOW                 = 16360,                // old vanilla summoning spell 28217

    MAX_ZOMBIE_LOCATIONS            = 3,
};

static const float aZombieSummonLoc[MAX_ZOMBIE_LOCATIONS][3] =
{
    {3267.9f, -3172.1f, 297.42f},
    {3253.2f, -3132.3f, 297.42f},
    {3308.3f, -3185.8f, 297.42f},
};

enum GluthActions
{
    GLUTH_MORTAL_WOUND,
    GLUTH_DECIMATE,
    GLUTH_ENRAGE,
    GLUTH_SUMMON_ZOMBIE_CHOW,
    GLUTH_ZOMBIE_CHOW_SEARCH,
    GLUTH_BERSERK,
    GLUTH_ACTIONS_MAX,
};

struct boss_gluthAI : public BossAI
{
    boss_gluthAI(Creature* creature) : BossAI(creature, GLUTH_ACTIONS_MAX), 
    m_instance(static_cast<instance_naxxramas*>(creature->GetInstanceData())),
    m_isRegularMode(creature->GetMap()->IsRegularDifficulty())
    {
        SetDataType(TYPE_GLUTH);
        AddCombatAction(GLUTH_MORTAL_WOUND, 10s);
        AddCombatAction(GLUTH_DECIMATE, 110s);
        AddCombatAction(GLUTH_ENRAGE, 25s);
        AddCombatAction(GLUTH_SUMMON_ZOMBIE_CHOW, 15s);
        AddCombatAction(GLUTH_ZOMBIE_CHOW_SEARCH, 3s);
        AddCombatAction(GLUTH_BERSERK, 8min);
    }

    instance_naxxramas* m_instance;
    bool m_isRegularMode;
    GuidList m_lZombieChowGuidList;

    void Reset() override
    {
        DoCastSpellIfCan(m_creature, SPELL_DOUBLE_ATTACK, CAST_TRIGGERED | CAST_AURA_NOT_PRESENT);
    }

    std::chrono::milliseconds GetSubsequentActionTimer(uint32 action)
    {
        switch (action)
        {
            case GLUTH_MORTAL_WOUND: return 10s;
            case GLUTH_DECIMATE: return 100s;
            case GLUTH_ENRAGE: return RandomTimer(20s, 30s);
            case GLUTH_SUMMON_ZOMBIE_CHOW: return 10s;
            case GLUTH_ZOMBIE_CHOW_SEARCH: return 3s;
            case GLUTH_BERSERK: return 5min;
        }
        return 0s;
    }

    void KilledUnit(Unit* pVictim) override
    {
        // Restore 5% hp when killing a zombie
        if (pVictim->GetEntry() == NPC_ZOMBIE_CHOW)
        {
            DoScriptText(EMOTE_ZOMBIE, m_creature);
            m_creature->SetHealth(m_creature->GetHealth() + m_creature->GetMaxHealth() * 0.05f);
        }
    }

    void JustSummoned(Creature* pSummoned) override
    {
        pSummoned->GetMotionMaster()->MoveChase(m_creature, ATTACK_DISTANCE);
        pSummoned->getThreatManager().addThreat(m_creature, 20.f);
        m_lZombieChowGuidList.push_back(pSummoned->GetObjectGuid());
    }

    void SummonedCreatureDespawn(Creature* pSummoned) override
    {
        m_lZombieChowGuidList.remove(pSummoned->GetObjectGuid());
    }

    // Replaces missing spell 29682
    void DoCallAllZombieChow()
    {
        for (GuidList::const_iterator itr = m_lZombieChowGuidList.begin(); itr != m_lZombieChowGuidList.end(); ++itr)
        {
            if (Creature* pZombie = m_creature->GetMap()->GetCreature(*itr))
            {
                if (!pZombie->IsAlive())
                    continue;
                pZombie->AttackStop(true, true);
                pZombie->AI()->SetReactState(REACT_PASSIVE);
                pZombie->GetMotionMaster()->MovePoint(1, m_creature->GetPosition(), FORCED_MOVEMENT_WALK);
            }
        }
        ResetCombatAction(GLUTH_SUMMON_ZOMBIE_CHOW, 15s);
    }

    // Replaces missing spell 28236
    void DoSearchZombieChow()
    {
        CreatureList zombiesInRange(m_lZombieChowGuidList.size());
        GetCreatureListWithEntryInGrid(zombiesInRange, m_creature, NPC_ZOMBIE_CHOW, 10.f);
        for (auto zombieItr = zombiesInRange.begin();zombieItr != zombiesInRange.end();)
        {
            if (!(*zombieItr) || !(*zombieItr)->IsAlive())
                zombieItr = zombiesInRange.erase(zombieItr);
            else
                zombieItr++;
        }

        if (zombiesInRange.empty())
            return;
        else if(zombiesInRange.size() == 1)
        {
            Creature* pZombie = zombiesInRange.front();
            if (!pZombie)
                return;
            m_creature->GetMotionMaster()->MoveCharge(*pZombie, 15.f, EVENT_CHARGE);
            DoCastSpellIfCan(pZombie, SPELL_ZOMBIE_CHOW_SEARCH_INSTAKILL_TARGETED);
            return;
        }
        m_creature->CastSpell(nullptr, SPELL_ZOMBIE_CHOW_SEARCH_INSTAKILL_AOE, TRIGGERED_OLD_TRIGGERED);
    }

    void ExecuteAction(uint32 action) override
    {
        switch (action)
        {
            case GLUTH_ZOMBIE_CHOW_SEARCH:
                DoSearchZombieChow();
                break;
            case GLUTH_SUMMON_ZOMBIE_CHOW:
                {
                    uint8 uiPos1 = urand(0, MAX_ZOMBIE_LOCATIONS - 1);
                    m_creature->SummonCreature(NPC_ZOMBIE_CHOW, aZombieSummonLoc[uiPos1][0], aZombieSummonLoc[uiPos1][1], aZombieSummonLoc[uiPos1][2], 0.0f, TEMPSPAWN_DEAD_DESPAWN, 0);

                    if (!m_isRegularMode)
                    {
                        uint8 uiPos2 = (uiPos1 + urand(1, MAX_ZOMBIE_LOCATIONS - 1)) % MAX_ZOMBIE_LOCATIONS;
                        m_creature->SummonCreature(NPC_ZOMBIE_CHOW, aZombieSummonLoc[uiPos2][0], aZombieSummonLoc[uiPos2][1], aZombieSummonLoc[uiPos2][2], 0.0f, TEMPSPAWN_DEAD_DESPAWN, 0);
                    }
                    break;
                }
            case GLUTH_MORTAL_WOUND:
                if (DoCastSpellIfCan(m_creature->GetVictim(), SPELL_MORTALWOUND) == CAST_OK)
                    break;
                return;
            case GLUTH_ENRAGE:
                if (DoCastSpellIfCan(m_creature, m_isRegularMode ? SPELL_ENRAGE : SPELL_ENRAGE_H) == CAST_OK)
                {
                    DoScriptText(EMOTE_BOSS_GENERIC_ENRAGED, m_creature);
                    break;
                }
                return;
            case GLUTH_DECIMATE:
                if (DoCastSpellIfCan(m_creature, m_isRegularMode ? SPELL_DECIMATE : SPELL_DECIMATE_H) == CAST_OK)
                {
                    DoScriptText(EMOTE_DECIMATE, m_creature);
                    DoCallAllZombieChow();
                    break;
                }
                return;
            case GLUTH_BERSERK:
                if (DoCastSpellIfCan(m_creature, SPELL_BERSERK) == CAST_OK)
                    break;
                return;
        }
        ResetCombatAction(action, GetSubsequentActionTimer(action));
    }
};

struct EatZombieChowAOE : SpellScript
{
    bool OnCheckTarget(const Spell* spell, Unit* target, SpellEffectIndex /*eff*/) const override
    {
        if (target->GetEntry() != NPC_ZOMBIE_CHOW)
            return false;
        return true;
    }
};

void AddSC_boss_gluth()
{
    Script* pNewScript = new Script;
    pNewScript->Name = "boss_gluth";
    pNewScript->GetAI = &GetNewAIInstance<boss_gluthAI>;
    pNewScript->RegisterSelf();

    RegisterSpellScript<EatZombieChowAOE>("spell_zombie_chow_search_instakill_aoe");
}
