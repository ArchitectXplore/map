#include "sparta/statistics/Histogram.hpp"
#include "sparta/statistics/StatisticSet.hpp"

namespace sparta
{
    void StatisticSet::onAddingChild_(TreeNode* child) {
        if(isFinalized()){
            throw SpartaException("Cannot add a child Counter once a StatisticSet is finalized. "
                                "Error with: ")
                << getLocation();
        }

        StatisticDef* stat = dynamic_cast<StatisticDef*>(child);
        if(nullptr != stat){
            // Add stat to stats_ list for tracking.
            stats_.push_back(stat);
            return;
        }

        CounterBase* ctr = dynamic_cast<CounterBase*>(child);
        if(nullptr != ctr){
            // Add Counter to ctrs_ list for tracking.
           ctrs_.push_back(ctr);
           return;
        }

        HistogramTreeNode* hist = dynamic_cast<HistogramTreeNode*>(child);
        if(nullptr != hist){
            // Add Histogram to hists_ list for tracking.
            hists_.push_back(hist);
            return;
        }

        throw SpartaException("Cannot add TreeNode child ")
            << child->getName() << " to StatisticSet " << getLocation()
            << " because the child is not a CounterBase or StatisticDef";
    }

    HistogramTreeNode* StatisticSet::getHistogram(const std::string& name) {
        return getChildAs<HistogramTreeNode>(name);
    }

    const HistogramTreeNode* StatisticSet::getHistogram(const std::string& name) const {
        return getChildAs<HistogramTreeNode>(name);
    }
} // namespace sparta