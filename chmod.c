/* vi: set sw=4 ts=4: */
/*
 * Mini chown/chmod/chgrp implementation for busybox
 *
 *
 * Copyright (C) 1999,2000,2001 by Lineo, inc.
 * Written by Erik Andersen <andersen@lineo.com>, <andersee@debian.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include "busybox.h"


struct stat *theMode;


static int fileAction(const char *fileName, struct stat *statbuf, void* junk)
{
	if (chmod(fileName, theMode->st_mode) == 0)
		return (TRUE);
	perror(fileName);
	return (FALSE);
}

int chmod_main(int argc, char **argv)
{
	int opt;
	int recursiveFlag = FALSE;

	/* do normal option parsing */
	while ((opt = getopt(argc, argv, "R")) > 0) {
		switch (opt) {
			case 'R':
				recursiveFlag = TRUE;
			default:
				show_usage();
		}
	}

	if (argc > optind && argc > 2 && argv[optind]) {
		/* Parse the specified mode */
		if (parse_mode(argv[optind], &(theMode->st_mode)) == FALSE) {
			error_msg_and_die( "unknown mode: %s", argv[optind]);
		}
	} else {
		error_msg_and_die(too_few_args);
	}

	/* Ok, ready to do the deed now */
	while (++optind < argc) {
		if (recursive_action (argv[optind], recursiveFlag, FALSE, FALSE, 
					fileAction, fileAction, NULL) == FALSE) {
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}

/*
Local Variables:
c-file-style: "linux"
c-basic-offset: 4
tab-width: 4
End:
*/
