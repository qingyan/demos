//build this source code: gcc this_file_name -lpthread -lm

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include<math.h>

#define MEGEXTRA 1000000

//for the demo1
void *print_message_function(void *ptr);

//for the demo2
void *say_hello(void *threadid);

//for the demo3
void *busy_works(void *threadid);

//for the demo4
void *works4demo(void *threadid);

pthread_attr_t universe_attr;
main()
{
    pthread_attr_init(&universe_attr);
    int enable_demo = 4;

    //demo 1
    printf("\n demo1\n");
    pthread_t thread1, thread2;

    char *msg1 = "Thread1";
    char *msg2 = "Thread2";

    int iret1, iret2;

    /* pthread_create
     *
     * Create a new thread, starting with execution of START-ROUTINE
     * getting passed ARG.  Creation attributed come from ATTR.  The new
     * handle is stored in *NEWTHREAD.  */
    iret1 = pthread_create(&thread1, NULL, print_message_function, (void *)msg1);
    iret2 = pthread_create(&thread2, NULL, print_message_function, (void *)msg2);
   
    /* pthread_join
     *
     * Make calling thread wait for termination of the thread TH.  The 
     * exit status of the thread is stored in *THREAD_RETURN, if THREAD_RETURN
     * is not NULL.
     *
     * This function is a cancellation point and therefore not marked with __THROW.  */

    //Wait until thread returns 
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("Thread 1 returns %d\n", iret1);
    printf("Thread 2 returns %d\n", iret2);

    int threads_num = 10;
    pthread_t threads[threads_num];
    int rc;
    long t;

    if(enable_demo == 2){
        //demo 2
        printf("\n demo2\n");
 
        for(t = 0; t < threads_num; t++){
            printf("In main : creating thread%ld\n", t);
            rc = pthread_create(&threads[t], NULL, say_hello, (void *)t);
    //        rc = pthread_create(&threads[t], NULL, say_hello, (void *)&t); 
        }

        //The pthread_exit() function will subsequently invoke into pthread_join()
        pthread_exit(NULL);
    }
    //demo 3
    printf(">>>\n   demo3\n");

    if(enable_demo == 3){
        threads_num = 9;
        threads[threads_num];
        
        rc = -1;
        t = 0;
        void *status;
        pthread_attr_t attr;

        //initialize the attr status
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);//Joinable or detached

        for(t=0; t < threads_num; t++){
            rc = pthread_create(&threads[t], &attr, busy_works, (void *)t);
            //catch the error and exit(-1)...
        }

        //free attr and wait other threads
        pthread_attr_destroy(&attr);
        for(t = 0; t < threads_num; t++){
           rc = pthread_join(threads[t], &status);
           //if(rc) to catch the errors
        }

        printf("Program completed exiting...\n");
        pthread_exit(NULL);
    }

    //demo 4 about the stack
    printf(">>>\n   demo4\n");
    if(enable_demo == 4){
        threads_num = 4;
        threads[threads_num];
       
        rc = -1;
        t = 0;
        size_t stack_size;
        
        pthread_attr_init(&universe_attr);
        pthread_attr_getstacksize(&universe_attr, &stack_size);
//       pthread_attr_setdetachstate(&universe_attr, PTHREAD_CREATE_DETACHED);//PTHREAD_CREATE_JOINABLE
        printf("default stack size =%li\n", stack_size);

        int N = 1000; // match the works4demo
        stack_size = sizeof(double) * N * N + MEGEXTRA;
        printf("needed stack size is %li\n", stack_size);
        pthread_attr_setstacksize(&universe_attr, stack_size);
        printf("stack size set, stack_size=%li bytes\n", stack_size);

        int i, j;
        for(t = 0; t < threads_num; t++){
            rc = pthread_create(&threads[t], &universe_attr, works4demo, (void *)t);
//            pthread_join(threads[t], NULL);
             //if(rc != 0) catch the exception
        }
        //the following log may be print before the log from threads, because we do not use pthread_join
        printf("demo4 completed creating threads t=%ld...\n", t);
        pthread_exit(NULL);
    }

    exit(0);
}

void *print_message_function(void *ptr)
{
    char *msg = (char *)ptr;
    printf("%s\n", msg);
}

void *say_hello(void *threadid)
{
    long tid;
    tid = (long)threadid;
    printf("hello world! said by thread%ld\n", tid);
    pthread_exit(NULL);
}

void *busy_works(void *threadid)
{
    long tid = (long)threadid;
    double result;
    int i;
    for(i = 0; i < 1000000; i++){
        result = result + sin(i) * tan(i);
    }
    printf("Thread%ld done, result=%e\n", tid, result);
    pthread_exit((void *)threadid);
}

void *works4demo(void *threadid)
{
    int N = 1000;
    double A[N][N];
    int i, j;
    long tid;
    size_t my_stack_size;

    tid = (long)threadid;
    pthread_attr_getstacksize(&universe_attr, &my_stack_size);
    printf("Thread %ld : load the my_stack_size=%li bytes\n", tid, my_stack_size);

    for(i = 0; i < N; i++){
        for(j = 0; j < N; j++){
            A[i][j] = (i * j)/3.452 + (N - i);
        }
    }
    pthread_exit(NULL);
}
