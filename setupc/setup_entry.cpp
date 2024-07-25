/*
 * $Id$
 *
 * Copyright (c) 2007-2010 Vyacheslav Frolov
 *
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
 *
 *
 * $Log$
 * Revision 1.2  2010/03/30 08:06:22  vfrolov
 * Fixed bugs item #2979007 "setupc command line limited to 200 chars"
 * Reported by Henrik Maier (hwmaier)
 *
 * Revision 1.1  2007/10/25 14:25:34  vfrolov
 * Initial revision
 *
 */

#include <windows.h>
#include <tchar.h>

extern int CALLBACK Main(const TCHAR* pProgName, const TCHAR* pCmdLine);

int _cdecl _tmain(int argc, TCHAR* argv[])
{
	TCHAR cmd[1024] = _T("");

	for (int i = 1; i < argc; i++) {
		int lenCmd = lstrlen(cmd);

		if (sizeof(cmd) / sizeof(cmd[0]) < lenCmd + 1 + lstrlen(argv[i]) + 1)
			break;

		lstrcpy(cmd + lenCmd, _T(" "));
		lenCmd++;
		lstrcpy(cmd + lenCmd, argv[i]);
	}
}
