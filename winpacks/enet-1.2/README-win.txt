Enet 1.2 library compiled and packed for windows mingw,
Jonni 21.12.2008

Instructions (for code::blocks):

*	copy include- and lib-directories somewhere, probably where
	mingw is so you don't need to specify their locations

*	in your Project Build Options (select your executable on the
	left, not one of Debug/Release) Linker settings, add "enet"
	and "ws2_32" to Link libraries. "enet" might have to be the
	first in the list.

*	PROFIT!!!1!!!11!
