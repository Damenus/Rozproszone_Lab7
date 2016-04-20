
#include <stdio.h>
#include <pthread.h>

#define MAX_PRIME 500
#define MAX_THREAD 10

struct dane {
	int start;
	int koniec;
};
//globalna struktura do której piszemy
struct wynik_programu {
	int primes[MAX_PRIME];		// tablica z liczbami pierwszymi
	int curr_p;			// ile w tablicy jest wpisanych liczb pierwszych
	pthread_mutex_t m;		// semafor chroniący dostępu do tablicy
}wynik;

void *f( void *i ) {

	struct dane *dana = i;
	int start = dana->start;
	int koniec = dana->koniec;
	int j, k;

	for ( j = start; j < koniec; j++) {	
		for( k = 2; k <= j; k++) {
	   		if( j % k == 0 )
	      		break; //gdy znajdziemy dzielnik, to dana liczba nie jest pierwsza
		}
		if ( k == j ) {
	  		//printf( "Liczba pierwsza: %d \n", j);
			pthread_mutex_lock(&wynik.m);			
			wynik.primes[wynik.curr_p] = j;
			wynik.curr_p++;
			pthread_mutex_unlock(&wynik.m);
		}
	}

}

int main() {
	int liczba_watkow = MAX_THREAD;
	int liczba_ostatnia = MAX_PRIME;
	pthread_t w[MAX_THREAD];
	struct dane struktura[MAX_THREAD];

	wynik.curr_p = 0;
	pthread_mutex_init(&wynik.m, NULL);

	//inicjalizacja struktur ktra mowi odkad dokad liczyc
	for (int i = 0; i < liczba_watkow; i++) {
		struktura[i].start = i * liczba_ostatnia / liczba_watkow;
		struktura[i].koniec =  (i + 1) * liczba_ostatnia / liczba_watkow;
	}

	//uruchomienie watkow
	for (int i = 0; i < liczba_watkow; i++) {

		pthread_create( &w[i], NULL, f, &struktura[i] );

	}

	//czekanie na koniec watkow
	for (int i = 0; i < liczba_watkow; i++) {

		pthread_join (w[i], NULL);

	}

	for (int i = 0; i < wynik.curr_p; i++) {
		
		printf( "Liczba pierwsza %d: %d \n", i, wynik.primes[i]);

	}

	printf( "Liczba liczb pierwszych: %d \n", wynik.curr_p);
	pthread_mutex_destroy(&wynik.m);
	

return 0;
}
