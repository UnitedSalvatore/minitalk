#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include "minitalk.h"
#include "ft_printf.h"

void    sig_receiver(int sig, siginfo_t *info, void *context)
{
	(void)context;
    if ((SIGUSR1 != sig) && (SIGUSR2 != sig))
        return ;
    while (Frame->is_full ||  Frame->is_completed)
        usleep(30);
    if (SIGUSR1 == sig)
        Frame->msg[Frame->ctr] |= Frame->bitmask;
    Frame->bitmask <<= 1;
    if (Frame->bitmask)
        return ;
    Frame->bitmask = 1;
    if ((!Frame->msg[Frame->ctr]))
    {
        Frame->is_completed = 1;
        kill(info->si_pid, SIGMSGRCVD);
    }
    (Frame->ctr)++;
    Frame->msg[Frame->ctr] = '\0';
    if (MSGSIZE == Frame->ctr)
        Frame->is_full = 1;
}

void    loop_printer(void)
{
    while(42)
    {
        pause();
        if (!Frame->is_full && !Frame->is_completed)
            continue ;
        ft_printf("%s", Frame->msg);
        if (Frame->is_completed)
            ft_printf("\n");
        Frame->ctr = 0;
        Frame->msg[Frame->ctr] = '\0';
        Frame->is_full = 0;
        Frame->is_completed = 0;
    }

}

int     main(void)
{
    pid_t   pid;
    struct  sigaction action;

    pid = getpid();
    ft_printf(INFO("Server pid %d \n"), pid);
    action.sa_flags = SA_SIGINFO;
    action.sa_sigaction = sig_receiver;
    if (sigaction(SIGUSR1, &action, NULL) || \
        sigaction(SIGUSR2, &action, NULL))
    {
        ft_dprintf(STDERR_FILENO, INFO("Error during signal initialization\n"));
        return(EXIT_FAILURE);
    }
    Frame->bitmask = 1;
    loop_printer();
    return(EXIT_SUCCESS);
}