/*
 * rtc.h
 *
 *  Created on: Aug 1, 2016
 *      Author: chris13524
 */

#ifndef INCLUDE_MODULES_CLOCK_RTC_H_
#define INCLUDE_MODULES_CLOCK_RTC_H_

typedef void (*RTCHandler)(void);
void rtc_handler(RTCHandler handler);
void rtc_initialize();

#endif /* INCLUDE_MODULES_CLOCK_RTC_H_ */
