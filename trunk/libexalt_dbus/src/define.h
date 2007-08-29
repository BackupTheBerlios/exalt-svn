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


#define EXALTD_SERVICE "org.e.Exalt"
#define EXALTD_INTERFACE_READ "org.e.Exalt.Read"
#define EXALTD_PATH "/org/e/Exalt"

#define exalt_dbus_read_call_new(member) dbus_message_new_method_call(EXALTD_SERVICE,EXALTD_PATH, EXALTD_INTERFACE_READ, member)

#define EXALT_DBUS_FREE(a) (a?free(a):NULL)
#endif   /* ----- #ifndef DEFINE_INC  ----- */

