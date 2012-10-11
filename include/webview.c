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
			webkit_web_view_load_string(ghtml_webview, content, NULL, NULL, in);
			g_free(actual_content);
		} else {
			fprintf(stderr, "%s: error: unable to get contents of `%s'\n", ghtml_app_name, file);
		}
	} else {
		charbuffer *data = charbuffer_from_void();
    	while (fgets(in, INPUT_BUFFER_MAX, stdin) == in) {
			charbuffer_write_format(data, "%s", in);
		}
		sprintf(in, "file://%s", ghtml_app_directory);
		webkit_web_view_load_string(ghtml_webview, data->pointer, NULL, NULL, in);
		charbuffer_free(data);
	}

}

JSValueRef ghtml_webview_exec (JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception) {

	void *usrcommand;

    void *jsstr = JSValueToStringCopy(ctx, arguments[0], NULL);
	if (!jsstr) return JSValueMakeUndefined(ctx);
	size_t length = JSStringGetMaximumUTF8CStringSize (jsstr);
	usrcommand = g_alloca (length * sizeof (gchar));
	JSStringGetUTF8CString (jsstr, usrcommand, length);

	char **nargv;
	int nargc, child_status;
	char *child_out, *child_error;

	JSObjectRef js_return = (JSObjectRef) JSValueMakeUndefined(ctx);

	if (g_shell_parse_argv(usrcommand, &nargc, &nargv, NULL)) {
		if (g_spawn_sync(NULL, nargv, NULL, G_SPAWN_LEAVE_DESCRIPTORS_OPEN | G_SPAWN_SEARCH_PATH | G_SPAWN_CHILD_INHERITS_STDIN, NULL, NULL, &child_out, &child_error, &child_status, NULL)) {
			js_return = JSValueToObject(ctx, arguments[0], NULL);
			JSObjectSetProperty(ctx, js_return, JSStringCreateWithUTF8CString("stdout"), JSValueMakeString(ctx,JSStringCreateWithUTF8CString(child_out)), kJSPropertyAttributeReadOnly, NULL);
			JSObjectSetProperty(ctx, js_return, JSStringCreateWithUTF8CString("stderr"), JSValueMakeString(ctx,JSStringCreateWithUTF8CString(child_error)), kJSPropertyAttributeReadOnly, NULL);
			JSObjectSetProperty(ctx, js_return, JSStringCreateWithUTF8CString("status"), JSValueMakeNumber(ctx, WEXITSTATUS(child_status)), kJSPropertyAttributeReadOnly, NULL);
			g_free(child_out); g_free(child_error);
		}
		g_strfreev(nargv);
	}

	JSStringRelease(jsstr);

	return js_return;
}

JSValueRef ghtml_webview_console_exec (JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception) {

	void *usrcommand;

    void *jsstr = JSValueToStringCopy(ctx, arguments[0], NULL);
	if (!jsstr) return JSValueMakeUndefined(ctx);
	size_t length = JSStringGetMaximumUTF8CStringSize (jsstr);
	usrcommand = g_alloca (length * sizeof (gchar));
	JSStringGetUTF8CString (jsstr, usrcommand, length);

	char **nargv;
	int nargc, child_status;

	JSObjectRef js_return = (JSObjectRef) JSValueMakeUndefined(ctx);

	if (g_shell_parse_argv(usrcommand, &nargc, &nargv, NULL)) {
		if (g_spawn_sync(NULL, nargv, NULL, G_SPAWN_LEAVE_DESCRIPTORS_OPEN | G_SPAWN_SEARCH_PATH | G_SPAWN_CHILD_INHERITS_STDIN, NULL, NULL, NULL, NULL, &child_status, NULL)) {
			js_return = (JSObjectRef) JSValueMakeNumber(ctx, WEXITSTATUS(child_status));
		}
		g_strfreev(nargv);
	}

	JSStringRelease(jsstr);

	return js_return;

}

JSValueRef ghtml_webview_quit (JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception) {
	void *usrstring;
	size_t length;
	ghtml_app_exit_value = JSValueToNumber(ctx, arguments[0], NULL);
	webkit_web_view_execute_script(ghtml_webview, "window.close();");
	return JSValueMakeUndefined(ctx);
}

JSValueRef ghtml_webview_puts (JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception) {
	void *usrstring;
	size_t length;
	void * jsstr = JSValueToStringCopy(ctx, arguments[0], NULL);
	length = JSStringGetMaximumUTF8CStringSize (jsstr);
	usrstring = g_alloca (length * sizeof (gchar));
	JSStringGetUTF8CString (jsstr, usrstring, length);
	JSStringRelease(jsstr);
	puts(usrstring);
	fflush(stdout);
	return JSValueMakeUndefined(ctx);
}

JSValueRef ghtml_webview_getenv (JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception) {
	void *usrstring;
	size_t length;
	void * jsstr = JSValueToStringCopy(ctx, arguments[0], NULL);
	length = JSStringGetMaximumUTF8CStringSize (jsstr);
	usrstring = g_alloca (length * sizeof (gchar));
	JSStringGetUTF8CString (jsstr, usrstring, length);
	JSStringRelease(jsstr);

	usrstring = (void*) g_getenv(usrstring);
	if (usrstring == NULL) return JSValueMakeUndefined(ctx);
	return JSValueMakeString(ctx,JSStringCreateWithUTF8CString(usrstring));

}

JSValueRef ghtml_webview_setenv (JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception) {
	void *usrstring;
	void *usrstring2;
	size_t length;
	void * jsstr = JSValueToStringCopy(ctx, arguments[0], NULL);
	void * jsstr2 = JSValueToStringCopy(ctx, arguments[1], NULL);
	length = JSStringGetMaximumUTF8CStringSize (jsstr);
	usrstring = g_alloca (length * sizeof (gchar));
	JSStringGetUTF8CString (jsstr, usrstring, length);
	JSStringRelease(jsstr);
	length = JSStringGetMaximumUTF8CStringSize (jsstr2);
	usrstring2 = g_alloca (length * sizeof (gchar));
	JSStringGetUTF8CString (jsstr2, usrstring2, length);
	JSStringRelease(jsstr2);

	return JSValueMakeBoolean(ctx, g_setenv(usrstring, usrstring2, TRUE));

}

JSValueRef ghtml_webview_delenv (JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception) {
	void *usrstring;
	size_t length;
	void * jsstr = JSValueToStringCopy(ctx, arguments[0], NULL);
	length = JSStringGetMaximumUTF8CStringSize (jsstr);
	usrstring = g_alloca (length * sizeof (gchar));
	JSStringGetUTF8CString (jsstr, usrstring, length);
	JSStringRelease(jsstr);
	g_unsetenv(usrstring);
	return JSValueMakeNull(ctx);
}

JSValueRef ghtml_webview_console_error (JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception) {
	char *usrstring;
	size_t length;
	void * jsstr = JSValueToStringCopy(ctx, arguments[0], NULL);
	length = JSStringGetMaximumUTF8CStringSize (jsstr);
	usrstring = g_alloca (length * sizeof (gchar));
	JSStringGetUTF8CString (jsstr, usrstring, length);
	JSStringRelease(jsstr);
	fprintf(stderr, "%s", usrstring);
	fflush(stderr);
	return JSValueMakeUndefined(ctx);
}

JSValueRef ghtml_webview_console_print (JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception) {
	char *usrstring;
	size_t length;
	void * jsstr = JSValueToStringCopy(ctx, arguments[0], NULL);
	length = JSStringGetMaximumUTF8CStringSize (jsstr);
	usrstring = g_alloca (length * sizeof (gchar));
	JSStringGetUTF8CString (jsstr, usrstring, length);
	JSStringRelease(jsstr);
	printf("%s", usrstring);
	fflush(stdout);
	return JSValueMakeUndefined(ctx);
}

void ghtml_webview_induct_view(void *this_window, void *this_frame, void *this_context, void *this_data) {

	if (! ghtml_webview_global_context) {	// This should only run once for the main window and frame.

		ghtml_webview_main_frame = this_frame;
		ghtml_webview_global_context = this_context;

		void *global = JSContextGetGlobalObject(this_context);

	 	void *jsconsolestr = JSStringCreateWithUTF8CString("console");
	 	void *jsconsole = (void *) JSObjectGetProperty(this_context, global, jsconsolestr, NULL);
	 	JSStringRelease(jsconsolestr);

		ghtml_webview_seed = seed_init_with_context (&ghtml_app_argc, &ghtml_app_argv, this_context);

		JSObjectSetProperty(this_context, jsconsole, JSStringCreateWithUTF8CString("error"), JSObjectMakeFunctionWithCallback(this_context, NULL, ghtml_webview_console_error), kJSPropertyAttributeReadOnly, NULL);
		JSObjectSetProperty(this_context, jsconsole, JSStringCreateWithUTF8CString("write"), JSObjectMakeFunctionWithCallback(this_context, NULL, ghtml_webview_puts), kJSPropertyAttributeReadOnly, NULL);
		JSObjectSetProperty(this_context, jsconsole, JSStringCreateWithUTF8CString("print"), JSObjectMakeFunctionWithCallback(this_context, NULL, ghtml_webview_console_print), kJSPropertyAttributeReadOnly, NULL);
		JSObjectSetProperty(this_context, jsconsole, JSStringCreateWithUTF8CString("exec"), JSObjectMakeFunctionWithCallback(this_context, NULL, ghtml_webview_console_exec), kJSPropertyAttributeReadOnly, NULL);

		JSObjectSetProperty(this_context, global, JSStringCreateWithUTF8CString("getenv"), JSObjectMakeFunctionWithCallback(this_context, NULL, ghtml_webview_getenv), kJSPropertyAttributeReadOnly, NULL);
		JSObjectSetProperty(this_context, global, JSStringCreateWithUTF8CString("delenv"), JSObjectMakeFunctionWithCallback(this_context, NULL, ghtml_webview_delenv), kJSPropertyAttributeReadOnly, NULL);
		JSObjectSetProperty(this_context, global, JSStringCreateWithUTF8CString("setenv"), JSObjectMakeFunctionWithCallback(this_context, NULL, ghtml_webview_setenv), kJSPropertyAttributeReadOnly, NULL);
		JSObjectSetProperty(this_context, global, JSStringCreateWithUTF8CString("puts"), JSObjectMakeFunctionWithCallback(this_context, NULL, ghtml_webview_puts), kJSPropertyAttributeReadOnly, NULL);
		JSObjectSetProperty(this_context, global, JSStringCreateWithUTF8CString("exec"), JSObjectMakeFunctionWithCallback(this_context, NULL, ghtml_webview_exec), kJSPropertyAttributeReadOnly, NULL);
		JSObjectSetProperty(this_context, global, JSStringCreateWithUTF8CString("quit"), JSObjectMakeFunctionWithCallback(this_context, NULL, ghtml_webview_quit), kJSPropertyAttributeReadOnly, NULL);

	}

}

void ghtml_webview_window_cleared(WebKitWebView *webkitwebview, WebKitWebFrame *frame, gpointer context, gpointer window, gpointer userdata) {
	ghtml_webview_induct_view(window, frame, context, userdata);
}

void ghtml_webview_initialize(void *this_container, void *this_file, bool as_transparent) {

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

	ghtml_webview_load(this_file);

}

