
SeedValue ghtml_webview_console_exec (SeedContext ctx, SeedObject function, SeedObject thisObject, size_t argumentCount, SeedValue arguments[], SeedException * exception) {

	char ** nargv;
	gchar * usrcommand = seed_value_to_string(ctx, arguments[0], exception);

	int nargc, child_status;

	SeedValue exec = (SeedValue) JSValueMakeUndefined(ctx);

	if (g_shell_parse_argv(usrcommand, &nargc, &nargv, NULL)) {

		if (g_spawn_sync(NULL, nargv, NULL, G_SPAWN_LEAVE_DESCRIPTORS_OPEN | G_SPAWN_SEARCH_PATH | G_SPAWN_CHILD_INHERITS_STDIN, NULL, NULL, NULL, NULL, &child_status, NULL)) {

			exec = (JSObjectRef) JSValueMakeNumber(ctx, WEXITSTATUS(child_status));
		}

		g_strfreev(nargv);

	}

	g_free(usrcommand);

	return exec;

}

