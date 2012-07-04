/****************************************************************************
 *
 *  Id: ipphone.h,v 1.0 2012/02/27 00:00:00
 *
 *  Júlio Cesar <juliocesarsaf@gmail.com>
 *
 ****************************************************************************
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 ****************************************************************************/
#include "linphonecore.h"

#ifndef IP_PHONE_H
#define IP_PHONE_H

#ifndef IP_PHONE_VERSION
#define IP_PHONE_VERSION "1.0"
#endif
#define MAX_PENDING_AUTH 8

typedef struct {
	LinphoneAuthInfo *elem[MAX_PENDING_AUTH];
	int nitems;
}IPphoneAuthStack;

typedef struct _SubList{
	MSList **vet;
	int length;
	int cursor;
	bool_t is_init;
}SubList;

typedef enum _StatusInitSubList{
	EMPTY, 
	ERROR, 
	SUCCESS
}StatusInitSubList;

typedef enum _IPphoneCallLogList{
	IPphoneMissed, 
	IPphoneReceived, 
	IPphoneDialed,
	ALL
}IPphoneCallLogList;

typedef enum _Position{
	LEFT,
	RIGHT,	
	UP, 
	DOWN,
}Position;

typedef enum _ProxyEditType{
	ServerAddress,
	Identity,
	EnableRegister,
	ExpirationTime,
	Route	
}ProxyEditType;

#define ipphone_free(ptr) if(!ptr) free(ptr)

/*The main API*/
void ipphone_init(LinphoneCore *lc, void * userdata);
void ipphone_uninit(LinphoneCore *lc);
void ipphone_iterate(LinphoneCore *lc);
int ipphone_call(LinphoneCore *lc, const char *url);
int ipphone_accept_call(LinphoneCore *lc);
int ipphone_terminate_call(LinphoneCore *lc);
const char *ipphone_core_get_version(void);
const char *ipphone_get_passwd(void);
void ipphone_set_passwd(const char *passwd);

/*Configuração de Proxy*/
LinphoneProxyConfig *ipphone_proxy_config_new(void);
int ipphone_proxy_config_set_server_addr(LinphoneProxyConfig *cfg, const char *server_addr);
int ipphone_proxy_config_set_identity(LinphoneProxyConfig *cfg, const char *identity);
void ipphone_proxy_config_enableregister(LinphoneProxyConfig *cfg, bool_t val);
void ipphone_proxy_config_expires(LinphoneProxyConfig *cfg, int expiration_time);
int ipphone_proxy_config_set_route(LinphoneProxyConfig *cfg, const char *route);
void ipphone_add_proxy_config(LinphoneCore *lc, LinphoneProxyConfig *cfg);
int ipphone_proxy_config_edit(LinphoneCore *lc, ProxyEditType proxy_edit_type, void *data_edit);
int ipphone_proxy_delete(LinphoneCore *lc);

/*Manipulação de Lista de Contatos*/
int ipphone_add_friend(LinphoneCore *lc, const char *url); 
int ipphone_delete_friend_all(LinphoneCore *lc);
void sublist_friend_call(LinphoneCore *lc, SubList *sub);
int sublist_friend_delete(LinphoneCore *lc, SubList *sub);
void sublist_edit_friend(LinphoneCore *lc, SubList *sub, const char *url);
char *ipphone_friend_get_addr(LinphoneFriend *lf);
char *ipphone_friend_get_name(LinphoneFriend *lf);
char *ipphone_friend_get_url(LinphoneFriend *lf);
void iphone_edit_friend(LinphoneCore *lc, LinphoneFriend *lf, const char *url);
void ipphone_get_friends_fields(void *data, char **fields, int index);
int is_friend_list_full(LinphoneCore *lc);

/*Manipulação de SubList*/
SubList *sublist_new();
void sublist_free(SubList *sub);
StatusInitSubList sublist_init(MSList *list, SubList *sublist, int length);
void sublist_uninit(SubList *sub);
void sublist_update(MSList *list, SubList *sublist, Position pos);
void print_sublist_contacts(SubList *sublist);

/*Funções para escrita das listas nos arquivos*/
void read_call_log_from_file(LinphoneCore *lc, const char *path_missed, const char *path_received, const char *path_dialed);
void read_friend_list_from_file(LinphoneCore *lc, const char *path);
void write_call_log_to_file(LinphoneCore *lc, MSList *list);
void write_friend_list_to_file(LinphoneCore *lc, const char *friend_file);

/*Manipulação de Call Logs*/
void ipphone_call_log_free(LinphoneCore *lc, IPphoneCallLogList type);
char *ipphone_calllog_get_dir(LinphoneCallLog *cl);
char *ipphone_calllog_get_status(LinphoneCallLog *cl);
const char *ipphone_calllog_get_from(LinphoneCallLog *cl);
const char *ipphone_calllog_get_to(LinphoneCallLog *cl);
const char *ipphone_calllog_get_date(LinphoneCallLog *cl);
int ipphone_calllog_get_duration(LinphoneCallLog *cl);

int ipphone_call_get_contacts(LinphoneCore *lc, char **username, int max_length);

#endif/* IP_PHONE_H */
