#' @title greedyRegistration
#' @description find transform between two image spaces
#' @param fixed target image (or vector/list of images)
#' @param moving moving image space (or vector/list of images)
#' @param metric metric
#' @param mode mode
#' @param parameters list of parameters obtained from greedyParameters()
#' @return antsrTransform
greedyRegistration <- function( fixed, moving, metric="SSD", mode="GREEDY", parameters=NA ) {

  fixedImg = fixed
  movingImg = moving

  ofile = paste0( tempfile(), ".nii.gz" )

  if ( !fixed@isVector) {
    fixedImg = mergeChannels(list(fixed))
  }

  if ( !moving@isVector) {
    movingImg = mergeChannels(list(moving))
  }

  results = .Call("greedyregistration", fixedImg, movingImg, metric, mode, ofile, PACKAGE="greedyr")

  if ( class(results)=="character" ) {
    results = antsrTransformFromDisplacementField( antsImageRead(results) )
  }

  return(results)
}
