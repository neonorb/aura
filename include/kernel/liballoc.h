/*
 * liballoc.h
 *
 *  Created on: Jul 31, 2016
 *      Author: chris
 */

#ifndef INCLUDE_KERNEL_LIBALLOC_H_
#define INCLUDE_KERNEL_LIBALLOC_H_

#include <boot/multiboot.h>

void memory_init(multiboot_info_t* mbd);

#endif /* INCLUDE_KERNEL_LIBALLOC_H_ */
