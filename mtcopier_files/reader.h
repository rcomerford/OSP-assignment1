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
		static void init(const string &name);

		/**
		 * Thread function which does the actual reading from the input file.
		 **/
		static void *runner(void *);

		/**
		 * Does the setup for and launches the thread.
		 */
		void run();

	private:

        /**
         * The stream doing the reading.
        */
		static std::ifstream in;
};

#endif // READER