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

#include "webview/js.c"

static gboolean ghtml_webview_close(WebKitWebView* this_webview, GtkWidget* this_window){
	gtk_widget_destroy(this_window);
	return TRUE;
}

static gboolean ghtml_webview_navigation_policy_request(WebKitWebView *this_webview, WebKitWebFrame *this_frame, WebKitNetworkRequest *this_request, WebKitWebNavigationAction *this_navigation_action, WebKitWebPolicyDecision *this_policy_decision, gpointer this_user_data) {
		webkit_web_policy_decision_ignore (this_policy_decision);
	return TRUE;
}

static void ghtml_webview_title_changed(WebKitWebView *this_webview, WebKitWebFrame *this_frame, gchar *this_title, gpointer this_user_data) {
	if (! ghtml_app_title) {
		gtk_window_set_title(ghtml_window, this_title);
	}
}

static void ghtml_webview_document_load_finished(WebKitWebView  *this_webview, WebKitWebFrame *this_frame, gpointer this_user_data) {
	return;
}

static void ghtml_webview_load_finished(WebKitWebView  *this_webview, WebKitWebFrame *this_frame, gpointer this_user_data) {

    g_signal_connect (G_OBJECT (this_webview), "navigation-policy-decision-requested",
    (void *) ghtml_webview_navigation_policy_request, NULL);

    g_signal_connect (G_OBJECT (this_webview), "document-load-finished",
    (void *) ghtml_webview_document_load_finished, NULL);

	if (ghtml_webview_load_statements->index) webkit_web_view_execute_script(ghtml_webview, ghtml_webview_load_statements->pointer);
	charbuffer_free(ghtml_webview_load_statements);

	gtk_widget_grab_focus(GTK_WIDGET(ghtml_webview));

    // Make sure the main window and all its contents are visible
    gtk_widget_show_all(ghtml_window);

}

#define INPUT_BUFFER_MAX 4096

void ghtml_webview_load(char *file) {

	char in[INPUT_BUFFER_MAX];

	if (! getcwd(in, INPUT_BUFFER_MAX)) return;

	if (ghtml_app_directory) g_free(ghtml_app_directory);

	ghtml_app_directory = g_strdup(in);

	if (file) {
		char *content, *actual_content; gsize length;
		if (g_file_get_contents(file, &actual_content, &length, NULL)) {
			content = actual_content;
			if (*content == '#' && *(content+1) == '!') {
				while (*content != 0 && *content != '\n') content++;
			}
			sprintf(in, "file://%s", ghtml_app_directory);
			webkit_web_view_load_string(ghtml_webview, content, ghtml_webview_mime_type, ghtml_webview_document_encoding, in);
			g_free(actual_content);
		} else {
			fprintf(stderr, "%s: error: unable to get contents of `%s'\n", ghtml_app_name, file);
			exit(1);
			return;
		}
	} else {
		ghtml_app_file = "stdin";
		charbuffer *data = charbuffer_from_void();
    	while (fgets(in, INPUT_BUFFER_MAX, stdin) == in) {
			charbuffer_write_format(data, "%s", in);
		}
		sprintf(in, "file://%s", ghtml_app_directory);
		webkit_web_view_load_string(ghtml_webview, data->pointer, ghtml_webview_mime_type, ghtml_webview_document_encoding, in);
		charbuffer_free(data);
	}

}

void ghtml_webview_window_cleared(WebKitWebView *webkitwebview, WebKitWebFrame *frame, gpointer context, gpointer window, gpointer userdata) {
	ghtml_webview_js_init(context, frame);
}

void ghtml_webview_initialize(void *this_container, bool as_transparent) {

	WebKitWebSettings *these_settings = webkit_web_settings_new ();

	if (as_transparent) {
		webkit_web_view_set_transparent(ghtml_webview, as_transparent);
	}

	gtk_container_add(this_container, ghtml_webview);

	g_signal_connect(ghtml_webview, "close-web-view", G_CALLBACK(ghtml_webview_close), ghtml_window);
    g_signal_connect(ghtml_webview, "load-finished", G_CALLBACK(ghtml_webview_load_finished), NULL);
    g_signal_connect(ghtml_webview, "title-changed", G_CALLBACK(ghtml_webview_title_changed), NULL);
    g_signal_connect(ghtml_webview, "window-object-cleared", G_CALLBACK(ghtml_webview_window_cleared), NULL);

	g_object_set (G_OBJECT(these_settings), "auto-resize-window", TRUE, NULL);
	g_object_set (G_OBJECT(these_settings), "resizable-text-areas", FALSE, NULL);
	g_object_set (G_OBJECT(these_settings), "enable-spatial-navigation", TRUE, NULL);
	g_object_set (G_OBJECT(these_settings), "javascript-can-access-clipboard", TRUE, NULL);
	g_object_set (G_OBJECT(these_settings), "enable-dom-paste", TRUE, NULL);
	g_object_set (G_OBJECT(these_settings), "enable-webgl", TRUE, NULL);
	g_object_set (G_OBJECT(these_settings), "enable-webaudio", TRUE, NULL);
	g_object_set (G_OBJECT(these_settings), "enable-private-browsing", TRUE, NULL);
	g_object_set (G_OBJECT(these_settings), "enable-universal-access-from-file-uris", TRUE, NULL);
	g_object_set (G_OBJECT(these_settings), "enable-file-access-from-file-uris", TRUE, NULL);

	if (ghtml_webview_explode == true) 	g_object_set (G_OBJECT(these_settings), "enable-frame-flattening", TRUE, NULL);
	if (ghtml_webview_motif_uri) g_object_set (G_OBJECT(these_settings), "user-stylesheet-uri", ghtml_webview_motif_uri, NULL);

	webkit_web_view_set_settings (ghtml_webview, these_settings);

    gtk_widget_show(ghtml_window);

	ghtml_webview_load(ghtml_app_file);

}

