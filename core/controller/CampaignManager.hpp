/**
 * \brief The manager for an entire campaign
 * 
 *	The CampaignManager allows a user-campaign access to all constant 
 *	simulator information and forwards single experiments to the JobServer. 
 *
 * \author Martin Hoffmann
 *
 */

#ifndef __CAMPAIGN_MANAGER_H__
#define __CAMPAIGN_MANAGER_H__

#include "SAL/SALInst.hpp"
#include "ExperimentData.hpp"
#include "jobserver/JobServer.hpp"
#include "controller/Campaign.hpp"

namespace fi
{
/**
 * \class CampaignManager
 * Class manageing an FI campaign.
 */
class CampaignManager 
{
   
  JobServer m_jobserver;
  Campaign* m_currentCampaign;
  
  public:
    CampaignManager(){};

    /**
     * Executes a user campaign
     */
    bool runCampaign(Campaign* c){ 
      m_currentCampaign = c;
      bool ret = c->run(); 
      m_jobserver.done(); 
      return ret;
    }
    
    /**
     * Returns a const reference for acquiring constant simlator specific information.
     * e.g., Registernames, to ease experiment data construction.
     * The campaign description is not allowed to change the simulator
     * state, as the actual simulation runs within another process (Minion) 
     * @return c constant reference to the current simulator backend.
     */
    sal::SimulatorController const& getSimulator() const { return sal::simulator; };

    /**
     * Add a experiment parameter set.
     * The user campaign has to allocate the Parameter object,
     * and deallocate it after result reception.
     * A Parameter set includes space for results.
     * @param exp A pointer to a ExperimentData set.
     */
    void addParam(ExperimentData* exp){ m_jobserver.addParam(exp); };
    
    /**
     * A user campaign can request a single result (blocking) from the queue.
     * 
     * @return Pointer to a parameter object with filled result data @see addParam. 
     */
    ExperimentData* getDone() { return m_jobserver.getDone(); };
    
    /**
     * Signal, that there will not come any further parameter sets.
     */
    void noMoreParameters() { m_jobserver.setNoMoreExperiments(); };
  
    /**
     * Wait actively, until all experiments expired.
     */
   // void waitForCompletion();
  
     /**
     * User campaign has finished..
     */
    void done() { m_jobserver.done(); };
   
   
    
};

extern CampaignManager campaignmanager;
} //end-of-namespace
#endif
