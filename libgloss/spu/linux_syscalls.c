/*
(C) Copyright IBM Corp. 2008
(C) Copyright Gustavo Ramos Carvalho. 2026

All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.
* Neither the name of IBM nor the names of its contributors may be
used to endorse or promote products derived from this software without
specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

Authors:
  Ken Werner <ken.werner@de.ibm.com> - original implementation
  Gustavo Ramos Carvalho <gc5142387@gmail.com> - non __linux_syscall

*/

#include "sys/linux_syscalls.h"
#include <errno.h>
#include <stddef.h>



#ifdef NON___LINUX_SYSCALL

__linux_syscall_implementation_t __linux_syscall_implementation = NULL;

#endif



int
__linux_syscall (struct spu_syscall_block *s)
{

#ifdef NON___LINUX_SYSCALL

    if (__linux_syscall_implementation == NULL)
    {

        errno = ENOSYS;

        return -1;

    }

    return __linux_syscall_implementation(s);

#else

  int ret;
  __vector unsigned int stopfunc = {
    0x00002104,			/* stop 0x2104  */
    (unsigned int) s,
    0x4020007f,			/* nop  */
    0x35000000			/* bi $0  */
  };

  void (*f) (void) = (void *) &stopfunc;
  asm ("sync");

  f ();

  if (s->nr_ret > -4096ull)
    {
      errno = -s->nr_ret;
      ret = -1;
    }
  else
    {
      ret = s->nr_ret;
    }
  return ret;

#endif

}
