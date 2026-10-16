
#include "Replies.hpp"

std::string ReplyCodeToString(ReplyCode code)
{
	std::stringstream replyCode;
	replyCode << std::setw(3) << std::setfill('0') << code;
	return replyCode.str();
}

/* static std::string ReplyMessage(ReplyCode code)
{
	switch (code)
	{
		case RPL_WELCOME:
			break;
		case RPL_YOURHOST:
			break;
		case RPL_CREATED:
			break;
		case RPL_MYINFO:
			break;
		case RPL_UMODEIS:
			break;
		case RPL_CHANNELMODEIS:
			break;
		case RPL_CREATIONTIME:
			break;
		case RPL_NOTOPIC:
			break;
		case RPL_TOPIC:
			break;
		case RPL_TOPICWHOTIME:
			break;
		case RPL_INVITING:
			break;
		case RPL_NAMREPLY:
			break;
		case RPL_ENDOFNAMES:
			break;
		case RPL_MOTD:
			break;
		case RPL_MOTDSTART:
			break;
		case RPL_ENDOFMOTD:
			break;
		case ERR_NOSUCHNICK:
			break;
		case ERR_NOSUCHCHANNEL:
			break;
		case ERR_CANNOTSENDTOCHAN:
			break;
		case ERR_TOOMANYCHANNELS:
			break;
		case ERR_NOORIGIN:
			break;
		case ERR_NORECIPIENT:
			break;
		case ERR_NOTEXTTOSEND:
			break;
		case ERR_UNKNOWNCOMMAND:
			break;
		case ERR_NOMOTD:
			break;
		case ERR_NONICKNAMEGIVEN:
			break;
		case ERR_ERRONEUSNICKNAME:
			break;
		case ERR_NICKNAMEINUSE:
			break;
		case ERR_USERNOTINCHANNEL:
			break;
		case ERR_NOTONCHANNEL:
			break;
		case ERR_USERONCHANNEL:
			break;
		case ERR_NOTREGISTERED:
			break;
		case ERR_NEEDMOREPARAMS:
			break;
		case ERR_ALREADYREGISTERED:
			break;
		case ERR_PASSWDMISMATCH:
			break;
		case ERR_KEYSET:
			break;
		case ERR_CHANNELISFULL:
			break;
		case ERR_UNKNOWNMODE:
			break;
		case ERR_INVITEONLYCHAN:
			break;
		case ERR_BADCHANNELKEY:
			break;
		case ERR_BADCHANMASK:
			break;
		case ERR_CHANOPRIVSNEEDED:
			break;
		case ERR_USERSDONTMATCH:
			break;
		case ERR_INVALIDMODEPARAM:
			break;
	}

} */