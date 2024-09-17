#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#define MAX_COUNT 30
#define MAX_TIME 10

void ChildProcess(int child_number); /* child process prototype */
void ParentProcess(void); /* parent process prototype */

int main(void) {
    pid_t pid;
    int status;
    int i;

    /* Seed the random number generator */
    srandom(time(NULL));

    for (i = 1; i <= 2; i++) {
        pid = fork();
        if (pid < 0) {
            perror("Fork failed");
            exit(1);
        } else if (pid == 0) {
            ChildProcess(i); /* This code runs in child process */
            exit(0); /* Ensure child exits here */
        }
    }

    /* Parent process waits for both children to complete */
    ParentProcess();

    return 0;
}

void ChildProcess(int child_number) {
    int i;
    pid_t pid = getpid();
    pid_t ppid = getppid();
    int iterations = random() % (MAX_COUNT + 1);  /* Random number of iterations, up to 30 */

    for (i = 0; i < iterations; i++) {
        int sleep_time = random() % (MAX_TIME + 1);  /* Random sleep time, up to 10 seconds */
        printf("Child Pid: %d is going to sleep!\n", pid);
        sleep(sleep_time);
        printf("Child Pid: %d is awake!\nWhere is my Parent: %d?\n", pid, ppid);
    }
}

void ParentProcess(void) {
    int status;
    pid_t pid;

    /* Parent waits for the first child to finish */
    pid = wait(&status);
    printf("Child Pid: %d has completed.\n", pid);
    
    /* Parent waits for the second child to finish */
    pid = wait(&status);
    printf("Child Pid: %d has completed.\n", pid);
}
