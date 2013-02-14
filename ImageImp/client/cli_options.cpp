#include "cli_options.h"

#include <iostream>

#define VERSION 0.4

using namespace std;

namespace ImageImp {

	void CommandLineOptions::processOptions() {
		while ((option_id = poptGetNextOpt(option_context)) >= 0) {
				switch (option_id) {
					case 'h':
						poptPrintUsage(option_context, stderr, 0);
						break;
					case 'v':
						std::cout << "Version: " << VERSION << std::endl;
						break;
				}
			}
	}
}
