/*
 * Grafo.h
 *
 *  Created on: 9-dic-2010
 */

#ifndef GRAFO_H_
#define GRAFO_H_

#include "Matrice.h"

class Grafo
{	protected:
	int vn, vm; // numero di nodi e numero di archi
	Matrice<bool> archi; // matrice di adiacenza

	void init( int nNo ) //	inizializzazione
	{	archi.init (1,nNo,1,nNo);
		vn=nNo;
		svuota();
		vm=0;
	}

public:
//	FUNZIONI NON COSTANTI
	Grafo( int nNo )
	{
		assert(nNo>=1);
		init(nNo);
	}
//	inserisce o elimina (i,j) a secondo se b � true o false
	void operator()( int i, int j, bool b )
	{	assert ( i>=1 && i<=n() && j>=1 && j<=n() );
		bool esistearco = archi(i,j);
		if ( (!esistearco && b) || (esistearco && !b) )
		{	archi(i,j) = b;
			if ( b )
				vm++;
			else
				vm--;
		}
	}

//	elimina tutti gli archi
	void svuota ()
	{
		for ( int i = 1; i <= archi.n(); i++ )
			for ( int j = 1; j <= archi.n(); j++ )
				archi(i,j) = false;
		vm = 0;
	}

//	assegnamento
	Grafo& operator= ( const Grafo& GC )
	{	if ( this == &GC ) return *this;
		vn = GC.n();
		init( GC.n() );
		for ( int i = 1; i <= archi.n(); i++ )
			for ( int j = 1; j <= archi.n(); j++ )
				if (GC(i,j))
					archi(i,j) = true;
				else
					archi(i,j) = false;
		return *this;
	}
//	restituisce true se esiste un percorso tra i e j
	bool percorso(int inizio, int fine, int precedente = 0) {
		for (int i = 1; i <= n(); i++) {
			if (operator()(inizio,i)) {
				if (i == fine) 
					return true;
				else if (i != precedente) 
					return percorso(i,fine,inizio);
			}
		}
		return false;
	}
//	rimuove tutti gli archi successivi
	void rimuovi_archi(int nodo) {
        for (int i = 1; i <= n(); i++) {
            if (operator()(nodo, i)) {
				operator()(nodo,i,false);
				operator()(i,nodo,false);
            }
        }
    }
//	FUNZIONI COSTANTI
	int n() const {return vn;}; // restituisce il numero di nodi
	int m() const {return vm;} // restituisce il numero di archi
//	restituisce vero o falso se l'arco (i,j) � nel grafo o meno
	bool operator() ( int i, int j ) const
	{	assert ( i >=1 && i<=n() && j>=1 && j<=n() );
		return archi(i,j);
	}

};


#endif /* GRAFO_H_ */
