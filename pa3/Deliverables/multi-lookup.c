#include "multi-lookup.h"

typedef struct req_argument {
  FILE * fs[5];
  FILE * service_log;
  int file_complete[5];
  int index;
  lifo * list;
  pthread_mutex_t mutex[9];
  int * jobs_done;
}req_argument;

typedef struct res_argument {
  lifo * list;
  pthread_mutex_t mutex[9];
  FILE * result_log;
  int * jobs_done;
}res_argument;

int main(int argc, char* argv[]) {

  UNUSED(argc);

  int request_num = atoi(argv[1]);
  int resolve_num = atoi(argv[2]);
  const char * req_log = argv[3];
  const char * res_log = argv[4];
  const char * filename1 = argv[5];
  const char * filename2 = argv[6];
  const char * filename3 = argv[7];
  const char * filename4 = argv[8];
  const char * filename5 = argv[9];

  FILE * f1 = fopen(filename1, "r");
  FILE * f2 = fopen(filename2, "r");
  FILE * f3 = fopen(filename3, "r");
  FILE * f4 = fopen(filename4, "r");
  FILE * f5 = fopen(filename5, "r");
  FILE * q_log = fopen(req_log, "a");
  FILE * s_log = fopen(res_log, "a");

  int i = 0;
  int completed_jobs = 0;
  lifo * list = make_lifo(20);

  if(request_num > MAX_REQUESTER_THREADS) {
    printf("ERROR: Maximum number of requestor threads is %d.\n", MAX_REQUESTER_THREADS);
    printf("You requested %d.\n", request_num);
    return 1;
  }
  if(resolve_num > MAX_RESOLVER_THREADS) {
    printf("ERROR: Maximum number of resolver threads is %d.\n", MAX_RESOLVER_THREADS);
    printf("You requested %d.\n", resolve_num);
    return 1;
  }

  pthread_t resolvers[MAX_RESOLVER_THREADS];
  pthread_t requesters[MAX_REQUESTER_THREADS];

  pthread_mutex_t mt[9];
  for(i = 0; i < 9; i++) {
    pthread_mutex_init(&mt[i], NULL);
  }

  req_argument req_arg;
  req_arg.index = 0;
  req_arg.fs[0] = f1;
  req_arg.fs[1] = f2;
  req_arg.fs[2] = f3;
  req_arg.fs[3] = f4;
  req_arg.fs[4] = f5;
  req_arg.mutex[0] = mt[0];
  req_arg.mutex[1] = mt[1];
  req_arg.mutex[2] = mt[2];
  req_arg.mutex[3] = mt[3];
  req_arg.mutex[4] = mt[4];
  req_arg.mutex[5] = mt[5];
  req_arg.mutex[6] = mt[6];
  req_arg.mutex[7] = mt[7];
  req_arg.mutex[8] = mt[8];
  req_arg.list = list;
  req_arg.file_complete[0] = 0;
  req_arg.file_complete[1] = 0;
  req_arg.file_complete[2] = 0;
  req_arg.file_complete[3] = 0;
  req_arg.file_complete[4] = 0;
  req_arg.jobs_done = &completed_jobs;
  req_arg.service_log = q_log;

  res_argument res_arg;
  res_arg.list = list;
  res_arg.mutex[0] = mt[0];
  res_arg.mutex[1] = mt[1];
  res_arg.mutex[2] = mt[2];
  res_arg.mutex[3] = mt[3];
  res_arg.mutex[4] = mt[4];
  res_arg.mutex[5] = mt[5];
  res_arg.mutex[6] = mt[6];
  res_arg.mutex[7] = mt[7];
  res_arg.mutex[8] = mt[8];
  res_arg.result_log = s_log;
  res_arg.jobs_done = &completed_jobs;

  int req_ret[MAX_REQUESTER_THREADS];
  int res_ret[MAX_RESOLVER_THREADS];

  struct timeval before;
  struct timeval after;

  gettimeofday(&before, NULL);

  // Initiate threads
  for(i = 0; i < request_num; i++) {
    req_ret[i] = pthread_create(&requesters[i], NULL, requester_thread, (void*) &req_arg);
    if(req_ret[i]) {
      printf("Failed to create requestor thread %d. Exiting\n", i);
      return 1;
    }
  }
  printf("All requesters created\n");
  for(i = 0; i < resolve_num; i++) {
    res_ret[i] = pthread_create(&resolvers[i], NULL, resolver_thread, (void*) &res_arg);
    if(res_ret[i]) {
      printf("Failed to create resolver thread %d. Exiting\n", i);
      return 1;
    }
  }
  printf("All resolvers created\n");

  // Join threads
  for(i = 0; i < request_num; i++) {
    pthread_join(requesters[i], NULL);
  }
  for(i = 0; i < resolve_num; i++) {
    pthread_join(resolvers[i], NULL);
  }
  gettimeofday(&after, NULL);

  struct timeval interval;
  interval.tv_usec = after.tv_usec - before.tv_usec;
  printf("Executed in %ld.\n", interval.tv_usec);
  printf("Had %d requesters and %d resolvers.\n", request_num, resolve_num);

  return 0;

}

void *requester_thread(void *ptr) {
  req_argument * args = (req_argument *)ptr;
  FILE * current_file;
  int index;
  pthread_mutex_lock(&args->mutex[ARGS]);
  current_file = args->fs[args->index];
  index = args->index;
  if(args->index == 4) {
    args->index = 0;
  }
  else {
    args->index++;
  }
  pthread_mutex_unlock(&args->mutex[ARGS]);
  char read[MAX_NAME_LENGTH];
  int i = 0;
  size_t l = 0;
  int j = 0;
  int k = 0;
  int serviced = 0;
  do {
    serviced++;
    printf("Started servicing file %d\n", index);
    do {
      do {
        pthread_mutex_lock(&args->mutex[index]);
        l = fread(&read[i], 1, 1, current_file);
        pthread_mutex_unlock(&args->mutex[index]);
        if(l == 0) {
          j = 1;
        }
        i++;
      } while(read[i - 1] != 0x0A);
      read[i - 1] = 0;
      printf("%s\n",read);
      i = 0;
      if(j == 0) {
        while(k == 0) {
          pthread_mutex_lock(&args->mutex[ARRAY]);
          if((is_full(args->list))) {
            pthread_mutex_unlock(&args->mutex[ARRAY]);
          }
          else {
            push(args->list, read);
            k = 1;
            pthread_mutex_unlock(&args->mutex[ARRAY]);
          }
        }
        k = 0;
      }
    } while(j == 0);
    pthread_mutex_lock(&args->mutex[ARGS]);
    if(args->file_complete[index] == 0) {
      args->file_complete[index] = 1;
      *args->jobs_done++;
    }
    pthread_mutex_unlock(&args->mutex[ARGS]);
    j = 0;

    pthread_mutex_lock(&args->mutex[ARGS]);
    while(args->file_complete[index] == 1 && *args->jobs_done <= 5) {
      if(args->index == 4) {
        args->index = 0;
      }
      else {
        args->index++;
      }
    }
    current_file = args->fs[args->index];
    index = args->index;
    if(args->index == 4) {
      args->index = 0;
    }
    else {
      args->index++;
    }
    pthread_mutex_unlock(&args->mutex[ARGS]);
    printf("Moved to next file\n");

  } while(*args->jobs_done <= 5);
  char id[MAX_NAME_LENGTH];
  pthread_mutex_lock(&args->mutex[SERVICED]);
  pthread_t thread_id = pthread_self();
  fwrite("Thread ", 1, 7, args->service_log);
  sprintf(id, "%lu", thread_id);
  fwrite(id, 1, strlen(id), args->service_log);
  fwrite(" serviced ", 1, 10, args->service_log);
  sprintf(id, "%d", serviced);
  fwrite(id, 1, strlen(id), args->service_log);
  pthread_mutex_unlock(&args->mutex[SERVICED]);
  printf("Printed to service output\n");
  return 0;
}

void * resolver_thread(void * ptr) {
  res_argument * args = (res_argument *)ptr;
  char input[MAX_NAME_LENGTH];
  char resolved_ip[MAX_NAME_LENGTH];
  int length = 0;
  while(!(is_empty(args->list))) {
    pthread_mutex_lock(&args->mutex[ARRAY]);
    length = pop(args->list, input);
    printf("popped\n");
    length--;
    pthread_mutex_unlock(&args->mutex[ARRAY]);
    printf("unlocked array\n");
    int ret = dnslookup(input, resolved_ip, MAX_IP_LENGTH);
    if(ret == UTIL_FAILURE) {
      resolved_ip[0] = 0;
    }
    printf("DNS lookup done\n");
    pthread_mutex_lock(&args->mutex[RESULTS]);
    fwrite(input, 1, length, args->result_log);
    fwrite(", ", 1, 2, args->result_log);
    fwrite(resolved_ip, 1, strlen(resolved_ip), args->result_log);
    fwrite("\n", 1, 1, args->result_log);
    pthread_mutex_unlock(&args->mutex[RESULTS]);
  }

  return 0;
}
