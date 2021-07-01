#include "ExceptionHelper.h"
#include <sstream>
ExceptionHelper::ExceptionHelper(int line, const char* file) noexcept : line(line),file(file)
{
}
const char* ExceptionHelper::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl<<GetOriginString();
	whatBuffer = oss.str(); //set buffer to avoid the dead memory 
	return whatBuffer.c_str();
}

const char* ExceptionHelper::GetType() const noexcept
{
	return "Exception Handler";
}

int ExceptionHelper::GetLine() const noexcept
{
	return line;
}

const std::string& ExceptionHelper::GetFile() const noexcept
{
	return file;
}

std::string ExceptionHelper::GetOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "[File] " << file << std::endl
		<< "[Line] " << line;
	return oss.str();
}

