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
SDName: Boss_Patchwerk
SD%Complete: 100
SDComment:
SDCategory: Naxxramas
EndScriptData */

#include "AI/ScriptDevAI/base/CombatAI.h"
#include "AI/ScriptDevAI/include/sc_common.h"
#include "naxxramas.h"

enum
{
    SAY_AGGRO1            = 13068,
    SAY_AGGRO2            = 13069,
    SAY_SLAY              = -1533019,
    SAY_DEATH             = 13070,

    EMOTE_BERSERK         = 11694,
    EMOTE_ENRAGE          = 7798,

    SPELL_HATEFULSTRIKE_PRIMER = 28307,
    SPELL_HATEFULSTRIKE   = 28308,
    SPELL_HATEFULSTRIKE_H = 59192,
    SPELL_ENRAGE          = 28131,
    SPELL_BERSERK         = 26662,
    SPELL_SLIMEBOLT       = 32309,
    SPELLSET_NORMAL       = 1602801,
    SPELLSET_BERSERK      = 1602802,
};

enum PatchwerkActions
{
    PATCHWERK_ENRAGE_LOW,
    PATCHWERK_ACTION_MAX,
    PATCHWERK_BERSERK,
    PATCHWERK_BERSERK_SILMEBOLT,
};

struct boss_patchwerkAI : public CombatAI
{
    boss_patchwerkAI(Creature* creature) : CombatAI(creature, PATCHWERK_ACTION_MAX),
        m_instance(static_cast<ScriptedInstance*>(creature->GetInstanceData()))
    {
        AddOnKillText(SAY_SLAY);
        AddTimerlessCombatAction(PATCHWERK_ENRAGE_LOW, true);
        AddCustomAction(PATCHWERK_BERSERK, true, [&]()
        {
            DoBroadcastText(EMOTE_BERSERK, m_creature);
            m_creature->CastSpell(nullptr, SPELL_BERSERK, TRIGGERED_OLD_TRIGGERED);
        });
        AddCustomAction(PATCHWERK_BERSERK_SILMEBOLT, true, [&]()
        {
            m_creature->SetSpellList(SPELLSET_BERSERK);
        });
    }

    void Reset() override
    {
        CombatAI::Reset();
        m_creature->SetSpellList(SPELLSET_NORMAL);
    }

    ScriptedInstance* m_instance;

    void JustDied(Unit* /*victim*/) override
    {
        DoBroadcastText(SAY_DEATH, m_creature);

        if (m_instance)
            m_instance->SetData(TYPE_PATCHWERK, DONE);
    }

    void Aggro(Unit* /*who*/) override
    {
        DoBroadcastText(urand(0, 1) ? SAY_AGGRO1 : SAY_AGGRO2, m_creature);

        ResetTimer(PATCHWERK_BERSERK, MINUTE * 6u * IN_MILLISECONDS);
        ResetTimer(PATCHWERK_BERSERK_SILMEBOLT, MINUTE * 6u * IN_MILLISECONDS);

        if (m_instance)
            m_instance->SetData(TYPE_PATCHWERK, IN_PROGRESS);
    }

    void JustReachedHome() override
    {
        if (m_instance)
            m_instance->SetData(TYPE_PATCHWERK, FAIL);
    }

    void ExecuteAction(uint32 action) override
    {
        if (action == PATCHWERK_ENRAGE_LOW)
            if (m_creature->GetHealthPercent() <= 5.f)
                if (DoCastSpellIfCan(nullptr, SPELL_ENRAGE) == CAST_OK)
                {
                    DoBroadcastText(EMOTE_ENRAGE, m_creature);
                    DisableCombatAction(action);
                }
    }
};

struct HatefulStrikePrimer : public SpellScript
{
    bool OnCheckTarget(const Spell* spell, Unit* target, SpellEffectIndex /*eff*/) const override
    {
        if (!target)
            return false;

        if (Unit* caster = spell->GetCaster())
        {

            Difficulty diff = caster->GetMap()->GetDifficulty();
            uint32 maxTargets = diff == RAID_DIFFICULTY_10MAN_NORMAL ? 2 : 3;
            ThreatList threatList = caster->getThreatManager().getThreatList();
            if (threatList.size() == 1)
                return true;

            uint32 i = 1;
            for (auto itr = ++threatList.begin(); i < maxTargets && itr != threatList.end(); i++)
            {
                if ((*itr)->getUnitGuid() == target->GetObjectGuid())
                    return true;
                itr++;
            }
        }
        return false;
    }

    void OnEffectExecute(Spell* spell, SpellEffectIndex effIdx) const override
    {
        if (effIdx == EFFECT_INDEX_0)
        {
            if (Unit* caster = spell->GetCaster())
            {
                // Target is filtered in Spell::FilterTargetMap
                Difficulty diff = caster->GetMap()->GetDifficulty();
                if (Unit* unitTarget = spell->GetUnitTarget())
                    caster->CastSpell(unitTarget, diff == RAID_DIFFICULTY_10MAN_NORMAL ? SPELL_HATEFULSTRIKE : SPELL_HATEFULSTRIKE_H, TRIGGERED_NONE);
            }
        }
    }
};

void AddSC_boss_patchwerk()
{
    Script* pNewScript = new Script;
    pNewScript->Name = "boss_patchwerk";
    pNewScript->GetAI = &GetNewAIInstance<boss_patchwerkAI>;
    pNewScript->RegisterSelf();

    RegisterSpellScript<HatefulStrikePrimer>("spell_hateful_strike_primer");
}
