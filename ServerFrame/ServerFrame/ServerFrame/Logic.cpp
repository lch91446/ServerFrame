#include "stdafx.h"
#include "Logic.h"
#include "App.h"
#include "Worker.h"

Logic::~Logic(void)
	
{
}

Logic::Logic( SessionPtr sess, const Header& header )
	: session_(sess), header_(header)
{}

