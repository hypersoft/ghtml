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

static void ghtml_window_destroy(GtkWidget* widget, GtkWidget* window){
	seed_simple_evaluate(ghtml_webview_main_context, "window.close()", NULL);
	gtk_widget_destroy(ghtml_window);
	gtk_main_quit();
	return;
}

gboolean ghtml_window_escaped (GtkAccelGroup *accel_group, GObject *acceleratable, guint keyval, GdkModifierType modifier) {
	webkit_web_view_execute_script(ghtml_webview, "window.close();");
	return true;
}

static void ghtml_window_initialize(int width, int height, bool as_dialog, void *file) {

	ghtml_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	GdkScreen* this_screen = gtk_widget_get_screen(ghtml_window);
	void* this_compositing = gdk_screen_get_rgba_visual(this_screen);

	g_signal_connect(ghtml_window, "destroy", G_CALLBACK(ghtml_window_destroy), NULL);

	void *ghtml_window_scrollable_content_area = gtk_scrolled_window_new(NULL, NULL);

	gtk_scrolled_window_set_policy(
		ghtml_window_scrollable_content_area, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC
	);

	if (ghtml_app_title) gtk_window_set_title(ghtml_window, ghtml_app_title);

	gtk_window_set_default_size(ghtml_window, width, height);

	//gtk_window_set_position(ghtml_window, GTK_WIN_POS_CENTER_ALWAYS);

	gtk_widget_set_visual(ghtml_window_scrollable_content_area, this_compositing);

	ghtml_webview = WEBKIT_WEB_VIEW(webkit_web_view_new());

	gtk_container_add(ghtml_window, ghtml_window_scrollable_content_area);

	if (as_dialog) {
		gtk_window_set_type_hint((void*) ghtml_window, GDK_WINDOW_TYPE_HINT_DIALOG);
		GtkAccelGroup *gtk_accel = gtk_accel_group_new ();
		GClosure *closure;
		closure = g_cclosure_new (G_CALLBACK (ghtml_window_escaped), NULL, NULL);
		gtk_accel_group_connect (gtk_accel, gdk_keyval_from_name ("Escape"), 0, GTK_ACCEL_LOCKED, closure);
		gtk_window_add_accel_group (ghtml_window, gtk_accel);
	}

	ghtml_webview_initialize(ghtml_window_scrollable_content_area, file, true);

}

