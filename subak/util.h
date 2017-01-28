#pragma once
#include <string>
namespace Util {
	class Utility {
	public :
		static int iso8601_to_second(std::string s)
		{
			// ISO 8601 Standard Format
			// PnYnMnDTnHnMnS
			// 'P' and 'T' are mandatory. Rest of those
			// are not. 

			// First locate 'P' and 'T' 

			// Since the first letter is always 'P',
			// period start from the next character
			std::string period = s.substr(1, s.find('T'));
			std::string time = s.substr(s.find('T') + 1);

			char placeholder_period[] = { 'Y', 'M', 'D' };
			int period_multiplier[] = { 60 * 60 * 24 * 265, 60 * 60 * 24 * 30, 60 * 60 * 24 };

			char placeholder_time[] = { 'H', 'M', 'S' };
			int time_multiplier[] = { 3600, 60, 1 };

			int total = 0;
			for (int i = 0; i < 3; i++) {
				int loc = period.find(placeholder_period[i]);

				if (loc != std::string::npos) {
					auto t = period.substr(0, loc);
					period.erase(0, loc + 1);
					total += atoi(t.c_str()) * period_multiplier[i];
				}
			}

			for (int i = 0; i < 3; i++) {
				int loc = time.find(placeholder_time[i]);

				if (loc != std::string::npos) {
					auto t = time.substr(0, loc);
					time.erase(0, loc + 1);
					total += atoi(t.c_str()) * time_multiplier[i];
				}
			}

			return total;
		}
	};
	
}