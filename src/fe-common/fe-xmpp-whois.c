/*
 * $Id$
 *
 * Copyright (C) 2007 Colin DIDIER
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *	  
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *			  
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */		 

#include "module.h"
#include "levels.h"
#include "module-formats.h"
#include "printtext.h"
#include "signals.h"

#include "xmpp-servers.h"
#include "xmpp-rosters-tools.h"

static void
sig_version(XMPP_SERVER_REC *server, const char *jid, const char *name,
    const char *version, const char *os)
{
	XMPP_ROSTER_USER_REC *user;
	char *str1, *str2 = NULL;
	const char *username;

	g_return_if_fail(jid != NULL);

	if (name == NULL && version == NULL && os == NULL)
		return;

	str1 = g_strconcat("is running ",
	    name != NULL ? name : "",
	    name != NULL && version != NULL ? " " : "",
	    version != NULL ? version : "",
	    (name != NULL || version != NULL) && os != NULL ? " - " : "",
	    os != NULL ? "on " : "",
	    os != NULL ? os : "", NULL);

	user = xmpp_rosters_find_user(server->roster, jid, NULL);
	if (user != NULL && user->name != NULL) {
		str2 = g_strconcat(" (", jid, ")", NULL);
		username = user->name;
	} else
		username = jid;

	printformat_module(MODULE_NAME, server, jid, MSGLEVEL_CRAP,
	    XMPPTXT_MESSAGE_EVENT, username, str1, str2);

	g_free(str1);
	g_free(str2);
}

static void
event_begin_of_vcard(XMPP_SERVER_REC *server, const char *jid)
{
	g_debug("VCARD: %s", jid);
}

static void
event_vcard_value(XMPP_SERVER_REC *server, const char *jid, const char *name,
    const char *value)
{
	g_debug("    %s: %s", name, value);
}

static void
event_vcard_subvalue(XMPP_SERVER_REC *server, const char *jid,
    const char *name, const char *adressing, const char *subname,
    const char *subvalue)
{
}   

static void
event_end_of_vcard(XMPP_SERVER_REC *server, const char *jid)
{
	g_debug("End of VCARD");
}

void
fe_xmpp_whois_init(void)
{
	signal_add("xmpp version", (SIGNAL_FUNC)sig_version);
	signal_add("xmpp begin of vcard", (SIGNAL_FUNC)event_begin_of_vcard);
	signal_add("xmpp vcard value", (SIGNAL_FUNC)event_vcard_value);
	signal_add("xmpp vcard subvalue", (SIGNAL_FUNC)event_vcard_subvalue);
	signal_add("xmpp end of vcard", (SIGNAL_FUNC)event_end_of_vcard);
}

void
fe_xmpp_whois_deinit(void)
{   
	signal_remove("xmpp version", (SIGNAL_FUNC)sig_version);
	signal_remove("xmpp begin of vcard",
	    (SIGNAL_FUNC)event_begin_of_vcard);
	signal_remove("xmpp vcard value", (SIGNAL_FUNC)event_vcard_value);
	signal_remove("xmpp vcard subvalue",
	    (SIGNAL_FUNC)event_vcard_subvalue);
	signal_remove("xmpp end of vcard", (SIGNAL_FUNC)event_end_of_vcard);
}
