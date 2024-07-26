#include "debug.hpp"

#include <Windows.h>
#include <mutex>

namespace dbg {
    // std::ostream myCerr(std::cerr.rdbuf());

	void dbg_print(const string& message) {
#ifdef _DEBUG_
		string msg = "[debug] ";
		msg += message;

        HANDLE hConsole = GetStdHandle(STD_ERROR_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
        WORD saved_attributes;

        GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
        saved_attributes = consoleInfo.wAttributes;

        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);

        cerr << msg << endl;

        SetConsoleTextAttribute(hConsole, saved_attributes);
#endif
	}
}