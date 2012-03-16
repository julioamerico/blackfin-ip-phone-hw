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

#ifndef IP_PHONE_H
#define IP_PHONE_H

#ifndef IP_PHONE_VERSION
#define IP_PHONE_VERSION "1.0"
#endif

typedef struct _SubList{
	MSList **vet;
	int length;
	int cursor;
	bool_t is_init;
}SubList;

typedef struct _GenericData{
	queue_t *event_queue;
	MSList *type_call_log;
}GenericData;

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
	UP, 
	DOWN
}Position;

typedef enum _ProxyEditType{
	ServerAddress,
	Identity,
	EnableRegister,
	ExpirationTime,
	Route	
}ProxyEditType;

/* These are callback for linphone core */
//funções estáticas não necessitam estar no .h
//static void ipphone_call_received(LinphoneCore *lc, const char *from);
//static void ipphone_prompt_for_auth(LinphoneCore *lc, const char *realm, const char *username);
//static void ipphone_display_something(LinphoneCore *lc, const char *something);
//static void ipphone_display_url(LinphoneCore *lc, const char *something, const char *url);
//static void ipphone_display_warning(LinphoneCore *lc, const char *something);
//static void stub();
//static void ipphone_notify_received(LinphoneCore *lc,LinphoneFriend *lf, const char *from, const char *status, const char *img);
//static void ipphone_new_unknown_subscriber(LinphoneCore *lc, LinphoneFriend *lf, const char *url);
//static void ipphone_bye_received(LinphoneCore *lc, const char *from);
//static void ipphone_text_received(LinphoneCore *lc, LinphoneChatRoom *cr, const char *from, const char *msg);
//static void ipphone_display_status(LinphoneCore *lc, const char *something);
//static void ipphone_general_state(LinphoneCore *lc, LinphoneGeneralState *gstate);
//static void ipphone_dtmf_received(LinphoneCore *lc, int dtmf);
//static void print_prompt(LinphoneCore *lc);
//static void ipphone_call_log_updated(LinphoneCore *lc, IPphoneCallLogList* calllog);

/*The main API*/
void ipphone_init(LinphoneCore *lc, const char *config_path, const char *friend_list, const char *missed_calls, const char *received_calls, const char  		  *dialed_numbers, void * userdata);
void ipphone_uninit(LinphoneCore *lc, SubList *sub);
void ipphone_iterate(LinphoneCore *lc);
int ipphone_call(LinphoneCore *lc, const char *url);
int ipphone_accept_call(LinphoneCore *lc);
int ipphone_terminate_call(LinphoneCore *lc);
const char *ipphone_core_get_version(void);

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
int ipphone_core_add_friend_sorted(LinphoneCore *lc, LinphoneFriend *lf, int (*compare_func)(const void *, const void*));
int ipphone_add_friend(LinphoneCore *lc, const char *url);
int ipphone_delete_friend_all(LinphoneCore *lc);
void sublist_friend_call(LinphoneCore *lc, Sublist *sub);
int sublist_friend_delete(LinphoneCore *lc, SubList *sub);
void sublist_edit_friend(LinphoneCore *lc, SubList *sub, const char *url);
char *ipphone_friend_get_addr(LinphoneFriend *lf);
char *ipphone_friend_get_name(LinphoneFriend *lf);
char *ipphone_friend_get_url(LinphoneFriend *lf);

/*Manipulação de SubList*/
SubList *sublist_new();
void sublist_free(SubList *sub);
StatusInitSubList sublist_init(MSList *list, SubList *sublist, int length);
void sublist_uninit(SubList *sub);
void sublist_update(MSList *list, SubList *sublist, Position pos);
void sublist_show(SubList *sublist);

/*Funções para escrita das listas nos arquivos*/
void read_call_log_from_file(LinphoneCore *lc, char *path_missed, char *path_received, char *path_dialed);
void read_friend_list_from_file(LinphoneCore *lc, char *path);
void write_call_log_to_file(LinphoneCore *lc, const char *received_file, const char *missed_file, const char *dialed_file);
void write_friend_list_to_file(LinphoneCore *lc, const char *friend_file);

/*Manipulação de Call Logs*/
void ipphone_call_log_free(LinphoneCore *lc, IPphoneCallLogList type);
char *ipphone_calllog_get_dir(LinphoneCallLog *cl);
char *ipphone_calllog_get_status(LinphoneCallLog *cl);
const char *ipphone_calllog_get_from(LinphoneCallLog *cl);
const char *ipphone_calllog_get_to(LinphoneCallLog *cl);
const char *ipphone_calllog_get_date(LinphoneCallLog *cl);
int ipphone_calllog_get_duration(LinphoneCallLog *cl);



/*TODO*/
//função para adicionar proxy  - ok
//função para deletar proxy  - implementar função para deletar informacaoes de autenticacao
//função para editar proxy  - ok
//estudar o processo de autenticação. Quando o proxy for editado o processo de REGISTAR é automático?
//ou é necessário realizar um unREGISTER e depois um REGISTER com o novo endereço de servidor


/*Gerenciamento de Lista de Contato*/
//adicionar amigo - implementado
//remover amigo - implementado
//remover todos os amigos - implementado
//editar amigo - implementado
//listar amigo
//implementar friend_list_to_file lembrar de passar parametro path
//implementar funções gets - implementado

/*Gerenciamento de Call Logs*/
//deletar call log - implementado
//visualizar call log
//get dos campos - implementado


/*Implementar CALLBACKS*/
//Alterar função log_completed para adequação a nova estrutura de dados utilizada, i.e, fazer data->type_call_log = *tmp;


#endif/* IP_PHONE_H */
