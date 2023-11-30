#ifndef EG_TERMINAL_COMMANDS_H__
#define EG_TERMINAL_COMMANDS_H__


#include <imterm/terminal.hpp>
#include <imterm/terminal_helpers.hpp>


namespace EG::Core {

struct custom_command_struct {
    bool should_close = false;
};

class terminal_commands : public ImTerm::basic_spdlog_terminal_helper<terminal_commands, custom_command_struct, misc::no_mutex> {
public:
    terminal_commands();

    static std::vector<std::string> no_completion(argument_type&) { return {}; }

    static void clear(argument_type&);
    static void echo(argument_type&);
    static void exit(argument_type&);
    static void help(argument_type&);
    static void quit(argument_type&);

    static void                     configure_term(argument_type&);
    static std::vector<std::string> configure_term_autocomplete(argument_type&);
};


} // namespace EG::Core

#endif /* EG_TERMINAL_COMMANDS_H__ */
