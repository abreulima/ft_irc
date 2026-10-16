#ifndef REPLIES_HPP
# define REPLIES_HPP

# include <string>
# include <sstream>

# define CRLF "\r\n"

enum ReplyCode
{
	/* ============================ REGISTRATION ============================ */
	RPL_WELCOME				= 1,
	RPL_YOURHOST			= 2,
	RPL_CREATED				= 3,
	RPL_MYINFO				= 4,
 
	/* ============================== REPLIES =============================== */
	RPL_UMODEIS				= 221,
	RPL_ENDOFWHO			= 315,
	RPL_CHANNELMODEIS		= 324,
	RPL_CREATIONTIME		= 329,
	RPL_NOTOPIC				= 331,
	RPL_TOPIC				= 332,
	RPL_TOPICWHOTIME		= 333,
	RPL_INVITING			= 341,
	RPL_WHOREPLY			= 352,
	RPL_NAMREPLY			= 353,
	RPL_ENDOFNAMES			= 366,
	RPL_MOTD				= 372,
	RPL_MOTDSTART			= 375,
	RPL_ENDOFMOTD			= 376,
 
	/* =============================== ERRORS =============================== */
	ERR_NOSUCHNICK			= 401,
	ERR_NOSUCHCHANNEL		= 403,
	ERR_CANNOTSENDTOCHAN	= 404,
	ERR_TOOMANYCHANNELS		= 405,
	ERR_NOORIGIN			= 409,
	ERR_NORECIPIENT			= 411,
	ERR_NOTEXTTOSEND		= 412,
	ERR_UNKNOWNCOMMAND		= 421,
	ERR_NOMOTD				= 422,
	ERR_NONICKNAMEGIVEN		= 431,
	ERR_ERRONEUSNICKNAME	= 432,
	ERR_NICKNAMEINUSE		= 433,
	ERR_USERNOTINCHANNEL	= 441,
	ERR_NOTONCHANNEL		= 442,
	ERR_USERONCHANNEL		= 443,
	ERR_NOTREGISTERED		= 451,
	ERR_NEEDMOREPARAMS		= 461,
	ERR_ALREADYREGISTERED	= 462,
	ERR_PASSWDMISMATCH		= 464,
	ERR_KEYSET				= 467,
	ERR_CHANNELISFULL		= 471,
	ERR_UNKNOWNMODE			= 472,
	ERR_INVITEONLYCHAN		= 473,
	ERR_BADCHANNELKEY		= 475,
	ERR_BADCHANMASK			= 476,
	ERR_CHANOPRIVSNEEDED	= 482,
	ERR_USERSDONTMATCH		= 502,
	ERR_INVALIDMODEPARAM	= 696
};

static std::string ReplyCodeToString(ReplyCode code)
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

#endif // REPLIES_HPP
