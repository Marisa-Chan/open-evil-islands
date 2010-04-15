/*
 *  This file is part of Cursed Earth.
 *
 *  Cursed Earth is an open source, cross-platform port of Evil Islands.
 *  Copyright (C) 2009-2010 Yanis Kurganov.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/*
 *  Some ideas are from open source of the greatest Qt Toolkit.
 *  Copyright (C) 2009 Nokia Corporation.
*/

#include <assert.h>

#include <pthread.h>

#include "celib.h"
#include "cealloc.h"
#include "cethread.h"

struct ce_thread {
	pthread_t thread;
};

ce_thread* ce_thread_new(void* (*func)(void*), void* arg)
{
	ce_thread* thread = ce_alloc(sizeof(ce_thread));
	pthread_create(&thread->thread, NULL, func, arg);
	return thread;
}

void ce_thread_del(ce_thread* thread)
{
	if (NULL != thread) {
		pthread_join(thread->thread, NULL);
		ce_free(thread, sizeof(ce_thread));
	}
}

void ce_thread_wait(ce_thread* thread)
{
	pthread_join(thread->thread, NULL);
}

struct ce_thread_mutex {
	pthread_mutex_t mutex;
};

ce_thread_mutex* ce_thread_mutex_new(void)
{
	ce_thread_mutex* mutex = ce_alloc(sizeof(ce_thread_mutex));
	pthread_mutex_init(&mutex->mutex, NULL);
	return mutex;
}

void ce_thread_mutex_del(ce_thread_mutex* mutex)
{
	if (NULL != mutex) {
		pthread_mutex_destroy(&mutex->mutex);
		ce_free(mutex, sizeof(ce_thread_mutex));
	}
}

void ce_thread_mutex_lock(ce_thread_mutex* mutex)
{
	pthread_mutex_lock(&mutex->mutex);
}

void ce_thread_mutex_unlock(ce_thread_mutex* mutex)
{
	pthread_mutex_unlock(&mutex->mutex);
}

struct ce_thread_cond {
	int waiter_count;
	int wakeup_count;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
};

ce_thread_cond* ce_thread_cond_new(void)
{
	ce_thread_cond* cond = ce_alloc(sizeof(ce_thread_cond));
	cond->waiter_count = 0;
	cond->wakeup_count = 0;
	pthread_mutex_init(&cond->mutex, NULL);
	pthread_cond_init(&cond->cond, NULL);
	return cond;
}

void ce_thread_cond_del(ce_thread_cond* cond)
{
	if (NULL != cond) {
		pthread_cond_destroy(&cond->cond);
		pthread_mutex_destroy(&cond->mutex);
		ce_free(cond, sizeof(ce_thread_cond));
	}
}

void ce_thread_cond_wake_one(ce_thread_cond* cond)
{
	pthread_mutex_lock(&cond->mutex);
	cond->wakeup_count = ce_min(cond->wakeup_count + 1, cond->waiter_count);
	pthread_cond_signal(&cond->cond);
	pthread_mutex_unlock(&cond->mutex);
}

void ce_thread_cond_wake_all(ce_thread_cond* cond)
{
	pthread_mutex_lock(&cond->mutex);
	cond->wakeup_count = cond->waiter_count;
	pthread_cond_broadcast(&cond->cond);
	pthread_mutex_unlock(&cond->mutex);
}

void ce_thread_cond_wait(ce_thread_cond* cond, ce_thread_mutex* mutex)
{
	pthread_mutex_lock(&cond->mutex);
	++cond->waiter_count;

	ce_thread_mutex_unlock(mutex);

	int code;
	do {
		code = pthread_cond_wait(&cond->cond, &cond->mutex);
		// many vendors warn of spurious wakeups from pthread_cond_wait,
		// even though POSIX doesn't allow for it...
	} while (0 == code && 0 == cond->wakeup_count);

	assert(cond->waiter_count > 0 && "internal error");
	--cond->waiter_count;

	if (0 == code) {
		assert(cond->wakeup_count > 0 && "internal error");
		--cond->wakeup_count;
	}

	pthread_mutex_unlock(&cond->mutex);

	ce_thread_mutex_lock(mutex);
}
