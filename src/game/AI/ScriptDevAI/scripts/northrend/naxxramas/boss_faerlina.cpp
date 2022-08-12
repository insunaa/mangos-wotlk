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
SDName: Boss_Faerlina
SD%Complete: 100
SDComment:
SDCategory: Naxxramas
EndScriptData */

#include "AI/ScriptDevAI/base/CombatAI.h"
#include "AI/ScriptDevAI/include/sc_common.h"
#include "naxxramas.h"

enum
{
    SAY_GREET                   = 12852,
    SAY_DEATH                   = 12853,
    SAY_SLAY_1                  = 12854,
    SAY_SLAY_2                  = 12855,
    SAY_AGGRO_1                 = 12856,
    SAY_AGGRO_2                 = 12857,
    SAY_AGGRO_3                 = 12858,
    SAY_AGGRO_4                 = 12859,

    EMOTE_BOSS_GENERIC_FRENZY   = 1191,

    // SOUND_RANDOM_AGGRO       = 8955,                     // soundId containing the 4 aggro sounds, we not using this

    SPELL_POISONBOLT_VOLLEY     = 28796,
    SPELL_POISONBOLT_VOLLEY_H   = 54098,
    SPELL_ENRAGE                = 28798,
    SPELL_ENRAGE_H              = 54100,
    SPELL_RAIN_OF_FIRE          = 28794,
    SPELL_RAIN_OF_FIRE_H        = 54099,
    SPELL_WIDOWS_EMBRACE        = 28732,
    SPELL_WIDOWS_EMBRACE_H      = 54097,
};

enum FaerlinaActions
{
    FAERLINA_POISON_BOLT,
    FAERLINA_RAIN_OF_FIRE,
    FAERLINA_ENRAGE,
    FAERLINA_ACTIONS_MAX,
    FAERLINA_ACHIEVEMENT_TIMER,
};

struct boss_faerlinaAI : public BossAI
{
    boss_faerlinaAI(Creature* creature) : BossAI(creature, FAERLINA_ACTIONS_MAX),
    m_instance(static_cast<instance_naxxramas *>(creature->GetInstanceData())),
    m_hasTaunted(false)
    {
        SetDataType(TYPE_FAERLINA);
        AddOnAggroText(SAY_AGGRO_1, SAY_AGGRO_2, SAY_AGGRO_3, SAY_AGGRO_4);
        AddOnKillText(SAY_SLAY_1, SAY_SLAY_2);
        AddOnDeathText(SAY_DEATH);
        m_isRegularMode = creature->GetMap()->IsRegularDifficulty();
        AddCombatAction(FAERLINA_POISON_BOLT, 8s);
        AddCombatAction(FAERLINA_RAIN_OF_FIRE, 16s);
        AddCombatAction(FAERLINA_ENRAGE, 60s);
        AddCustomAction(FAERLINA_ACHIEVEMENT_TIMER, true, [&](){
            m_gracePeriodActive = false;
        });
    }

    instance_naxxramas* m_instance;
    bool m_isRegularMode;
    bool m_hasTaunted;
    bool m_gracePeriodActive;

    void Aggro(Unit* who) override
    {
        CombatAI::Aggro(who);
        m_gracePeriodActive = true;
        ResetTimer(FAERLINA_ACHIEVEMENT_TIMER, 30s);
    }

    void MoveInLineOfSight(Unit* pWho) override
    {
        if (!m_hasTaunted && pWho->GetTypeId() == TYPEID_PLAYER && m_creature->IsWithinDistInMap(pWho, 80.0f) &&  m_creature->IsWithinLOSInMap(pWho))
        {
            DoBroadcastText(SAY_GREET, m_creature);
            m_hasTaunted = true;
        }

        ScriptedAI::MoveInLineOfSight(pWho);
    }

    // Widow's Embrace prevents frenzy and poison bolt, if it removes frenzy, next frenzy is sceduled in 60s
    // It is likely that this _should_ be handled with some dummy aura(s) - but couldn't find any
    void SpellHit(Unit* /*pCaster*/, const SpellEntry* pSpellEntry) override
    {
        // Check if we hit with Widow's Embrave
        if (pSpellEntry->Id == SPELL_WIDOWS_EMBRACE || pSpellEntry->Id == SPELL_WIDOWS_EMBRACE_H)
        {
            bool bIsFrenzyRemove = false;

            // If we remove the Frenzy, the Enrage Timer is reseted to 60s
            if (m_creature->HasAura(m_isRegularMode ? SPELL_ENRAGE : SPELL_ENRAGE_H))
            {
                ResetCombatAction(FAERLINA_ENRAGE, 60s);
                m_creature->RemoveAurasDueToSpell(m_isRegularMode ? SPELL_ENRAGE : SPELL_ENRAGE_H);

                bIsFrenzyRemove = true;
            }

            // Achievement 'Momma said Knock you out': If we removed OR delayed the frenzy, the criteria is failed
            if ((bIsFrenzyRemove || !m_gracePeriodActive) && m_instance)
                m_instance->SetSpecialAchievementCriteria(TYPE_ACHIEV_KNOCK_YOU_OUT, false);

            // In any case we prevent Frenzy and Poison Bolt Volley for Widow's Embrace Duration (30s)
            // We do this be setting the timers to at least bigger than 30s
            if (bIsFrenzyRemove || !m_gracePeriodActive)
            {
                DelayCombatAction(FAERLINA_ENRAGE, 30s);
                DelayCombatAction(FAERLINA_POISON_BOLT, RandomTimer(33s, 38s));
            }
        }
        static_cast<BossAI*>(m_creature->AI())->TimeSinceEncounterStart();
    }

    std::chrono::milliseconds GetSubsequentActionTimer(uint32 action)
    {
        switch (action)
        {
            case FAERLINA_POISON_BOLT: return 11s;
            case FAERLINA_RAIN_OF_FIRE: return 16s;
            case FAERLINA_ENRAGE: return 60s;
        }
        return 0s;
    }

    void ExecuteAction(uint32 action) override
    {
        switch (action)
        {
            case FAERLINA_POISON_BOLT:
                if (DoCastSpellIfCan(m_creature->GetVictim(), m_isRegularMode ? SPELL_POISONBOLT_VOLLEY : SPELL_POISONBOLT_VOLLEY_H) == CAST_OK)
                    break;
                return;
            case FAERLINA_RAIN_OF_FIRE:
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    if (DoCastSpellIfCan(pTarget, m_isRegularMode ? SPELL_RAIN_OF_FIRE : SPELL_RAIN_OF_FIRE_H) == CAST_OK)
                        break;
                }
                return;
            case FAERLINA_ENRAGE:
                if (DoCastSpellIfCan(m_creature, m_isRegularMode ? SPELL_ENRAGE : SPELL_ENRAGE_H) == CAST_OK)
                {
                    DoBroadcastText(EMOTE_BOSS_GENERIC_FRENZY, m_creature);
                    break;
                }
                return;
        }
        ResetCombatAction(action, GetSubsequentActionTimer(action));
    }
};

void AddSC_boss_faerlina()
{
    Script* pNewScript = new Script;
    pNewScript->Name = "boss_faerlina";
    pNewScript->GetAI = &GetNewAIInstance<boss_faerlinaAI>;
    pNewScript->RegisterSelf();
}
