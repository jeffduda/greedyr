#' antsrext_test1
#'
#' Test function that calls ANTsR routines
#'
#' @return int
#' @author Duda JT
#' @import ANTsR Rcpp
#' @examples
#' img = ANTsR::antsImageRead( ANTsR::getANTsRData( "r16" ) )
#' img2 = ANTsR::antsImageRead( ANTsR::getANTsRData( "r30" ) )
#' flag = greedyregistration(img, img2)
#' @export antsrext_test1()


greedyregistration <- function( fixed, moving, metric="SSD" ) {

  fixedImg = fixed
  movingImg = moving

  if ( !fixed@isVector) {
    fixedImg = mergeChannels(list(fixed))
  }

  if ( !moving@isVector) {
    movingImg = mergeChannels(list(moving))
  }

  results = .Call("greedyregistration", fixedImg, movingImg, metric, PACKAGE="greedyr")
  return(results)
}
