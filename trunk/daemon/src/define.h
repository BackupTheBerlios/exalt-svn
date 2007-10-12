/*
 * =====================================================================================
 *
 *       Filename:  define.h
 *
 *    Description:  define some variables
 *
 *        Version:  1.0
 *        Created:  08/29/2007 01:40:41 PM CEST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:   (Watchwolf), Atton Jonathan <watchwolf@watchwolf.fr>
 *        Company:
 *
 * =====================================================================================
 */

#ifndef  DEFINE_INC
#define  DEFINE_INC

#define __UNUSED__ __attribute__((unused))

#define EXALTD_SERVICE "org.e.Exalt"
#define EXALTD_INTERFACE_READ "org.e.Exalt.Read"
#define EXALTD_INTERFACE_WRITE "org.e.Exalt.Write"
#define EXALTD_PATH "/org/e/Exalt"

#define EXALTD_PIDFILE "/var/run/exaltd.pid"
#define EXALTD_LOGFILE "/var/log/exaltd.log"

#define CONF_FILE "/etc/exaltd.eet"

#endif   /* ----- #ifndef DEFINE_INC  ----- */

