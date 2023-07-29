#ifndef INC_COMMANDS_HPP_
#define INC_COMMANDS_HPP_

#include <vector>
#include <memory>

#include <string>

class Command
{
public:
    Command(const char* name): name_(name)
    {}
    virtual ~Command() {}

    virtual std::string Execute() = 0;
    virtual std::string Help() = 0;

    const char* getName()
    {
        return name_;
    }

private:
    const char* name_;

};

class CommandManager
{
public:
    static CommandManager& getInstance();
    ~CommandManager();

    void Init();
    void Add(std::shared_ptr<Command> f);

private:
    CommandManager();
    std::vector<std::shared_ptr<Command>> commands_;
};

class HelpCommand : public Command
{
	HelpCommand():Command("help")
	{
		CommandManager::getInstance().Add(shared_ptr<Command> this);
	}
	virtual ~HelpCommand() {}

	virtual std::string Execute()
	{
		return "This is the help execute";
	}
	virtual std::string Help()
	{
		return "This is the help command";
	}
};

#endif /* INC_COMMANDS_HPP_ */
