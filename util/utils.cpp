/*******************************************************************************
*                                                                              *
* utils.c -- miscellaneous non-GUI routines                                    *
*                                                                              *
* Copyright (C) 2002 Mark Edel                                                 *
*                                                                              *
* This is free software; you can redistribute it and/or modify it under the    *
* terms of the GNU General Public License as published by the Free Software    *
* Foundation; either version 2 of the License, or (at your option) any later   *
* version. In addition, you may distribute versions of this program linked to  *
* Motif or Open Motif. See README for details.                                 *
*                                                                              *
* This software is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or        *
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License        *
* for more details.*                                                           *
*                                                                              *
* You should have received a copy of the GNU General Public License along with *
* software; if not, write to the Free Software Foundation, Inc., 59 Temple     *
* Place, Suite 330, Boston, MA  02111-1307 USA                                 *
*                                                                              *
*******************************************************************************/

#include "utils.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>

#define DEFAULT_NEDIT_HOME ".nedit"

static const char *hiddenFileNames[N_FILE_TYPES] = {".nedit", ".neditmacro", ".neditdb"};
static const char *plainFileNames[N_FILE_TYPES] = {"nedit.rc", "autoload.nm", "nedit.history"};

static void buildFilePath(char *fullPath, const char *dir, const char *file);
static bool isDir(const char *file);
static bool isRegFile(const char *file);

/* return non-nullptr value for the current working directory.
   If system call fails, provide a fallback value */
const char *GetCurrentDir(void) {
	static char curdir[MAXPATHLEN];

	if (!getcwd(curdir, (size_t)MAXPATHLEN)) {
		perror("nedit: getcwd() fails");
		strcpy(curdir, ".");
	}
	return (curdir);
}

/* return non-nullptr value for the current working directory.
   If system call fails, provide a fallback value */
std::string GetCurrentDirEx(void) {
	return GetCurrentDir();
}

/* return a non-nullptr value for the user's home directory,
   without trailing slash.
   We try the  environment var and the system user database. */
const char *GetHomeDir(void) {
	const char *ptr;
	static char homedir[MAXPATHLEN] = "";
	struct passwd *passwdEntry;
	size_t len;

	if (*homedir) {
		return homedir;
	}
	ptr = getenv("HOME");
	if (!ptr) {
		passwdEntry = getpwuid(getuid());
		if (passwdEntry && *(passwdEntry->pw_dir)) {
			ptr = passwdEntry->pw_dir;
		} else {
			/* This is really serious, so just exit. */
			perror("nedit: getpwuid() failed ");
			exit(EXIT_FAILURE);
		}
	}
	strncpy(homedir, ptr, sizeof(homedir) - 1);
	homedir[sizeof(homedir) - 1] = '\0';
	/* Fix trailing slash */
	len = strlen(homedir);
	if (len > 1 && homedir[len - 1] == '/') {
		homedir[len - 1] = '\0';
	}
	return homedir;
}

/* return a non-nullptr value for the user's home directory,
   without trailing slash.
   We try the  environment var and the system user database. */
std::string GetHomeDirEx(void) {
	return GetHomeDir();
}

/*
** Return a pointer to the username of the current user in a statically
** allocated string.
*/
const char *GetUserName(void) {
	/* cuserid has apparently been dropped from the ansi C standard, and if
	   strict ansi compliance is turned on (on Sun anyhow, maybe others), calls
	   to cuserid fail to compile.  Older versions of nedit try to use the
	   getlogin call first, then if that fails, use getpwuid and getuid.  This
	   results in the user-name of the original terminal being used, which is
	   not correct when the user uses the su command.  Now, getpwuid only: */

	const struct passwd *passwdEntry;
	static char *userName = nullptr;

	if (userName)
		return userName;

	passwdEntry = getpwuid(getuid());
	if (!passwdEntry) {
		/* This is really serious, but sometimes username service
		   is misconfigured through no fault of the user.  Be nice
		   and let the user start nc anyway. */
		perror("nedit: getpwuid() failed - reverting to $USER");
		return getenv("USER");
	} else {
	
		// NOTE(eteran): so, this is effecively a one time memory leak
		//               it is tollerable, but probably should be 
		//               improved in the future.
		userName = (char *)malloc(strlen(passwdEntry->pw_name) + 1);
		strcpy(userName, passwdEntry->pw_name);
		return userName;
	}
}

/*
** Return a pointer to the username of the current user in a statically
** allocated string.
*/
std::string GetUserNameEx(void) {
	std::string str(GetUserName());
}

/*
** Writes the hostname of the current system in string "hostname".
**
** NOTE: This function used to be called "GetHostName" but that resulted in a
** linking conflict on VMS with the standard gethostname function, because
** VMS links case-insensitively.
*/
const char *GetNameOfHost(void) {
	static char hostname[MAXNODENAMELEN + 1];
	static bool hostnameFound = false;

	if (!hostnameFound) {
		struct utsname nameStruct;
		int rc = uname(&nameStruct);
		if (rc < 0) {
			/* Shouldn't ever happen, so we better exit() here */
			perror("nedit: uname() failed ");
			exit(EXIT_FAILURE);
		}
		strcpy(hostname, nameStruct.nodename);
		hostnameFound = true;
	}
	return hostname;
}

/*
** Create a path: $HOME/filename
** Return "" if it doesn't fit into the buffer
*/
void PrependHome(const char *filename, char *buf, size_t buflen) {

	const char *const homedir = GetHomeDir();
	
	int n = snprintf(buf, buflen, "%s/%s", homedir, filename);
	if(n >= buflen) {
		buf[0] = '\0';
	}
}

/*
** Create a path: $HOME/filename
** Return "" if it doesn't fit into the buffer
*/
std::string PrependHomeEx(view::string_view filename) {

	// TODO(eteran): there is likely a way to make this more efficient
	//               and make less copies
	std::string path = GetHomeDirEx();
	path.append("/");
	path.append(filename.to_string());
	return path;
}

/*
**  Returns a pointer to the name of an rc file of the requested type.
**
**  Preconditions:
**      - MAXPATHLEN is set to the max. allowed path length
**      - fullPath points to a buffer of at least MAXPATHLEN
**
**  Returns:
**      - nullptr if an error occurs while creating a directory
**      - Pointer to a static array containing the file name
**
*/
const char *GetRCFileName(int type) {

	static char rcFiles[N_FILE_TYPES][MAXPATHLEN + 1];
	static bool namesDetermined = false;

	if (!namesDetermined) {
		char *nedit_home;
		int i;

		if ((nedit_home = getenv("NEDIT_HOME")) == nullptr) {
			/*  No NEDIT_HOME */

			/* Let's try if ~/.nedit is a regular file or not. */
			char legacyFile[MAXPATHLEN + 1];
			buildFilePath(legacyFile, GetHomeDir(), hiddenFileNames[NEDIT_RC]);
			if (isRegFile(legacyFile)) {
				/* This is a legacy setup with rc files in $HOME */
				for (i = 0; i < N_FILE_TYPES; i++) {
					buildFilePath(rcFiles[i], GetHomeDir(), hiddenFileNames[i]);
				}
			} else {
				/* ${HOME}/.nedit does not exist as a regular file. */
				/* FIXME: Devices, sockets and fifos are ignored for now. */
				char defaultNEditHome[MAXPATHLEN + 1];
				buildFilePath(defaultNEditHome, GetHomeDir(), DEFAULT_NEDIT_HOME);
				if (!isDir(defaultNEditHome)) {
					/* Create DEFAULT_NEDIT_HOME */
					if (mkdir(defaultNEditHome, 0777) != 0) {
						perror("nedit: Error while creating rc file directory"
						       " $HOME/" DEFAULT_NEDIT_HOME "\n"
						       " (Make sure all parent directories exist.)");
						return nullptr;
					}
				}

				/* All set for DEFAULT_NEDIT_HOME, let's copy the names */
				for (i = 0; i < N_FILE_TYPES; i++) {
					buildFilePath(rcFiles[i], defaultNEditHome, plainFileNames[i]);
				}
			}
		} else {
/*  $NEDIT_HOME is set. */
			/* FIXME: Is this required? Does VMS know stat(), mkdir()? */
			if (!isDir(nedit_home)) {
				/* Create $NEDIT_HOME */
				if (mkdir(nedit_home, 0777) != 0) {
					perror("nedit: Error while creating rc file directory $NEDIT_HOME\n"
					       "nedit: (Make sure all parent directories exist.)");
					return nullptr;
				}
			}

			/* All set for NEDIT_HOME, let's copy the names */
			for (i = 0; i < N_FILE_TYPES; i++) {
				buildFilePath(rcFiles[i], nedit_home, plainFileNames[i]);
			}
		}

		namesDetermined = true;
	}

	return rcFiles[type];
}

/*
**  Returns a pointer to the name of an rc file of the requested type.
**
**  Preconditions:
**      - MAXPATHLEN is set to the max. allowed path length
**      - fullPath points to a buffer of at least MAXPATHLEN
**
**  Returns:
**      - nullptr if an error occurs while creating a directory
**      - Pointer to a static array containing the file name
**
*/
std::string GetRCFileNameEx(int type) {
	
	if(const char *r = GetRCFileName(type)) {
		return r;
	}
	
	return std::string();
}

/*
**  Builds a file path from 'dir' and 'file', watching for buffer overruns.
**
**  Preconditions:
**      - MAXPATHLEN is set to the max. allowed path length
**      - 'fullPath' points to a buffer of at least MAXPATHLEN
**      - 'dir' and 'file' are valid strings
**
**  Postcondition:
**      - 'fullpath' will contain 'dir/file'
**      - Exits when the result would be greater than MAXPATHLEN
*/
static void buildFilePath(char *fullPath, const char *dir, const char *file) {
	if (MAXPATHLEN < strlen(dir) + strlen(file) + 2) {
		/*  We have no way to build the path. */
		fprintf(stderr, "nedit: rc file path too long for %s.\n", file);
		exit(EXIT_FAILURE);
	}

	/*  The length is already checked */
	snprintf(fullPath, MAXPATHLEN, "%s/%s", dir, file);
}

/*
**  Returns true if 'file' is a directory, false otherwise.
**  Links are followed.
**
**  Preconditions:
**      - None
**
**  Returns:
**      - True for directories, false otherwise
*/
static bool isDir(const char *file) {
	struct stat attribute;

	return ((stat(file, &attribute) == 0) && S_ISDIR(attribute.st_mode));
}

/*
**  Returns true if 'file' is a regular file, false otherwise.
**  Links are followed.
**
**  Preconditions:
**      - None
**
**  Returns:
**      - True for regular files, false otherwise
*/
static bool isRegFile(const char *file) {
	struct stat attribute;

	return ((stat(file, &attribute) == 0) && S_ISREG(attribute.st_mode));
}
