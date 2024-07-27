/*
 * $Id$
 *
 * Copyright (c) 2007 Vyacheslav Frolov
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
 * Revision 1.1  2007/10/31 10:16:55  vfrolov
 * Initial revision
 *
 *
 */

#include "stdafx.h"
#include "portprms.h"
#include "pinmap.h"
#include "FormMain.h"
#include <Windows.h>
using namespace SetupApp;

//[STAThreadAttribute]
//int main(array<System::String ^> ^/*args*/)
//{
//	// Enabling Windows XP visual effects before any controls are created
//	return 0;
//}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	Application::Run(gcnew FormMain());
	return 0;
}
