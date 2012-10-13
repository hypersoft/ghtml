
SeedValue ghtml_webview_exec (SeedContext ctx, SeedObject function, SeedObject thisObject, gsize argumentCount, const SeedValue arguments[], SeedException * exception) {

	char ** nargv;
	gchar * usrcommand = seed_value_to_string(ctx, arguments[0], exception);

	int nargc, child_status;

	SeedValue exec = (SeedValue) JSValueMakeUndefined(ctx);

	gchar *exec_child_out, *exec_child_err;
	gint exec_child_status;

	if (g_shell_parse_argv(usrcommand, &nargc, &nargv, NULL)) {

		if (g_spawn_sync(NULL, nargv, NULL, G_SPAWN_LEAVE_DESCRIPTORS_OPEN | G_SPAWN_SEARCH_PATH | G_SPAWN_CHILD_INHERITS_STDIN, NULL, NULL, &exec_child_out, &exec_child_err, &exec_child_status, NULL)) {

			exec = JSValueToObject(ctx, arguments[0], NULL);

			seed_object_set_property(ctx, exec, "stdout", seed_value_from_string(ctx, exec_child_out, exception));
			g_free(exec_child_out); 

			seed_object_set_property(ctx, exec, "stderr", seed_value_from_string(ctx, exec_child_err, exception));
			g_free(exec_child_err); 

			seed_object_set_property(ctx, exec, "status", seed_value_from_int(ctx, WEXITSTATUS(exec_child_status), exception));

		}

		g_strfreev(nargv);

	}

	g_free(usrcommand);

	return exec;

}

