// Matrice.h: interface for the Matrice class.
//
//////////////////////////////////////////////////////////////////////

#ifndef MATRICE_H
#define MATRICE_H

#include "Vettore.h"

template <class T>
class Matrice: public Vettore< Vettore<T> >
{
protected:
	int vjmin, vjmax; // indici minimo e massimo di colonnna
	//	funzione di utilità per copiare una matrice
	void copia( const Matrice& MC )
	{	for ( int i = Vettore<VetT>::imin(); i <= Vettore<VetT>::imax(); i++ )
			for ( int j = jmin(); j <= jmax(); j++ )
				(*this)(i,j) = MC(i,j);
	}

public:
	typedef Vettore<T> VetT;
//	FUNZIONI COSTANTI
	//	true se matrice nulla
	bool nulla( ) const { return Vettore<VetT>::nullo(); }
	//	indice minimo di colonna
	int jmin( ) const {return vjmin;}
	//	indice massimo di colonna
	int jmax( ) const {return vjmax;}
	//	restituisce il numero di colonne
	int m( ) const {return jmax()-jmin()+1;}
	//	restituisce il valore elemento (i,j)
	virtual const T& operator() ( int i, int j ) const
	{	assert(! nulla() );
		return (*this)[i][j];
	}

//	FUNZIONI NON COSTANTI
	//	costruttore di matrice nulla
	Matrice( ) {vjmin = 1; vjmax = 0;}
	//	dimensionamento a partire da una matrice nulla
	void init( int iMn, int iMx, int jMn, int jMx )
	{
		assert ( nulla() );
		assert ( iMn <= iMx && jMn <= jMx );
		Vettore<VetT>::init(iMn,iMx); // crea il vettore di righe
		for ( int i = iMn; i <= iMx; i++ )
			(*this)[i].init(jMn,jMx); // crea ogni singola riga
		vjmin = jMn; vjmax = jMx;
	}

	//	costruttore di matrice non nulla
	Matrice( int iMn, int iMx, int jMn, int jMx )
	{	init(iMn,iMx,jMn,jMx);
	}
	//	costruttore per copia profonda
	Matrice( const Matrice& M )
	{	init(M.imin(),M.imax(),M.jmin(),M.jmax());
		copia(M);
	}
	//	restitusce l'elemento(i,j) come l-value per eventuali modifiche
	virtual T& operator() ( int i, int j )
	{	assert(! nulla() );
		return (*this)[i][j];
	}
	//	assegnamento profondo
	Matrice& operator= ( const Matrice& MC )
	{	if ( this == &MC ) return *this;
		assert ( VetT::imin() == MC.imin() && VetT::imax() == MC.imax() &&
			jmin() == MC.jmin() && jmax() == MC.jmax() );
		copia(MC);
 		return *this;
	}
};
#endif

