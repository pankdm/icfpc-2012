#pragma once

#ifdef unix
inline void clear_screen() {
	system("clear");
}
#else
inline void clear_screen() {
	system("CLS");
}
#endif