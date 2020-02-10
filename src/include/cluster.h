#pragma once

#include <string>
#include <vector>

namespace DSA
{
    class Cluster
    {
        public:
            Cluster();
            Cluster(const Cluster & rhs) = default;
            long Process(const std::string & inFilename, const std::string & outFileName);
        
        private:
            typedef std::vector<float>               FeatureCollectionT;
            typedef std::vector<float>::iterator     FeatureCollectionIterT;

            // Tracking it separately to process input and output in various formats.
            // Ex - input can be fed through network as opposed to file, likewise
            //      output may need a graph in addition to file.
            FeatureCollectionT          m_allFeatures;    
            FeatureCollectionT          m_allCenters;    

            long ProcessFile(const std::string & inFileName);
            long ProcessFeatures();
            bool CanBeClustered(float current, float previous);
            void FindCenter(float sum, size_t start, size_t end);
            void DumpFeatures(std::string message, size_t start, size_t end);
            std::string GetFormattedOutput(FeatureCollectionT & featureCollection, size_t start, size_t end);
            void WriteOutput(const std::string & fileName);
    };
}