/* -- description --
  This program creates a ring of N processes that send a signal round
  and round cnt times. It is invoked with ./ring N leader cnt where N
  is the number of processes it creates, leader is the process ID of the
  leader process, and cnt is the number of signals sent by every process

  Name: Steeve Johan Otoka Eyota
  Date: 2021-06-02
*/

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

#define BILLION 1.e9;

/*
  The sigaction handler receives a signal as parameter but does not do anything
*/ 
void handle_sigusr1(int signal){
  // does nothing
}

int main(int argc, char ** argv){

  int N = atoi(argv[1]); // number of processes to create
  int leader; // process ID of the leader process
  int leader_flag = 0; // flag to tell the leader process apart
  int child_PID; // process ID of the child process
  int cnt = atoi(argv[3]); // number of signals to be sent by each process
  int cnt_save = cnt; // variable saving the value of cnt

  if(atoi(argv[2]) == 0){ // the leader process assgins leader to its pid
    leader = getpid();
    leader_flag = 1;
  }
  else{ // the child process receives the pid of the leader process
    leader = atoi(argv[2]);
  }

  // time structure
  struct timespec start, end;
  double elapsed_time;

  if(leader_flag == 1){ // the leader process records the starting time
    int start_time = clock_gettime(CLOCK_REALTIME, &start);

    if(start_time == -1){ // checking for errors
      perror("Failure: clock_gettime");
    }
  }  

  char* pathname = "./ring"; // name of the file
  char Nbuffer[20]; // buffer storing the next value of N
  char leaderPID[20]; // buffer storing the leader's PID
  
  sprintf(Nbuffer, "%d", N-1);
  sprintf(leaderPID, "%d", leader);

  // sigaction
  struct sigaction sa;
  sa.sa_handler = &handle_sigusr1;
  sigemptyset(&sa.sa_mask);
  sigaddset(&sa.sa_mask, SIGUSR1);
  sa.sa_flags = 0;
  if(sigaction(SIGUSR1, &sa, NULL) == -1){ // checking for errors
    perror("Error with sigaction");
  }

  // sigset
  sigset_t sig_mask;
  sigemptyset(&sig_mask);

  if(N == 1){ // the last process sends cnt signals to the leader process
    while (cnt > 0){
      kill(leader, SIGUSR1);
      sigsuspend(&sig_mask);
      cnt--;
    }
  }
  else{ // a parent process creates a child process
    child_PID = fork();

    if(child_PID < 0){ // checking for errors
      printf("Error with process\n");
      return 1;
    }
    if(child_PID == 0){ // child process execution
      execlp(pathname, pathname, Nbuffer, leaderPID, argv[3], NULL);
      perror("Error with execlp"); // checking for errors
    }
    else{ // parent process execution
      while (cnt > 0){
        sigsuspend(&sig_mask);
        kill(child_PID, SIGUSR1);
        cnt--;
      }
    }
  }

  wait(NULL); // waiting for all processes to finish
  
  if(leader_flag == 1){ // the leader process records the starting time
    int end_time = clock_gettime(CLOCK_REALTIME, &end);

    if(end_time == -1){ // checking of errors
      perror("Failure: clock_gettime");
    }
    else{ // calculating the elapsed time
      elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/BILLION;

      printf("%d %7.3e\n", cnt_save, elapsed_time);
    }
  }  
}