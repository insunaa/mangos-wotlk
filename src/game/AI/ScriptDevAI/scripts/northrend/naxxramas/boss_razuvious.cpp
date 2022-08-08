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
SDName: Boss_Razuvious
SD%Complete: 95%
SDComment: TODO: Timers need confirmation
SDCategory: Naxxramas
EndScriptData */

#include "AI/ScriptDevAI/base/CombatAI.h"
#include "AI/ScriptDevAI/ScriptDevAIMgr.h"
#include "AI/ScriptDevAI/include/sc_common.h"
#include "AI/ScriptDevAI/include/sc_creature.h"
#include "AI/ScriptDevAI/include/sc_grid_searchers.h"
#include "Entities/Unit.h"
#include "Spells/SpellDefines.h"
#include "naxxramas.h"

enum
{
    SAY_COMMAND1 = 13072,
    SAY_COMMAND2 = 13073,
    SAY_COMMAND3 = 13074,
    SAY_AGGRO1   = 13075,
    SAY_AGGRO2   = 13076,
    SAY_AGGRO3   = 13077,
    SAY_AGGRO4   = 13078,
    SAY_DEATH    = 13079,
    SAY_SLAY1    = 13080,
    SAY_SLAY2    = 13081,
    SAY_SLAY3    = 13082,

    SPELL_UNBALANCING_STRIKE = 55470,
    SPELL_DISRUPTING_SHOUT   = 55543,
    SPELL_DISRUPTING_SHOUT_H = 29107,
    SPELL_JAGGED_KNIFE       = 55550,
    SPELL_HOPELESS           = 29125,
    SPELL_FORCED_OBEDIENCE   = 55479,
    SPELL_OBEDIENCE_CHAINS   = 55520,
    
    SPELL_TAUNT              = 29060,
};

static const float resetZ = 285.0f;         // Above this altitude, Razuvious is outside his combat area (in the stairs) and should reset (leashing)

enum RazuviousActions
{
    RAZUVIOUS_UNBALANCING_STRIKE,
    RAZUVIOUS_DISRUPTING_SHOUT,
    RAZUVIOUS_JAGGED_KNIFE,
    RAZUVIOUS_MAX_ACTIONS
};

struct boss_razuviousAI : public CombatAI
{
    boss_razuviousAI(Creature* creature) : CombatAI(creature, RAZUVIOUS_MAX_ACTIONS),
    m_instance (static_cast<instance_naxxramas*>(creature->GetInstanceData()))
    {
        m_creature->GetCombatManager().SetLeashingCheck([&](Unit*, float, float, float z) { return z > resetZ; });
        m_isRegularMode = creature->GetMap()->IsRegularDifficulty();
        SetDataType(TYPE_RAZUVIOUS);
        AddOnKillText(SAY_SLAY1, SAY_SLAY2, SAY_SLAY3);
        AddOnAggroText(SAY_AGGRO1, SAY_AGGRO2, SAY_AGGRO3, SAY_AGGRO4);
        AddOnDeathText(SAY_DEATH);
        AddCombatAction(RAZUVIOUS_UNBALANCING_STRIKE, 30s);
        AddCombatAction(RAZUVIOUS_DISRUPTING_SHOUT, 15s);
        AddCombatAction(RAZUVIOUS_JAGGED_KNIFE, 10s, 15s);
    }

    instance_naxxramas* m_instance;
    bool m_isRegularMode;

    void JustDied(Unit* /*pKiller*/) override
    {
        CombatAI::JustDied();
        DoCastSpellIfCan(m_creature, SPELL_HOPELESS, CAST_TRIGGERED);
    }

    void SpellHit(Unit* /*caster*/, const SpellEntry* spell) override
    {
        // Every time a Deathknight Understudy taunts Razuvious, he will yell its disappointment
        if (spell->Id == SPELL_TAUNT)
        {
            switch (urand(0, 3))
            {
                case 0: DoBroadcastText(SAY_COMMAND1, m_creature); break;
                case 1: DoBroadcastText(SAY_COMMAND2, m_creature); break;
                case 2: DoBroadcastText(SAY_COMMAND3, m_creature); break;
            }
        }
    }

    std::chrono::milliseconds GetSubsequentActionTimer(uint32 action)
    {
        switch (action)
        {
            case RAZUVIOUS_UNBALANCING_STRIKE: return 30s;
            case RAZUVIOUS_DISRUPTING_SHOUT: return 25s;
            case RAZUVIOUS_JAGGED_KNIFE: return 10s;
        }
        return 0s;
    }

    void ExecuteAction(uint32 action) override
    {
        switch (action)
        {
            case RAZUVIOUS_UNBALANCING_STRIKE:
                if (DoCastSpellIfCan(m_creature->GetVictim(), SPELL_UNBALANCING_STRIKE) == CAST_OK)
                    break;
                return;
            case RAZUVIOUS_DISRUPTING_SHOUT:
                if (DoCastSpellIfCan(m_creature, m_isRegularMode ? SPELL_DISRUPTING_SHOUT : SPELL_DISRUPTING_SHOUT_H) == CAST_OK)
                    break;
                return;
            case RAZUVIOUS_JAGGED_KNIFE:
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    if (DoCastSpellIfCan(pTarget, SPELL_JAGGED_KNIFE) == CAST_OK)
                        break;
                }
                return;
        }
        ResetCombatAction(action, GetSubsequentActionTimer(action));
    }
};

struct npc_obedienceCrystalAI : public Scripted_NoMovementAI
{
    npc_obedienceCrystalAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature) { }

    void Reset() override
    {
        m_creature->RemoveAllCooldowns();
    }
};

bool NpcSpellClick_npc_obedienceCrystal(Player* pPlayer, Creature* pClickedCreature, uint32 uiSpellId)
{
    if (pClickedCreature->GetEntry() == NPC_OBEDIENCE_CRYSTAL)
    {
        if (pClickedCreature->IsNonMeleeSpellCasted(true))
            return false;

        CreatureList understudies;
        bool castSuccess = false;
        GetCreatureListWithEntryInGrid(understudies, pClickedCreature, NPC_DEATHKNIGHT_UNDERSTUDY, 60.f);
        for (auto understudy : understudies)
        {
            if (!castSuccess)
            {
                pPlayer->CastSpell(nullptr, uiSpellId, TRIGGERED_OLD_TRIGGERED);
                castSuccess = true;
            }
            if (understudy->GetCharmer() && understudy->GetCharmer()->GetObjectGuid() == pPlayer->GetObjectGuid())
                pClickedCreature->CastSpell(understudy, SPELL_OBEDIENCE_CHAINS, TRIGGERED_OLD_TRIGGERED);
        }
        return true;
    }

    return true;
}

struct ForcedObedience : public AuraScript, public SpellScript
{
    void OnInit(Spell* spell) const override
    {
        spell->SetFilteringScheme(EFFECT_INDEX_0, true, SCHEME_CLOSEST);
    }

    bool OnCheckTarget(const Spell* spell, Unit* target, SpellEffectIndex idx) const override
    {
        if (target->HasCharmer())
            return false;
        return true;
    }

    void OnApply(Aura* aura, bool apply) const override
    {
        if (!apply)
            if (aura->GetId() == SPELL_FORCED_OBEDIENCE)
                aura->GetTarget()->InterruptSpellsCastedOnMe();
    }
};

void AddSC_boss_razuvious()
{
    Script* pNewScript = new Script;
    pNewScript->Name = "boss_razuvious";
    pNewScript->GetAI = &GetNewAIInstance<boss_razuviousAI>;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_obedienceCrystal";
    pNewScript->GetAI = &GetNewAIInstance<npc_obedienceCrystalAI>;
    pNewScript->pNpcSpellClick = &NpcSpellClick_npc_obedienceCrystal;
    pNewScript->RegisterSelf();

    RegisterSpellScript<ForcedObedience>("spell_forced_obedience");
}
