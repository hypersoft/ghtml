/*
	This file is part of ghtml.

    ghtml is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ghtml is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ghtml.  If not, see <http://www.gnu.org/licenses/>.

*/

int   ghtml_app_exit_value = 0;
char *ghtml_app_name = 0;

int   ghtml_app_argc;
char **ghtml_app_argv;

char *ghtml_app_title = 0;
char *ghtml_app_directory = 0;
void *ghtml_window = 0; /* GtkWidget */
void *ghtml_window_scrollable_content_area = 0; /* GtkWidget */

void *ghtml_webview = 0; /* WebKitWebView */

void *ghtml_webview_main_frame = 0;
void *ghtml_webview_global_context = 0;
void *ghtml_webview_mime_type = NULL;
void *ghtml_webview_document_encoding = NULL;
void *ghtml_webview_motif_uri = 0;
bool ghtml_webview_explode = false;
bool ghtml_webview_private_browsing = true;

charbuffer *ghtml_webview_load_statements = 0;

SeedEngine *ghtml_webview_seed;

