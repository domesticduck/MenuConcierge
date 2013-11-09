
#pragma once

#include <spc/spcbase.h>
#include <sstream>
#include <vector>
#include <string>

namespace spc {
	
	class MenuConcierge : public SPCBase {
	public:
		// メインプロシージャ
		void onInitialize();
		
	};

}

spc::MenuConcierge spcApp;
