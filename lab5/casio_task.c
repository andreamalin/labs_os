#include <sys/time.h>
#include <stdlib.h>
#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <time.h>
// !!!!!! This value is CPU-dependent !!!!!!

//#define LOOP_ITERATIONS_PER_MILLISEC 155000

#define LOOP_ITERATIONS_PER_MILLISEC 178250
//#define LOOP_ITERATIONS_PER_MILLISEC 193750


#define MILLISEC 	1000
#define MICROSEC	1000000
#define NANOSEC 	1000000000



/*   This is how CASIO tasks are specified 

In the file linux-2.6.24-casio/include/linux/sched.h

also in the file /usr/include/bits/sched.h (the user space scheduler file)

struct sched_param {
	int sched_priority;
#ifdef CONFIG_SCHED_SMS_POLICY
	int casio_pid;
	unsigned long long deadline;
#endif
};
*/

double min_offset,max_offset; //seconds
double min_exec_time,max_exec_time; //seconds
double min_inter_arrival_time,max_inter_arrival_time; //seconds

unsigned int casio_id,jid=1; // Id de la task y subtask
struct itimerval inter_arrival_time; // donde se guarda el timer actual

// Correr 1 ms
void burn_1millisecs() {
	unsigned long long i;
	for(i=0; i<LOOP_ITERATIONS_PER_MILLISEC; i++);
}

// Usando el cpu segun los miliseg por cada ciclo
void burn_cpu(long milliseconds){
	long i;
	for(i=0; i<milliseconds; i++)
		burn_1millisecs();
}

// Resetear los valores del sched
void clear_sched_param(struct sched_param *param)
{
	param->casio_id=-1;
	param->deadline=0;
}

// Se obtiene el pid y deadline de la tarea actual
void print_task_param(struct sched_param *param)
{
    	printf("\npid[%d]\n",param->casio_id);
	printf("deadline[%llu]\n",param->deadline);
}

// Se resetean los valores a 0
void clear_signal_timer(struct itimerval *t)
{
	t->it_interval.tv_sec = 0;
	t->it_interval.tv_usec = 0;
	t->it_value.tv_sec = 0;
	t->it_value.tv_usec = 0;
}

// El tiempo que tomara cada ciclo
void set_signal_timer(struct itimerval *t,double secs)
{
	t->it_interval.tv_sec = 0;
	t->it_interval.tv_usec =0 ;
	t->it_value.tv_sec = (int)secs; // 
	t->it_value.tv_usec = (secs-t->it_value.tv_sec)*MICROSEC;
	
}

// Se obtiene cuanto tiempo tomo hacer la tarea
double get_time_value(double min, double max)
{
	if(min==max)
		return min;
	return (min + (((double)rand()/RAND_MAX)*(max-min)));
}

// Se comienza a ejecutar la tarea
void start_task(int s)
{
	printf("\nTask(%d) has just started\n",casio_id);
	set_signal_timer(&inter_arrival_time,get_time_value(min_offset,max_offset));
	setitimer(ITIMER_REAL, &inter_arrival_time, NULL);
}

// Se realiza una task
void do_work(int s)
{
	// Se manda la primera señal con la task a realizar
	signal(SIGALRM, do_work);
	// Se settea el timer de cada senal
	set_signal_timer(&inter_arrival_time,get_time_value(min_inter_arrival_time,max_inter_arrival_time));
	// Se setea un timer con el valor de ITIMER_REAL -> decrementa en tiempo real apuntando guardando el valor en inter_arrival_time
	setitimer(ITIMER_REAL, &inter_arrival_time, NULL);

	clock_t start, end;
	double elapsed=0;
	start = clock();
	// Por cada subtask se obtiene su tiempo de comienzo y su tiempo final
	printf("Job(%d,%d) starts\n",casio_id,jid);
	burn_cpu(get_time_value(min_exec_time,max_exec_time)*MILLISEC);
	end = clock();
	elapsed = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("Job(%d,%d) ends (%f)\n",casio_id,jid,elapsed);
	// Se cambia de subtask
	jid++;
}

// Funcion para saber si la task completa se ha completado
void end_task(int s)
{
	printf("\nTask(%d) has finished\n",casio_id);
	exit(0);
}

int main(int argc, char** argv) {

	struct sched_param param;
	
	unsigned long long seed;
	int i;
	// Reseteando valores
	clear_signal_timer(&inter_arrival_time);
	clear_sched_param(&param);

	// Prioridad de las tasks
	param.sched_priority = 1;

	// Se obtienen los valores de los parametros
	casio_id=param.casio_id=atoi(argv[1]); // Se obtiene el id
	min_exec_time=atof(argv[2]); // Tiempo minimo de ejecucion
	max_exec_time=atof(argv[3]); // Tiempo maximo de ejecucion
	min_inter_arrival_time=atof(argv[4]); // Minimo tiempo de llegada
	max_inter_arrival_time=atof(argv[5]); // Maximo tiempo de llegada
	param.deadline=atof(argv[6])*NANOSEC; // Deadline de la tarea pasado a nanoseg
	min_offset=atof(argv[7]); // Minimo offeset
	max_offset=atof(argv[8]); // Maximo offset
	seed=atol(argv[9]); // Semilla

	srand(seed); // Obteniendo un numero aleatorio
	signal(SIGUSR1, start_task); // Primera senal para hacer la tarea
	signal(SIGALRM, do_work); // Segunda senal para comenzar la tarea
	signal(SIGUSR2, end_task); // Terca senal para finalizar la tarea

	// Obteniendo los prints del txt
	print_task_param(&param);
	printf("Before sched_setscheduler:priority %d\n",sched_getscheduler(0));

	if ( sched_setscheduler( 0, SCHED_CASIO , &param ) ==-1){
		perror("ERROR");
	}
	printf("After sched_setscheduler:priority %d\n",sched_getscheduler(0));

	//Wait for signals while the task is idle
	while(1){
		pause();	
	}

	return 0;
}
	
	
	

	
	
	
	

