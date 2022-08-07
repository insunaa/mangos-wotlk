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
SD%Complete: 85%
SDComment: TODO: Timers and sounds need confirmation - orb handling for normal-mode is missing
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
    //ToDo: Replace all Text with BroadcastText
    SAY_AGGRO1               = -1533120,
    SAY_AGGRO2               = -1533121,
    SAY_AGGRO3               = -1533122,
    SAY_SLAY1                = -1533123,
    SAY_SLAY2                = -1533124,
    SAY_COMMAND1             = -1533125,
    SAY_COMMAND2             = -1533126,
    SAY_COMMAND3             = -1533127,
    SAY_COMMAND4             = -1533128,
    SAY_DEATH                = -1533129,

    SPELL_UNBALANCING_STRIKE = 55470,
    SPELL_DISRUPTING_SHOUT   = 55543,
    SPELL_DISRUPTING_SHOUT_H = 29107,
    SPELL_JAGGED_KNIFE       = 55550,
    SPELL_HOPELESS           = 29125,
    SPELL_FORCED_OBEDIENCE   = 55479,
    SPELL_OBEDIENCE_CHAINS   = 55520,
};

enum RazuviousActions
{
    RAZUVIOUS_UNBALANCING_STRIKE,
    RAZUVIOUS_DISRUPTING_SHOUT,
    RAZUVIOUS_JAGGED_KNIFE,
    RAZUVIOUS_COMMAND_SOUND,
    RAZUVIOUS_MAX_ACTIONS
};

struct boss_razuviousAI : public CombatAI
{
    boss_razuviousAI(Creature* creature) : CombatAI(creature, RAZUVIOUS_MAX_ACTIONS),
    m_instance (static_cast<instance_naxxramas*>(creature->GetInstanceData()))
    {
        m_isRegularMode = creature->GetMap()->IsRegularDifficulty();
        SetDataType(TYPE_RAZUVIOUS);
        AddOnKillText(SAY_SLAY1, SAY_SLAY2);
        AddOnAggroText(SAY_AGGRO1, SAY_AGGRO2, SAY_AGGRO3);
        AddOnDeathText(SAY_DEATH);
        AddCombatAction(RAZUVIOUS_UNBALANCING_STRIKE, 30s);
        AddCombatAction(RAZUVIOUS_DISRUPTING_SHOUT, 15s);
        AddCombatAction(RAZUVIOUS_JAGGED_KNIFE, 10s, 15s);
        AddCombatAction(RAZUVIOUS_COMMAND_SOUND, 40s);
    }

    instance_naxxramas* m_instance;
    bool m_isRegularMode;

    void JustDied(Unit* /*pKiller*/) override
    {
        CombatAI::JustDied();
        DoCastSpellIfCan(m_creature, SPELL_HOPELESS, CAST_TRIGGERED);
    }

    std::chrono::milliseconds GetSubsequentActionTimer(uint32 action)
    {
        switch (action)
        {
            case RAZUVIOUS_UNBALANCING_STRIKE: return 30s;
            case RAZUVIOUS_DISRUPTING_SHOUT: return 25s;
            case RAZUVIOUS_JAGGED_KNIFE: return 10s;
            case RAZUVIOUS_COMMAND_SOUND: return 40s;
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
            case RAZUVIOUS_COMMAND_SOUND:
                switch (urand(0, 3))
                {
                    case 0: DoScriptText(SAY_COMMAND1, m_creature); break;
                    case 1: DoScriptText(SAY_COMMAND2, m_creature); break;
                    case 2: DoScriptText(SAY_COMMAND3, m_creature); break;
                    case 3: DoScriptText(SAY_COMMAND4, m_creature); break;
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
}
