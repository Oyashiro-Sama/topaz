/*
===========================================================================

Copyright (c) 2010-2015 Darkstar Dev Teams

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see http://www.gnu.org/licenses/

This file is part of DarkStar-server source code.

===========================================================================
*/

#include "attack_state.h"

#include "../../entities/battleentity.h"

#include "../../utils/battleutils.h"
#include "../../packets/action.h"

CAttackState::CAttackState(CBattleEntity* PEntity, uint16 targid) :
    CState(PEntity, targid),
    m_PEntity(PEntity),
    m_attackTime(1s)
{
    UpdateTarget();
}

bool CAttackState::Update(time_point tick)
{
    auto PTarget = static_cast<CBattleEntity*>(GetTarget());
    if (!PTarget || PTarget->isDead())
    {
        return true;
    }
    if (CanAttack(PTarget))
    {
        //CanAttack may have set target id to 0 (disengage from out of range)
        if (m_PEntity->PAIBattle()->GetBattleTargetID() == 0)
        {
            return true;
        }
        action_t action;
        if (m_PEntity->PAIBattle()->OnAttack(*this, action))
        {
            m_PEntity->loc.zone->PushPacket(m_PEntity, CHAR_INRANGE_SELF, new CActionPacket(action));
        }
        if (m_PEntity->PAIBattle()->GetBattleTargetID() == 0)
        {
            return true;
        }
    }
    return false;
}

void CAttackState::Cleanup(time_point tick)
{
    m_PEntity->PAIBattle()->OnDisengage();
}

void CAttackState::UpdateTarget(uint16 targid)
{
    m_errorMsg.reset();
    auto newTargid = m_PEntity->PAIBattle()->GetBattleTargetID();
    auto PNewTarget = m_PEntity->PAIBattle()->IsValidTarget(newTargid, TARGET_ENEMY, m_errorMsg);
    if (!PNewTarget)
    {
        m_PEntity->PAIBattle()->ChangeTarget(0);
        newTargid = 0;
    }
    if (targid != newTargid)
    {
        if (targid != 0)
        {
            m_PEntity->PAIBattle()->OnChangeTarget(PNewTarget);
            SetTarget(newTargid);
            if (!PNewTarget)
            {
                m_errorMsg.reset();
                return;
            }
        }
    }
    CState::UpdateTarget(m_PEntity->PAIBattle()->GetBattleTargetID());
}

bool CAttackState::CanAttack(CBattleEntity* PTarget)
{
    m_attackTime -= (m_PEntity->PAI->getTick() - m_PEntity->PAI->getPrevTick());

    if (m_attackTime < 0ms)
    {
        auto ret = m_PEntity->PAIBattle()->CanAttack(PTarget, m_errorMsg);

        if (ret && !m_errorMsg)
        {
            m_attackTime += std::chrono::milliseconds(m_PEntity->GetWeaponDelay(false));
        }
        return true;
    }
    return false;
}