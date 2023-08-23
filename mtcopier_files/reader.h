/**
 * Startup code provided by Paul Miller for Operating Systems Principles
*/

#include <pthread.h>
#include <fstream>
#include <iostream>
#include <string>

#ifndef READER
#define READER

using std::cout;
using std::endl;
using std::ifstream;
using std::string;
using std::ios;

class reader
{
	public:

		/**
		 * Initialise the shared data for the class.
		 */
		static bool init(const string& FILE_NAME);

		/**
		 * Does the setup for and launches the thread.
		 */
		pthread_t run();

		/**
		 * Thread function which does the actual reading (producing).
		 **/
		static void *runner(void *);

        /**
         * Public mutexes to facilitate thread synchronisation.
        */
        static pthread_mutex_t queue_lock;
		static pthread_cond_t item_removed_signal;
		static pthread_cond_t item_added_signal;

	private:

        /**
         * The stream doing the reading.
        */
		static ifstream in;

};

#endif // READER