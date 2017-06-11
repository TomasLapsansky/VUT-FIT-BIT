/**
 * @Name IOS Projekt 2
 * @author Tomas Lapsansky (xlapsa00)
 * @file proj2.c
 * @date 14.4.2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h> ///< srand()

#include <unistd.h> ///< fork()

#include <fcntl.h> ///< open() write()

#include <sys/types.h>
#include <sys/wait.h> ///< wait()
#include <sys/shm.h> ///<shm - zdielana pamat
#include <sys/ipc.h> ///<shmget flags
#include <sys/stat.h> ///<shmget flags

#include <sys/sem.h>
#include <semaphore.h>
#include <errno.h>

void adult_generator(int A, int AGT, int AWT, int key);
void child_generator(int C, int CGT, int CWT, int key);
void adult(int A, int AWT, int key);
void child(int C, int CWT, int key);


sem_t *memory0_mutex;
sem_t *memory1_mutex;
sem_t *counter_mutex;

sem_t *adult_leave_mutex;
sem_t *child_enter_mutex;

/**
 * Main
 * @brief Nacitava argumenty a spusta procesy
 * @param A pocet Adult procesov
 * @param C pocet Child procesov
 * @param AGT maximalna hodnota doby v ms, po ktorej je generovany novy proces Adult
 * @param CGT maximalna hodnota doby v ms, po ktorej je generovany novy proces Child
 * @param AWT maximalna hodnota doby v ms, pocas ktorej proces Adult simuluje cinnost v centre
 * @param CWT maximalna hodnota doby v ms, pocas ktorej proces Child simuluje cinnost v centre
 */
 
int main(int argc, char* argv[])
{
	if(argc != 7) ///< overenie poctu argumentov
	{
		fprintf(stderr, "Wrong number of arguments!\n");
		return 1;
	}
	
	int A = strtol(argv[1], NULL, 10);
	if(!(A > 0)) ///< overenie spravnej hodnoty A
	{
		fprintf(stderr, "Wrong parameter of argument (A > 0)\n");
		return 1;
	}
	
	int C = strtol(argv[2], NULL, 10);
	if(!(C > 0)) ///< overenie spravnej hodnoty C
	{
		fprintf(stderr, "Wrong parameter of argument (C > 0)\n");
		return 1;
	}
	
	int AGT = strtol(argv[3], NULL, 10);
	if(!(AGT >= 0 && AGT < 5001)) ///< overenie spravnej hodnoty AGT
	{
		fprintf(stderr, "Wrong parameter of argument (AGT >= 0 and AGT < 5001)\n");
		return 1;
	}
	
	int CGT = strtol(argv[4], NULL, 10);
	if(!(CGT >= 0 && CGT < 5001)) ///< overenie spravnej hodnoty CGT
	{
		fprintf(stderr, "Wrong parameter of argument (CGT >= 0 and CGT < 5001)\n");
		return 1;
	}
	
	int AWT = strtol(argv[5], NULL, 10);
	if(!(AWT >= 0 && AWT < 5001)) ///< overenie spravnej hodnoty AWT
	{
		fprintf(stderr, "Wrong parameter of argument (AWT >= 0 and AWT < 5001)\n");
		return 1;
	}
	
	int CWT = strtol(argv[6], NULL, 10);
	if(!(CWT >= 0 && CWT < 5001)) ///< overenie spravnej hodnoty CWT
	{
		fprintf(stderr, "Wrong parameter of argument (CWT >= 0 and CWT < 5001)\n");
		return 1;
	}
	
	srand(time(NULL)); ///< pre generovanie nahodnych cisel z intervalov pre AGT, CGT, AWT, CWT
	
	if((memory0_mutex = sem_open("memory0_mutex", O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED)
		fprintf(stderr, "Errno: %d\n", errno);
		
	if((memory1_mutex = sem_open("memory1_mutex", O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED)
		fprintf(stderr, "Errno: %d\n", errno);
		
	if((counter_mutex = sem_open("counter_mutex", O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED)
		fprintf(stderr, "Errno: %d\n", errno);
		
	if((adult_leave_mutex = sem_open("adult_leave_mutex", O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED)
		fprintf(stderr, "Errno: %d\n", errno);
		
	if((child_enter_mutex = sem_open("child_enter_mutex", O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED)
		fprintf(stderr, "Errno: %d\n", errno);
	
	int key = shmget(IPC_PRIVATE, 512, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR); ///< specificky kluc pre pristup do zdielanej pamate
	
	int* memory = (int*)shmat(key, NULL, 0);
	
	///
	/// Struktura memory:
	/// [0] : CA
	/// [1] : CC
	/// [2] : key
	/// [3] : count
	///
	
	memory[2] = open("proj2.out", O_CREAT | O_WRONLY | O_TRUNC, 0666); ///< otvori subor na zapis, systemove volanie ///< | O_TRUNC
	memory[3] = 0; ///< counter vypisov
	
	for(int i = 0; i < 2; i++)
	{
		pid_t pid = fork();
		
		if((i == 0) && (pid == 0)) ///< prvy child proces (parent)
		{
			adult_generator(A, AGT, AWT, key);
			exit(0);
		}
		else if((i == 1) && (pid == 0)) ///< druhy child proces (child)
		{
			child_generator(C, CGT, CWT, key);
			exit(0);
		}
	}
	
	while(wait(NULL) > 0)
	{
		
	}
	
	///<Docasne
	dprintf(memory[2], "Finish %d\n", getppid());
	///
	
	if(sem_unlink("memory0_mutex") != 0)
		fprintf(stderr, "Error: %d\n", errno);
		
	if(sem_unlink("memory1_mutex") != 0)	
		fprintf(stderr, "Error: %d\n", errno);
		
	if(sem_unlink("counter_mutex") != 0)
		fprintf(stderr, "Error: %d\n", errno);
		
	if(sem_unlink("adult_leave_mutex") != 0)
		fprintf(stderr, "Error: %d\n", errno);
		
	if(sem_unlink("child_enter_mutex") != 0)
		fprintf(stderr, "Error: %d\n", errno);
	
	close(memory[2]);
	shmdt(memory); ///< zahod odkaz na zdielanu pamat
	
	shmctl(key, IPC_RMID, NULL); ///< uvolnenie zdielanej pamate
	
	exit(0);
}

/**
 * Adult generator je child proces hlavneho procesu, generuje adult procesy
 * @param A pocet procesov Adult ktore sa vygeneruju
 * @param AGT maximalna hodnota doby v ms, po ktorej je generovany novy proces Adult
 * @param AWT maximalna hodnota doby v ms, pocas ktorej proces Adult simuluje cinnost v centre
 * @param key kluc pre pristup do zdielanej pamate
 */

void adult_generator(int A, int AGT, int AWT, int key)
{	
	int* memory = (int*)shmat(key, NULL, 0);
	
	memory[0] = 0; ///<CA = 0
	
	shmdt(memory); ///< zahod odkaz na zdielanu pamat
	
	for(int i = 1; i <= A; i++)
	{
		pid_t pid = fork();
		if(pid == 0) ///< child proces
		{
			adult(i, (rand() % (AWT + 1)), key);
			exit(0);
		}
		usleep(AGT);
	}
	
	while(wait(NULL) > 0)
	{
		
	}
	
	exit(0);
}

/**
 * Child generator je child proces hlavneho procesu, generuje child procesy
 * @param C pocet procesov Child ktore sa vygeneruju
 * @param CGT maximalna hodnota doby v ms, po ktorej je generovany novy proces Child
 * @param CWT maximalna hodnota doby v ms, pocas ktorej proces Child simuluje cinnost v centre
 * @param key kluc pre pristup do zdielanej pamate
 */

void child_generator(int C, int CGT, int CWT, int key)
{
	int* memory = (int*)shmat(key, NULL, 0);
	
	memory[1] = 0; ///<CC = 0
	
	shmdt(memory); ///< zahod odkaz na zdielanu pamat
	
	for(int i = 1; i <= C; i++)
	{
		pid_t pid = fork();
		
		if(pid == 0) ///< child proces
		{
			child(i, (rand() % (CWT + 1)), key);
			exit(0);
		}
		usleep(CGT);
	}
	
	while(wait(NULL) > 0)
	{
		
	}
	
	exit(0);
}

/**
 * Simulator Adult procesu
 * @param A poradove cislo procesu
 * @param AWT doba ktoru ma proces simulovat pracu v centre
 * @param key kluc pre pristup do zdielanej pamate
 */

void adult(int A, int AWT, int key)
{
	int* memory = (int*)shmat(key, NULL, 0);
	
	sem_wait(counter_mutex);
	
		memory[3]++;
		dprintf(memory[2], "%d\t: A %d\t: started\n", memory[3], A);
		
	sem_post(counter_mutex);
	
	sem_wait(memory0_mutex);
	
		memory[0]++; //***Atomicky zapis do CA - zdielana pamat TODO
		
			sem_wait(counter_mutex);
			
				memory[3]++;
				dprintf(memory[2], "%d\t: A %d\t: enter\n", memory[3], A);
				
			sem_post(counter_mutex);
			
	sem_post(memory0_mutex);
		
	usleep(AWT);
	
	sem_wait(counter_mutex);
	
		memory[3]++;
		dprintf(memory[2], "%d\t: A %d\t: Trying to leave\n", memory[3], A);//*** Overenie dostupnosti (Atomicke) + wait na zaklade pomeru CA a CC
		
	sem_post(counter_mutex);
	
	if(memory[1] > (3 * (memory[0] - 1)))///< CC > 3 * (CA - 1)
	{
		dprintf(memory[2], "A waitin\n");
	}
	
	sem_wait(memory0_mutex);
	
		memory[0]--;
		
		sem_wait(counter_mutex);
		
			memory[3]++;
			dprintf(memory[2], "%d\t: A %d\t: leave\n", memory[3], A);
			
		sem_post(counter_mutex);
		
	sem_post(memory0_mutex);
	
	sem_wait(counter_mutex);
	
		memory[3]++;
		dprintf(memory[2], "%d\t: A %d\t: finished\n", memory[3], A); //***Pockanie na ukoncenie hlavneho procesu - respektive, na counter number
		
	sem_post(counter_mutex);
	
	shmdt(memory);
}

/**
 * Simulator Child procesu
 * @param C poradove cislo procesu
 * @param CWT doba ktoru ma proces simulovat pracu v centre
 * @param key kluc pre pristup do zdielanej pamate
 */

void child(int C, int CWT, int key)
{
	int* memory = (int*)shmat(key, NULL, 0);
	
	sem_wait(counter_mutex);
	
		memory[3]++;
		dprintf(memory[2], "%d\t: C %d\t: started\n", memory[3], C);
		
	sem_post(counter_mutex);
	
	if((memory[1] + 1) > (3 * (memory[0])))///< (CC + 1) > 3 * CA
	{
		dprintf(memory[2], "C waitin\n");
	}
	
	sem_wait(memory1_mutex);
	
		memory[1]++; //***Atomicky zapis do CC - zdielana pamat TODO
		
		sem_wait(counter_mutex);
		
			memory[3]++;
			dprintf(memory[2], "%d\t: C %d\t: enter\n", memory[3], C);
			
		sem_post(counter_mutex);
		
	sem_post(memory1_mutex);
	
	usleep(CWT);
	
	sem_wait(memory1_mutex);
		memory[1]--;
		
		sem_wait(counter_mutex);
		
			memory[3]++;
			dprintf(memory[2], "%d\t: C %d\t: leave\n", memory[3], C);
			
		sem_post(counter_mutex);
		
	sem_post(memory1_mutex);
	
	
	
	sem_wait(counter_mutex);
	
		memory[3]++;
		dprintf(memory[2], "%d\t: C %d\t: finished\n", memory[3], C);
		
	sem_post(counter_mutex);
}

//***IDEAS
//***Semafor pre overenie dostupnosti centra pre procesy adult a child
//***Praca so semaforom druheho procesu pomocou sem_post