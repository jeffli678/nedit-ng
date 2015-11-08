/*******************************************************************************
*                                                                              *
* managedList.h -- Nirvana Editor Managed List Header File                     *
*                                                                              *
* Copyright 2002 The NEdit Developers                                          *
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

#ifndef MANAGEDLIST_H_
#define MANAGEDLIST_H_

#include <X11/Intrinsic.h>

Widget CreateManagedList(Widget parent, const char *name, Arg *args, int argC, void **itemList, int *nItems, int maxItems, int nColumns, void *(*getDialogDataCB)(void *, int, int *, void *), void *getDialogDataArg, void (*setDialogDataCB)(void *, void *), void *setDialogDataArg, void (*freeItemCB)(void *));
Widget ManageListAndButtons(Widget listW, Widget deleteBtn, Widget copyBtn, Widget moveUpBtn, Widget moveDownBtn, void **itemList, int *nItems, int maxItems, void *(*getDialogDataCB)(void *, int, int *, void *), void *getDialogDataArg, void (*setDialogDataCB)(void *, void *), void *setDialogDataArg, void (*freeItemCB)(void *));
int UpdateManagedList(Widget listW, int explicitRequest);
int ManagedListSelectedIndex(Widget listW);
void ChangeManagedListData(Widget listW);
void SelectManagedListItem(Widget listW, int itemIndex);
void AddDeleteConfirmCB(Widget listW, int (*deleteConfirmCB)(int, void *), void *deleteConfirmArg);

#endif
