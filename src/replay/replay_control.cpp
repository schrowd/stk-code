//
//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2026 Markuss Milais
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 3
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include "replay/replay_control.hpp"
#include "utils/log.hpp"
#include <cstddef>

ReplayControl *ReplayControl::m_replay_control = NULL;

void ReplayControl::setRate(double rate)
{
    // Checks the rate and clamps it if it's not between 0.1 and 4
    if (rate < 0.1)
    {
        m_rate = 0.1;
    }
    else if (rate > 4.0)
    {
        m_rate = 4.0;
    }
    else
    {
        m_rate = rate;
    }
}

void ReplayControl::reset()
{
    m_is_playing =  true;
    m_head       =  0.0;
    m_rate       =  1.0;
    m_duration   = -1.0;
}

// Moves the head to the desired time (the target).
// The value of target would be set by an input system: this is
// beyond the scope of this replay controller and as such
// remains deliberately unimplemented.
void ReplayControl::seek(double target)
{
    if (m_duration == -1) return;

    if (target < 0.0)
    {
        m_head = 0.0;
    }
    else if (target > m_duration)
    {
        m_head = m_duration;
    }
    else
    {
        m_head = target;
    }
}

// Checks if the replay is paused: if not, it adds the product of
// dt (in seconds) and the rate to the head of the playback, thus
// advancing it. It then checks if the head is at or after the
// duration of the replay; if it is, the head gets clamped to 
// the duration and the replay pauses. It also checks for -1.0
// (unknown duration) before clamping so that an unknown duration
// isn't treated as the end of the replay.
double ReplayControl::advance(double dt)
{
    if (m_is_playing == false)
    {
        return m_head;
    }
    m_head += dt * m_rate;
    if (m_head >= m_duration && m_duration != -1.0)
    {
        m_head = m_duration;
        m_is_playing = false;
        return m_head;
    }
    return m_head;
}


