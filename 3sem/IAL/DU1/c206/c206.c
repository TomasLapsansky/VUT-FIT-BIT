	
/* c206.c **********************************************************}
{* Téma: Dvousměrně vázaný lineární seznam
**
**                   Návrh a referenční implementace: Bohuslav Křena, říjen 2001
**                            Přepracované do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Kamil Jeřábek, říjen 2017
**
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou
** typu tDLList (DL znamená Double-Linked a slouží pro odlišení
** jmen konstant, typů a funkcí od jmen u jednosměrně vázaného lineárního
** seznamu). Definici konstant a typů naleznete v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu
** s výše uvedenou datovou částí abstrakce tvoří abstraktní datový typ
** obousměrně vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu před prvním použitím,
**      DLDisposeList ... zrušení všech prvků seznamu,
**      DLInsertFirst ... vložení prvku na začátek seznamu,
**      DLInsertLast .... vložení prvku na konec seznamu, 
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek, 
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku, 
**      DLDeleteFirst ... zruší první prvek seznamu,
**      DLDeleteLast .... zruší poslední prvek seznamu, 
**      DLPostDelete .... ruší prvek za aktivním prvkem,
**      DLPreDelete ..... ruší prvek před aktivním prvkem, 
**      DLPostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vloží nový prvek před aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na další prvek seznamu,
**      DLPred .......... posune aktivitu na předchozí prvek seznamu, 
**      DLActive ........ zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce
** explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam 
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální proměnná -- příznak ošetření chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/
	
	L->First = NULL;
	L->Act = NULL;
	L->Last = NULL;
 
}

void DLDisposeList (tDLList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Rušené prvky seznamu budou korektně
** uvolněny voláním operace free. 
**/
	
	tDLElemPtr item = L->First;
	tDLElemPtr next;
	
	while(item != NULL) {
		next = item->rptr;
		
		item->lptr = NULL;
		item->data = 0;
		item->rptr = NULL;
		free(item);
		
		item = next;
	}
	
	L->First = NULL;
	L->Act = NULL;
	L->Last = NULL;
	
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vloží nový prvek na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	
	tDLElemPtr item = (tDLElemPtr)malloc(sizeof(struct tDLElem));
	if(item == NULL) {
		DLError();
		return;
	}
	
	item->data = val;
	item->lptr = NULL;
	item->rptr = L->First;
	
	if(L->First != NULL) {
		L->First->lptr = item;
	}
	
	L->First = item;
	
	if(L->Last == NULL) {
		L->Last = item;
	}
	
}

void DLInsertLast(tDLList *L, int val) {
/*
** Vloží nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/ 	
	
	tDLElemPtr item = (tDLElemPtr)malloc(sizeof(struct tDLElem));
	if(item == NULL) {
		DLError();
		return;
	}
	
	item->data = val;
	item->lptr = L->Last;
	item->rptr = NULL;
	
	if(L->Last != NULL) {
		L->Last->rptr = item;
	}
	
	L->Last = item;
	
	if(L->First == NULL) {
		L->First = item;
	}
	
}

void DLFirst (tDLList *L) {
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
	
	L->Act = L->First;
}

void DLLast (tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
	
	L->Act = L->Last;
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/

	if(L->First == NULL) {
		DLError();
		return;
	}
	
	*val = L->First->data;
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
	
	if(L->First == NULL) {
		DLError();
		return;
	}
	
	*val = L->Last->data;
}

void DLDeleteFirst (tDLList *L) {
/*
** Zruší první prvek seznamu L. Pokud byl první prvek aktivní, aktivita 
** se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/
	
	if(L->First == NULL) {	//prazdny zoznam
		return;
	}
	
	if(L->First == L->Act) {	//vymaze Active prvok, ak bol First
		L->Act = NULL;
	}
	
	tDLElemPtr item = L->First;	//item = First item
	
	if(item->rptr == NULL) {	//ak L obsahuje len 1 item
		L->First = NULL;
		L->Last = NULL;
		free(item);
		
		return;
	}
	
	L->First = item->rptr;
	L->First->lptr = NULL;
	
	//vynulovanie dat a Free
	item->data = 0;
	item->rptr = NULL;
	free(item);
	
}	

void DLDeleteLast (tDLList *L) {
/*
** Zruší poslední prvek seznamu L. Pokud byl poslední prvek aktivní,
** aktivita seznamu se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/
	
	if(L->First == NULL) {	//prazdny zoznam
		return;
	}
	
	if(L->Last == L->Act) {	//vymaze Active prvok, ak bol Last
		L->Act = NULL;
	}
	
	tDLElemPtr item = L->Last;	//item = Last item
	
	if(item->lptr == NULL) {	//ak L obsahuje len 1 item
		L->First = NULL;
		L->Last = NULL;
		free(item);
		
		return;
	}
	
	L->Last = item->lptr;
	L->Last->rptr = NULL;
	
	//vynulovanie dat a Free
	item->data = 0;
	item->lptr = NULL;
	free(item);
	
}

void DLPostDelete (tDLList *L) {
/*
** Zruší prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se neděje.
**/
	
	if(L->Act == NULL)			//bez aktivneho prvku
		return;
	
	if(L->Act->rptr == NULL)	//neexistuje dalsi prvok
		return;
	
	if(L->Act->rptr == L->Last)	//ak je nasledujuci prvok Last, prestavi hodnotu L->Last
		L->Last = L->Act;
	
	tDLElemPtr item = L->Act->rptr;
	
	L->Act->rptr = item->rptr;	//nastavi rptr Act prvku
	item->data = 0;				//vynuluje data
	
	if(item->rptr != NULL)		//nastavi lprt nasledujuceho prvku, ak existuje
		item->rptr->lptr = L->Act;
	
	free(item);					//free item
}

void DLPreDelete (tDLList *L) {
/*
** Zruší prvek před aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se neděje.
**/
	
	if(L->Act == NULL)			//bez aktivneho prvku
		return;
	
	if(L->Act->lptr == NULL)	//neexistuje predosli prvok
		return;
	
	if(L->Act->lptr == L->First)	//ak je nasledujuci prvok First, prestavi hodnotu L->First
		L->First = L->Act;
	
	tDLElemPtr item = L->Act->lptr;
	
	L->Act->lptr = item->lptr;	//nastavi rptr Act prvku
	item->data = 0;				//vynuluje data
	
	if(item->lptr != NULL)		//nastavi lprt predosleho prvku, ak existuje
		item->lptr->rptr = L->Act;
	
	free(item);					//free item
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vloží prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	
	if(L->Act == NULL)		//bez aktivneho prvku
		return;
	
	tDLElemPtr item = (tDLElemPtr)malloc(sizeof(struct tDLElem));
	if(item == NULL) {
		DLError();
		return;
	}
	
	if(L->Act == L->Last)	//nastavi Last na item, ak je item poslednym prvkom
		L->Last = item;
	
	//nastavenie dat pre item
	item->lptr = L->Act;
	item->data = val;
	item->rptr = L->Act->rptr;
	L->Act->rptr = item;
	
	if(item->rptr != NULL)	//nastavi lptr nasledujuceho prvku, ak nie je NULL
		item->rptr->lptr = item;
}

void DLPreInsert (tDLList *L, int val) {
/*
** Vloží prvek před aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	
	if(L->Act == NULL)		//bez aktivneho prvku
		return;
	
	tDLElemPtr item = (tDLElemPtr)malloc(sizeof(struct tDLElem));
	if(item == NULL) {
		DLError();
		return;
	}
	
	if(L->Act == L->First)	//nastavi First na item, ak je item prvym prvkom
		L->First = item;
	
	//nastavenie dat pre item
	item->lptr = L->Act->lptr;
	item->data = val;
	item->rptr = L->Act;
	L->Act->lptr = item;
	
	if(item->lptr != NULL)	//nastavi rptr predosleho prvku, ak nie je NULL
		item->lptr->rptr = item;
}

void DLCopy (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/
		
	if(L->Act == NULL) {
		DLError();
		return;
	}
	
	*val = L->Act->data;
}

void DLActualize (tDLList *L, int val) {
/*
** Přepíše obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedělá nic.
**/
	
	if(L->Act == NULL) {
		return;
	}
	
	L->Act->data = val;
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
**/
	
	if(L->Act == NULL) {
		return;
	}
	
	L->Act = L->Act->rptr;
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na předchozí prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
**/
	
	if(L->Act == NULL) {
		return;
	}
	
	L->Act = L->Act->lptr;
}

int DLActive (tDLList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Funkci je vhodné implementovat jedním příkazem return.
**/
	
	return ((L->Act != NULL) ? 1 : 0);
}

/* Konec c206.c*/
