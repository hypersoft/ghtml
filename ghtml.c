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

void ghtml_die(int code) {

	ghtml_app_died = code;

	if (ghtml_webview_main_context)
		seed_simple_evaluate(ghtml_webview_main_context, "window.close()", NULL);

	if (ghtml_window) gtk_main_quit();

	// if gtk was not running we need to cleanup & exit as we won't be exiting
	// this thread after the above call.

	charbuffer_free(ghtml_webview_load_statements);
	charbuffer_free(ghtml_webview_environment_scripts);

	exit(code);

}

#include "include/webview.c"

#include "include/window.c"

#include "include/help.c"

void ghtml_check_required_file(char *type, char *file, bool checkExec) {

	/* Test if file is null */
	if (! file || ! *file) {
		g_printerr ("%s: error: %s was not specified\n", ghtml_app_name, type);
		ghtml_die(1);
	}

	/* Test if file exists. */
	if (!g_file_test(file, G_FILE_TEST_EXISTS)) {
		g_printerr ("%s: error: %s `%s' not found\n", ghtml_app_name, type, file);
		ghtml_die(1);
	}

	/* Test if file is directory */
	if (g_file_test(file, G_FILE_TEST_IS_DIR)) {
		g_printerr (
			"%s: error: `%s' is not a %s, it is a directory\n", 
			ghtml_app_name, file, type
		);
		ghtml_die(1);
	}

	/* Possibly test if the file is executable */
	if (checkExec && !g_file_test(file, G_FILE_TEST_IS_EXECUTABLE)) {
		g_printerr (
			"%s: error: `%s' is not an executable %s file.\n", 
			ghtml_app_name, file, type
		);
		ghtml_die(1);
	}

}

int parse_options(int argc, char * argv[], char * subopt) {

	bool dialog = false; char *file = subopt;
	int width = 0, height = 0, i = -1;

    if (argc >= 1) {
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
					break;
				} else {
					goto missing_required_string;
				}
            }
            if (g_str_equal(argv[i], "--mime-type") || g_str_equal(argv[i], "-m")) {
				if (argv[i + 1]) {
	                ghtml_webview_mime_type = argv[++i];
					continue;
				} else {
					goto missing_required_string;
				}
            }
            if (g_str_equal(argv[i], "--encoding") || g_str_equal(argv[i], "-E")) {
				if (argv[i + 1]) {
	                ghtml_webview_document_encoding = argv[++i];
					continue;
				} else {
					goto missing_required_string;
				}
            }
            if (g_str_equal(argv[i], "--dialog") || g_str_equal(argv[i], "-d")) {
                dialog = true;
                continue;
            }
            if (g_str_equal(argv[i], "--96dpi")) {
                ghtml_webview_96dpi = true;
                continue;
            }
            if (g_str_equal(argv[i], "--no-geo")) {
                ghtml_webview_geolocation = false;
                continue;
            }
            if (g_str_equal(argv[i], "--developer") || g_str_equal(argv[i], "-D")) {
                ghtml_webview_developer = true;
                continue;
            }
            if (g_str_equal(argv[i], "--user-agent")) {
				if (argv[i + 1]) {
					ghtml_webview_user_agent = argv[++i];
					continue;
				} else {
					goto missing_required_string;
				}
            }
            if (g_str_equal(argv[i], "--on-load") || g_str_equal(argv[i], "-o")) {
				if (argv[i + 1]) {
	               	charbuffer_write_format(ghtml_webview_load_statements, "%s\n", argv[++i]);
					continue;
				} else {
					goto missing_required_string;
				}
            }
            if (g_str_equal(argv[i], "--js-rc") || g_str_equal(argv[i], "-j")) {
				if (argv[i + 1]) {
					ghtml_check_required_file("JavaScript file", argv[++i], false);
	               	charbuffer_write_format(
						ghtml_webview_environment_scripts, 
						"Seed.include('%s');\n", argv[i]
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
            if (g_str_equal(argv[i], "--explode") || g_str_equal(argv[i], "-e")) {
                ghtml_webview_explode = true;
                continue;
            }
            if (g_str_has_prefix(argv[i], "--help")) {
                ghtml_help(argv[i]);
				exit(0);
                break;
            }

			if (g_str_has_prefix(argv[i], "-") && argv[i][1] != '-') {
				char *combo = argv[i], item = 0;

				/* combo must match: -[deDfmtEojs] or -[deD]+[fmtEojs] 
				IF not, this is an invalid request */

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
					if (item == 'm') {
						if (! *(combo + 1) && argv[i + 1]) {
						    ghtml_webview_mime_type = argv[++i];
						    break;			                						
						} else {
							goto missing_required_string;
						}
					}
					if (item == 'E') {
						if (! *(combo + 1) && argv[i + 1]) {
						    ghtml_webview_document_encoding = argv[++i];
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
					if (item == 'j') {
						if (! *(combo + 1) && argv[i + 1]) {
							ghtml_check_required_file("JavaScript file", argv[++i], false);
					       	charbuffer_write_format(
								ghtml_webview_environment_scripts, 
								"Seed.include('%s');\n", argv[i]
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
						dialog = true;
						continue;
					}
					if (item == 'D') {
						ghtml_webview_developer = true;
						continue;
					}
					if (item == 'e') {
		                ghtml_webview_explode = true;
						continue;
					}
					fprintf(stderr, 
						"%s: error: unrecognized short option: %c\n", 
					ghtml_app_name, item);
					ghtml_die(1);
				}
				continue;
			}

			fprintf(stderr, 
				"%s: error: unrecognized long option: %s\n", 
			ghtml_app_name, argv[i]);
			ghtml_die(1);
			break;

        }

		if (! ghtml_app_argc) {
			ghtml_app_argc = (argc - (i+1));
			ghtml_app_argv = argv + (i+1);
		}

	}

	if (file) ghtml_check_required_file("main file", file, false); // Might exit

	ghtml_app_file = file;
	ghtml_window_initialize(width, height, dialog, ghtml_app_file);	

jump_ship:  
	return 0;

missing_required_integer:
	fprintf(stderr, 
		"%s: error: option: %s requires integer argument\n", 
	ghtml_app_name, argv[i]);
    ghtml_die(1);

missing_required_string:
	fprintf(stderr, 
		"%s: error: option: %s requires string argument\n", 
	ghtml_app_name, argv[i]);
    ghtml_die(1);

}

int main(int argc, char *argv[]) {

    ghtml_app_name = argv[0];

    gtk_init(0, NULL);

	argc--; argv++;

	ghtml_webview_load_statements = charbuffer_from_void();
	ghtml_webview_environment_scripts = charbuffer_from_void();

	if (argc) {
		if (g_str_has_prefix(argv[0], "--file-opts ") && argv[1]) {
			char **opts = 0, *file = argv[1]; int optc = 0;
			if (g_shell_parse_argv(argv[0], &optc, &opts, NULL)) {
				argc -= 2; argv += 2;
				ghtml_app_argc = argc; ghtml_app_argv = argv;
				parse_options(optc - 1, opts + 1, file);
				gtk_main();
				g_strfreev(opts);
			}
		} else {
			parse_options(argc, argv, NULL);
			gtk_main();
		}
	} else {
		ghtml_window_initialize(0, 0, false, NULL);	
		gtk_main();
	}

	charbuffer_free(ghtml_webview_load_statements);
	charbuffer_free(ghtml_webview_environment_scripts);

    return ghtml_app_died;

}
