#pragma once

#define FEATURE_DELIMITER     ","
#define MAX_FEATURE_DISTANCE  10 //  If the feature distance is > 10 degreee of separation between two
                                 //  adjacent features, then its likely that the latter feature belong 
                                 //  to a different cluster. 
                                 //  The core idea is there exist a space (no feature detected) 
                                 //  through which the robot can navigate( if needed ).
                                 //  Btw, 10 degree is choosed based on the data set and it can be
                                 //  overridden through command line (not implemented).

#define MIN_DISTANCE_FROM_CLUSTER_CENTER    3  // Based on the data set, if the lowest feature exist within 
                                               // 3 degree of separation from the cluster center it *probably* 
                                               // means the width of the object is narrow or its an outlier.
                                               // And so robot can go around it OR can reposition to get
                                               // a better image at it OR ignoer it (outlier).
                                               // Ideally in this case we need to take into account of "r" 
                                               // to rule out narrow case. 
                                              
#define INVALID_INDEX       -1

#define SUCCESS                         0
// Error values start outside the general range
#define ERR_FILE_FAILED_TO_OPEN         5000 
#define ERR_NO_FEATURES                 5001