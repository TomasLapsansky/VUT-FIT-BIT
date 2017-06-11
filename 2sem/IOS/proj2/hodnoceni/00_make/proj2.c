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

#include <signal.h>

void adult_generator(int A, int AGT, int AWT, int key);
void child_generator(int C, int CGT, int CWT, int key);
void adult(int A, int AWT, int key);
void child(int C, int CWT, int key);

sem_t *shared_memory_mutex;

sem_t *adult_leave_mutex;
sem_t *child_enter_mutex;

sem_t *terminate_mutex;

int glob_key; ///< pridany globalny kluc pre pristup do pamati, kvoli ukoncovacim signalom, bolo by jednoduhsie prestat pouzivat key, no je to detail

/**
 * End
 * @brief Uvolnuje semafory a zdielanu pamat
 * @param key kluc ku zdielanej pamati, ak == -1, zdielana pamat nebola vytvorena a teda nebude ani uvolnovana
 */

void end(int key)
{
	if(sem_unlink("shared_memory_mutex") != 0)
		fprintf(stderr, "Error: %d\n", errno);
		
	if(sem_unlink("adult_leave_mutex") != 0)
		fprintf(stderr, "Error: %d\n", errno);
		
	if(sem_unlink("child_enter_mutex") != 0)
		fprintf(stderr, "Error: %d\n", errno);
		
	if(sem_unlink("terminate_mutex") != 0)
		fprintf(stderr, "Error: %d\n", errno);
		
	if(key != -1)
	{
		shmctl(key, IPC_RMID, NULL); ///< uvolnenie zdielanej pamate
	}
}

/**
 * Catch
 * @brief Zachytenie signalu SIGUSR1 hlavnym procesom, uvolnenie semaforov, uzatvorenie suboru a odoslanie SIGTERM
 * vsetkym child procesom
 */

void catch()
{
	end(glob_key);
	killpg(getpgrp(), SIGTERM); ///< zasle SIGTERM kazdemu clenovy skupiny == child procesu
	exit(0);
}

/**
 * Catch_term
 * @brief Zachytenie signalu hlavnym SIGTERM vsetkymi procesmi a ich ukoncenie
 */

void catch_term()
{
	exit(0);
}

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
	signal(SIGUSR1, catch); ///< Hlavny proces prijma signaly SIGUSR1 od generator procesov v pripade nepodareneho forku
	signal(SIGTERM, catch_term); ///< Po prijati signalu SIGTERM sa proces explicitne ukonci

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
	AGT = AGT * 1000; ///< konvert z micro na mili
	
	int CGT = strtol(argv[4], NULL, 10);
	if(!(CGT >= 0 && CGT < 5001)) ///< overenie spravnej hodnoty CGT
	{
		fprintf(stderr, "Wrong parameter of argument (CGT >= 0 and CGT < 5001)\n");
		return 1;
	}
	CGT = CGT * 1000; ///< konvert z micro na mili
	
	int AWT = strtol(argv[5], NULL, 10);
	if(!(AWT >= 0 && AWT < 5001)) ///< overenie spravnej hodnoty AWT
	{
		fprintf(stderr, "Wrong parameter of argument (AWT >= 0 and AWT < 5001)\n");
		return 1;
	}
	AWT = AWT * 1000; ///< konvert z micro na mili
	
	int CWT = strtol(argv[6], NULL, 10);
	if(!(CWT >= 0 && CWT < 5001)) ///< overenie spravnej hodnoty CWT
	{
		fprintf(stderr, "Wrong parameter of argument (CWT >= 0 and CWT < 5001)\n");
		return 1;
	}
	CWT = CWT * 1000; ///< konvert z micro na mili
	
	srand(time(NULL)); ///< pre generovanie nahodnych cisel z intervalov pre AGT, CGT, AWT, CWT
	
	if((shared_memory_mutex = sem_open("shared_memory_mutex", O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED)
	{
		fprintf(stderr, "Errno: %d\n", errno);
		end(-1);
		return 2;
	}
		
	if((adult_leave_mutex = sem_open("adult_leave_mutex", O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED)
	{
		fprintf(stderr, "Errno: %d\n", errno);
		end(-1);
		return 2;
	}
		
	if((child_enter_mutex = sem_open("child_enter_mutex", O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED)
	{
		fprintf(stderr, "Errno: %d\n", errno);
		end(-1);
		return 2;
	}
		
	if((terminate_mutex = sem_open("terminate_mutex", O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED)
	{
		fprintf(stderr, "Errno: %d\n", errno);
		end(-1);
		return 2;
	}
	
	int key = shmget(IPC_PRIVATE, 512, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR); ///< specificky kluc pre pristup do zdielanej pamate
	glob_key = key;
	
	if(key == -1)
	{
		fprintf(stderr, "Error: %d\n", errno);
		end(-1);
		return 2;
	}
	
	int* memory = (int*)shmat(key, NULL, 0);
	
	memory[7] = 0;
	memory[8] = A + C; ///< nastavi memory[8] na pocet signalov ktore bude ocakavat
	
	///
	/// V buducnosti radsej pouzit strukturu, nie indexovane pole = lepsia prehladnost kodu :-) 
	///
	/// Struktura memory:
	/// [0] : CA
	/// [1] : CC
	/// [2] : file*
	/// [3] : count
	/// [4] : QA
	/// [5] : QC 
	/// [6] : To-end-Adults
	/// [7] : Ended processes
	/// [8] : Number of processes
	///
	
	memory[2] = open("proj2.out", O_CREAT | O_WRONLY | O_TRUNC, 0666); ///< otvori subor na zapis, systemove volanie ///< | O_TRUNC
	memory[3] = 0; ///< counter vypisov
	memory[4] = 0;
	memory[5] = 0;
	memory[6] = A; ///< pocet procesov Adult ktore sa este neukoncili
	
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
		} else if(pid == -1)///< v pripade ze sa fork() nepodari
		{
			fprintf(stderr, "Nepodarilo sa vytvorit generator proces.\n");
			kill(getpid(), SIGUSR1);
		}
	}
	
	while(wait(NULL) > 0)
	{
		
	}
	
	close(memory[2]);
	shmdt(memory); ///< zahod odkaz na zdielanu pamat
	
	end(key); ///< zavola ukoncujucu funkciu
	
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
		} else if(pid == -1)
		{
			fprintf(stderr, "Nepodarilo sa vytvorit dostatocny pocet Adult procesov.\n");
			kill(getppid(), SIGUSR1);
			exit(2); ///< Ukoncenie pre nepokracovanie generovania dalsich procesov
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
		} else if(pid == -1)
		{
			fprintf(stderr, "Nepodarilo sa vytvorit dostatocny pocet Child procesov.\n");
			kill(getppid(), SIGUSR1);
			exit(2); ///< Ukoncenie pre nepokracovanie generovania dalsich procesov
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
	
	sem_wait(shared_memory_mutex);
	
		memory[3]++;
		dprintf(memory[2], "%d\t: A %d\t: started\n", memory[3], A);
		
	sem_post(shared_memory_mutex);
	
	sem_wait(shared_memory_mutex);
	
		memory[0]++; ///<Atomicky zapis do CA - zdielana pamat
			
		memory[3]++;
		dprintf(memory[2], "%d\t: A %d\t: enter\n", memory[3], A);
			
		if(memory[5] > 0) ///< uvolnenie child_enter_mutex fronty pri vstupe adult procesu
		{
			if(memory[5] > 3)
			{
				for(int i = 0; i < 3; i++)
				{
					sem_post(child_enter_mutex);
					memory[5]--;
				}
				
			} else
			{
				for(int i = memory[5]; i > 0; i--)
				{
					sem_post(child_enter_mutex);
					memory[5]--;
				}
			}
		}
			
	sem_post(shared_memory_mutex);
		
	usleep(AWT);
	
	sem_wait(shared_memory_mutex);
	
		memory[3]++;
		dprintf(memory[2], "%d\t: A %d\t: trying to leave\n", memory[3], A);//*** Overenie dostupnosti (Atomicke) + wait na zaklade pomeru CA a CC
	
		if((memory[1] > (3 * (memory[0] - 1))))///< CC > 3 * (CA - 1)
		{	
			memory[3]++;
			memory[4]++;
			dprintf(memory[2], "%d\t: A %d\t: waiting : %d : %d\n", memory[3], A, memory[0], memory[1]);
			
			while((memory[1] > (3 * (memory[0] - 1)))) ///< opatovne overovanie podmienky
			{
				sem_post(shared_memory_mutex);
				
				sem_wait(adult_leave_mutex);
				
				sem_wait(shared_memory_mutex);
			}
			
			memory[4]--;
			
			memory[0]--;
						
			memory[3]++;
			dprintf(memory[2], "%d\t: A %d\t: leave\n", memory[3], A);
			memory[6]--;
			memory[7]++;
			
		} else
		{
			memory[0]--;
		
			memory[3]++;
			dprintf(memory[2], "%d\t: A %d\t: leave\n", memory[3], A);
			memory[6]--;
			memory[7]++;
		}
		
		if(memory[7] == memory[8])
		{
			for(int i = 0; i < memory[8]; i++)
			{
				sem_post(terminate_mutex);
			}
		}
	
	sem_post(shared_memory_mutex);
	
	sem_wait(terminate_mutex); ///< proces caka v rade na uvolnenie
	
	sem_wait(shared_memory_mutex);
	
		memory[3]++;
		dprintf(memory[2], "%d\t: A %d\t: finished\n", memory[3], A); //***Pockanie na ukoncenie hlavneho procesu - respektive, na counter number
		
	sem_post(shared_memory_mutex);
	
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
	
	sem_wait(shared_memory_mutex);
	
		memory[3]++;
		dprintf(memory[2], "%d\t: C %d\t: started\n", memory[3], C);
		
	sem_post(shared_memory_mutex);
	
	sem_wait(shared_memory_mutex);
	
		if(memory[6] == 0) ///< explicitny pristup v pripade, ze sa vsetky adult procesy uz ukoncili
		{
		
			memory[1]++; ///<Atomicky zapis do CC - zdielana pamat
		
			memory[3]++;
			dprintf(memory[2], "%d\t: C %d\t: enter\n", memory[3], C);
			
		} else if(((memory[1] + 1) > (3 * (memory[0]))))///< (CC + 1) > 3 * CA
		{
			
			memory[3]++;
			dprintf(memory[2], "%d\t: C %d\t: waiting : %d : %d\n", memory[3], C, memory[0], memory[1]);
			
			while(((memory[1] + 1) > (3 * (memory[0]))) && (memory[6] != 0)) ///< opakovane overenie podmienky, kedze adult ignoruje Child procesy cakajuce vo fronte, moze nastat situacia ze sa adult procesy vycerpaju, vtedy je child procesom vstup povoleny
			{
				memory[5]++;
				sem_post(shared_memory_mutex);
				
				sem_wait(child_enter_mutex);
				
				sem_wait(shared_memory_mutex);
			}
				//memory[5]--; ///< odstranene - zabezpecuje vstupujuci adult proces
				
				memory[1]++; ///<Atomicky zapis do CC - zdielana pamat
		
				memory[3]++;
				dprintf(memory[2], "%d\t: C %d\t: enter\n", memory[3], C);
			
		} else
		{
		
			memory[1]++; ///<Atomicky zapis do CC - zdielana pamat
			
			memory[3]++;
			dprintf(memory[2], "%d\t: C %d\t: enter\n", memory[3], C);
			
		}
		
	sem_post(shared_memory_mutex);
	
	usleep(CWT);
	
	sem_wait(shared_memory_mutex);
	
		memory[3]++;
		dprintf(memory[2], "%d\t: C %d\t: trying to leave\n", memory[3], C);
		
		memory[1]--;
		
		memory[3]++;
		dprintf(memory[2], "%d\t: C %d\t: leave\n", memory[3], C);
		memory[7]++;
		
		if(memory[5] > 0) ///< uvolni miesto ak je iny child proces vo fronte na vstup
		{
			sem_post(child_enter_mutex);
			memory[5]--;
		}
		
		if(memory[4] > 0)
		{
			if((memory[1] == 0) || ((memory[1] - 1) <= (3 * (memory[0] - 1))))///< (CC - 1) <= 3 * (CA - 1)
			{
				sem_post(adult_leave_mutex);
			}
		}
		
		if(memory[7] == memory[8])
		{
			for(int i = 0; i < memory[8]; i++)
			{
				sem_post(terminate_mutex);
			}
		}
		
	sem_post(shared_memory_mutex);
	
	sem_wait(terminate_mutex); ///< proces caka v rade na uvolnenie
	
	sem_wait(shared_memory_mutex);
	
		memory[3]++;
		dprintf(memory[2], "%d\t: C %d\t: finished\n", memory[3], C);
		
	sem_post(shared_memory_mutex);
}
