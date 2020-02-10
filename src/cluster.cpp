#include <fstream>
#include <sstream>
#include "./include/constants.h"
#include "./include/cluster.h"
#include "./include/utils.h"

using namespace std;
using namespace Logger;
namespace DSA
{

    Cluster::Cluster()
    {
    }

    void Cluster::WriteOutput(const string & fileName)
    {
        string result = GetFormattedOutput(m_allCenters, 0, ( m_allCenters.size() - 1 ) ) ; 
        ofstream outfile;
        outfile.open (fileName, std::ios::trunc);
        outfile << result << "\n";
        outfile.close();

        Log(" ======= Success - centers %s =======", result.c_str());
    }

    long Cluster::Process(const std::string & inFilename, const std::string & outFileName)
    {
        m_allCenters.clear();
        m_allFeatures.clear();

        Log("======= Processing %s ===========", inFilename.c_str());

        long result = ProcessFile( inFilename );
        if ( result != SUCCESS )
            return result;

        result = ProcessFeatures();

        if ( result == SUCCESS )
            WriteOutput( outFileName );
        
        return result;
    }

    // start and end are inclusive index
    string Cluster::GetFormattedOutput(FeatureCollectionT & featureCollection, size_t start, size_t end)
    {

        if (start < 0 || end >= featureCollection.size())
            return "";

        stringstream ss;
        ss.precision(1);
        for ( size_t i = start; i <= end; i++ )
        {
            float feature = featureCollection[ i ];
            if ( ! ss.str().empty() )
                ss <<  ",";
            ss << fixed << feature;
        }

        return ss.str();
    }

    void Cluster::DumpFeatures( std::string message, size_t start, size_t end )
    {
        message += GetFormattedOutput(m_allFeatures, start, end) ;
        Log( "%s", message.c_str() );
    }

    bool Cluster::CanBeClustered( float current, float previous )
    {
        float adjacentDistance = current - previous;
        
        Log("Info: Distance %f Cur %f prev %f", adjacentDistance, current, previous );

        if ( adjacentDistance > MAX_FEATURE_DISTANCE )
            return false;

        return true;
    }

    void Cluster::FindCenter( float sum, size_t start, size_t end )
    {
        // One element is considered as a outlier.
        if ( (start >= end) || ( (end - start) == 1) )
        {
            Log("Info: Skipping feature, reason - either outlier or invalid index. start %d end %d ", 
                start, end);
            return;
        }

        if ( end >= m_allFeatures.size() )
        {
            Log( "Error: Invalid start index %d end  %d sum %f", start, end, sum );
            return;
        }

        // Add 1 to be inclusive
        float totalNumberOfClusterFeatures = (float)( (end - start) + 1)  ; 

        float mean = sum / totalNumberOfClusterFeatures ;

        // Make sure the cluster size is of certain minimum size.
        // Since all the features are ascending order the earliest or starting
        // feature value is used to calculate if the object is of certain size limit
        // See comments for MIN_DISTANCE_FROM_CLUSTER_CENTER.
        float minimimumFeatureValue = m_allFeatures[ start ];

        float deltaFromMean = mean - minimimumFeatureValue; 
        if ( deltaFromMean <= MIN_DISTANCE_FROM_CLUSTER_CENTER )
        {
            DumpFeatures(" Info: Skipping potentially small width (or less visible) cluster ", 
                           start, end );
            Log(" Info: Mean %f Min feature index %d value %f delta %f", mean, start, 
                  minimimumFeatureValue, deltaFromMean );
            return;
        } 

        Log(" >>>>>>>> Found cluster center at %f using sum %f,"
            " range: feature[ %d ] = %f ==> feature[ %d ] = %f deltaFromMean %f ",
               mean, sum, start, minimimumFeatureValue, end, m_allFeatures[end], deltaFromMean );
        m_allCenters.push_back( mean );
    }
 
    long Cluster::ProcessFeatures()
    {
        size_t totalNumFeatures = m_allFeatures.size();
        if ( !totalNumFeatures )
        {
            Log("No features found.");
            return ERR_NO_FEATURES;
        }

        float previousFeature = m_allFeatures[0];
        float sumSofar = m_allFeatures[0];
        int minIndex = 0;  
        size_t curIndex = 1;
        // Size of 1 is considered as outlier.   
        for (; curIndex < totalNumFeatures; curIndex++ )
        {
            float currentFeature = m_allFeatures[ curIndex ];
 
            if ( currentFeature < previousFeature )
            {
                Log("Skipping unsorted feature. feature[ %d ] %f feature[ %d ] %f ", curIndex - 1, 
                        m_allFeatures[ curIndex - 1 ], curIndex, currentFeature ); 
                continue; 
            }

            if ( CanBeClustered( currentFeature, previousFeature) )
            {
                // The current feature belongs to the group of features
                // we have identified so far, hence keep summing up.
                sumSofar += currentFeature;
                previousFeature = currentFeature;
                continue;
            }

            // Now we can not cluster the current feature with the features we have seen so far.
            // So treat as the start of the new cluster and process the feature set we have collected
            // so far. 
      
            FindCenter(sumSofar, minIndex, curIndex - 1);

            // Reset the minimum index to be current  
            minIndex = curIndex;
            sumSofar = currentFeature;
            previousFeature = currentFeature;
        }

        // Process the last set. The function ignores a set of 1 as its treated as outlier.
        FindCenter(sumSofar, minIndex, curIndex - 1);

        if ( m_allCenters.size() == 0)
            Log("Info: No clusters found");

        return SUCCESS;
    }

    long Cluster::ProcessFile(const std::string & inFileName)
    {
        ifstream  inFile(inFileName);
        if ( ! inFile.is_open() )
        {
            Log("Failed to open file");
            return ERR_FILE_FAILED_TO_OPEN;
        }

        string  allFeatures;
        inFile >> allFeatures;

        size_t start = 0;
        size_t end = allFeatures.find_first_of( FEATURE_DELIMITER );

        // Minor performance improvement can be done by doing in-place
        // calculation while doing a file read. In the interest of clarity
        // and keeping the logic of processing data points separate the file
        // is read as a separate step.
        while ( start != string::npos )
        {
            size_t length = (end == string::npos) ? end : (end - start);

            string  feature = allFeatures.substr( start, length );
         
            
            start = (end == string::npos) ? end : (end + 1) ;
            
            end = allFeatures.find_first_of( FEATURE_DELIMITER, start );
            if ( feature.empty() )
                continue;

            stringstream temp(feature);
            float currentFeature = 0;
            temp >> currentFeature;

            m_allFeatures.push_back( currentFeature );
        }

        return SUCCESS;
    }
}