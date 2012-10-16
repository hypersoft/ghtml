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

void * ghtml_webview_inspector_window = 0;
void * ghtml_webview_inspector = 0;
void * ghtml_webview_inspector_window_scroller = 0;

gboolean ghtml_webview_inspector_window_show(void *inspector, void * data) {

    gtk_widget_show_all(ghtml_webview_inspector_window);

	return true;

}

WebKitWebView * ghtml_webview_inspect(void * inspector, void * webview, void * data) {

	ghtml_webview_inspector_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	gtk_window_set_transient_for(ghtml_webview_inspector_window, ghtml_window);
	gtk_window_set_destroy_with_parent(ghtml_webview_inspector_window, true);

	gtk_window_set_position(
		ghtml_webview_inspector_window, 
		GTK_WIN_POS_CENTER_ON_PARENT
	);

	g_signal_connect_swapped(ghtml_webview_inspector_window, "destroy",
        G_CALLBACK(gtk_widget_destroy), G_OBJECT(ghtml_webview_inspector_window)
	);

    GtkWidget *newWebView = webkit_web_view_new();
    gtk_container_add(GTK_CONTAINER(ghtml_webview_inspector_window), newWebView);

	g_signal_connect(G_OBJECT (inspector), "show-window",
		G_CALLBACK(ghtml_webview_inspector_window_show), NULL
	);

	gtk_window_set_default_size(ghtml_webview_inspector_window, 400, 500);
	gtk_window_set_title(ghtml_webview_inspector_window, "Web Inspector");

	gtk_window_set_type_hint(
		ghtml_webview_inspector_window, GDK_WINDOW_TYPE_HINT_DIALOG
	);

	return (void *) newWebView;

}

void ghtml_webview_inspector_init(void *inspector) {

	ghtml_webview_inspector = inspector;
	g_signal_connect(inspector, "inspect-web-view", G_CALLBACK(ghtml_webview_inspect), NULL);
	//g_signal_connect (inspector, "show-window", G_CALLBACK(show_inpector_window), NULL));

}
