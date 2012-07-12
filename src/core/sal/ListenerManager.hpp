#ifndef __LISTENER_MANAGER_HPP__
  #define __LISTENER_MANAGER_HPP__

#include <cassert>
#include <list>
#include <vector>
#include <algorithm>

#include "Listener.hpp"
#include "BufferCache.hpp"

namespace fail {

class ExperimentFlow;

/**
 * Buffer-list for a specific experiment; acts as a simple storage container
 * for listeners to watch for:
 */
typedef std::list<BaseListener*>    bufferlist_t;
/**
 * List of listeners that match the current simulator listener; these listeners will
 * be triggered next (the list is used temporarily).
 */
typedef std::vector<BaseListener*>  firelist_t;
/**
 * List of listeners that have been deleted during a toggled experiment flow while
 * triggering the listeners in the fire-list. This list is used to skip already
 * deleted listeners (therefore it is used temporarily either).
 */
typedef std::vector<BaseListener*>  deletelist_t;

/**
 * Cache classes for the most commonly used types of listeners, utilising static typing.
 * Apart from that, they work like bufferlist_t.
 */
typedef BufferCache<BPListener*> bp_cache_t;
typedef bp_cache_t::iterator bp_iter_t;
typedef BufferCache<IOPortListener*> io_cache_t;
typedef io_cache_t::iterator io_iter_t;
/**
 * \class ListenerManager
 *
 * \brief This class manages the listeners of the Fail* implementation.
 *
 * If a listener is triggered, the internal data structure will be updated (i.e.,
 * the listener will be removed from the so called buffer-list and added to the
 * fire-list). Additionally, if an experiment-flow deletes an "active" listener
 * which is currently stored in the fire-list, the listener (to be removed) will
 * be added to a -so called- delete-list. This ensures to prevent triggering
 * "active" listeners which have already been deleted by a previous experiment
 * flow. (See makeActive() and fireActiveListener() for implementation specific
 * details.) ListenerManager is part of the SimulatorController and "outsources"
 * it's listener management.
 */
class ListenerManager {
private:
	// TODO: List separation of "critical types"? Hashing/sorted lists? (-> performance!)
	bufferlist_t m_BufferList; //!< the storage for listeners added by exp.
	firelist_t m_FireList; //!< the active listeners (used temporarily)
	deletelist_t m_DeleteList; //!< the deleted listeners (used temporarily)
	BaseListener* m_pFired; //!< the recently fired Listener-object
	bp_cache_t m_Bp_cache; //!< the storage cache for breakpoint listeners
	io_cache_t m_Io_cache; //!< the storage cache for port i/o listeners
	friend bp_iter_t bp_cache_t::makeActive(ListenerManager &ev_list, bp_iter_t idx);
	friend io_iter_t io_cache_t::makeActive(ListenerManager &ev_list, io_iter_t idx);
public:
	/**
	 * The iterator of this class used to loop through the list of added
	 * listeners. To retrieve an iterator to the first element, call \c begin().
	 * \c end() returns the iterator, pointing after the last element.
	 * (This behaviour equals the STL iterator in C++.)
	 */
	typedef bufferlist_t::iterator iterator;

	ListenerManager() : m_pFired(NULL) { }
	~ListenerManager();
	/**
	 * Adds the specified listener object for the given ExperimentFlow to the
	 * list of listeners to be watched for.
	 * @param li pointer to the listener object to be added (cannot be \c NULL)
	 * @param flow the listener context (a pointer to the experiment object
	 *        which is interested in such listeners (cannot be \c NULL)
	 * @return the id of the added listener object, that is ev->getId()
	 */
	void add(BaseListener* li, ExperimentFlow* flow);
	/**
	 * Removes the listener based upon the specified \a ev pointer (requires
	 * to loop through the whole buffer-list).
	 * @param li the pointer of the listener to be removed; if ev is set to
	 *        \c NULL, all listeners (for \a all experiments) will be
	 *        removed
	 */
	void remove(BaseListener* li);
	/**
	 * Behaves like remove(BaseListener*) and additionally updates the provided
	 * iterator.
	 * @param it the iterator pointing to the Listener object to be removed
	 * @return the updated iterator which will point to the next element
	 */
	iterator remove(iterator it);
private:
	/**
	 * Internal implementation of remove(iterator it) that allows
	 * to skip the delete-list.
	 * @param it the iterator pointing to the Listener object to be removed
	 * @param skip_deletelist \c true to skip the deletion of the Listener object
	 *        or \false to behave like \c remove(iterator)
	 * @return the updated iterator which will point to the next element
	 */
	iterator m_remove(iterator it, bool skip_deletelist);
public:
	/**
	 * Returns an iterator to the beginning of the internal data structure.
	 * Don't forget to update the returned iterator when calling one of the
	 * modifying methods like makeActive() or remove(). Therefore you need
	 * to call the iterator-based variants of makeActive() and remove().
	 * \code
	 * [X|1|2| ... |n]
	 *  ^
	 * \endcode
	 * @return iterator to the beginning
	 */
	iterator begin() { return (m_BufferList.begin()); }
	/**
	 * Returns an iterator to the end of the interal data structure.
	 * Don't forget to update the returned iterator when calling one of the
	 * modifying methods like makeActive() or remove(). Therefore you need
	 * to call the iterator-based variants of makeActive() and remove().
	 * \code
	 * [1|2| ... |n]X
	 *              ^
	 * \endcode
	 * @return iterator to the end
	 */
	iterator end() { return (m_BufferList.end()); }
	/**
	 * Removes all listeners for the specified experiment.
	 * @param flow pointer to experiment context (0 = all experiments)
	 */
	void remove(ExperimentFlow* flow);
	/**
	 * Retrieves the number of experiments which currently have active
	 * listeners. This number is trivially equal to the (current) total
	 * number of ExperimentFlow-objects.
	 * @return number of experiments having active listeners
	 */
	size_t getContextCount() const;
	/**
	 * Retrieves the total number of buffered listeners. This doesn't include
	 * the listeners in the fire- or delete-list.
	 * @return the total listener count (for all flows)
	 */
	size_t getListenerCount() const { return m_BufferList.size(); }
	/**
	 * Retrieves the recently triggered listener object. To map this object to it's
	 * context (i.e., the related \c ExerimentFlow), use \c getLastFiredDest().
	 * @return a pointer to the recent listener or \c NULL if nothing has been
	 *         triggered so far
	 */
	BaseListener* getLastFired() { return (m_pFired); }
	/**
	 * Retrieves the ExperimentFlow-object for the given BaseListener (it's
	 * \a context).
	 * @param li the listener object to be looked up
	 * @return a pointer to the context of \a pEv or \c NULL if the
	 *         corresponding context could not be found
	 */
	ExperimentFlow* getExperimentOf(BaseListener* li);
	/**
	 * Moves the listeners from the (internal) buffer-list to the fire-list.
	 * To actually fire the listeners, call triggerActiveListeners().
	 * Returns an updated iterator which points to the next element.
	 * @param ev the listener to trigger
	 * @return returns the updated iteration, pointing to the next element
	 *         after makeActive returns, "it" is invalid, so the returned
	 *         iterator should be used to continue the iteration
	 *
	 * TODO: Improve naming (instead of "makeActive")?
	 */
	iterator makeActive(iterator it);
	/**
	 * Triggers the active listeners. Each listener is triggered if it has not
	 * recently been removed (id est: is not found in the delete-list). See
	 * makeActive() for more details. The recently triggered listener can be
	 * retrieved by calling \a getLastFired(). After all listeners have been
	 * triggered, the (internal) fire- and delete-list will be cleared.
	 */
	void triggerActiveListeners();
	/**
	 * Retrieves the BPListener buffer cache.
	 * @returns the buffer cache
	 */
	inline bp_cache_t &getBPBuffer() { return m_Bp_cache; }
	/**
	 * Retrieves the IOPortListener buffer cache.
	 * @returns the buffer cache
	 */
	inline io_cache_t &getIOBuffer() { return m_Io_cache; }
private:
	/**
	 * Add an listener to its appropriate cache.
	 * @param li the listener to add
	 */
	void addToCaches(BaseListener* li);
	/**
	 * Remove an listener from its cache.
	 * @param li the listener to remove
	 */
	void removeFromCaches(BaseListener* li);
	/**
	 * Clear the listener caches.
	 */
	void clearCaches();
};

} // end-of-namespace: fail

#endif // __LISTENER_MANAGER_HPP__
