#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <unistd.h>
#include "Wait.h"
#include <sys/wait.h>

Wait::Wait(int argc, char **argv)
    : POSIXApplication(argc, argv)
{
    parser().setDescription("Wait for a child process to stop or terminate");
    parser().registerPositional("PID", "Process ID of child to wait for");
}

Wait::~Wait()
{
}

Wait::Result Wait::exec()
{
    const char * pid = arguments().get("PID");
    int status;

    // Check input for letters
    for (int i = 0; i < strlen(pid); i++) {
        if(!isdigit(pid[i])) {
            errno = EINVAL;
            ERROR("Invalid PID: " << arguments().get("PID"));
            return InvalidArgument;
        }
    }    

    // No letters found, Wait now
    if(waitpid(atoi(pid), &status, 0) == -1) {
        ERROR("Failed to wait: " << strerror(errno));
        return NotFound;
    }

    // Done
    return Success;
}
