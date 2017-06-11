/**
 * @file proj3.h
 * @brief Dokumentace zdrojovy kodu IZP
 * @author Tomas Lapsansky (xlapsa00)
 */

/**
 * @defgroup Deklaracie
 * @brief deklaracie funkcii
 * @{
 */

struct obj_t {
	int id;
	float x;
	float y;
};

struct cluster_t {
	int size;
	int capacity;
	struct obj_t *obj;
};

/**
 * @}
 */

extern const int CLUSTER_CHUNK;

/**
 * @defgroup Praca so zhlukmi
 * @brief Praca s jednotlivymi zhlukmi
 * @{
 */

/**
 * Inicializacia zhluku
 * @brief Alokacia kapacity zhluku
 * @param *c - smernik na zhluk
 * @param cap - pozadovana capacita zhluku
 * @pre *c != NULL, cap > 0
 * @post Alokovana kapacita
 */

void init_cluster(struct cluster_t *c, int cap);

/**
 * Dealokovanie prvkov zhluku
 * @brief Dealokovanie jednotlivych prvkov zhluku
 * @param *c - smernik na zhluk
 * @pre *c != NULL
 * @post Dealokovana kapacita, capacity = 0, size = 0
 */

void clear_cluster(struct cluster_t *c);

/**
 * Realokacia zhluku
 * @brief Zmena alokovanej pamate zhluku
 * @param *c - smernik na zhluk
 * @param new_cap - nova pozadovana capacita zhluku
 * @pre *c != NULL, new_cap > 0
 * @post Realokovana kapacita
 * @return Vrati smernik na zhluk
 */

struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap);

/**
 * Prida objekt obj na koniec zhluku c
 * @param *c - smernik na zhluk
 * @param obj - objekt obj_t
 * @pre *c != NULL
 * @post obj bol pridany do zhluku, v pripade potreby bola jeho alokovana pamat navysena
 */

void append_cluster(struct cluster_t *c, struct obj_t obj);

/**
 * Zlucenie zhlukov
 * @brief Spoji zhluk c1 so zhlukom c2, c2 ostane zachovany
 * @param *c1 - smernik na zhluk1
 * @param *c2 - smernik na zhluk2
 * @pre *c1 != NULL, *c2 != NULL
 * @post = Spojenie zhlokov
 */

void merge_clusters(struct cluster_t *c1, struct cluster_t *c2);

/**
 * Triedenie zhluku
 * @brief Zoradi objekty zhluku
 * @param *c - smernik na zhluk
 * @pre *c != NULL
 * @post Zhluk ma roztriedene objekty podla ich indexu
 */

void sort_cluster(struct cluster_t *c);

/**
 * @}
 */

/**
 * @defgroup Praca s polom zhlukov
 * @{
 */

/**
 * Odstranenie zhluku
 * @brief Odstrani zhluk na indexe idx z pola, posunie indexy ostatnych prvkov
 * @param *carr - smernik na pole zhlukov
 * @param narr - pocet zhlukov v carr
 * @param idx - index zhluku ktory ma byt odstraneny
 * @pre *carr != NULL, narr > 1, idx > 0
 */

int remove_cluster(struct cluster_t *carr, int narr, int idx);

/**
 * Vzdialenost dvoch obj_t
 * @brief Vypocita vzdialenost dvoch objektov na zaklade ich suradnic
 * @param *o1 - smernik na prvy obj_t
 * @param *o2 - smernik na druhy obj_t
 * @pre *o1 != NULL, *o2 != NULL
 * @return Float vzdialenost dvoch objektov
 */

float obj_distance(struct obj_t *o1, struct obj_t *o2);

/**
 * Vzdialenost dvoch zhlukov
 * @brief Vypocita vzdialenost dvoch zhlukov na zaklade ich najvziadelenejsich objektov
 * @param *c1 - smernik na prvy zhluk
 * @param *c2 - smernik na druhy zhluk
 * @pre *c1 != NULL, *c2 != NULL
 * @return Float vzdialenost dvoch zhlukov
 */

float cluster_distance(struct cluster_t *c1, struct cluster_t *c2);

/**
 * Hlada najblizsie zhluky
 * @brief Hlada v poli zhlukov zhluky s najmansou vzdialenostou
 * @param *carr - smernik na pole zhlukov
 * @param narr - pocet zhlukov v carr
 * @param *c1 - smernik kde bude zapisany prvy zhluk z dvoch najblizsich
 * @param *c2 - smernik kde bude zapisany druhy zhluk z dvoch najblizsich
 * @pre *carr != NULL, narr > 1
 * @post Do smernikov *c1 a *c2 zapise adresy najblizsich zhlukov
 */

void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2);

/**
 * Vypisanie zhlukov
 * @brief Vypise objekty zhluku
 * @param *c - smernik na zhluk
 * @pre *c != NULL
 */

void print_cluster(struct cluster_t *c);

/**
 * Nacita zhluky zo suboru
 * @brief Nacita kazdy objekt zapisany v subore filename do samostatneho zhluku
 * @param *filename - String s nazvom suboru
 * @param **arr - smernik za zaciatok pola zhlukov
 * @pre *filename != NULL, **arr != NULL
 * @return Vracia pocet nacitanych objektov
 */

int load_clusters(char *filename, struct cluster_t **arr);

/**
 * Vypis zhlukov
 * @brief vypise vsetky zhluky s ich objektami
 * @param *carr - smernik na pole zhlukov
 * @param narr - pocet zhlukov v carr
 * @pre *carr !=NULL, narr > 0
 * @post vypise vsetky zhluky z pola carr
 */

void print_clusters(struct cluster_t *carr, int narr);

/**
 * @}
 */
