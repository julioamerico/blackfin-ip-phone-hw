#include <eXosip2/eXosip.h>
#include <string.h>
#include "linphonecore.h"
#include "ipphone.h"
#include "queue.h"
#include "lcd.h"


const char *missed_calls = "/mnt/missed_calls";          /* sdcard */
const char *received_calls = "/mnt/received_calls";      /* sdcard */
const char *dialed_numbers = "/mnt/dialed_numbers";      /* sdcard */
const char *friend_list = "/mnt/friend_list";            /* sdcard */
const char *config_path = "/etc/config/.linphonerc";     /* flash  */
const char *phone_info_path = "/etc/config/.phone_info"; /* flash  */
const char *nw_settings = "/etc/config/nw_settings";     /* flash  */
IPphoneAuthStack auth_stack;

static void ipphone_call_received(LinphoneCore *lc, const char *from){
	queue_insert((main_queue_t *)lc->data, FSM_EVNT_CALL_IN_INVITE);
}

static void ipphone_prompt_for_auth(LinphoneCore *lc, const char *realm, const char *username){
	LinphoneAuthInfo *pending_auth;
	
	if (auth_stack.nitems + 1 > MAX_PENDING_AUTH )
	{
		printf("Can't accept another authentication request.\n Consider incrementing MAX_PENDING_AUTH macro.\n");
		return;
	} 
	
	pending_auth = linphone_auth_info_new(username,NULL,NULL,NULL,realm);
	auth_stack.elem[auth_stack.nitems++] = pending_auth;
}

static void ipphone_display_something(LinphoneCore *lc, const char *something){
	if(!strcmp(something, "User is busy."))
		something = "STATUS: BUSY        ";
	else
	if(!strcmp(something, "User does not want to be disturbed."))
		something = "STATUS: DECLINED    ";
	else
	if(!strcmp(something, "Could not reach destination."))
		something = "STATUS: UNREACHABLE ";
  else
  {
    printf("something_display:%s\n",something);
    return;
  }

	lcd_write_justified(LCD_WRITE_LEFT_JUSTIFIED, 1, something);
	queue_insert((main_queue_t *)lc->data, FSM_EVNT_LINPHONE_CALL_FAIL);
}

static void ipphone_display_url(LinphoneCore *lc, const char *something, const char *url){
}
static void ipphone_display_warning(LinphoneCore *lc, const char *something){
}
static void stub(){
}
static void ipphone_notify_received(LinphoneCore *lc,LinphoneFriend *lf, const char *from, const char *status, const char *img){
}
static void ipphone_new_unknown_subscriber(LinphoneCore *lc, LinphoneFriend *lf, const char *url){
}
static void ipphone_bye_received(LinphoneCore *lc, const char *from){
	queue_insert((main_queue_t *)lc->data, FSM_EVNT_CALL_END);
	//passar a informação from
}
static void ipphone_text_received(LinphoneCore *lc, LinphoneChatRoom *cr, const char *from, const char *msg){
}

static void ipphone_display_status(LinphoneCore *lc, const char *something){
	fsm_evnt_t new_event = -1;
	
	if(!strcmp(something, "User is temporarily unavailable."))
		something = "STATUS: UNAVAILABLE ";
	else
	if(!strcmp(something, "User cannot be found at given address."))
		something = "STATUS: UNREACHABLE ";
	else
	if(!strcmp(something, "Call declined."))
		something = "STATUS: DECLINED    ";
	else
	if(!strcmp(something, "Service Unavailable"))
		something = "STATUS: UNAVAILABLE ";
	else
	if(!strcmp(something, "Address Incomplete"))
		something = "STATUS: UNREACHABLE ";
	else
	if(!strcmp(something, "Could not reach destination."))
		something = "STATUS: UNAVAILABLE ";
  else
	{
    printf("something_status:%s\n",something);
		return;
	}

  lcd_write_justified(LCD_WRITE_LEFT_JUSTIFIED, 1, something);
  queue_insert((main_queue_t *)lc->data, FSM_EVNT_LINPHONE_CALL_FAIL);
}

static void ipphone_general_state(LinphoneCore *lc, LinphoneGeneralState *gstate){
	fsm_evnt_t new_event = -1;
	
	switch(gstate->new_state) {
		case GSTATE_POWER_OFF:
			break;
		case GSTATE_POWER_STARTUP:
			break;
		case GSTATE_POWER_ON:
			break;
		case GSTATE_POWER_SHUTDOWN:
			break;
		case GSTATE_REG_NONE:
			break;
		case GSTATE_REG_OK:
			printf("registrado\n");
			break;
		case GSTATE_REG_FAILED:
			new_event = FSM_EVNT_REG_FAILED;
			break;
		case GSTATE_CALL_IDLE:
			break;
		case GSTATE_CALL_OUT_INVITE:
			break;
		case GSTATE_CALL_OUT_CONNECTED:
			new_event = FSM_EVNT_CALL_OUT_CONNECTED;
			break;
		case GSTATE_CALL_IN_INVITE:
			break;
		case GSTATE_CALL_IN_CONNECTED:
			new_event = FSM_EVNT_CALL_IN_CONNECTED;
			break;
		case GSTATE_CALL_END:
			break;
		case GSTATE_CALL_ERROR:
			break;
	}
	printf("estado: %d\n", gstate->new_state);
	if(new_event != -1)
		queue_insert((main_queue_t *)lc->data, new_event);
}

static void ipphone_dtmf_received(LinphoneCore *lc, int dtmf){
}

static void print_prompt(LinphoneCore *lc){
}

static void ipphone_call_log_updated(LinphoneCore *lc, LinphoneCallLog *calllog){
	MSList **tmp;
	switch(calllog->dir){
		case LinphoneCallIncoming:
			if(calllog->status == LinphoneCallMissed)
				tmp = &(lc->m_calls);
			else 
			if(calllog->status == LinphoneCallSuccess)
				tmp = &(lc->r_calls);
			break;
		case LinphoneCallOutgoing:
			tmp = &(lc->d_numbers);
			break;
	}
	write_call_log_to_file(lc, *tmp);
}

const char *ipphone_version = IP_PHONE_VERSION;

const LinphoneCoreVTable vtable = {
	.show =(ShowInterfaceCb) stub,
	.inv_recv = ipphone_call_received,
	.bye_recv = ipphone_bye_received, 
	.notify_recv = ipphone_notify_received,
	.new_unknown_subscriber = ipphone_new_unknown_subscriber,
	.auth_info_requested = ipphone_prompt_for_auth,
	.display_status = ipphone_display_status,
	.display_message = ipphone_display_something,
	.call_log_updated = ipphone_call_log_updated,
	.display_warning = ipphone_display_warning,
	.display_url = ipphone_display_url,
	.display_question = (DisplayQuestionCb)stub,
	.text_received = ipphone_text_received,
        .general_state = ipphone_general_state,
	.dtmf_received = ipphone_dtmf_received
};
static void ipphone_auth_final(LinphoneCore *lc){
	LinphoneAuthInfo *pending_auth = auth_stack.elem[auth_stack.nitems - 1];
	char phone_name[20];
	char password[20];
	int fd;

  if ((fd = fopen(phone_info_path, "r")) == NULL)
    return;
	fscanf(fd, "phone_name = %s\n", phone_name);
  fscanf(fd, "password = %s", password);
  fclose(fd);

	if(password){
		linphone_auth_info_set_passwd(pending_auth, password);
		linphone_core_add_auth_info(lc, pending_auth);
		--(auth_stack.nitems);
	}
}
static void init_nw_settings(){
	FILE *file;
	char dhcp[2];
	char ip[16];
	char netmask[16];
	char gateway[16];
	char dns[16];
	char cmd[62];
	if(!(file = fopen(nw_settings, "r"))){
		if(!(file = fopen(nw_settings, "w+"))){		
			return;	
		}
		fprintf(file,"DHCP_ENABLED:y\n");
		fprintf(file,"IP_ADDRESS:10.1.1.118\n");
		fprintf(file,"NETMASK:255.255.255.0\n");
		fprintf(file,"GATEWAY:10.1.1.2\n");
		fprintf(file,"DNS_DOMAIN:10.1.1.115\n");
		fseek(file,0,SEEK_SET);
	}
	fscanf(file,"DHCP_ENABLED:%s\n",dhcp);
	if(strcmp(dhcp,"y") == 0){
		system("dhcpcd &");
	}
	else{
		fscanf(file,"IP_ADDRESS:%s\n",ip);
		fscanf(file,"NETMASK:%s\n",netmask);
		fscanf(file,"GATEWAY:%s\n",gateway);
		fscanf(file,"DNS_DOMAIN:%s\n",dns);
	
		snprintf(cmd,62,"ifconfig eth0 %s netmask %s up",ip, netmask);
		system(cmd);
		snprintf(cmd,62,"route add default gw %s",gateway);
		system(cmd);
	}
	fclose(file);
}

static void init_phone_info(void)
{
  FILE *file;
  
	if(!(file = fopen(phone_info_path, "r")))
	{
    if(!(file = fopen(phone_info_path, "w")))
      return;
    fprintf(file,"phone_name = USER\npassword = 123456");
		fclose(file);
  }
}

void ipphone_init(LinphoneCore *lc, void * userdata)
{
	init_nw_settings();
  init_phone_info();
	auth_stack.nitems = 0;
	linphone_core_init(lc, &vtable, config_path, userdata);
	read_call_log_from_file(lc, missed_calls, received_calls, dialed_numbers);
	read_friend_list_from_file(lc, friend_list);
}

void ipphone_uninit(LinphoneCore *lc){
	ipphone_call_log_free(lc, ALL);
	//linphone_core_destroy(lc);
}

void ipphone_iterate(LinphoneCore *lc){
	linphone_core_iterate(lc);
	if (auth_stack.nitems){
		ipphone_auth_final(lc);
	}
	if (lp_config_needs_commit(lc->config)){
		lp_config_sync(lc->config);
		printf("back-up\n");
	}
}
static inline int min(int a, int b){ 
	return (a < b) ? a : b; 
}
static int friend_cmp_addr(const void* data_list, const void *data){
	char *addr = (char *)data;
	LinphoneFriend *data_friend_list = (LinphoneFriend *)data_list;
	return strcmp(addr, ipphone_friend_get_addr(data_friend_list));
}
static int friend_cmp_name(const void* data_list, const void *data){
	char *name = (char *)data;
	LinphoneFriend *data_friend_list = (LinphoneFriend *)data_list;
	return strcmp(name, ipphone_friend_get_name(data_friend_list));
}
int ipphone_call(LinphoneCore *lc, const char *url){
	MSList *friend;
	if(!strchr(url, '@')){
		friend = ms_list_find_custom(lc->friends, friend_cmp_name, url);
		if(friend){
			url = ipphone_friend_get_addr(friend->data);
		}
	}
	if(lc->call != NULL){
		return -1;	
	}
	else{
		return linphone_core_invite(lc, url);
	}
}

int ipphone_accept_call(LinphoneCore *lc){
	return linphone_core_accept_call(lc, NULL);
}

int ipphone_terminate_call(LinphoneCore *lc){
	return linphone_core_terminate_call(lc, NULL);
}
const char *ipphone_core_get_version(void){
	return ipphone_version;
}

/*Proxy*/
/*linphone_proxy_config_destroy não desaloca a estrutura LinphoneProxyConfig em si, apenas os seus campos*/
LinphoneProxyConfig * ipphone_proxy_config_new(){
	return linphone_proxy_config_new();
}
/*Quando o valor retornado for 0 as strings de entrada deverão ser desalocadas*/
int ipphone_proxy_config_set_server_addr(LinphoneProxyConfig *cfg, const char *server_addr){
	if(linphone_proxy_config_set_server_addr(cfg, server_addr) < 0){
		if (cfg->reg_proxy != NULL) ms_free(cfg->reg_proxy);
		cfg->reg_proxy=NULL;
		return 1;
	}
	return 0;
}

int ipphone_proxy_config_set_identity(LinphoneProxyConfig *cfg, const char *identity){
	linphone_proxy_config_set_identity(cfg, identity);
	return (cfg->reg_identity) ? 0 : 1;
}
void ipphone_proxy_config_enableregister(LinphoneProxyConfig *cfg, bool_t val){
	cfg->reg_sendregister = val;
}

void ipphone_proxy_config_expires(LinphoneProxyConfig *cfg, int expiration_time){
	if (expiration_time <= 0) expiration_time = 600;
	cfg->expires = expiration_time;
}

int ipphone_proxy_config_set_route(LinphoneProxyConfig *cfg, const char *route){
	linphone_proxy_config_set_route(cfg, route);
	return (cfg->reg_route) ? 0 : 1;
}

static int ipphone_core_add_proxy_config(LinphoneCore *lc, LinphoneProxyConfig *cfg){
	MSList *proxy_list = linphone_core_get_proxy_config_list(lc);
	LinphoneProxyConfig* proxy_cfg;

	if(ms_list_size(proxy_list) != 0){
		proxy_cfg = proxy_list->data;
		linphone_core_remove_proxy_config(lc, proxy_list->data);
		linphone_proxy_config_destroy(proxy_cfg);	
	}
	return linphone_core_add_proxy_config(lc,cfg);
}

void ipphone_add_proxy_config(LinphoneCore *lc, LinphoneProxyConfig *cfg){
	linphone_proxy_config_enableregister(cfg, TRUE);
	ipphone_core_add_proxy_config(lc, cfg);
	linphone_core_set_default_proxy(lc, cfg);
	lp_config_set_int(lc->config,"sip","default_proxy",linphone_core_get_default_proxy(lc,NULL));
}

int ipphone_get_default_proxy(LinphoneCore *lc, LinphoneProxyConfig **proxy){
	int status;
	
	status = linphone_core_get_default_proxy(lc, proxy);
	if(status == -1)
		*proxy = ipphone_proxy_config_new();
	return status;
}
void ipphone_proxy_unregister(LinphoneCore *lc, LinphoneProxyConfig *proxy){
	linphone_proxy_config_edit(proxy);
	ipphone_proxy_config_enableregister(proxy, FALSE);
	linphone_proxy_config_done(proxy);
	eXosip_lock();
	eXosip_register_remove(proxy->rid);
	eXosip_unlock();
}
void ipphone_proxy_config_done(LinphoneCore *lc, LinphoneProxyConfig *proxy){
	if(!lc->default_proxy)
		ipphone_add_proxy_config(lc, proxy);
	else
		linphone_proxy_config_done(proxy);
}
int ipphone_proxy_config_edit(LinphoneCore *lc, ProxyEditType proxy_edit_type, void *data_edit){
	LinphoneProxyConfig* proxy_cfg;
	int err;

	linphone_core_get_default_proxy(lc, &proxy_cfg);
	linphone_proxy_config_edit(proxy_cfg);
	switch(proxy_edit_type){
		case ServerAddress:
			err = ipphone_proxy_config_set_server_addr(proxy_cfg, (const char *)data_edit);
			break;
		case Identity:
			err = ipphone_proxy_config_set_identity(proxy_cfg, (const char *)data_edit);
			break;
		case EnableRegister:
			ipphone_proxy_config_enableregister(proxy_cfg, *(bool_t *)data_edit);
			err = 0;
			break;
		case ExpirationTime:
			ipphone_proxy_config_expires(proxy_cfg, *(int *)data_edit);
			err = 0;
			break;
		case Route:
			err = ipphone_proxy_config_set_route(proxy_cfg, (const char *)data_edit);
			break;
	}
	linphone_proxy_config_done(proxy_cfg);
	return err;
}

int ipphone_proxy_delete(LinphoneCore *lc){
	MSList *elem;
	LinphoneProxyConfig* proxy_cfg;

	elem = linphone_core_get_proxy_config_list(lc);
	if(!elem){
		return 1;	
	}
	while((elem = linphone_core_get_proxy_config_list(lc)) != NULL){
		proxy_cfg = elem->data;
		linphone_core_remove_proxy_config(lc, proxy_cfg);
		linphone_proxy_config_destroy(proxy_cfg);		
	}
	//ipphone_auth_delete();
	return 0;
	
}
/*END PROXY*/

/*FRIEND*/
static void ipphone_friend_apply(LinphoneFriend *fr, LinphoneCore *lc){
	if (fr->url==NULL) {
		ms_warning("No sip url defined.");
		return;
	}
	fr->lc=lc;
	
	if (fr->inc_subscribe_pending){
		switch(fr->pol){
			case LinphoneSPWait:
				linphone_friend_notify(fr,EXOSIP_SUBCRSTATE_PENDING,LINPHONE_STATUS_PENDING);
				break;
			case LinphoneSPAccept:
				if (fr->lc!=NULL)
				  {
					linphone_friend_notify(fr,EXOSIP_SUBCRSTATE_ACTIVE,fr->lc->presence_mode);
				  }
				break;
			case LinphoneSPDeny:
				linphone_friend_notify(fr,EXOSIP_SUBCRSTATE_TERMINATED,LINPHONE_STATUS_CLOSED);
				break;
		}
		fr->inc_subscribe_pending=FALSE;
	}
	if (fr->subscribe && fr->out_did==-1){
		
		__linphone_friend_do_subscribe(fr);
	}
	ms_message("linphone_friend_apply() done.");
}

/*CALLBACK para a função de inserção ordenada na lista*/
static int friend_cmp(const void *data, const void* data_list){
	LinphoneFriend *data_friend = (LinphoneFriend *)data;
	LinphoneFriend *data_friend_list = (LinphoneFriend *)data_list;
	return strcmp(linphone_friend_get_name(data_friend), linphone_friend_get_name(data_friend_list));	
}

int is_friend_list_full(LinphoneCore *lc){
	return (ms_list_size(lc->friends) >= lc->max_friend_list);
}
static int ipphone_core_add_friend_sorted(LinphoneCore *lc, LinphoneFriend *lf, int (*compare_func)(const void *, const void*))
{
	if(ms_list_size(lc->friends) < lc->max_friend_list){
		ms_return_if_fail(lf->lc==NULL);
		ms_return_if_fail(lf->url!=NULL);
		lc->friends=ms_list_insert_sorted(lc->friends,lf,compare_func);
		write_friend_list_to_file(lc, friend_list);
		ipphone_friend_apply(lf,lc);
		return 0;
	}
	linphone_friend_destroy(lf);
	return 1;
}

int ipphone_add_friend(LinphoneCore *lc, const char *url){
	LinphoneFriend *newFriend = linphone_friend_new_with_addr(url);
	if(!newFriend){
		return -1;
	}

	return ipphone_core_add_friend_sorted(lc, newFriend, friend_cmp);
}

int ipphone_delete_friend_all(LinphoneCore *lc){
	MSList *elem;	
	if(!ms_list_size(lc->friends)){
		return 1;
	}
	while((elem = lc->friends) != NULL){
		linphone_core_remove_friend(lc, elem->data);
	}
	write_friend_list_to_file(lc, friend_list);
}

void sublist_friend_call(LinphoneCore *lc, SubList *sub){
	const char *addr;	
		
	addr = ipphone_friend_get_addr(sub->vet[sub->cursor]->data);
	ipphone_call(lc, addr);
}

void ipphone_call_log_call(LinphoneCore *lc, LinphoneCallLog *call_log, const char *(*ipphone_calllog_get)(LinphoneCallLog*))
{
  const char *addr;
  addr = ipphone_calllog_get(call_log);
  ipphone_call(lc, addr);
}

int sublist_friend_delete(LinphoneCore *lc, SubList *sub){
	int list_size, i;
	MSList *delete_elem;

	list_size = ms_list_size(lc->friends);
	if(--list_size < 0){
		return 1;
	}
	delete_elem = sub->vet[sub->cursor];   
	for(i = sub->cursor; i < sub->length - 1; i++){
		sub->vet[i] = sub->vet[i + 1];	
	}
	if(list_size < sub->length){
		linphone_core_remove_friend(lc, delete_elem->data);
		write_friend_list_to_file(lc, friend_list);
		sub->length = list_size;
		if(sub->cursor >= sub->length){
			sub->cursor = sub->length - 1;	
		}
		return 0;	
	}
	if(sub->vet[i]->next == NULL){
		sub->vet[i] = lc->friends;
	}
	else{
		sub->vet[i] = sub->vet[i]->next; 
	}
	linphone_core_remove_friend(lc, delete_elem->data);
	write_friend_list_to_file(lc, friend_list);
	return 0;
}

int sublist_is_empty(SubList *sub)
{
	if (sub->cursor < 0)
		return 1;
	else
		return 0;
}
void ipphone_edit_friend(LinphoneCore *lc, LinphoneFriend *lf, const char *url){
	linphone_core_remove_friend(lc, lf);
	ipphone_add_friend(lc, url);
	write_friend_list_to_file(lc, friend_list);	
}
void ipphone_get_friends_fields(void *data, char **fields, int index){
	LinphoneFriend *lf = (LinphoneFriend *)data;
	osip_uri_t *uri;
	if(osip_uri_init(&uri)){
		printf("Erro ipphone_get_friends_fields\n");		
		return;
	}
	if(osip_uri_parse(uri,ipphone_friend_get_addr(lf))){
		printf("Erro ipphone_get_friends_fields\n");		
		return;
	}
	switch(index){
		case 0:
			*fields = strdup(ipphone_friend_get_name(lf));
			break;
		case 1:
			*fields = strdup(osip_uri_get_username(uri));
			break;
		case 2:
			*fields = strdup(osip_uri_get_host(uri));
			break;
	}
	osip_uri_free(uri); 
}

void ipphone_get_account_fields(void *data, char **fields, int index)
{
	LinphoneProxyConfig *lp = (LinphoneProxyConfig *)data;
	char *stun_server;
	char string[1] = {'\0'};
	static char phone_name[20], password[20];
	static int aux = 0;
	osip_uri_t *uri;
	FILE *fd;

	if(osip_uri_init(&uri))
	{
    printf("Erro ipphone_get_account_fields\n");
    return;
  }

	if (lp == NULL)
	{
		*fields = strdup(string);
		return;
	}

  if(osip_uri_parse(uri,lp->reg_identity))
  {
    printf("Erro ipphone_get_account_fields\n");
    return;
  }
	
	if(!aux)
	{
	  if ((fd = fopen(phone_info_path, "r")) == NULL)
  	{
	  	*fields = strdup(string);
    	return;
 	 	}
		fscanf(fd, "phone_name = %s\n", phone_name);
  	fscanf(fd, "password = %s", password);
  	fclose(fd);
		aux = 1;
	}

	switch(index)
	{
		case 0:
			*fields = strdup(phone_name);
			break;
		case 1:
			*fields = strdup(osip_uri_get_username(uri));
			break;
		case 2:
			*fields = strdup(password);
			break;
		case 3:
			*fields = strdup(osip_uri_get_host(uri));
			aux = 0;
			break;
		case 4:
			stun_server = linphone_core_get_stun_server(lp->lc);
			if (stun_server)
				*fields = strdup(stun_server);
			else
				*fields = strdup(string);
			break;
	}
}

void sublist_edit_friend(LinphoneCore *lc, SubList *sub, const char *url){
	linphone_core_remove_friend(lc, sub->vet[sub->cursor]->data);
	ipphone_add_friend(lc, url);
	write_friend_list_to_file(lc, friend_list);	
}
char *ipphone_friend_get_addr(LinphoneFriend *lf){
	return linphone_friend_get_addr(lf); 
}

char *ipphone_friend_get_name(LinphoneFriend *lf){
	return linphone_friend_get_name(lf);
}

char *ipphone_friend_get_url(LinphoneFriend *lf){
	return linphone_friend_get_url(lf);
}
/*END FRIEND*/

/*SUBLIST*/
SubList *sublist_new(){
	SubList *sub = (SubList *)malloc(sizeof(SubList));
	
	sub->is_init = FALSE;
	return sub;	
}

void sublist_free(SubList *sub){
	if(!sub)
		return;
	if(!sub->vet)
		free(sub->vet);	
	free(sub);
}
StatusInitSubList sublist_init(MSList *list, SubList *sublist, int length){	
	int i;
	if(sublist->is_init){
		return SUCCESS;
	}
	if(ms_list_size(list)  == 0)
		return EMPTY;
	if(length > ms_list_size(list)){
		length = ms_list_size(list);
	}	
	if((sublist->vet = (MSList **)malloc(length*sizeof(MSList *))) == NULL)
	{
		return ERROR;
	}
	sublist->length = length;
	sublist->cursor = 0;
	sublist->is_init = TRUE;
	for(i = 0; i < length; i++)
	{
		sublist->vet[i] = list;
		list = ms_list_next(list);
	}
	return SUCCESS;
	 
}
void sublist_uninit(SubList *sub){
	free(sub->vet);
	sub->is_init = FALSE;
}

void sublist_update(MSList *list, SubList *sublist, Position pos){
	if(sublist->length < 1)
		return;
	switch(pos){
		case DOWN:	
			if(++sublist->cursor > sublist->length - 1){
				int i;				
				sublist->cursor = sublist->length - 1;
				for(i = 0; i < sublist->cursor; i++){
					sublist->vet[i] = sublist->vet[i + 1];
				}
				if(sublist->vet[i]->next == NULL){
					sublist->vet[i] = list;			
				}
				else
					sublist->vet[i] = sublist->vet[i]->next; 
			} 
			break;
		case UP:
			if(--sublist->cursor < 0){
				int i;
				sublist->cursor = 0;
				for(i = sublist->length - 1; i > 0; i--){
					sublist->vet[i] = sublist->vet[i - 1];				
				}
				if(sublist->vet[i]->prev == NULL){
					MSList *elem = list;
					while(ms_list_next(elem) != NULL){
						elem = ms_list_next(elem);
					}
					sublist->vet[i] = elem;						
				}
				else
					sublist->vet[i] = sublist->vet[i]->prev;
						
			}
			break;
	}	
}
void print_sublist_contacts(SubList *sublist){
	int i;
	LinphoneFriend *lf;
	for(i = 0; i < sublist->length; i++){
		lf = sublist->vet[i]->data;
		lcd_write_justified(LCD_WRITE_LEFT_JUSTIFIED, (2+i), ipphone_friend_get_name(lf));
	}
	lcd_write_justified(LCD_WRITE_RIGHT_JUSTIFIED, (2+sublist->cursor), "<");
}
static int sip_uri_get_username(const char *uri, char **username, int max_length){
	char *str, *str2;
	int length;
	
	str = strstr(uri, "<sip:");
	str2 = strchr(uri, '@');
	if(!str || !str2)
		return 1;
	length = str2 - str - 5;
	*username = strndup(str + 5, min(length,max_length));
	return 0;
}
static int sip_uri_get_addr(const char *uri, char **addr){
	char *begin, *end;
	int length;	
	begin = strchr(uri, '<');
	end = strchr(uri, '>');
	if(!begin || !end)
		return 1;
	length = end - begin - 1;
	*addr = strndup(begin + 1, length);
	return 0;
}
void search_contacts(LinphoneCore *lc, char *uri, char **username, int max_length){
	MSList *friend;
	char *addr, *friend_name;	
	sip_uri_get_addr(uri,&addr);
	friend = ms_list_find_custom(lc->friends, friend_cmp_addr, addr);
	
	if(!friend){
		sip_uri_get_username(uri,username,max_length);	
	}
	else{
		friend_name = ipphone_friend_get_name(friend->data);
		*username = strndup(friend_name, min(strlen(friend_name),max_length));
	}

	ipphone_free(addr);
}
void print_sublist_call_logs(LinphoneCore *lc, SubList *sublist, const char *(*ipphone_calllog_get)(LinphoneCallLog*))
{
	int i;
	LinphoneCallLog	*cl;
	char *username;
		
	for (i = 0; i < sublist->length; i++)
	{
		cl = sublist->vet[i]->data;
		search_contacts(lc, ipphone_calllog_get(cl), &username, 20);
		lcd_write_justified(LCD_WRITE_LEFT_JUSTIFIED, (2+i), "                    ");
		lcd_write_justified(LCD_WRITE_LEFT_JUSTIFIED, (2+i), username);
		ipphone_free(username);
	}
	lcd_write_justified(LCD_WRITE_RIGHT_JUSTIFIED, (2+sublist->cursor), "<");
}

/*Funções para escrita das listas nos arquivos*/
static int write_call_log(void *data, void *file_log){
	LinphoneCallLog *log = (LinphoneCallLog *)data;
	FILE *file = (FILE *)file_log;
	int count = 0;
	if(log == NULL)
		return 0;
	count += fprintf(file, "%d\n",log->dir);
	count += fprintf(file, "%d\n",log->status);
	count += fprintf(file, "%s\n",log->from);
	count += fprintf(file, "%s\n",log->to);
	count += fprintf(file, "%s\n",log->start_date);
	count += fprintf(file, "%d\n",log->duration);
	return 0;
}

static int write_friend_list(void *data, void *file_friend){
	LinphoneFriend *friend = (LinphoneFriend *)data;
	FILE *file = (FILE *)file_friend;
	char *tmp;
	if(friend == NULL)
		return 0;
	if (friend->url != NULL){
		osip_from_to_str(friend->url, &tmp);
		if (tmp == NULL) {
			return 0;
		}
		fprintf(file, "%s\n", tmp);
		osip_free(tmp);
	}
	fprintf(file, "%d\n", friend->in_did);
	fprintf(file, "%d\n", friend->out_did);
	fprintf(file, "%d\n", friend->sid);
	fprintf(file, "%d\n", friend->nid);
	fprintf(file, "%d\n", friend->pol);
	fprintf(file, "%d\n", friend->status);
	//Implementar gravação do proxy, temporariamente setado para -1
	fprintf(file, "%d\n", -1);
	fprintf(file, "%d\n", friend->subscribe);
	return 0; //repensar o valor de retorno de todas estas funções	
	
}
static int MSList_to_file(MSList *list, const char *path, int (*write_data)(void *, void *)){
	FILE *file_log;
	int count = 0;
	if((file_log = fopen(path, "w+")) == NULL){
		return -1;	
	}
	for (;list!=NULL;list=ms_list_next(list))
	{
		count += write_data(list->data, file_log);
	}
	fclose(file_log);
	return count;	
}

static int eof(FILE *file){
	int ch = fgetc(file);

	ungetc(ch, file);
	return ch == EOF ? 1 : 0;	
}

static char *read_str(FILE *file){
	char *str, ch;
	int length = 0;

	while((ch = fgetc(file)) != '\n'){		
		length++;
	}
	fseek(file, -1*(length + 1), SEEK_CUR);
	str = malloc((length + 1)*sizeof(char));
	fscanf(file, "%[^\n]s", str);
	fgetc(file);
	return str;	
}

static void *read_call_log(void *file_log){
	FILE *file = (FILE *)file_log;
	LinphoneCallLog *log = ms_new0(LinphoneCallLog,1);

	fscanf(file, "%d\n",&log->dir);
	fscanf(file, "%d\n",&log->status);
	log->from = read_str(file);
	log->to = read_str(file);
	fscanf(file, "%[^\n]s", log->start_date);
	fgetc(file);
	fscanf(file, "%d\n", &log->duration);
	return (void *)log;	
}
static void *read_friend_list(void *file_friend){
	FILE *file = (FILE *)file_friend;
	LinphoneFriend *friend;
	char *url;
	int tmp;

	url = read_str(file);
	friend = linphone_friend_new_with_addr(url);
	fscanf(file, "%d\n", &friend->in_did);
	fscanf(file, "%d\n", &friend->out_did);
	fscanf(file, "%d\n", &friend->sid);
	fscanf(file, "%d\n", &friend->nid);
	fscanf(file, "%d\n", &friend->pol);
	fscanf(file, "%d\n", &friend->status);
	fscanf(file, "%d\n", &tmp); //temporário: para leitura de proxy
	fscanf(file, "%d\n", &friend->subscribe);
	return (void *)friend;	
}

static MSList * MSList_from_file(MSList *list, const char *path, void * (*read_data)(void *)){
	FILE *file;
	void *data;

	if(!(file = fopen(path, "r+"))){
		return NULL;	
	}
	while(!eof(file)){
		data = read_data(file);
		list = ms_list_append(list, data);	
	}
	fclose(file);
	return list;		
}

void read_call_log_from_file(LinphoneCore *lc, const char *path_missed, const char *path_received, const char *path_dialed){
	lc->m_calls = MSList_from_file(lc->m_calls, path_missed, read_call_log);
	lc->r_calls = MSList_from_file(lc->r_calls, path_received, read_call_log);
	lc->d_numbers = MSList_from_file(lc->d_numbers, path_dialed, read_call_log);
}

void read_friend_list_from_file(LinphoneCore *lc, const char *path){
	MSList *elem;
	
	lc->friends = MSList_from_file(lc->friends, path, read_friend_list);
	elem = lc->friends;	
	while(elem != NULL){
		ipphone_friend_apply(elem->data, lc);
		elem = ms_list_next(elem);		
	}
}

void write_call_log_to_file(LinphoneCore *lc, MSList *list){
	const char *path;

	if(list == lc->r_calls){
		path = received_calls;	
	}else{
		if(list == lc->m_calls){
			path = missed_calls;
		}		
		else{
			path = dialed_numbers;
		}
	}
	MSList_to_file(list, path, write_call_log);
}

void write_friend_list_to_file(LinphoneCore *lc, const char *friend_file){
	MSList_to_file(lc->friends, friend_file, write_friend_list);
}

/*CALL LOG*/
void ipphone_call_log_free(LinphoneCore *lc, IPphoneCallLogList type){
	MSList *elem, **cl;	
	switch(type){
		case IPphoneMissed:
			cl = &(lc->m_calls);
			break;
		case IPphoneReceived:
			cl = &(lc->r_calls);
			break;
		case IPphoneDialed:
			cl = &(lc->d_numbers);
			break;
		case ALL:
			ipphone_call_log_free(lc, IPphoneMissed);
			ipphone_call_log_free(lc, IPphoneReceived);
			ipphone_call_log_free(lc, IPphoneDialed);
			return;		
	}
	elem = *cl;
	while(elem != NULL){
		if(elem->data != NULL)
			linphone_call_log_destroy((LinphoneCallLog*)elem->data);
		*cl = ms_list_remove_link(*cl, elem);
		elem = *cl;
	}
	return;		
}
/*Após utilizar estas funções desalocar a string*/
char *ipphone_calllog_get_dir(LinphoneCallLog *cl){
	char *dir;	
	switch(cl->dir){
		case LinphoneCallOutgoing:
			dir = "Outgoing";
			break;
		case LinphoneCallIncoming:
			dir = "Incoming";
			break;
		default:
			dir = "unknown";
	}
	dir = (char *)malloc(sizeof(char)*(strlen(dir) + 1));
	return dir;
}

char *ipphone_calllog_get_status(LinphoneCallLog *cl){
	char *status;
	switch(cl->status){
		case LinphoneCallSuccess:
			status = "Completed";
			break;
		case LinphoneCallAborted:
			status = "Aborted";
			break;
		case LinphoneCallMissed:
			status = "Missed";
			break;
		default:
			status = "unknown";
	}
	return status;
}
const char *ipphone_calllog_get_from(LinphoneCallLog *cl){
	return cl->from;
}

const char *ipphone_calllog_get_to(LinphoneCallLog *cl){
	return cl->to;
}

const char *ipphone_calllog_get_date(LinphoneCallLog *cl){
	return cl->start_date;
}

int ipphone_calllog_get_duration(LinphoneCallLog *cl){
	return cl->duration;
}
/*END CALL LOG*/

int ipphone_call_get_contacts(LinphoneCore *lc, char **username, int max_length){
	char *uri = NULL, *addr, *friend_name;
	LinphoneCallLog *calllog;
	MSList *friend;

	if(!lc->call)
		return 1;

	calllog = lc->call->log;
	switch(calllog->dir){
		case LinphoneCallIncoming:
			uri = calllog->from; 	
			break;
		case LinphoneCallOutgoing:
			uri = calllog->to;
			break;
	}
	sip_uri_get_addr(uri,&addr);
	friend = ms_list_find_custom(lc->friends, friend_cmp_addr, addr);
	if(!friend){
		sip_uri_get_username(uri,username,max_length);	
	}
	else{
		friend_name = ipphone_friend_get_name(friend->data);
		*username = strndup(friend_name, min(strlen(friend_name),max_length));
	}
	ipphone_free(addr);
	return 0;
}
void ipphone_get_nw_static_fields(void *data, char **fields, int index){
	char str[] = {'\0'};
	FILE *file = (FILE *)data;
	char ip[16];

	if(!file){
		*fields = strdup(str);
		return;
	}
	switch(index){
		case 0:
			fscanf(file,"IP_ADDRESS:%s\n",ip);
			break;
		case 1:
			fscanf(file,"NETMASK:%s\n",ip);
			break;
		case 2:
			fscanf(file,"GATEWAY:%s\n",ip);
			break;
		case 3:
			fscanf(file,"DNS_DOMAIN:%s\n",ip);
			break;
		default:
			break;
	}
	*fields = strdup(ip);
}
void ipphone_get_call_log_fields(void *data, char **fields, int index){
	char date[20];
	char *uri;
	void **vet = (void **)data;
	int hr,min,sec;
	struct tm tm;
	LinphoneCallLog *cl = (LinphoneCallLog *)vet[1];
	LinphoneCore *lc = (LinphoneCore *)vet[0]; 
	MSList *friend;
		
	switch(index){
		case 0:
			switch(cl->dir){
				case LinphoneCallIncoming:
					uri = cl->from; 	
					break;
				case LinphoneCallOutgoing:
					uri = cl->to;
					break;
			}
			search_contacts(lc, uri, fields, 20);
			break;
		case 1:
			strptime(cl->start_date,"%c",&tm);
			snprintf(date,20,"%d/%d/%d",tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
			*fields = strdup(date);
			break;
		case 2:
			strptime(cl->start_date,"%c",&tm);
			snprintf(date,20,"%d:%d",tm.tm_hour, tm.tm_min);
			*fields = strdup(date);
			break;
		case 3:
			sec = cl->duration%60;
			hr = cl->duration/3600;
			min = (cl->duration/60)%60;
			snprintf(date,20,"%d:%d:%d",hr,min,sec);
			*fields = strdup(date);
			break;
	}
}
void ipphone_set_stun_server(LinphoneCore *lc, char *stun_server)
{
	net_config_t *config=&lc->net_conf;
	
	if (stun_server)
	{
		linphone_core_set_stun_server(lc,stun_server);
	  linphone_core_set_firewall_policy(lc,LINPHONE_POLICY_USE_STUN);
		lp_config_set_string(lc->config,"net","stun_server",config->stun_server);
	}
	else
	{
		linphone_core_set_firewall_policy(lc,LINPHONE_POLICY_NO_FIREWALL);
	}
 
	 lp_config_set_int(lc->config,"net","firewall_policy",config->firewall_policy);
}

