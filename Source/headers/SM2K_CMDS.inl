#pragma once
#include "SM2K.h"
#include "CmdTools.inl"

namespace SM2K 
{

// COMMAND ALIASES
#define	_ECHO	echo, ECHO, eh
#define	_OPEN	open, OPEN, op
#define	_CLOSE	close, CLOSE, cl
#define	_QUIT	quit, QUIT, q

#define	_START	start, START, st
#define	_STOP	stop, STOP, stp
#define	_SKIP	skip, skip_forward, skipforward, SKIP, sk
#define	_BACK	back, skip_back, skipback, BACK, bk
#define	_PAUSE	pause, PAUSE, pau
#define	_RESUME	resume, RESUME, res

// TODO: Add New Command Name And Alias Tokens Here.^^^
//-------------------------------------------

#ifdef SM2K_USING_CMD_INIT

#define	__ECHO		ADD_CMD_TOKENS(_ECHO)
#define	__OPEN		ADD_CMD_TOKENS(_OPEN)
#define	__CLOSE		ADD_CMD_TOKENS(_CLOSE)
#define	__QUIT		ADD_CMD_TOKENS(_QUIT)

#define	__START		ADD_CMD_TOKENS(_START)
#define	__STOP		ADD_CMD_TOKENS(_STOP)
#define	__SKIP		ADD_CMD_TOKENS(_SKIP)
#define	__BACK		ADD_CMD_TOKENS(_BACK)
#define	__PAUSE		ADD_CMD_TOKENS(_PAUSE)
#define	__RESUME	ADD_CMD_TOKENS(_RESUME)

// TODO: Add New Command Tokens Here.^^^

#define INIT_CMDS	__ECHO __OPEN __CLOSE __START __STOP __PAUSE __RESUME __SKIP __BACK __QUIT	//<- Add the token define here.

#endif // SM2K_USING_CMD_INIT

};




