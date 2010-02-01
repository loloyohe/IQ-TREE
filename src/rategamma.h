/***************************************************************************
 *   Copyright (C) 2009 by BUI Quang Minh   *
 *   minh.bui@univie.ac.at   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef RATEGAMMA_H
#define RATEGAMMA_H

#include <rateheterogeneity.h>

class PhyloTree;
/**
Discrete gamma distributed site-rate model from Yang 1994

	@author BUI Quang Minh <minh.bui@univie.ac.at>
*/
class RateGamma: virtual public RateHeterogeneity
{
public:
	/**
		constructor
		@param ncat number of rate categories
		@param tree associated phylogenetic tree
	*/
    RateGamma(int ncat, PhyloTree *tree);

	/**
		destructor
	*/
    virtual ~RateGamma();

	/**
		@return the number of rate categories
	*/
	virtual int getNRate() { return ncategory; }


	/**
		@param category category ID from 0 to #category-1
		@return the rate of the specified category
	*/
	virtual double getRate(int category) { return rates[category]; }

	/** discrete Gamma according to Yang 1994 (JME 39:306-314).
		It takes 'ncategory' and 'gamma_shape' variables as input. On output, it write to 'rates' variable.
	*/
	void computeRates();


	/**
		optimize parameters. Default is to optimize gamma shape 
		@return the best likelihood 
	*/
	virtual double optimizeParameters();

	/**
		override function from Optimization class, used by the minimizeOneDimen() to optimize
		gamma shape parameter
	*/
	virtual double computeFunction(double shape);


	/**
		return the number of dimensions
	*/
	virtual int getNDim() { return 1; }

	/**
		write information
		@param out output stream
	*/
	virtual void writeInfo(ostream &out);

	/**
		write parameters, used with modeltest
		@param out output stream
	*/
	virtual void writeParameters(ostream &out);

protected:

	/**
		number of rate categories
	*/
	int ncategory;

	/**
		rates, containing ncategory elements
	*/
	double *rates;


	/**
		the gamma shape parameter 'alpha'
	*/
	double gamma_shape;



	//Normally, beta is assigned equal to alpha
	double cmpPerPointGamma (const double prob, const double shape);

	/***********************************************************
	NUMERICAL SUBROUTINES
	THE FOLLOWING CODE COMES FROM tools.c in Yang's PAML package 
	***********************************************************/
	/** returns ln(gamma(alpha)) for alpha>0, accurate to 10 decimal places.
	   Stirling's formula is used for the central polynomial part of the procedure.
	   Pike MC & Hill ID (1966) Algorithm 291: Logarithm of the gamma function.
	   Communications of the Association for Computing Machinery, 9:684
	 
	*/
	double cmpLnGamma (double alpha);

	/** returns the incomplete gamma ratio I(x,alpha) where x is the upper
	   limit of the integration and alpha is the shape parameter.
	   returns (-1) if in error
	   (1) series expansion     if (alpha>x || x<=1)
	   (2) continued fraction   otherwise
	   RATNEST FORTRAN by
	   Bhattacharjee GP (1970) The incomplete gamma integral.  Applied Statistics,
	   19: 285-287 (AS32)
	*/
	double cmpIncompleteGamma (double x, double alpha, double ln_gamma_alpha);

	/** functions concerning the CDF and percentage points of the gamma and
	   Chi2 distribution
	   returns z so that Prob{x<z}=prob where x ~ N(0,1) and (1e-12)<prob<1-(1e-12)
	   returns (-9999) if in error
	   Odeh RE & Evans JO (1974) The percentage points of the normal distribution.
	   Applied Statistics 22: 96-97 (AS70)
	 
	   Newer methods:
	     Wichura MJ (1988) Algorithm AS 241: the percentage points of the
	       normal distribution.  37: 477-484.
	     Beasley JD & Springer SG  (1977).  Algorithm AS 111: the percentage 
	       points of the normal distribution.  26: 118-121.
	*/
	double cmpPointNormal (double prob);


	/** returns z so that Prob{x<z}=prob where x is Chi2 distributed with df=v
	   returns -1 if in error.   0.000002<prob<0.999998
	   RATNEST FORTRAN by
	       Best DJ & Roberts DE (1975) The percentage points of the 
	       Chi2 distribution.  Applied Statistics 24: 385-388.  (AS91)
	   Converted into C by Ziheng Yang, Oct. 1993.
	*/
	double cmpPointChi2 (double prob, double v);

	/* THE END OF THE CODES COMMING FROM tools.c in Yang's PAML package */

};




#endif
