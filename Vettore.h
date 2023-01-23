// Vettore.h: the Vettore template class.
//
//////////////////////////////////////////////////////////////////////

#ifndef VETTORE_H
#define VETTORE_H

#include <assert.h>
#include <iostream>
using namespace std;

template <class T>
class Vettore
{
	friend bool operator== ( const Vettore<T>& V, const Vettore<T>& VC )
    {   
        if ( &V == &VC ) return true;
        if ( V.imin() != VC.imin() || V.imax() != VC.imax() ) 
            return false; 
        for ( int i = V.imin(); i <= V.imax(); i++ ) 
            if ( !(V[i] == VC[i]) ) return false;
        return true;
    };
    
	friend Vettore<T> operator+ ( const Vettore<T>& V1, const Vettore<T>& V2 )
    {   
        assert ( V1.imin() == V2.imin() && V1.imax() == V2.imax());
        Vettore<T> V(V1.imin(),V1.imax());
        for ( int i = V.imin(); i <= V.imax(); i++ )
            V[i] = V1[i] + V2[i];
        return V;
    }


protected:
	T* elementi; // array degli elementi
	int vimin, vimax; // indice minimo e massimo

public:
//	FUNZIONI COSTANTI
	// true se vettore nullo
	bool nullo() const { return elementi==0; }
	// indice minimo del vettore
	int imin() const { return vimin;}
	// indice massimo del vettore
	int imax() const { return vimax;}
	// restituisce il numero di elementi
	int n() const { return imax()-imin()+1;}
	// valore dell'elemento i-mo del vettore
	const T& operator[] ( int i ) const
    {   
        assert ( !nullo() );
        assert ( i >= imin() && i <= imax() );
        return elementi[i-vimin];
    }

//	FUNZIONI NON COSTANTI
	Vettore() { elementi = 0; vimin=1; vimax=0;}
	void init( int iMn, int iMx)
    {   
        assert (iMx>=iMn);
        elementi = new T[iMx-iMn+1]; 
        vimin = iMn; 
        vimax = iMx;
    }
	Vettore( int iMn, int iMx )
	{	init(iMn,iMx);
	}
	// costruttore per copia profonda
	Vettore( const Vettore& VC )
	{	assert ( VC.n() > 0 );
		vimin = VC.imin();
		vimax = VC.imax();
		elementi = new T[vimax-vimin+1];
		for ( int i = imin(); i <= imax(); i++ )
			(*this)[i] = VC[i];
	}
	T& operator[] (int i)
    {   
        assert ( !nullo() );
        assert ( i >= imin() && i <= imax() );
        return elementi[i-vimin];
    }
	Vettore& operator= ( const Vettore& VC )
    {   
        if ( this == &VC ) return *this;
        assert ( imin() == VC.imin() && imax() == VC.imax() ); 
        for ( int i = imin(); i <= imax(); i++ ) 
            (*this)[i] = VC[i];
        return *this;
    }
	// distruttore profondo
	virtual ~Vettore() { delete [] elementi;}
};
#endif
