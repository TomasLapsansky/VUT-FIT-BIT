/* Header file with all the essential definitions for a given type of MCU */
#include "MK60D10.h"

#include <time.h>
#include <stdlib.h>

/* Macros for bit-level registers manipulation */
#define GPIO_PIN_MASK	0x1Fu
#define GPIO_PIN(x)		(((1)<<(x & GPIO_PIN_MASK)))

/* Constants specifying delay loop duration */
#define	DELAY_T 200000

#define BTN_SW2 0x400 // Port E, bit 10
#define BTN_SW3 0x1000 // Port E, bit 12
#define BTN_SW4 0x8000000 // Port E, bit 27
#define BTN_SW5 0x4000000 // Port E, bit 26
#define BTN_SW6 0x800 // Port E, bit 11

int pressed_up = 0, pressed_down = 0, pressed_left = 0, pressed_right = 0;
unsigned int compare = 0x200;

#define ROWS 7
#define COLS 5

/* type of dropped item */
int type = 0;

/* rotation of dropped item */
int position = 0;

/* global matrix > ^ */
int matrix[COLS][ROWS] = {0};

void StartGame(void);

/* A delay function */
void delay(long long bound) {

  long long i;
  for(i=0;i<bound;i++);
}

/* Initialize the MCU - basic clock settings, turning the watchdog off */
void MCUInit(void)  {
    MCG_C4 |= ( MCG_C4_DMX32_MASK | MCG_C4_DRST_DRS(0x01) );
    SIM_CLKDIV1 |= SIM_CLKDIV1_OUTDIV1(0x00);
    WDOG_STCTRLH &= ~WDOG_STCTRLH_WDOGEN_MASK;
}

void PortsInit(void)
{
    /* Turn on all port clocks */
    SIM->SCGC5 = SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK;

    int ptaMask = 0;
    ptaMask &= 0x0;
    int ptdMask = 0;
    ptdMask &= 0x0;

    PORTE->PCR[10] = PORT_PCR_MUX(0x01); // SW2
    PORTE->PCR[12] = PORT_PCR_MUX(0x01); // SW3
    PORTE->PCR[27] = PORT_PCR_MUX(0x01); // SW4
    PORTE->PCR[26] = PORT_PCR_MUX(0x01); // SW5
    PORTE->PCR[11] = PORT_PCR_MUX(0x01); // SW6

    PORTE->PCR[28] = PORT_PCR_MUX(0x01);
    PORTA->PCR[6] = PORT_PCR_MUX(0x01);
    ptaMask |= (0x1 << 6);
    PORTA->PCR[7] = PORT_PCR_MUX(0x01);
    ptaMask |= (0x1 << 7);
    PORTA->PCR[8] = PORT_PCR_MUX(0x01);
    ptaMask |= (0x1 << 8);
    PORTA->PCR[9] = PORT_PCR_MUX(0x01);
    ptaMask |= (0x1 << 9);
    PORTA->PCR[10] = PORT_PCR_MUX(0x01);
    ptaMask |= (0x1 << 10);
    PORTA->PCR[11] = PORT_PCR_MUX(0x01);
    ptaMask |= (0x1 << 11);
    PORTA->PCR[24] = PORT_PCR_MUX(0x01);
    ptaMask |= (0x1 << 24);
    PORTA->PCR[25] = PORT_PCR_MUX(0x01);
    ptaMask |= (0x1 << 25);
    PORTA->PCR[26] = PORT_PCR_MUX(0x01);
    ptaMask |= (0x1 << 26);
    PORTA->PCR[27] = PORT_PCR_MUX(0x01);
    ptaMask |= (0x1 << 27);
    PORTA->PCR[28] = PORT_PCR_MUX(0x01);
    ptaMask |= (0x1 << 28);
    PORTA->PCR[29] = PORT_PCR_MUX(0x01);
    ptaMask |= (0x1 << 29);

    PORTD->PCR[12] = PORT_PCR_MUX(0x01);
    ptdMask |= (0x1 << 12);
    PORTD->PCR[13] = PORT_PCR_MUX(0x01);
    ptdMask |= (0x1 << 13);
    PORTD->PCR[14] = PORT_PCR_MUX(0x01);
    ptdMask |= (0x1 << 14);

    /* Change corresponding PTB port pins as outputs */
    PTA->PDDR = GPIO_PDDR_PDD(0x1F8007E0);	//00011111100000000000011111100000b
    PTD->PDDR = GPIO_PDDR_PDD(0x3400);		//0011010000000000b
    PTE->PDDR = GPIO_PDDR_PDD(0x1000000);

    PTA->PDOR |= GPIO_PDDR_PDD(ptaMask);
    PTD->PDOR |= GPIO_PDDR_PDD(ptdMask);
    PTE->PDOR |= GPIO_PDDR_PDD(0x1000000);
}

void LPTMR0Init(void)
{
    SIM_SCGC5 |= SIM_SCGC5_LPTIMER_MASK;

    LPTMR0_CSR = 0;

    LPTMR0_PSR = (LPTMR_PSR_PBYP_MASK
                 | LPTMR_PSR_PCS(1));

    LPTMR0_CMR = 0xFFFFFF;

    LPTMR0_CSR = (LPTMR_CSR_TCF_MASK | LPTMR_CSR_TIE_MASK);

    //NVIC_EnableIRQ(LPTMR0_IRQn);

    LPTMR0_CSR |= LPTMR_CSR_TEN_MASK;   // Turn ON LPTMR0 and start counting
}

void RowsOn(int i) {
	if (matrix[0][i-1] != 0)
		PTA->PDOR |= 0x1 << 28;

	if(matrix[1][i-1] != 0)
		PTD->PDOR |= 0x1 << 13;

	if(matrix[2][i-1] != 0)
		PTA->PDOR |= 0x1 << 8;

	if(matrix[3][i-1] != 0)
		PTA->PDOR |= 0x1 << 7;

	if(matrix[4][i-1] != 0)
		PTA->PDOR |= 0x1 << 26;
}

void RowsOff(void) {
	PTA->PDOR &= 0xEFFFFFFF;	// 4 row
	PTD->PDOR &= 0xFFFFDFFF;	// 5 row
	PTA->PDOR &= 0xFFFFFEFF;	// 6 row
	PTA->PDOR &= 0xFFFFFF7F;	// 7 row
	PTA->PDOR &= 0xFBFFFFFF;	// 8 row
}

void RenderDisplay(void) {

	/* check every rank if full */
	for(int j = ROWS-1; j >= 0; j--) {
		int bool = 1;
		for(int i = 0; i < COLS; i++) {
			if(matrix[i][j] != 1)
				bool = 0;
		}

		/* delete and shift */
		if(bool == 1) {
			if(j == ROWS-1) {

				for(int x = 0; x < COLS; x++) {
					matrix[x][j] = 0;
				}

			} else {

				for(int y = j; y < ROWS-1; y++) {
					for(int x = 0; x < COLS; x++) {
						if(matrix[x][y+1] != 2)
							matrix[x][y] = matrix[x][y+1];
					}
				}

			}
		}
	}

	int dl = 10;

	PTA->PDOR ^= 0x1 << 24;	// 1 col
	RowsOn(1);
	delay(dl);
	RowsOff();
	PTA->PDOR ^= 0x1 << 24;

	PTA->PDOR ^= 0x1 << 27;	// 2 col
	RowsOn(2);
	delay(dl);
	RowsOff();
	PTA->PDOR ^= 0x1 << 27;

	PTA->PDOR ^= 0x1 << 6;	// 3 col
	RowsOn(3);
	delay(dl);
	RowsOff();
	PTA->PDOR ^= 0x1 << 6;

	PTD->PDOR ^= 0x1 << 12;	// 4 col
	RowsOn(4);
	delay(dl);
	RowsOff();
	PTD->PDOR ^= 0x1 << 12;

	PTA->PDOR ^= 0x1 << 25;	// 5 col
	RowsOn(5);
	delay(dl);
	RowsOff();
	PTA->PDOR ^= 0x1 << 25;

	PTA->PDOR ^= 0x1 << 10;	// 6 col
	RowsOn(6);
	delay(dl);
	RowsOff();
	PTA->PDOR ^= 0x1 << 10;

	PTA->PDOR ^= 0x1 << 9;	// 7 col
	RowsOn(7);
	delay(dl);
	RowsOff();
	PTA->PDOR ^= 0x1 << 9;

}

void GameOver(void) {
	while(1) {

		// Start new game
		if (!pressed_up && !(GPIOE_PDIR & BTN_SW5))
		{
			pressed_up = 1;
			StartGame();
		}
		else if (GPIOE_PDIR & BTN_SW5) pressed_up = 0;

		// End game
		if (!pressed_down && !(GPIOE_PDIR & BTN_SW3))
		{
			pressed_down = 1;
			while(1);
		}
		else if (GPIOE_PDIR & BTN_SW3) pressed_down = 0;

	}
}

void Generator(void) {

	int r = rand();

	r = r % 4;

	position = 1;

	if(r == 0) {

		type = 1;
		if(matrix[2][6] != 0) {
			GameOver();
		}

		matrix[2][6] = 2;

	} else if(r == 1) {

		type = 2;
		if(matrix[2][6] != 0 || matrix[3][6] != 0) {
			GameOver();
		}

		matrix[2][6] = 2;
		matrix[3][6] = 2;

	} else if(r == 2) {

		type = 3;
		if(matrix[2][6] != 0 || matrix[3][6] != 0 || matrix[2][5] != 0) {
			GameOver();
		}

		matrix[2][6] = 2;
		matrix[3][6] = 2;
		matrix[2][5] = 2;

	} else if(r == 3) {

		type = 4;
		if(matrix[2][6] != 0 || matrix[3][6] != 0 || matrix[2][5] != 0 || matrix[3][5] != 0) {
			GameOver();
		}

		matrix[2][6] = 2;
		matrix[3][6] = 2;
		matrix[2][5] = 2;
		matrix[3][5] = 2;
	}

}

void ClearMatrix(void) {
	for(int i = 0; i < COLS; i++) {
		for(int j = 0; j < ROWS; j++) {
			matrix[i][j] = 0;
		}
	}
}

void MoveRight(void) {

	for(int i = 0; i < ROWS; i++) {
		if(matrix[4][i] == 2)
			return;
	}

	int newMatrix[COLS][ROWS];

	for(int i = 0; i < COLS; i++) {
		for(int j = 0; j < ROWS; j++) {
			newMatrix[i][j] = matrix[i][j];
		}
	}

	for(int i = COLS-1; i >= 0; i--) {
		for(int j = 0; j < ROWS; j++) {
			if(newMatrix[i][j] == 2) {
				if(newMatrix[i+1][j] != 0)
					return;
				else {
					newMatrix[i+1][j] = 2;
					newMatrix[i][j] = 0;
				}
			}
		}
	}

	for(int i = 0; i < COLS; i++) {
		for(int j = 0; j < ROWS; j++) {
			matrix[i][j] = newMatrix[i][j];
		}
	}
}

void MoveLeft(void) {

	for(int i = 0; i < ROWS; i++) {
		if(matrix[0][i] == 2)
			return;
	}

	int newMatrix[COLS][ROWS];

	for(int i = 0; i < COLS; i++) {
		for(int j = 0; j < ROWS; j++) {
			newMatrix[i][j] = matrix[i][j];
		}
	}

	for(int i = 0+1; i < COLS; i++) {
		for(int j = 0; j < ROWS; j++) {
			if(newMatrix[i][j] == 2) {
				if(newMatrix[i-1][j] != 0)
					return;
				else {
					newMatrix[i-1][j] = 2;
					newMatrix[i][j] = 0;
				}
			}
		}
	}

	for(int i = 0; i < COLS; i++) {
		for(int j = 0; j < ROWS; j++) {
			matrix[i][j] = newMatrix[i][j];
		}
	}

}

void Rotate(void) {

	if(type == 0 || type == 1 || type == 4)
		return;

	if(type == 2) {

		if(position == 1 || position == 3) {
			for(int i = COLS-1; i > 0; i--) {
				for(int j = ROWS-1-1; j > 0; j--) {

					if(matrix[i][j] == 2) {
						if(matrix[i-1][j+1] == 0) {
							matrix[i][j] = 0;
							matrix[i-1][j+1] = 2;
							position = 2;
							return;
						} else {
							return;
						}
					}

				}
			}
		} else if(position == 2 || position == 4) {
			for(int i = 0; i < COLS-1; i++) {
				for(int j = ROWS-1; j > 0; j--) {
					if(matrix[i][j] == 2) {
						if(matrix[i+1][j-1] == 0) {
							matrix[i][j] = 0;
							matrix[i+1][j-1] = 2;
							position = 1;
							return;
						} else {
							return;
						}
					}
				}
			}
		}

	} else if(type == 3) {
		if(position == 1) {

			for(int i = COLS-1; i >= 0; i--) {
				for(int j = ROWS-1; j > 0; j--) {
					if(matrix[i][j] == 2) {
						if(matrix[i][j-1] == 0) {
							matrix[i][j] = 0;
							matrix[i][j-1] = 2;
							position = 2;
							return;
						} else {
							return;
						}
					}
				}
			}

		} else if(position == 2) {

			for(int i = 0; i < COLS; i++) {
				for(int j = ROWS-1; j >= 0; j--) {
					if(matrix[i][j] == 2) {
						if(matrix[i+1][j] == 0) {
							matrix[i][j] = 0;
							matrix[i+1][j] = 2;
							position = 3;
							return;
						} else {
							return;
						}
					}
				}
			}

		} else if(position == 3) {

			for(int i = 0; i < COLS; i++) {
				for(int j = 0; j < ROWS-1; j++) {
					if(matrix[i][j] == 2) {
						if(matrix[i][j+1] == 0) {
							matrix[i][j] = 0;
							matrix[i][j+1] = 2;
							position = 4;
							return;
						} else {
							return;
						}
					}
				}
			}

		} else if(position == 4) {

			for(int i = COLS; i > 0; i--) {
				for(int j = 0; j < ROWS; j++) {
					if(matrix[i][j] == 2) {
						if(matrix[i-1][j] == 0) {
							matrix[i][j] = 0;
							matrix[i-1][j] = 2;
							position = 1;
							return;
						} else {
							return;
						}
					}
				}
			}

		}
	}

}

void TransformGameBrick(void) {
	for(int i = 0; i < COLS; i++) {
		for(int j = 0; j < ROWS; j++) {
			if(matrix[i][j] == 2) {
				matrix[i][j] = 1;
			}
		}
	}
}

void GameEvent(void) {

	/* check last rank */
	for(int i = 0; i < COLS; i++) {
		if(matrix[i][0] == 2) {
			TransformGameBrick();
			Generator();
			return;
		}
	}

	int newMatrix[COLS][ROWS];

	/* copy */
	for(int i = 0; i < COLS; i++) {
		for(int j = 0; j < ROWS; j++) {
			newMatrix[i][j] = matrix[i][j];
		}
	}

	/* game move */
	for(int i = 0; i < COLS; i++) {
		for(int j = 1; j < ROWS; j++) {
			if(newMatrix[i][j] == 2) {
				if(newMatrix[i][j-1] != 0) {
					TransformGameBrick();
					Generator();
					return;
				} else {
					newMatrix[i][j-1] = 2;
					newMatrix[i][j] = 0;
				}
			}
		}
	}

	/* copy back */
	for(int i = 0; i < COLS; i++) {
		for(int j = 0; j < ROWS; j++) {
			matrix[i][j] = newMatrix[i][j];
		}
	}

}

void StartGame(void) {

	ClearMatrix();

	RowsOff();

    Generator();

    int before_secs = 0;
    int secs = 0;
	while (1) {

		LPTMR0_CNR = 0;
		secs = LPTMR0_CNR / 1000;

		if(secs != before_secs) {
			before_secs = secs;
			GameEvent();
		}

		RenderDisplay();

		if (!pressed_right && !(GPIOE_PDIR & BTN_SW2))
		{
			pressed_right = 1;
			MoveRight();
		}
		else if (GPIOE_PDIR & BTN_SW2) pressed_right = 0;

		if (!pressed_left && !(GPIOE_PDIR & BTN_SW4))
		{
			pressed_left = 1;
			MoveLeft();
		}
		else if (GPIOE_PDIR & BTN_SW4) pressed_left = 0;

		if (!pressed_up && !(GPIOE_PDIR & BTN_SW5))
		{
			pressed_up = 1;
			Rotate();
		}
		else if (GPIOE_PDIR & BTN_SW5) pressed_up = 0;
	}
}

int main(void)
{
    MCUInit();
    PortsInit();
    LPTMR0Init();
    srand(time(NULL));

    StartGame();

    return 0;
}
