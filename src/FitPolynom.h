///////////////////////////////
// DO NOT USE THIS VERSION !!!
// USE THE ONE FROM base/numeric 
///////////////////////////////

#ifndef __FITPOLYNOM_H 
#define __FITPOLYNOM_H

#include <math.h>
#include <vector>
#include <gsl/gsl_multifit.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_min.h>
#include <iterator>
#include <stdexcept>
#include <iostream>
#include <gsl/gsl_poly.h>


namespace dsp
{
  //template for calculating a polynomial fit
  //
  //Parameters
  //degree  ==> number of returned parameters (order of the polynom + 1)
  //all dereferenced values must be float compatible 
  //the result will be a vector of coefficients with the highest order at the end
  //TODO 
  //add parameter for number of points to prevent counting them
  template<typename TIter1,typename TIter2,typename TResult>
    bool fitPolynom(int degree, TIter1 start_iter_x,
        TIter1 end_iter_x,
        TIter2 start_iter_y,
        TIter2 end_iter_y,
        std::vector<TResult> &result,
        double &chisq)
    {
      gsl_multifit_linear_workspace *ws;
      gsl_matrix *cov, *X;
      gsl_vector *y, *c;
      result.clear();

      int i, j;

      //TODO get rid of this !!!
      //we have to count the number of points first;
      TIter1 iter = start_iter_x;
      int number_of_points = 0;
      while(iter != end_iter_x)
      {
        ++iter;
        ++number_of_points;
      }

      //allocate memory
      //TODO find a solution so we do not have to allocate memory all the time
      //Maybe use a object polyfit 
      X = gsl_matrix_alloc(number_of_points, degree);
      y = gsl_vector_alloc(number_of_points);
      c = gsl_vector_alloc(degree);
      cov = gsl_matrix_alloc(degree, degree);

      for(i=0; i <number_of_points; i++) 
      {
        gsl_matrix_set(X, i, 0, 1.0);
        for(j=0; j < degree; j++) {
          gsl_matrix_set(X, i, j, pow((float)*(start_iter_x), j));
        }
        gsl_vector_set(y, i, (float)*(start_iter_y));

        ++start_iter_x;
        ++start_iter_y;
      }

      ws = gsl_multifit_linear_alloc(number_of_points, degree);
      gsl_multifit_linear(X, y, c, cov, &chisq, ws);

      /* store result ... */
      for(i=0; i < degree; i++)
        result.push_back(gsl_vector_get(c, i));

      gsl_multifit_linear_free(ws);
      gsl_matrix_free(X);
      gsl_matrix_free(cov);
      gsl_vector_free(y);
      gsl_vector_free(c);
      return true;
    }

  template<typename TIter,typename TResult>
    bool fitPolynom(int degree, TIter start_iter,
        TIter end_iter,
        std::vector<TResult> &result,double &chisq)
    {
      //generate x vector
      std::vector<float> x;

      TIter iter = start_iter;
      for(int i=0;iter != end_iter;++i,++iter)
        x.push_back(i);
      return fitPolynom<typename std::vector<float>::iterator,TIter,TResult>(degree,x.begin(),x.end(),start_iter,end_iter,result,chisq);
    }
    
  //calculates the coefficients of a polynomial which is derivation of the given polynomial
  //the given coefficients must sorted in ascending order (highest order last)
  template<typename TIter1,typename TIter2>
    void derivePolynom(TIter1 start_coefficient,TIter2 end_coefficient,TIter2 start_result)
    {
        TIter2 result = start_result;
        ++start_coefficient;
        for(int i = 1;start_coefficient!= end_coefficient;++start_coefficient,++i,++result)
            *result = *start_coefficient*i;
        if(result == start_result)
            *result = 0;
    }
    void derivePolynom(std::vector<double> &coefficients,std::vector<double> &result)
    {
        result.resize(std::max(1,(int)coefficients.size()-1));
        derivePolynom(coefficients.begin(),coefficients.end(),result.begin());
    }

    //calculates the roots of the polynomial given by the coefficients
    //coefficients must be in ascending order
    //the roots are stored as result[2n] = real part, result[2n+1] imaginer part
    void calcPolyRoots(const std::vector<double> &coefficients,std::vector<double> &roots)
    {
        roots.resize((coefficients.size()-1)*2);
        gsl_poly_complex_workspace * w = gsl_poly_complex_workspace_alloc (coefficients.size());
        gsl_poly_complex_solve (&coefficients[0], coefficients.size(), w, &roots[0]);
        gsl_poly_complex_workspace_free (w);
    }

    //calculates the value of a polynomial on a given position
    //coefficients order is from lowest to highest
    double calcPolyVal(const std::vector<double> &coefficients,double position)
    {
        if(coefficients.empty())
            throw std::runtime_error("calcPolyVal: No coefficients are given!");

        double result = 0;
        double temp = 1;
        std::vector<double>::const_iterator iter = coefficients.begin();
        for(;iter != coefficients.end();++iter,temp = temp*position)
            result += temp*(*iter);

        return result;
    }

};
#endif
