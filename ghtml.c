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

#include "include/help.c"

#include "include/webview.c"

#include "include/window.c"

int main(int argc, char *argv[]) {

	char *file = 0;
	int width = 0, height = 0, i = 0;
	bool dialog = false;

    ghtml_app_name = argv[0];
    ghtml_app_argc = argc;
	ghtml_app_argv = argv;

    gtk_init(&argc, &argv);

	ghtml_webview_load_statements = charbuffer_from_void();

    if (argc > 1)
        while (++i < argc) {

			if (g_str_equal(argv[i], "--")) break;

            if (g_str_equal(argv[i], "--width")) {
				if (argv[i + 1]) {
	                if ( ! sscanf(argv[++i], "%i", &width)) goto missing_required_integer;
					continue;							
				} else {
					goto missing_required_integer;
				}
            }
            if (g_str_equal(argv[i], "--height")) {
				if (argv[i + 1]) {
	                if ( ! sscanf(argv[++i], "%i", &height)) goto missing_required_integer;
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
					continue;
				} else {
					goto missing_required_string;
				}
            }
            if (g_str_equal(argv[i], "--dialog") || g_str_equal(argv[i], "-d")) {
                dialog = true;
                continue;
            }
            if (g_str_equal(argv[i], "--on-load") || g_str_equal(argv[i], "-o")) {
				if (argv[i + 1]) {
	               	charbuffer_write_format(ghtml_webview_load_statements, "%s\n", argv[++i]);
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
                ghtml_print_help(argv[i]);
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
						    break;			                						
						} else {
							goto missing_required_string;
						}
					}
					if (item == 'o') {
						if (! *(combo + 1) && argv[i + 1]) {
			               	charbuffer_write_format(ghtml_webview_load_statements, "%s\n", argv[++i]);
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
						dialog = true;
						continue;
					}
					if (item == 'e') {
		                ghtml_webview_explode = true;
						continue;
					}
					fprintf(stderr, 
						"%s: error: unrecognized short option: %c\n", 
					ghtml_app_name, item);
					return ghtml_app_exit_value;
				}
				continue;
			}

			fprintf(stderr, 
				"%s: error: unrecognized option request: %s\n", 
			ghtml_app_name, argv[i]);
			goto jump_ship;

        }

	ghtml_window_initialize(width, height, dialog, file);
	
	gtk_main();

	g_free (ghtml_webview_seed);

	jump_ship:  
    return ghtml_app_exit_value;

missing_required_integer:
	fprintf(stderr, 
		"%s: error: option: %s requires integer argument\n", 
	ghtml_app_name, argv[i]);
    return ghtml_app_exit_value;

missing_required_string:
	fprintf(stderr, 
		"%s: error: option: %s requires string argument\n", 
	ghtml_app_name, argv[i]);
    return ghtml_app_exit_value;

}
