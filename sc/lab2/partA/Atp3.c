#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "p3_helper.h"

#define DEBUG

#ifdef DEBUG
#define DEBUG_PRINTF(...) printf("DEBUG: "__VA_ARGS__)
#else
#define DEBUG_PRINTF(...) do {} while (0)
#endif

//The number of jobs to process
#define JOBS_NO 8 

//The number of concurrent workers
#define SIMULTANEOUS_WORKERS_MAX 2

// typedef struct {
// 	unsigned long input_value; //The value we want to compute
// 	unsigned char processed_flag; // boolean - Flag the result
// 	unsigned long result; //Stores the result after processing
// 	unsigned int worker_pid; //Worker process id - to track the process pid.
// } JOB_Handler;

void dispatch_jobs_v0(JOB_Handler jobs[], int number_of_jobs)
{
	//Process all jobs - monolithic approach
	for (int i=0; i<number_of_jobs; i++){
		DEBUG_PRINTF("Working job: %d (%lu)\n", i+1, jobs[i].input_value);
		JOB_update_result_and_processing_status_clear(jobs + i, prime_count(jobs[i].input_value));
	}

	DEBUG_PRINTF("All jobs processed!\n");
}

void dispatch_jobs_v1(JOB_Handler jobs[], int number_of_jobs)
{	
	//Process all jobs - One process (worker) per job
	for (int i = 0;  i < number_of_jobs; i++){ 
		pid_t pid = fork();

		if (pid == 0) //If value is zero in the child task
		{
			DEBUG_PRINTF("Working job: %d (%lu)\n", i+1, jobs[i].input_value);
			int value = prime_count(jobs[i].input_value);
			exit(value);
		}
		else //Dad waits for child to end
		{	
			int status; 
			wait(&status);

			if (WIFEXITED(status))
			{
				JOB_update_result_and_processing_status_clear(&jobs[i],WEXITSTATUS(status));
			}
			
		}

	}
	
}

void dispatch_jobs_v2(JOB_Handler jobs[], int number_of_jobs)
{
	//Process all jobs - One process (worker) per job, with
	//a limit on the number of simultaneous workers (SIMULTANEOUS_WORKERS_MAX)

}

int main(int argc, char const *argv[])
{
	/* 
	 * In  this main function we'll 
	 * 1 - Parametrize a set of job tasks 
	 * 2 - Call the job processing function 
	 * 3 - Print the processing results 
	 */

	JOB_Handler local_var_job_array[JOBS_NO];
	
	//Seeding...
	srand(time(NULL));

	//Fill in some random values
	for (int i=0; i<JOBS_NO; i++)
		JOB_init(local_var_job_array + i, (1+(rand() % 10))*10000);  //[1..10]*10000	
	
	//Process all jobs - blocking function
	//dispatch_jobs_v0(local_var_job_array, JOBS_NO);
	dispatch_jobs_v1(local_var_job_array, JOBS_NO);
	//dispatch_jobs_v2(local_var_job_array, JOBS_NO);

	//Print the results
	for (int i=0; i<JOBS_NO; i++){
		printf("Job %d, ", i+1);

		if (local_var_job_array[i].processed_flag) 
			printf(" Processed. Result=%lu", local_var_job_array[i].result);
		else 
			printf(" Not Processed.");

		printf("\n");
	}

	return 0;
}
