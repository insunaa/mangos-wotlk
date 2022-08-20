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
SDName: Boss_Heigan
SD%Complete: 65
SDComment: Missing traps dance
SDCategory: Naxxramas
EndScriptData */

#include "AI/ScriptDevAI/base/CombatAI.h"
#include "AI/ScriptDevAI/include/sc_common.h"
#include "Entities/Player.h"
#include "naxxramas.h"

enum
{
    PHASE_GROUND            = 1,
    PHASE_PLATFORM          = 2,

    SAY_AGGRO1              = -1533109,
    SAY_AGGRO2              = -1533110,
    SAY_AGGRO3              = -1533111,
    SAY_SLAY                = -1533112,
    SAY_TAUNT1              = -1533113,
    SAY_TAUNT2              = -1533114,
    SAY_TAUNT3              = -1533115,
    SAY_TAUNT4              = -1533117,
    SAY_CHANNELING          = -1533116,
    SAY_DEATH               = -1533118,
    EMOTE_TELEPORT          = -1533136,
    EMOTE_RETURN            = -1533137,

    // Spells by boss
    SPELL_DECREPIT_FEVER    = 29998,
    SPELL_DECREPIT_FEVER_H  = 55011,
    SPELL_DISRUPTION        = 29310,
    SPELL_TELEPORT          = 30211,
    SPELL_PLAGUE_CLOUD      = 29350,
    SPELL_PLAGUE_WAVE_SLOW  = 29351,
    SPELL_PLAGUE_WAVE_FAST  = 30114,

    NPC_PLAGUE_WAVE         = 17293,
};

static const float resetX = 2825.0f;                // Beyond this X-line, Heigan is outside his room and should reset (leashing)

enum HeiganActions
{
    HEIGAN_FEVER,
    HEIGAN_DISRUPTION,
    HEIGAN_ERUPTION,
    HEIGAN_TAUNT,
    HEIGAN_PHASE_PLATFORM,
    HEIGAN_PHASE_GROUND,
    HEIGAN_START_CHANNELING,
    HEIGAN_ACTIONS_MAX,
};

struct boss_heiganAI : public BossAI
{
    boss_heiganAI(Creature* creature) : BossAI(creature, HEIGAN_ACTIONS_MAX),
    m_instance(static_cast<instance_naxxramas*>(creature->GetInstanceData())),
    m_isRegularMode(creature->GetMap()->IsRegularDifficulty())
    {
        m_creature->GetCombatManager().SetLeashingCheck([&](Unit*, float x, float /*y*/, float /*z*/)
        {
            float respawnX, respawnY, respawnZ;
            m_creature->GetRespawnCoord(respawnX, respawnY, respawnZ);
            return m_creature->GetDistance2d(respawnX, respawnY) > 90.f || x > resetX;
        });
        SetDataType(TYPE_HEIGAN);
        AddOnKillText(SAY_SLAY);
        AddOnDeathText(SAY_DEATH);
        AddOnAggroText(SAY_AGGRO1, SAY_AGGRO2, SAY_AGGRO3);
        AddCombatAction(HEIGAN_FEVER, 4s);
        AddCombatAction(HEIGAN_DISRUPTION, 5s);
        AddCombatAction(HEIGAN_ERUPTION, 5s);
        AddCombatAction(HEIGAN_TAUNT, 20s, 60s);
        AddCombatAction(HEIGAN_PHASE_PLATFORM, 90s);
        AddCombatAction(HEIGAN_PHASE_GROUND, true);
        AddCombatAction(HEIGAN_START_CHANNELING, true);
        Reset();
    }

    instance_naxxramas* m_instance;
    bool m_isRegularMode;

    uint8 m_phase;

    void Reset() override
    {
        m_phase = PHASE_GROUND;
        SetReactState(REACT_AGGRESSIVE);
        SetRootSelf(false);
        SetMeleeEnabled(true);
        StopEruptions();
        BossAI::Reset();
    }

    void EnterEvadeMode() override
    {
        if (m_instance->GetPlayerInMap(true, false))
            return;
        BossAI::EnterEvadeMode();
    }

    void StartEruptions(uint32 spellId)
    {
        // Clear current plague waves controller spell before applying the new one
        if (Creature* trigger = GetClosestCreatureWithEntry(m_creature, NPC_PLAGUE_WAVE, 100.0f))
        {
            trigger->RemoveAllAuras();
            trigger->CastSpell(trigger, spellId, TRIGGERED_OLD_TRIGGERED);
        }
    }

    void StopEruptions()
    {
        // Reset Plague Waves
        if (Creature* trigger = GetClosestCreatureWithEntry(m_creature, NPC_PLAGUE_WAVE, 100.0f))
            trigger->RemoveAllAuras();
    }

    std::chrono::milliseconds GetSubsequentActionTimer(uint32 action)
    {
        switch (action)
        {
            case HEIGAN_FEVER: return 21s;
            case HEIGAN_DISRUPTION: return 10s;
            case HEIGAN_TAUNT: return RandomTimer(20s, 70s);
            case HEIGAN_PHASE_PLATFORM: return 90s;
            case HEIGAN_PHASE_GROUND: return 45s;
            default: return 0s;
        }
    }

    void ExecuteAction(uint32 action) override
    {
        switch (action)
        {
            case HEIGAN_FEVER:
            {
                DoCastSpellIfCan(m_creature, m_isRegularMode ? SPELL_DECREPIT_FEVER : SPELL_DECREPIT_FEVER_H);
                break;
            }
            case HEIGAN_DISRUPTION:
            {
                DoCastSpellIfCan(m_creature, SPELL_DISRUPTION);
                break;
            }
            case HEIGAN_TAUNT:
            {
                switch (urand(0, 3))
                {
                    case 0: DoBroadcastText(SAY_TAUNT1, m_creature); break;
                    case 1: DoBroadcastText(SAY_TAUNT2, m_creature); break;
                    case 2: DoBroadcastText(SAY_TAUNT3, m_creature); break;
                    case 3: DoBroadcastText(SAY_TAUNT4, m_creature); break;
                }
                break;
            }
            case HEIGAN_PHASE_PLATFORM:
            {
                if (DoCastSpellIfCan(m_creature, SPELL_TELEPORT) == CAST_OK)
                {
                    StopEruptions();
                    SetRootSelf(true);
                    DoBroadcastText(EMOTE_TELEPORT, m_creature);
                    m_creature->GetMotionMaster()->MoveIdle();
                    SetReactState(REACT_PASSIVE);
                    m_creature->AttackStop();
                    m_creature->SetTarget(nullptr);
                    m_phase = PHASE_PLATFORM;
                    DisableCombatAction(action);
                    DisableCombatAction(HEIGAN_DISRUPTION);
                    DisableCombatAction(HEIGAN_FEVER);
                    ResetCombatAction(HEIGAN_PHASE_GROUND, GetSubsequentActionTimer(HEIGAN_PHASE_GROUND));
                    ResetCombatAction(HEIGAN_START_CHANNELING, 1s);
                    ResetCombatAction(HEIGAN_ERUPTION, 3s);
                }
                return;
            }
            case HEIGAN_PHASE_GROUND:
            {
                ResetAllTimers();
                StopEruptions();
                SetRootSelf(false);
                SetReactState(REACT_AGGRESSIVE);
                m_creature->InterruptNonMeleeSpells(true);
                DoBroadcastText(EMOTE_RETURN, m_creature);
                m_creature->GetMotionMaster()->MoveChase(m_creature->GetVictim());
                m_phase = PHASE_GROUND;
                DisableCombatAction(action);
                ResetCombatAction(HEIGAN_ERUPTION, 0s);
                return;
            }
            case HEIGAN_ERUPTION:
            {
                sLog.outError("Eruption called. Phase: %d", m_phase);
                StartEruptions(m_phase == PHASE_GROUND ? SPELL_PLAGUE_WAVE_SLOW : SPELL_PLAGUE_WAVE_FAST);
                DisableCombatAction(action);
                return;
            }
            case HEIGAN_START_CHANNELING:
            {
                DoBroadcastText(SAY_CHANNELING, m_creature);
                DoCastSpellIfCan(m_creature, SPELL_PLAGUE_CLOUD);
                DisableCombatAction(action);
                return;
            }
        }
        ResetCombatAction(action, GetSubsequentActionTimer(action));
    }
};

struct PlagueWaveController : public AuraScript
{
    void OnPeriodicTrigger(Aura* aura, PeriodicTriggerData& /* data */) const override
    {
        Unit* triggerTarget = aura->GetTriggerTarget();
        uint32 spellForTick[6] = { 30116, 30117, 30118, 30119, 30118, 30117 };  // Circling back and forth through the 4 plague areas
        uint32 tick = (aura->GetAuraTicks() - 1) % 6;

        triggerTarget->CastSpell(triggerTarget, spellForTick[tick], TRIGGERED_OLD_TRIGGERED, nullptr, aura, aura->GetCasterGuid(), nullptr);
    }
};

void AddSC_boss_heigan()
{
    Script* pNewScript = new Script;
    pNewScript->Name = "boss_heigan";
    pNewScript->GetAI = &GetNewAIInstance<boss_heiganAI>;
    pNewScript->RegisterSelf();

    RegisterSpellScript<PlagueWaveController>("spell_plague_wave_controller");
}
