#ifndef CMDPARSE_H_INCLUDED
#define CMDPARSE_H_INCLUDED


enum PARSE_RESULT {
	PARSE_OK,
	PARSE_ERROR,
	PARSE_END
};




PARSE_RESULT CmdParse_ExecLineEx(struct TMachine* machine,const char* source,int& readpos,int& line);
PARSE_RESULT CmdParse_ExecLine(struct TMachine* machine,const char* source);



#endif

