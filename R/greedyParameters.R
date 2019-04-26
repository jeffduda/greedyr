#' @title greedyParameters
#' @description default parameters list for greedyRegistration
#' @param dim image dimension (2 or 3)
#' @return antsrTransform
greedyParameters <- function( dim=NA ) {

  # List should have
  # dimension = int
  # image_pairs = list of 2 element vectors
  # transforms = list of initial transforms
  # metric = factor
  # timeStepMode = factor
  # mode = factor
  # dof = int
  # smoothing_epsilons_pre
  # smoothing_epsilons_post
  # metric_radius = int
  # fixed_mask = antsImage
  # moving_mask = antsImage
  params = .Call("defaultGreedyParameters", package="greedyr")

  if ( !is.na(dim) ) params$dim = dim


  return(params)
}
