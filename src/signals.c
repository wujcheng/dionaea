/********************************************************************************
 *                               Dionaea
 *                           - catches bugs -
 *
 *
 *
 * Copyright (C) 2009  Paul Baecher & Markus Koetter
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 * 
 * 
 *             contact nepenthesdev@gmail.com  
 *
 *******************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <ev.h>
#include <glib.h>

#include "dionaea.h"
#include "signals.h"
#include "modules.h"
#include "log.h"

#define D_LOG_DOMAIN "log"

void sigint_cb(struct ev_loop *loop, struct ev_signal *w, int revents)
{
	g_warning("%s loop %p w %p revents %i",__PRETTY_FUNCTION__, loop, w, revents);
	ev_unloop (loop, EVUNLOOP_ALL);
}

void sighup_cb(struct ev_loop *loop, struct ev_signal *w, int revents)
{
	g_warning("%s loop %p w %p revents %i",__PRETTY_FUNCTION__, loop, w, revents);

	// modules ...
	modules_hup();

	// loggers hup
	for (GList *it = g_dionaea->logging->loggers; it != NULL; it = it->next)
	{
		struct logger *l = it->data;
		g_message("Logger %p hup %p", l, l->log);
		if (l->hup != NULL)
			l->hup(l->data);
	}
}

