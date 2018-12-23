#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/mman.h>
#include <assert.h>

typedef struct son{
  pid_t pid;
  int num_elems;
  int offset;
} son_t;

typedef int pipe_t[2];

static int syncro[2];

static int init_syncro(void){
  char go = 'a';
  pipe(syncro);
  write(syncro[1],&go,1);
}

static void lock(void){
  char res;
  read(syncro[0],&res,1);
}

static void unlock(void){
  char go = 'b';
  write(syncro[1],&go,1);
}

static int send_all(int fd, void *buffer, int size){
  ssize_t ret = 0;
  do{
    ret += write(fd,(char *)buffer+ret,size-ret);    
  } while(ret != size);
  return 0;
}

static int recv_all(int fd, void *buffer, int size){
  ssize_t ret = 0;
  do{
    ret += read(fd,(char *)buffer+ret,size-ret);    
  } while(ret != size);
  return 0;
}

static int find_max(volatile int *first,int size)
{
  int idx;
  int max = *first;

  for(idx = 1; idx < size ; idx++){
    if(*(first + idx) > max)
      max = *(first + idx);
  }  

  return max;
}

int main(int argc, char *argv[]){
  if(argc == 3){
    int num_fils = atoi(argv[1]);
    int num_vals = atoi(argv[2]);
    pipe_t *dest = NULL;
    son_t *sons  = NULL;
    int *values  = NULL;
    int max;
    int idx;
    void *base_ptr = NULL;
    volatile int *array = NULL;
    volatile int *global_max = NULL; 

    dest = (pipe_t *)malloc(num_fils*sizeof(pipe_t));
    sons = (son_t  *)malloc(num_fils*sizeof(son_t));
    memset(sons,0,num_fils*sizeof(son_t));

    base_ptr = mmap(NULL, (num_vals + 1)*sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    assert(base_ptr);

    global_max = (int *)base_ptr; 
    array = ++global_max;

    fprintf(stdout,"Array start @%p\n",array);
    
    srand(getpid());
    for(idx = 0; idx < num_vals; idx++){
      array[idx] = rand();
      //fprintf(stdout,"Master[%i]:%i\n",idx,array[idx]);
    }

    *global_max = array[0];
    
    init_syncro();

    for(idx = 0; idx < num_fils; idx++){
      sons[idx].num_elems = num_vals/num_fils;
      if((num_vals%num_fils) && (idx < num_vals%num_fils))
	sons[idx].num_elems++;
      if (idx > 0)
	sons[idx].offset = sons[idx-1].num_elems+sons[idx-1].offset;
      
      pipe(dest[idx]);
      
      sons[idx].pid = fork();
      
      if(0 == sons[idx].pid){
	int idx2;
	int offset;
	int size;
	int local_max;
	
	close(dest[idx][1]);
	for(idx2 = 0 ; idx2 < idx ; idx2++){
	  close(dest[idx2][1]);
	}

	recv_all(dest[idx][0],&offset,sizeof(int));
	recv_all(dest[idx][0],&size,sizeof(int));
	fprintf(stdout,"Proc[%i] : handle %i elements @ %p\n",getpid(),size,array+offset);

	local_max = find_max(array+offset,size);	
	lock();
	if (local_max > *global_max)
	  *global_max = local_max;
	unlock();	
	
	exit(0);
      } else if(sons[idx].pid > 0){
	close(dest[idx][0]);
      }
    }
    
    for(idx = 0; idx < num_fils; idx++){
      send_all(dest[idx][1],&sons[idx].offset,sizeof(int));
      send_all(dest[idx][1],&sons[idx].num_elems,sizeof(int));
    }

    fprintf(stdout,"============> Global max is %i (probably false)\n",*global_max);
    
    for(idx = 0; idx < num_fils; idx++){
      wait(NULL);
    }

    fprintf(stdout,"============> Global max is %i\n",*global_max);
  }  
  return 0;
}
  
