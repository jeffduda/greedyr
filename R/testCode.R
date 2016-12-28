#' antsrext_test1
#'
#' Test function that calls ANTsR routines
#'
#' @return antsImage
#' @author Duda JT
#' @import ANTsR Rcpp
#' @examples
#' antsrext_test1()
#' @export antsrext_test1()
greedyr_test1 <- function(  ) {

  img = ANTsR::makeImage( c(64, 64) )

  return( img )
}

greedyr_test2 <- function( ) {

  result = .Call( "makeImage2" )
  return(result)

}

greedyr_test3 <- function( ) {
  direction = matrix(data=0, nrow=2, ncol=2)
  diag(direction) = rep(1, 2)

  results = .Call( "makeImage4" )
  return(results)
}

greedyr <- function( ) {
  results = .Call("greedyr")
  return(results)
}
