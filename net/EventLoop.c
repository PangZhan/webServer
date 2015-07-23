#include "EventLoop.h"
#include "Epoll.h"
#include "Event.h"
#include "SignalEvent.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

void eventLoopInit(struct EventLoop* loop)
{
	assert(loop != NULL);

#ifdef DEBUG
	printf("event loop init.\n");
#endif

	loop->epoll = (struct Epoll*)malloc(sizeof(struct Epoll));
	assert(loop->epoll != NULL);
	epollInit(loop->epoll);
	
	loop->sevent = (struct SignalEvent*)malloc(sizeof(struct SignalEvent));
	assert(loop->sevent != NULL);
	signalInit(loop->sevent, loop);
}

void eventLoopAdd(struct EventLoop* loop, struct Event* event)
{
	assert(loop != NULL && event != NULL);

#ifdef DEBUG
	printf("event loop add: type = %d\n", event->type);
#endif	
	
	switch(event->type)
	{
		case EV_READ:
		case EV_WRITE:
			epollAdd(loop->epoll, event);
			break;
		
		case EV_SIGNAL:
			signalAdd(loop->sevent, event);
			break;

		case EV_TIMER:
			break;

		default:
			printf("unkown event\n");
			break;
	}
}

void eventLoopDel(struct EventLoop* loop, struct Event* event)
{
	assert(loop != NULL && event != NULL);

#ifdef DEBUG
	printf("event loop  delete: type = %d\n", event->type);
#endif	
	
	switch(event->type)
	{
		case EV_READ:
		case EV_WRITE:
			epollDelete(loop->epoll, event);
			break;
		
		case EV_SIGNAL:
			signalDel(loop->sevent, event);
			break;

		case EV_TIMER:
			break;

		default:
			printf("unkown event\n");
			break;
	}	
}

void eventLoopDispatch(struct EventLoop* loop)
{
	assert(loop != NULL);
	
	epollDispatch(loop->epoll, -1);
}

void eventLoopClose(struct EventLoop* loop)
{
	assert(loop != NULL);

	epollClose(loop->epoll);
}


