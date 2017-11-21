/*******************************************************************************
*                                                                              *
* preference.h -- Nirvana Editor Preferences Header File                       *
*                                                                              *
* Copyright 2004 The NEdit Developers                                          *
*                                                                              *
* This is free software; you can redistribute it and/or modify it under the    *
* terms of the GNU General Public License as published by the Free Software    *
* Foundation; either version 2 of the License, or (at your option) any later   *
* version. In addition, you may distribute versions of this program linked to  *
* Motif or Open Motif. See README for details.                                 *
*                                                                              *
* This software is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or        *
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for    *
* more details.                                                                *
*                                                                              *
* You should have received a copy of the GNU General Public License along with *
* software; if not, write to the Free Software Foundation, Inc., 59 Temple     *
* Place, Suite 330, Boston, MA  02111-1307 USA                                 *
*                                                                              *
* Nirvana Text Editor                                                          *
* July 31, 2001                                                                *
*                                                                              *
*******************************************************************************/

#ifndef PREFERENCES_H_
#define PREFERENCES_H_

#include "IndentStyle.h"
#include "SearchType.h"
#include "ShowMatchingStyle.h"
#include "TruncSubstitution.h"
#include "VirtKeyOverride.h"
#include "WrapMode.h"
#include "WrapStyle.h"

#include <vector>

class QWidget;
class QString;
class LanguageMode;
class QFont;
class QDialog;
class Input;
class Settings;

enum ColorTypes : int;

constexpr int PLAIN_LANGUAGE_MODE = -1;

bool GetPrefAlwaysCheckRelTagsSpecs();
bool GetPrefAppendLF();
bool GetPrefAutoSave();
bool GetPrefAutoScroll();
bool GetPrefBacklightChars();
bool GetPrefBeepOnSearchWrap();
bool GetPrefFindReplaceUsesSelection();
bool GetPrefFocusOnRaise();
bool GetPrefForceOSConversion();
bool GetPrefHighlightSyntax();
bool GetPrefHonorSymlinks();
bool GetPrefKeepSearchDlogs();
bool GetPrefOpenInTab();
bool GetPrefRepositionDialogs();
bool GetPrefSaveOldVersion();
bool GetPrefSearchDlogs();
bool GetPrefSortTabs();
bool GetPrefTabBar();
bool GetPrefUndoModifiesSelection();
bool GetPrefWarnExit();
IndentStyle GetPrefAutoIndent(int langMode);
int FindLanguageMode(const QString &languageName);
int FindLanguageMode(const QStringRef &languageName);
int GetPrefCols();
int GetPrefEmTabDist(int langMode);
int GetPrefGlobalTabNavigate();
int GetPrefInsertTabs();
int GetPrefISearchLine();
int GetPrefLineNums();
int GetPrefMapDelete();
int GetPrefMatchSyntaxBased();
int GetPrefMaxPrevOpenFiles();
int GetPrefRows();
int GetPrefShowPathInWindowsMenu();
int GetPrefSmartTags();
int GetPrefSortOpenPrevMenu();
int GetPrefStatsLine();
int GetPrefStdOpenDialog();
int GetPrefStickyCaseSenseBtn();
int GetPrefTabBarHideOne();
int GetPrefTabDist(int langMode);
int GetPrefToolTips();
int GetPrefTypingHidesPointer();
int GetPrefWarnFileMods();
int GetPrefWarnRealFileMods();
int GetPrefWrapMargin();
int GetVerticalAutoScroll();
QString GetPrefBacklightCharTypes();
QString GetPrefBoldFontName();
QString GetPrefBoldItalicFontName();
QString GetPrefColorName(ColorTypes index);
QString GetPrefDelimiters();
QString GetPrefFontName();
QString GetPrefGeometry();
QString GetPrefItalicFontName();
QString GetPrefServerName();
QString GetPrefShell();
QString GetPrefTagFile();
QString GetPrefTitleFormat();
QString GetPrefTooltipBgColor();
QString LanguageModeName(int mode);
SearchType GetPrefSearch();
ShowMatchingStyle GetPrefShowMatching();
TruncSubstitution GetPrefTruncSubstitution();
VirtKeyOverride GetPrefOverrideVirtKeyBindings();
WrapMode GetPrefSearchWraps();
WrapStyle GetPrefWrap(int langMode);

bool ParseErrorEx(QWidget *toDialog, const QString &string, int stoppedAt, const QString &errorIn, const QString &message);

Settings &GetSettings();

bool ReadNumericFieldEx(Input &in, int *value);
bool ReadQuotedStringEx(Input &in, QString *errMsg, QString *string);
bool SkipDelimiterEx(Input &in, QString *errMsg);

int SkipOptSeparatorEx(QChar separator, Input &in);
QString MakeQuotedStringEx(const QString &string);
QString ReadSymbolicFieldEx(Input &input);
void ImportPrefFile(const QString &filename);
void MarkPrefsChanged();
void RestoreNEditPrefs();
void SaveNEditPrefsEx(QWidget *parent, bool quietly);
void SetPrefAppendLF(bool state);
void SetPrefAutoIndent(IndentStyle state);
void SetPrefAutoSave(bool state);
void SetPrefAutoScroll(bool state);
void SetPrefBacklightChars(bool state);
void SetPrefBeepOnSearchWrap(bool state);
void SetPrefBoldFont(const QString &fontName);
void SetPrefBoldItalicFont(const QString &fontName);
void SetPrefColorName(ColorTypes index, const QString &name);
void SetPrefCols(int nCols);
void SetPrefEmTabDist(int tabDist);
void SetPrefFindReplaceUsesSelection(bool state);
void SetPrefFocusOnRaise(bool);
void SetPrefFont(const QString &fontName);
void SetPrefGlobalTabNavigate(bool state);
void SetPrefHighlightSyntax(bool state);
void SetPrefInsertTabs(bool state);
void SetPrefISearchLine(bool state);
void SetPrefItalicFont(const QString &fontName);
void SetPrefKeepSearchDlogs(bool state);
void SetPrefLineNums(bool state);
void SetPrefMatchSyntaxBased(bool state);
void SetPrefOpenInTab(bool state);
void SetPrefRepositionDialogs(bool state);
void SetPrefRows(int nRows);
void SetPrefSaveOldVersion(bool state);
void SetPrefSearchDlogs(bool state);
void SetPrefSearch(SearchType searchType);
void SetPrefSearchWraps(bool state);
void SetPrefShell(const QString &shell);
void SetPrefShowMatching(ShowMatchingStyle state);
void SetPrefShowPathInWindowsMenu(bool state);
void SetPrefSmartTags(bool state);
void SetPrefSortOpenPrevMenu(bool state);
void SetPrefSortTabs(bool state);
void SetPrefStatsLine(bool state);
void SetPrefTabBarHideOne(bool state);
void SetPrefTabBar(bool state);
void SetPrefTabDist(int tabDist);
void SetPrefTitleFormat(const QString &format);
void SetPrefToolTips(bool state);
void SetPrefUndoModifiesSelection(bool);
void SetPrefWarnExit(bool state);
void SetPrefWarnFileMods(bool state);
void SetPrefWarnRealFileMods(bool state);
void SetPrefWrap(WrapStyle state);
void SetPrefWrapMargin(int margin);

QFont GetPrefDefaultFont();
QFont GetPrefBoldFont();
QFont GetPrefBoldItalicFont();
QFont GetPrefItalicFont();

#if defined(REPLACE_SCOPE)
void SetPrefReplaceDefScope(int scope);
int GetPrefReplaceDefScope();
#endif

bool PreferencesChanged();
QString ImportedSettingsFile();

extern std::vector<LanguageMode> LanguageModes;

#endif
