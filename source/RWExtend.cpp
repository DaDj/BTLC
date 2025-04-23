
#include "RWExtend.h"

RwTexture* MyRwReadTexture(char* name, char* Maskname)
{
	return ((RwTexture * (__cdecl*)(char const*, char const*))0x4C7510)(name, Maskname);
}