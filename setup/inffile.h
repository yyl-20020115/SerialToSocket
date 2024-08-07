/*
 * $Id$
 *
 * Copyright (c) 2006-2011 Vyacheslav Frolov
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
 * Revision 1.4  2011/12/15 15:51:48  vfrolov
 * Fixed types
 *
 * Revision 1.3  2010/05/27 11:16:46  vfrolov
 * Added ability to put the port to the Ports class
 *
 * Revision 1.2  2006/10/19 13:28:50  vfrolov
 * Added InfFile::UninstallAllInfFiles()
 *
 * Revision 1.1  2006/07/28 12:16:42  vfrolov
 * Initial revision
 *
 *
 */

#ifndef _C0C_INFFILE_H_
#define _C0C_INFFILE_H_

class InfFile {
  public:
    struct InfFileField {
      const TCHAR*pSection;
      const TCHAR*pKey;
      int         nField;
      const TCHAR*pFieldValue;
    };

    struct InfFileUninstall {
      const InfFile::InfFileField *pRequiredFields;
      bool queryConfirmation;
    };

  public:
    InfFile(const TCHAR*pInfName, const TCHAR*pNearPath);
    ~InfFile();

    bool Test(const InfFileField *pFields, bool showErrors = TRUE) const;

    const TCHAR*Path() const { return pPath; }
    const TCHAR*OemPath(bool showErrors = TRUE) const;
    const TCHAR*ClassGUID(bool showErrors = TRUE) const;
    const TCHAR*Class(bool showErrors = TRUE) const;
    const TCHAR*Provider(bool showErrors = TRUE) const;
    const TCHAR*DriverVer(bool showErrors = TRUE) const;
    const TCHAR*UninstallInfTag(bool showErrors = TRUE) const;

    bool UninstallFiles(const TCHAR*pFilesSection) const;

    bool InstallOEMInf() const;
    bool UninstallOEMInf() const;

    static bool UninstallAllInfFiles(
        const InfFileUninstall *pInfFileUninstallList,
        const TCHAR *const *ppOemPathExcludeList);

  protected:
      TCHAR*pPath;
    mutable TCHAR*pOemPath;
    mutable TCHAR*pClassGUID;
    mutable TCHAR*pClass;
    mutable TCHAR*pProvider;
    mutable TCHAR*pDriverVer;
    mutable TCHAR*pUninstallInfTag;
    mutable HINF hInf;
};

#endif /* _C0C_PARAMS_H_ */
