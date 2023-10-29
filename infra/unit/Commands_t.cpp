#include "gtest/gtest.h"

#include "Commands.hpp"

#include <iostream>
#include <string>

class TestCommand : public Command
{
public:
	TestCommand():Command("test_command")
	{
		CommandManager::getInstance().Add(std::shared_ptr<Command>(this));
	}
	~TestCommand() override {std::cout<<"CTOR_HELP_CMD" <<std::endl;}

	const std::string Execute(std::string args[]) override
	{
		return "This is the test execute";
	}
	const std::string Help() override
	{
		return "This is the test command";
	}
};

TEST(CommandsTest, test_command)
{
	CommandManager& mgr = CommandManager::getInstance();
	ASSERT_EQ("No command found", mgr.Execute("test_command"));

	Command* testCmd = new TestCommand();
	ASSERT_EQ("This is the test execute", mgr.Execute("test_command"));

	ASSERT_EQ("test_command\tThis is the test command\n", mgr.Help());

	Command* helpCmd = new HelpCommand();
	ASSERT_EQ("test_command\tThis is the test command\nhelp\tThis is the help command\n", mgr.Execute("help"));
}

TEST(CommandsTest, setdi_command)
{
	CommandManager& mgr = CommandManager::getInstance();

	Command* setdiCmd = new SetDI();
	Command* getdiCmd = new GetDI();

	mgr.Execute("setdi 2 100");
	ASSERT_EQ("100", mgr.Execute("getdi 2"));

	mgr.Execute("setdi 2 200");
	ASSERT_EQ("200", mgr.Execute("getdi 2"));

	mgr.Execute("setdi 1 50");
	ASSERT_EQ("50", mgr.Execute("getdi 1"));

	mgr.Execute("setdi 2 200");
	ASSERT_EQ("200", mgr.Execute("getdi 2"));
}


