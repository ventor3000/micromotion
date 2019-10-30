
#include "HAL.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "Config.h"
#include "Machine.h"
#include "CmdParse.h"

static char errorMessage[80];


enum COMMAND:int {
  CMD_EOF=0,
  CMD_LI=('L'<<8)|'I',
  CMD_BG=('B'<<8)|'G',
  CMD_WT=('W'<<8)|'T',
};



// ERROR UTILITES
void RaiseBufferFullError() {
	sprintf(errorMessage, "Unable to que instruction, buffer overflow");
}

/// LEXING UTILITIES
static char ReadChar(const char* cmd,int& readpos) {
  char r=cmd[readpos];
  if(r!='\0') // never go past EOL
    readpos++;
  return r;  
}

static char PeekChar(const char* cmd,int readpos) {
  char r=cmd[readpos];
  return r;  
}

static void SkipWhities(const char* cmd,int& readpos) {
  do {
    char c=cmd[readpos];

    if(c== '\0' || c=='\n' || (c>' ' && c<='~')) // non whitespace found
      return;
      
    readpos++;
  } while(true);
    
}

bool CheckEOL(const char*source,int &readpos,int& line) {
	// return true if we reached end of line (and skips '\n' if existing)
	// otherwise returns false.
  char ch=PeekChar(source,readpos);
  if(ch=='\0')
    return true;
  else if(ch=='\n') { 
    readpos++;
    line++;
    return true;
  }
  else
    return false;
}




// PARSING UTILITIES
// General logic for thoose functions are that they trigger an error and return false on failure.
// otherwise returns true


static COMMAND ParseCommand(const char* source, int& readpos) {
	// our command is always two uppercase letters
	COMMAND cmd = (COMMAND)((ReadChar(source, readpos) << 8) | ReadChar(source, readpos));
	return cmd;
}

static bool ParseInteger(const char* source, int & readpos, int64_t* res) {
	int scanres = 0;
	SkipWhities(source, readpos);

	scanres = sscanf(source + readpos, "%lld", res);

	if (scanres <= 0) {
		sprintf(errorMessage, "Number expected");
		return false;
	}

	while (isdigit(source[readpos]))
		readpos++;

	return true;

}

static bool ParseInteger(const char* source, int & readpos, int* res) {
	int scanres = 0;
	SkipWhities(source, readpos);

	scanres = sscanf(source + readpos, "%d", res);

	if (scanres <= 0) {
		sprintf(errorMessage, "Number expected");
		return false;
	}

	while (isdigit(source[readpos]) ||source[readpos]=='-' || source[readpos]=='.')
		readpos++;

	return true;
}

static bool ParseInteger(const char* source, int & readpos, long* res) {
	int scanres = 0;
	SkipWhities(source, readpos);

	scanres = sscanf(source + readpos, "%ld", res);

	if (scanres <= 0) {
		sprintf(errorMessage, "Number expected");
		return false;
	}

	while (isdigit(source[readpos]))
		readpos++;

	return true;

}

bool ParseEndOfLine(const char* source,int& readpos,int& line,const char* cmdname) {
  SkipWhities(source,readpos);
  if(!CheckEOL(source,readpos,line)) {
    sprintf(errorMessage,"End of line expected after %s",cmdname);
    HAL_PrintLn(errorMessage);
    return false;
  }

  return true;
  
}

bool ParseIntegerEOL(struct TMachine* machine, const char* source, int& readpos, int& line, const char* cmdname,int64_t* num) {

	if (!ParseInteger(source, readpos, num))
		return false;
	
	if (!ParseEndOfLine(source, readpos, line, cmdname))
		return false;
	return true;
}

bool ParseIntegerEOL(struct TMachine* machine, const char* source, int& readpos, int& line, const char* cmdname, long* num) {

	if (!ParseInteger(source, readpos, num))
		return false;

	if (!ParseEndOfLine(source, readpos, line, cmdname))
		return false;
		
	return true;
}

/*
bool ParseNamedNumber(struct TMachine* machine, const char* source, int& readpos, int& line, char& name, long* num) {

	SkipWhities(source, readpos);
	char ch = ReadChar(source, readpos);
	
	SkipWhities();

	if (!isalpha(ch)) {
		sprintf(errorMessage, "Letter expected"));
		HAL_PrintLn(errorMessage);
	}

	return ParseInteger(source, readpos, num);
}*/

// COMMAND PARSING FUNCTIONS
PARSE_RESULT ParseBG(struct TMachine* machine, const char* source, int& readpos, int& line) {
	if (!ParseEndOfLine(source, readpos, line, "BG")) {
		sprintf(errorMessage, "No data expected after BG");
		return PARSE_ERROR;
	}

	Machine_StartCycle(machine);

	return PARSE_OK;
}


PARSE_RESULT ParseWT(struct TMachine* machine, const char* source, int& readpos, int& line) {
	long milliseconds;

	if (!ParseIntegerEOL(machine, source, readpos, line, "WT", &milliseconds))
		return PARSE_ERROR;

	if (!Machine_QueWait(machine, milliseconds)) {
		RaiseBufferFullError();
		return PARSE_ERROR;
	}

	return PARSE_OK;
}


PARSE_RESULT ParseLI(struct TMachine* machine, const char* source, int& readpos, int& line) {

	COORDINATE coords[AXIS_COUNT];
	memset(coords, 0, sizeof(COORDINATE)*AXIS_COUNT);

	int numcoord = 0;
	char name;
	COORDINATE num;
	int64_t time=-1;

	while (true) {



		SkipWhities(source, readpos);

		
		

		if (CheckEOL(source, readpos, line))
			break;

		char name = ReadChar(source, readpos);
		int index = -1;
		bool res;

		switch (name) {
			case 'X': index = 0; break;
			case 'Y': index = 1; break;
			case 'Z': index = 2; break;
			case 'A': index = 3; break;
			case 'B': index = 4; break;
			case 'C': index = 5; break;
			case 'D': index = 6; break;
			case 'E': index = 7; break;
			case 'T': ParseInteger(source, readpos, &time); continue;
			default:
				sprintf(errorMessage, "Unknown argument name '%c'", name);
				HAL_PrintLn(errorMessage);
				return PARSE_ERROR;
		}

		
		if (!ParseInteger(source, readpos, &coords[index])) 
			return PARSE_ERROR;

		if(coords[index]!=0) // dont count coord if no move (0 relative move is pointless)
			numcoord++;
	}

	if (time <= 0) {
		sprintf(errorMessage, "Missing or invalid time T for LI");
		HAL_PrintLn(errorMessage);
		return PARSE_ERROR;
	}

	// que movement if sane
	if (numcoord>0) {
		if (!Machine_QueInterpolation(machine, time*1000LL, coords)) { // TODO: make a way of specifying time
			RaiseBufferFullError();
			return PARSE_ERROR;
		}
	}

	return PARSE_OK;
}

PARSE_RESULT CmdParse_ExecLineEx(struct TMachine* machine,const char* source,int& readpos,int& line) {
  COMMAND cmd;


  SkipWhities(source,readpos);

  if(CheckEOL(source,readpos,line)) {
    return PARSE_OK; // blank line => accepted
  }
  
  cmd=ParseCommand(source,readpos);

  switch(cmd) {
    case CMD_EOF: // end of file
      HAL_PrintLn("\nEnd of file found");
      return PARSE_END;
    case CMD_LI:
      HAL_PrintLn("\nLI parsed");
      return ParseLI(machine,source,readpos,line);
    case CMD_WT:
		HAL_PrintLn("\nWT parsed");
		return ParseWT(machine, source, readpos, line);
    case CMD_BG:
      HAL_PrintLn("\nBG parsed");
      return ParseBG(machine,source,readpos,line);
    default:
        sprintf(errorMessage,"\nUnknown command '%c%c'",(char)( (cmd&0xff00)>>8),(char)(cmd&0xff));
        HAL_PrintLn(errorMessage);
        return PARSE_ERROR;
  }
}

PARSE_RESULT CmdParse_ExecLine(struct TMachine* m,const char* source) {
  int line=1,readpos=0;
  return CmdParse_ExecLineEx(m,source,readpos,line);
}

