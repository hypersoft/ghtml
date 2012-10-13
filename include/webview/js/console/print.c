
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

