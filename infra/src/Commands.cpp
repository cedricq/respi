#include "Commands.hpp"

#include <memory>

CommandManager& CommandManager::getInstance()
{
	static CommandManager cmdmgr;
	return cmdmgr;
}

void CommandManager::Init()
{

}
void CommandManager::Add(std::shared_ptr<Command> f)
{
	commands_.push_back(f);
}
const std::string CommandManager::Execute(const std::string& str)
{
    for( const auto c : commands_)
    {
        if ( str.find(c->getName()) != -1 )
        {
        	std::cout<<str <<std::endl;
        	std::string words[] = {"\0", "\0", "\0"};
        	int pos = str.find(" ");
        	std::cout<<pos <<std::endl;

        	int i = 0;
        	int prepos = -1;
        	while ( pos != -1 && i <=2 )
        	{
        		words[i] = str.substr(prepos+1, pos - (prepos+1));
        		std::cout<<"substr " <<prepos+1 <<" " <<pos <<std::endl;
        		std::cout<<"words[" <<i <<"] = " <<words[i] <<std::endl;
        		prepos  = pos;
				pos = str.find(" ", pos+1);
				std::cout<<pos <<std::endl;
				i++;
        	}
        	if ( pos == -1 && i <=2 )
			{
				words[i] = str.substr(prepos+1);
			}

        	return c->Execute(words);
        }
    }
    return "No command found";
}

const std::string CommandManager::Help()
{
	std::string output = "";
    for( const auto c : commands_)
    {
        output.append(c->getName());
        output.append("\t");
        output.append(c->Help());
        output.append("\n");
    }
    return output;
}

CommandManager::CommandManager()
{}

CommandManager::~CommandManager()
{
	std::cout<<"CTOR_CMD_MGR" <<std::endl;}
