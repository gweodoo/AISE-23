#ifndef CHAT_H
#define CHAT_H

#include <string.h>
#include <stdio.h>

typedef enum
{
	HELLO,
	SPEAK,
	MP,
	KICK
}chat_command_t;


struct chat_command
{
	chat_command_t cmd;
	char login[32];
	char text[512];
};

static inline void chat_command_init(struct chat_command * cmd, chat_command_t code, char * login, char * text)
{
	memset(cmd, 0, sizeof(struct chat_command));
	cmd->cmd = code;
	snprintf(cmd->login, 32, "%s", login);
	snprintf(cmd->text, 512, "%s", text);
}

static inline void chat_command_hello(struct chat_command * cmd, char * login)
{
	chat_command_init(cmd, HELLO, login, "");
}


static inline void chat_command_speak(struct chat_command * cmd, char * login, char * text)
{
	chat_command_init(cmd, SPEAK, login, text);
}



static inline void chat_command_mp(struct chat_command * cmd, char * login, char * text)
{
	chat_command_init(cmd, MP, login, text);
}

static inline void chat_command_kick(struct chat_command * cmd, char * login)
{
	chat_command_init(cmd, KICK, login, "");
}


#endif