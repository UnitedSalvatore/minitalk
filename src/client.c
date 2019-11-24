#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include "minitalk.h"
#include "libft.h"
#include "ft_printf.h"
#include "unistd.h"

void    send_char(pid_t const pid, char const byte)
{
    uint8_t bitmask;
    int     sig;

    bitmask  = 1;
    while (bitmask)
    {
        sig = (byte & bitmask) ? SIGUSR1 : SIGUSR2;
        bitmask <<= 1;
        if (kill(pid, sig))
        {
            ft_dprintf(STDERR_FILENO, INFO("An error occurred while sending a signal\n"));
            exit(EXIT_FAILURE);
        }
        usleep(600);
    }
}

void    send_message(pid_t pid, char const *message)
{
    while (*message)
        send_char(pid, *message++);
    send_char(pid, *message);
}

void    server_return_status(int sig)
{
    if (SIGMSGBLKD == sig)
    {
        ft_printf(INFO("Server rejected message\n"));
        exit(EXIT_FAILURE);
    }
    if (SIGMSGRCVD == sig)
        ft_printf(INFO("Server acknowledged receipt of message\n"));
}

int     main(int argc, char const *argv[])
{
    pid_t       pid;
    char const *msg;

    if (3 != argc)
    {
        ft_dprintf(STDERR_FILENO, "usage ./client SERVER_PID message\n");
        return (EXIT_FAILURE);
    }
    pid = ft_atoi(argv[1]);
    msg = argv[2];
    signal(SIGMSGRCVD, server_return_status);
    signal(SIGMSGBLKD, server_return_status);
    send_message(pid, msg);
    return (EXIT_SUCCESS);
}