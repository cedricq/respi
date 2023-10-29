#ifndef INC_COMMANDS_HPP_
#define INC_COMMANDS_HPP_

#include <vector>
#include <memory>

#include <string>
#include <iostream>

class Command
{
public:
    Command(const std::string& name): name_(name)
    {}
    virtual ~Command() {std::cout<<"CTOR_CMD" <<std::endl;}

    virtual const std::string Execute(std::string args[]) = 0;
    virtual const std::string Help() = 0;

    const std::string& getName()
    {
        return name_;
    }

private:
    const std::string name_;

};

class CommandManager
{
public:
    static CommandManager& getInstance();
    ~CommandManager();

    void Init();
    void Add(std::shared_ptr<Command> f);
    const std::string Execute(const std::string& str);
    const std::string Help();

private:
    CommandManager();
    std::vector<std::shared_ptr<Command>> commands_;
};

class HelpCommand : public Command
{
public:
	HelpCommand():Command("help")
	{
		CommandManager::getInstance().Add(std::shared_ptr<Command>(this));
	}
	~HelpCommand() override {std::cout<<"CTOR_HELP_CMD" <<std::endl;}

	const std::string Execute(std::string args[]) override
	{
		return CommandManager::getInstance().Help();
	}
	const std::string Help() override
	{
		return "This is the help command";
	}
};


class SetDI : public Command
{
public:
	SetDI():Command("setdi")
	{
		CommandManager::getInstance().Add(std::shared_ptr<Command>(this));
	}
	~SetDI() override {std::cout<<"CTOR_SETDI_CMD" <<std::endl;}

	const std::string Execute(std::string args[]) override
	{
		for (int i = 0; i < 3; i++)
		{
			std::cout<<i <<" " <<args[i] <<std::endl;
		}
		return "xxxxxxxxxxxxx";
	}
	const std::string Help() override
	{
		return "xxxxxxxxxxxxxxx";
	}
};

class GetDI : public Command
{
public:
	GetDI():Command("getdi")
	{
		CommandManager::getInstance().Add(std::shared_ptr<Command>(this));
	}
	~GetDI() override {std::cout<<"CTOR_GETDI_CMD" <<std::endl;}

	const std::string Execute(std::string args[]) override
	{
		return "xxxxxxxxxxxxx";
	}
	const std::string Help() override
	{
		return "xxxxxxxxxxxxxxx";
	}
};


#endif /* INC_COMMANDS_HPP_ */
