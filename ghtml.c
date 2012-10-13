/*
	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <gtk/gtk.h>
#include <webkit/webkit.h>

#include <seed.h>
#include <JavaScriptCore/JavaScript.h>

#include <unistd.h>
/* */

#include <fcntl.h>
/* */

#include "include/charbuffer.c"

#include "include/global.c"

#include "include/webview.c"

#include "include/window.c"

#include "include/help.c"

bool ghtml_xargs_require_file = false;
int ghtml_webview_width = 0, ghtml_webview_height = 0;
bool ghtml_webview_dialog = false;

int parse_arguments(int argc, char *argv[], bool init) {

	int i = 0;
	char *file = 0;

	if (ghtml_xargs_require_file) {
		if (argv[++i]) {
            file = argv[i];
			goto delegate_control;
		} else goto missing_required_string;
	}

    if (argc > 1) {
        while (++i < argc) {

			if (g_str_equal(argv[i], "--")) break;

            if (g_str_equal(argv[i], "--width")) {
				if (argv[i + 1]) {
	                if ( ! sscanf(argv[++i], "%i", &ghtml_webview_width)) 
						goto missing_required_integer;
					continue;							
				} else {
					goto missing_required_integer;
				}
            }
            if (g_str_equal(argv[i], "--height")) {
				if (argv[i + 1]) {
	                if ( ! sscanf(argv[++i], "%i", &ghtml_webview_height)) 
						goto missing_required_integer;
					continue;
				} else {
					goto missing_required_integer;
				}
            }
            if (g_str_equal(argv[i], "--title") || g_str_equal(argv[i], "-t")) {
				if (argv[i + 1]) {
	                ghtml_app_title = argv[++i];
					continue;
				} else {
					goto missing_required_string;
				}
            }
            if (g_str_equal(argv[i], "--file") || g_str_equal(argv[i], "-f")) {
				if (argv[i + 1]) {
	                file = argv[++i];
					puts(file);
					goto delegate_control;
				} else {
					goto missing_required_string;
				}
            }
            if (g_str_equal(argv[i], "--mime-type")) {
				if (argv[i + 1]) {
	                ghtml_webview_mime_type = argv[++i];
					continue;
				} else {
					goto missing_required_string;
				}
            }
            if (g_str_equal(argv[i], "--charset")) {
				if (argv[i + 1]) {
	                ghtml_webview_document_encoding = argv[++i];
					continue;
				} else {
					goto missing_required_string;
				}
            }
            if (g_str_equal(argv[i], "--dialog") || g_str_equal(argv[i], "-d")) {
                ghtml_webview_dialog = true;
                continue;
            }
            if (g_str_equal(argv[i], "--on-load") || g_str_equal(argv[i], "-o")) {
				if (argv[i + 1]) {
	               	charbuffer_write_format(
						ghtml_webview_load_statements, "%s\n", argv[++i]
					);
					continue;
				} else {
					goto missing_required_string;
				}
            }
            if (g_str_equal(argv[i], "--style-sheet") || g_str_equal(argv[i], "-s")) {
				if (argv[i + 1]) {
	                ghtml_webview_motif_uri = argv[++i];
					continue;
				} else {
					goto missing_required_string;
				}
            }
            if (g_str_equal(argv[i], "--explode-frames") || g_str_equal(argv[i], "-e")) {
                ghtml_webview_explode = true;
                continue;
            }
            if (g_str_has_prefix(argv[i], "--help")) {
                ghtml_help(argv[i]);
				goto jump_ship;
                break;
            }

			if (g_str_has_prefix(argv[i], "-") && argv[i][1] != '-') {
				char *combo = argv[i], item = 0;
				while (item = *(++combo)) {
					if (item == 't') {
						if (! *(combo + 1) && argv[i + 1]) {
						    ghtml_app_title = argv[++i];
						    break;			                						
						} else {
							goto missing_required_string;
						}
					}
					if (item == 'f') {
						if (! *(combo + 1) && argv[i + 1]) {
						    file = argv[++i];
						    goto delegate_control;		                						
						} else {
							goto missing_required_string;
						}
					}
/*					if (item == 'm') {*/
/*						if (! *(combo + 1) && argv[i + 1]) {*/
/*						    ghtml_webview_mime_type = argv[++i];*/
/*						    break;			                						*/
/*						} else {*/
/*							goto missing_required_string;*/
/*						}*/
/*					}*/
/*					if (item == 'E') {*/
/*						if (! *(combo + 1) && argv[i + 1]) {*/
/*						    ghtml_webview_document_encoding = argv[++i];*/
/*						    break;			                						*/
/*						} else {*/
/*							goto missing_required_string;*/
/*						}*/
/*					}*/
					if (item == 'o') {
						if (! *(combo + 1) && argv[i + 1]) {
			               	charbuffer_write_format(
								ghtml_webview_load_statements, "%s\n", argv[++i]
							);
						    break;			                						
						} else {
							goto missing_required_string;
						}
					}
					if (item == 's') {
						if (! *(combo + 1) && argv[i + 1]) {
				            ghtml_webview_motif_uri = argv[++i];
     					    break;
						} else {
							goto missing_required_string;
						}
					}
					if (item == 'd') {
						ghtml_webview_dialog = true;
						continue;
					}
					if (item == 'e') {
		                ghtml_webview_explode = true;
						continue;
					}
					fprintf(stderr, 
						"%s: error: unrecognized short option: %c\n", 
					ghtml_app_name, item);
					return 1;
				}
				continue;
			}

			break;

        }

	    ghtml_app_argc = (argc - i);
		ghtml_app_argv = argv + i;

	}

	ghtml_app_file = file;

	if (init) {
		ghtml_window_initialize(ghtml_webview_width, ghtml_webview_height, ghtml_webview_dialog);
		gtk_main();
	}

jump_ship:
	return 1;

missing_required_integer:
	fprintf(stderr, 
		"%s: error: option: %s requires integer argument\n", 
	ghtml_app_name, argv[i]);
    exit(1);

missing_required_string:
	if (g_str_equal(argv[0], "parse:")) {
		ghtml_xargs_require_file = true;
		return 1;
	}
	fprintf(stderr, 
		"%s: error: option: %s requires string argument\n", 
	ghtml_app_name, argv[i]);
    exit(1);

delegate_control:

    ghtml_app_argc = (argc - ++i);
	ghtml_app_argv = argv + i;

	ghtml_app_file = file;
	if (init) {
		ghtml_window_initialize(ghtml_webview_width, ghtml_webview_height, ghtml_webview_dialog);
		gtk_main();
	}
	return 1;

}

int main(int argc, char *argv[]) {

    ghtml_app_name = argv[0];

	ghtml_webview_load_statements = charbuffer_from_void();

    gtk_init(&argc, &argv);

	int nargc; char **nargv;

	if (argc > 1) {
		if (g_str_has_prefix(argv[1], "parse: ") && g_shell_parse_argv(argv[1], &nargc, &nargv, NULL) && nargc > 1) {
			parse_arguments(nargc, nargv, false);
			g_strfreev(nargv);
			--argc; ++argv;
			if (argc > 1) parse_arguments(argc, argv, true);
		} else {
			parse_arguments(argc, argv, true);
		}
	}
	
	g_free (ghtml_webview_seed);

	jump_ship:  
    return EXIT_SUCCESS;

}
