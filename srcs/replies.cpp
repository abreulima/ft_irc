
#include "replies.hpp"

std::string ReplyCodeToString(ReplyCode code)
{
	std::stringstream replyCode;
	replyCode << std::setw(3) << std::setfill('0') << code;
	return replyCode.str();
}

std::string ReplyMessage(ReplyCode code)
{
	switch (code)
	{
		case RPL_WELCOME: 
		case RPL_YOURHOST:
		case RPL_CREATED:
		case RPL_MYINFO:
		case RPL_UMODEIS:
		case RPL_CHANNELMODEIS:
		case RPL_CREATIONTIME:
		case RPL_NOTOPIC:
		case RPL_TOPIC:
		case RPL_TOPICWHOTIME:
		case RPL_INVITING:
		case RPL_NAMREPLY:
		case RPL_ENDOFNAMES:
		case RPL_MOTD:
		case RPL_MOTDSTART:
		case RPL_ENDOFMOTD:
		case ERR_NOSUCHNICK:
		case ERR_NOSUCHCHANNEL:
		case ERR_CANNOTSENDTOCHAN:
		case ERR_TOOMANYCHANNELS:
		case ERR_NOORIGIN:
		case ERR_NORECIPIENT:
		case ERR_NOTEXTTOSEND:
		case ERR_UNKNOWNCOMMAND:
		case ERR_NOMOTD:
		case ERR_NONICKNAMEGIVEN:
		case ERR_ERRONEUSNICKNAME:
		case ERR_NICKNAMEINUSE:
		case ERR_USERNOTINCHANNEL:
		case ERR_NOTONCHANNEL:
		case ERR_USERONCHANNEL:
		case ERR_NOTREGISTERED:
		case ERR_NEEDMOREPARAMS:
		case ERR_ALREADYREGISTERED:
		case ERR_PASSWDMISMATCH:
		case ERR_KEYSET:
		case ERR_CHANNELISFULL:
		case ERR_UNKNOWNMODE:
		case ERR_INVITEONLYCHAN:
		case ERR_BADCHANNELKEY:
		case ERR_BADCHANMASK:
		case ERR_CHANOPRIVSNEEDED:
		case ERR_USERSDONTMATCH:
		case ERR_INVALIDMODEPARAM:
		default: 					return std::string("");
	}

}