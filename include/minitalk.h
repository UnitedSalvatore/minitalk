#ifndef MINITALK_H
#define MINITALK_H

#include <stdint.h>

#define MSGSIZE 7

typedef struct	s_frame
{
	unsigned	is_full:1;
	unsigned	is_completed:1;
	uint8_t 	bitmask;
	unsigned	ctr;
	char 		msg[(MSGSIZE + 1) * sizeof(char)];
}				t_frame;

typedef struct	s_scratchpad
{
	t_frame		frame;
}				t_scratchpad;

t_scratchpad	g_scratchpad;

#define Scratchpad (&g_scratchpad)
#define Frame	   (&Scratchpad->frame)

#define INFO(str) ("Minitalk INFO: " str)
#define SIGMSGRCVD SIGUSR1
#define SIGMSGBLKD SIGUSR2

#endif /* MINITALK_H */