#include "Include.h"

volatile Error_t g_error_code = ERROR_NONE;

Error_t Error_GetCode(void)
{
  return g_error_code;
}
void Error_SetCode(Error_t code)
{
  g_error_code = code;
}

void Error_ResetCode(void)
{
	Error_SetCode(ERROR_NONE);
}






