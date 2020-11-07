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

===========================================================================
*/

#ifndef _SERVERUTILS_H
#define _SERVERUTILS_H

#include "../../common/cbasetypes.h"
#include "../../common/mmo.h"
#include "../entities/npcentity.h"
#include <vector>

namespace serverutils
{
    int32   GetVar(const char* var);
    void    SetVar(const char* var, int32 value);
    int32   RemoveVarsWithTag(const char* varsTag);
    void    AddVar(const char* var, int32 value);
}
#endif
