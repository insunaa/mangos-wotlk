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
SDName: Boss_Loatheb
SD%Complete: 100
SDComment:
SDCategory: Naxxramas
EndScriptData */

#include "AI/ScriptDevAI/base/CombatAI.h"
#include "AI/ScriptDevAI/include/sc_common.h"
#include "naxxramas.h"
#include <chrono>

enum
{
    EMOTE_AURA_BLOCKING     = 32334,
    EMOTE_AURA_WANE         = 32805,
    EMOTE_AURA_FADING       = 32335,

    SPELL_DEATHBLOOM        = 29865,
    SPELL_DEATHBLOOM_H      = 55053,
    SPELL_INEVITABLE_DOOM   = 29204,
    SPELL_INEVITABLE_DOOM_H = 55052,
    SPELL_NECROTIC_AURA     = 55593,
    SPELL_NECROTIC_PRE_WARN = 60929,
    SPELL_NECROTIC_WARN     = 59481,
    SPELL_SUMMON_SPORE      = 29234,
    SPELL_BERSERK           = 26662,

    NPC_SPORE               = 16286
};

enum LoathebActions
{
    LOATHEB_INEVITABLE_DOOM,
    LOATHEB_BERSERK,
    LOATHEB_SOFT_ENRAGE,
    LOATHEB_ACTIONS_MAX,
};

struct boss_loathebAI : public BossAI
{
    boss_loathebAI(Creature* creature) : BossAI(creature, LOATHEB_ACTIONS_MAX),
    m_instance(static_cast<instance_naxxramas*>(creature->GetInstanceData())),
    m_isRegularMode(creature->GetMap()->IsRegularDifficulty())
    {
        SetDataType(TYPE_LOATHEB);
        AddCombatAction(LOATHEB_INEVITABLE_DOOM, 2min);
        AddCombatAction(LOATHEB_SOFT_ENRAGE, 5min);
        if (!m_isRegularMode)
            AddCombatAction(LOATHEB_BERSERK, 12min);
    }

    instance_naxxramas* m_instance;
    bool m_isRegularMode;
    std::chrono::seconds m_doomTimer = 30s;

    void Reset() override
    {
        m_doomTimer = 30s;
    }

    void JustSummoned(Creature* pSummoned) override
    {
        if (pSummoned->GetEntry() != NPC_SPORE)
            return;

        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            pSummoned->AddThreat(pTarget);
    }

    void SummonedCreatureJustDied(Creature* pSummoned) override
    {
        if (pSummoned->GetEntry() == NPC_SPORE && m_instance)
            m_instance->SetSpecialAchievementCriteria(TYPE_ACHIEV_SPORE_LOSER, false);
    }

    std::chrono::milliseconds GetSubsequentActionTimer(uint32 action)
    {
        switch (action)
        {
            case LOATHEB_INEVITABLE_DOOM: return m_doomTimer;
            case LOATHEB_BERSERK: return 5min;
        }
        return 0s;
    }

    void ExecuteAction(uint32 action) override
    {
        switch (action)
        {
            case LOATHEB_INEVITABLE_DOOM:
            {
                DoCastSpellIfCan(m_creature, m_isRegularMode ? SPELL_INEVITABLE_DOOM : SPELL_INEVITABLE_DOOM_H);
                break;
            }
            case LOATHEB_BERSERK:
            {
                DoCastSpellIfCan(m_creature, SPELL_BERSERK);
                break;
            }
            case LOATHEB_SOFT_ENRAGE:
            {
                m_doomTimer = 15s;
                DisableCombatAction(action);
                return;
            }
        }
        ResetCombatAction(action, GetSubsequentActionTimer(action));
    }
};

struct AuraPreWarning : public AuraScript
{
    void OnApply(Aura* aura, bool apply) const override
    {
        if(!apply)
            if (Unit* target = aura->GetTarget())
                DoBroadcastText(EMOTE_AURA_WANE, target);
    }
};

struct AuraWarning : public AuraScript
{
    void OnApply(Aura* aura, bool apply) const override
    {
        if (!apply)
            if (Unit* target = aura->GetTarget())
                DoBroadcastText(EMOTE_AURA_FADING, target);
    }
};

void AddSC_boss_loatheb()
{
    Script* pNewScript = new Script;
    pNewScript->Name = "boss_loatheb";
    pNewScript->GetAI = &GetNewAIInstance<boss_loathebAI>;
    pNewScript->RegisterSelf();

    RegisterSpellScript<AuraPreWarning>("spell_loatheb_prewarn");
    RegisterSpellScript<AuraWarning>("spell_loatheb_warn");
}
