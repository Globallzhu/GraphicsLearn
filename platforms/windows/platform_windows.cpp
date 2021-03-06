#include "platform.hpp"
#include <windows.h>
 
std::string ResourcePath(std::string fileName) {
	char executablePath[1024];
   DWORD charsCopied = GetModuleFileName(NULL, executablePath, 1024);
   if (charsCopied > 0 && charsCopied < 1024) {
		return std::string(executablePath) + "\\..\\" + fileName;
	}
   else {
	   return "";
	}
}

