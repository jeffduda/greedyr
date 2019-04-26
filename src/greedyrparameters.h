#include <exception>
#include <vector>
#include <string>
#include <RcppANTsR.h>
#include "GreedyParameters.h"

// Convert from struct to Rcpp::List
Rcpp::List greedyParametersToR( GreedyParameters param );

// Convert from Rcpp::List to struct
GreedyParameters greedyParametersFromR( SEXP rparam );

// Get Rcpp::List filled with default values
RcppExport SEXP defaultGreedyParameters( void );
