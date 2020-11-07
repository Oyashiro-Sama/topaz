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

#include <time.h>
#include <iostream>
#include <chrono>
#include "../utils/serverutils.h"
#include "../map.h"

namespace serverutils
{
    int32 GetVar(const char* var)
    {
        int32 value = 0;
        int32 ret = Sql_Query(SqlHandle, "SELECT value FROM server_variables WHERE name = '%s' LIMIT 1;", var);
        if (ret != SQL_ERROR && Sql_NumRows(SqlHandle) != 0 && Sql_NextRow(SqlHandle) == SQL_SUCCESS)
        {
            value = (int32)Sql_GetIntData(SqlHandle, 0);
        }
        return value;
    }

    void SetVar(const char* var, int32 value)
    {
        if (value == 0)
        {
            Sql_Query(SqlHandle, "DELETE FROM server_variables WHERE name = '%s' LIMIT 1;", var);
        }
        else
        {
            Sql_Query(SqlHandle, "INSERT INTO server_variables VALUES ('%s', %i) ON DUPLICATE KEY UPDATE value = %i;", var, value, value);
        }
    }

    int32 RemoveVarsWithTag(const char* varsTag)
    {
        return Sql_Query(SqlHandle, "DELETE FROM server_variables WHERE name LIKE '[%s]%%';", varsTag);
    }

    void AddVar(const char* var, int32 value)
    {
        Sql_Query(SqlHandle, "INSERT INTO server_variables VALUES ('%s', %i) ON DUPLICATE KEY UPDATE value = %i;", var, value, value);
    }
}
