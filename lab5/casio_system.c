#include <sys/time.h>
#include <signal.h>
#include <stdlib.h>
#include <sched.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>

#define  BUF_LEN			200
#define  CASIO_TASKS_NUM		20	

// Estructura por cada task
struct casio_tasks_config {
	int pid;
	double min_exec;
	double max_exec;
	double min_inter_arrival;
	double max_inter_arrival;
	double deadline;
	double min_offset;
	double max_offset;
};

pid_t casio_tasks_pid [CASIO_TASKS_NUM];
int casio_tasks_num=0;

// Se devuelve el valor entero del string obtenido
int get_int_val (char* str) 
{
	char* s = str;
	int val;
	for (s = str;*s!='\t';s++);
	*s='\0';
	val=atoi(str);
	return val;
}
// Reseteando los valores de la task
void clear_casio_tasks_config_info(struct casio_tasks_config *tasks, int num)
{
	int i;
	
	for(i=0;i<num;i++){
		tasks[i].pid=0;
		tasks[i].min_exec=0;
		tasks[i].max_exec=0;
		tasks[i].min_inter_arrival=0;
		tasks[i].max_inter_arrival=0;
		tasks[i].deadline=0;
		tasks[i].min_offset=0;
		tasks[i].max_offset=0;
	}
}
// Se obtiene la configuracion actual de la task
void get_casio_task_config_info(char * str, struct casio_tasks_config *tasks,int *n)
{
	char *s ,*s1;
	int i=0;
	s = s1=str;
	while(i<7){
		if(*s=='\t'){
			*s='\0';
			switch(i){
                // Se obtiene el pid de la task para comenzarla
				case 0:
					tasks[*n].pid = atoi(s1);
					s1=s+1;
					i++;
				break;
                // Esta aun dentro del rango de ejecucion
				case 1:
					tasks[*n].min_exec = atof(s1);
					s1=s+1;
					i++;
				break;
                // Llego a su maximo de tiempo
				case 2:
					tasks[*n].max_exec = atof(s1);
					s1=s+1;
					i++;
				break;
                // Esta dentro de su rango de tiempo de llegada
				case 3:
					tasks[*n].min_inter_arrival = atof(s1);
					s1=s+1;
					i++;
				break;
                // Esta en su tiempo de salida
				case 4:
					tasks[*n].max_inter_arrival = atof(s1);
					s1=s+1;
					i++;
				break;
                // Llego a su tiempo de deadline (terminar task)
				case 5:
					tasks[*n].deadline = atof(s1);
					s1=s+1;
					i++;
				break;
                // Se obtiene el offset de la tarea (margen de error)
				case 6:
					tasks[*n].min_offset = atof(s1);
					s1=s+1;
					i++;
					tasks[*n].max_offset = atof(s1);
				break;

			}
			
		}
		s++;
	}
	(*n)++;	
}

// Funcion para obtener la informacion de la task dependiendo de cual este en ejecucion actualmente
void get_casio_tasks_config_info(char *file, int *duration, struct casio_tasks_config *tasks,int *n)
{
	char buffer[BUF_LEN];
	int count=0;
	FILE* fd  = fopen(file, "r");
	*n=0;
	buffer[0]='\0';
	while( (fgets(buffer, BUF_LEN, fd))!=NULL) {
		if(strlen(buffer)>1){
			switch(count){
				case 0:
					*duration=get_int_val(buffer);
					count++;
				break;
				default:
					get_casio_task_config_info(buffer, tasks,n);
			}
		}
		buffer[0]='\0';
	}
	fclose(fd);
	
}

// Se comienza a ejecutar las tasks
void start_simulation()
{
	int i;
	printf("I will send a SIGUSR1 signal to start all tasks\n");
	for(i=0;i<casio_tasks_num;i++){
		kill(casio_tasks_pid[i],SIGUSR1);
	}
	
}
// Se finalizan todas las tasks
void end_simulation(int signal)
{
	int i;
	printf("I will send a SIGUSR2 signal to finish all tasks\n");
	for(i=0;i<casio_tasks_num;i++){
		kill(casio_tasks_pid[i],SIGUSR2);
	}
	
}
// Por si no se mandan todos los parametros
void help(char* name)
{
	fprintf(stderr, "Usage: %s file_name (system configuration)\n",	name);
	exit(0);
}
int main(int argc, char *argv[])
{
	// Inicializando variables
	int duration,i,j,k,n;
	struct casio_tasks_config casio_tasks_config[CASIO_TASKS_NUM];
	struct itimerval sim_time;
	char arg[CASIO_TASKS_NUM][BUF_LEN],*parg[CASIO_TASKS_NUM];
    // Obteniendo un numero random
	srand(time(NULL));
    // Finalizar programa si faltan parametros
	if(argc!=2){
		help(argv[0]);
	}

    // Reseteando configuracion de las tasks
	clear_casio_tasks_config_info(casio_tasks_config, CASIO_TASKS_NUM);
	// Obteniendo informacion de todas las tasks
	get_casio_tasks_config_info(argv[1],&duration,casio_tasks_config,&casio_tasks_num);

    // Obteniendo valores iniciales de los tiempos
	sim_time.it_interval.tv_sec = 0;
	sim_time.it_interval.tv_usec = 0;
	sim_time.it_value.tv_sec = duration;
	sim_time.it_value.tv_usec = 0;
    // Se manda la primera senal y se comienza el timer de duracion de la senal
	signal(SIGALRM, end_simulation);
	setitimer(ITIMER_REAL, &sim_time, NULL);	

    // Por cada task se obtiene su estado actualmente
	for(i=0;i<casio_tasks_num;i++){
		strcpy(arg[0],"casio_task");	
				
		sprintf(arg[1],"%d",casio_tasks_config[i].pid);				
		sprintf(arg[2],"%f",casio_tasks_config[i].min_exec);			
		sprintf(arg[3],"%f",casio_tasks_config[i].max_exec);			
		sprintf(arg[4],"%f",casio_tasks_config[i].min_inter_arrival);			
		sprintf(arg[5],"%f",casio_tasks_config[i].max_inter_arrival);
		sprintf(arg[6],"%f",casio_tasks_config[i].deadline);	
		sprintf(arg[7],"%f",casio_tasks_config[i].min_offset);		
		sprintf(arg[8],"%f",casio_tasks_config[i].max_offset);	
		sprintf(arg[9],"%ld",rand());			
		n=10;
		for(k=0;k<n;k++){
			//printf("%s  ",arg[k]);
			parg[k]=arg[k];
		}
		parg[k]=NULL;
		
		casio_tasks_pid[i]=fork();
		if(casio_tasks_pid[i]==0){
			execv("./casio_task",parg);
			perror("Error: execv\n");
			exit(0);
		}		
		sleep(1);
	}
	
	start_simulation();  //time zero of the execution
	//waiting for the end of the simulation
	pause();

	for(i=0;i<casio_tasks_num;i++){
		wait(NULL);
	}

	printf("All tasks have finished properly!!!\n");
	return 0;	

}
