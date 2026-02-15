
#ifndef _SYS_SYSCALL_H
#define _SYS_SYSCALL_H


#ifdef __cplusplus
extern "C" {
#endif


#ifdef GENERIC___SEND_TO_PPE

typedef int (* __send_to_ppe_implementation_t)(unsigned int, unsigned int, void *);



extern __send_to_ppe_implementation_t __send_to_ppe_implementation;

#endif



int __send_to_ppe(unsigned int signalcode, unsigned int opcode, void *data);


#ifdef __cplusplus
}
#endif


#endif
